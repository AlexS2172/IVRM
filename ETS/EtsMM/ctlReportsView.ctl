VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{1BCC7098-34C1-4749-B1A3-6C109878B38F}#1.0#0"; "vspdf8.ocx"
Object = "{54850C51-14EA-4470-A5E4-8C5DB32DC853}#1.0#0"; "vsprint8.ocx"
Begin VB.UserControl ctlReportsView 
   ClientHeight    =   7095
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8520
   BeginProperty Font 
      Name            =   "Arial"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   KeyPreview      =   -1  'True
   ScaleHeight     =   7095
   ScaleWidth      =   8520
   Begin VB.PictureBox picNav 
      BorderStyle     =   0  'None
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   0
      ScaleHeight     =   375
      ScaleWidth      =   6975
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   600
      Width           =   6975
      Begin VB.ComboBox cbxZoom 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3900
         Style           =   2  'Dropdown List
         TabIndex        =   7
         Top             =   0
         Width           =   1575
      End
      Begin VB.CommandButton btnNav 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Index           =   4
         Left            =   2760
         Picture         =   "ctlReportsView.ctx":0000
         Style           =   1  'Graphical
         TabIndex        =   6
         ToolTipText     =   "Last Page"
         Top             =   0
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Index           =   3
         Left            =   2340
         Picture         =   "ctlReportsView.ctx":004B
         Style           =   1  'Graphical
         TabIndex        =   5
         ToolTipText     =   "Next Page"
         Top             =   0
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Index           =   2
         Left            =   480
         Picture         =   "ctlReportsView.ctx":0092
         Style           =   1  'Graphical
         TabIndex        =   4
         ToolTipText     =   "Previous Page"
         Top             =   0
         Width           =   375
      End
      Begin VB.CommandButton btnNav 
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   300
         Index           =   1
         Left            =   60
         Picture         =   "ctlReportsView.ctx":00DA
         Style           =   1  'Graphical
         TabIndex        =   3
         ToolTipText     =   "First Page"
         Top             =   0
         Width           =   375
      End
      Begin VB.Label lblZoom 
         Alignment       =   1  'Right Justify
         Caption         =   "Zoom :"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   3180
         TabIndex        =   10
         Top             =   60
         Width           =   675
      End
      Begin VB.Label lblPage 
         Alignment       =   2  'Center
         Caption         =   "Page 1 of 1"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Height          =   315
         Left            =   900
         TabIndex        =   9
         Top             =   60
         Width           =   1395
      End
   End
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   840
      Top             =   4680
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
   Begin VSPrinter8LibCtl.VSPrinter vsPrinter 
      Height          =   4395
      Left            =   0
      TabIndex        =   1
      Top             =   2640
      Width           =   7635
      _cx             =   13467
      _cy             =   7752
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      MousePointer    =   0
      BackColor       =   -2147483643
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      BeginProperty HdrFont {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Courier New"
         Size            =   14.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      AutoRTF         =   -1  'True
      Preview         =   -1  'True
      DefaultDevice   =   0   'False
      PhysicalPage    =   -1  'True
      AbortWindow     =   -1  'True
      AbortWindowPos  =   0
      AbortCaption    =   "Printing..."
      AbortTextButton =   "Cancel"
      AbortTextDevice =   "on the %s on %s"
      AbortTextPage   =   "Now printing Page %d of"
      FileName        =   ""
      MarginLeft      =   1440
      MarginTop       =   1440
      MarginRight     =   1440
      MarginBottom    =   1440
      MarginHeader    =   0
      MarginFooter    =   0
      IndentLeft      =   0
      IndentRight     =   0
      IndentFirst     =   0
      IndentTab       =   720
      SpaceBefore     =   0
      SpaceAfter      =   0
      LineSpacing     =   100
      Columns         =   1
      ColumnSpacing   =   180
      ShowGuides      =   2
      LargeChangeHorz =   300
      LargeChangeVert =   300
      SmallChangeHorz =   30
      SmallChangeVert =   30
      Track           =   0   'False
      ProportionalBars=   -1  'True
      Zoom            =   22.6325757575758
      ZoomMode        =   3
      ZoomMax         =   400
      ZoomMin         =   10
      ZoomStep        =   25
      EmptyColor      =   -2147483636
      TextColor       =   0
      HdrColor        =   0
      BrushColor      =   0
      BrushStyle      =   0
      PenColor        =   0
      PenStyle        =   0
      PenWidth        =   0
      PageBorder      =   0
      Header          =   ""
      Footer          =   ""
      TableSep        =   "|;"
      TableBorder     =   7
      TablePen        =   0
      TablePenLR      =   0
      TablePenTB      =   0
      NavBar          =   3
      NavBarColor     =   -2147483633
      ExportFormat    =   0
      URL             =   ""
      Navigation      =   3
      NavBarMenuText  =   "Whole &Page|Page &Width|&Two Pages|Thumb&nail"
      AutoLinkNavigate=   0   'False
      AccessibleName  =   ""
      AccessibleDescription=   ""
      AccessibleValue =   ""
      AccessibleRole  =   9
   End
   Begin VSFlex7Ctl.VSFlexGrid fgRes 
      Height          =   1095
      Left            =   0
      TabIndex        =   8
      Top             =   1320
      Visible         =   0   'False
      Width           =   7575
      _cx             =   13361
      _cy             =   1931
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
   Begin VSPDF8LibCtl.VSPDF8 vsPdfCtrl 
      Left            =   7800
      Top             =   1680
      Author          =   ""
      Creator         =   ""
      Title           =   ""
      Subject         =   ""
      Keywords        =   ""
      Compress        =   3
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
      End
      Begin VB.Menu mnuSeparator1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlReportsView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1


Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label
Public WithEvents imgStop As vB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As vB.Image

Private m_gdFlt As New clsGridDef
Private m_gdRes As New clsGridDef

Private m_nFilter(PFC_REPORT To PFC_LAST_COLUMN) As Long
Private m_nFilterPrev(PFC_REPORT To PFC_LAST_COLUMN) As Long
Private m_dMaxStrikeDifPrev As Double

Private m_bInProc As Boolean
Private m_bShutDown As Boolean
Private m_bRecalc As Boolean
Private m_bLastQuoteReqNow As Boolean
Private m_bDataLoad As Boolean

Private m_QuoteRequests As New clsRpRequestColl
Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_REPORTS_FILTER To GT_REPORTS_RESULTS) As Boolean
Private m_GridLock(GT_REPORTS_FILTER To GT_REPORTS_RESULTS) As New clsGridLock
Private m_Grp As New clsRpGrpAtom
Private m_Und As EtsMmReportsLib.MmRpUndColl
 
Public m_frmOwner As Form

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
Private m_bGroupRequestSupported As Boolean

Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Private m_GreeksByMonth As New EtsMmReportsLib.MmRpGreeksByMonthColl
Private m_GreeksSummary As New EtsMmReportsLib.MmRpGreeksSummaryColl
Private m_MatrixByStock As New EtsMmReportsLib.MmRpMatrixByStockColl
Private m_Synthetic As New EtsMmReportsLib.MmRpSyntheticColl
Private m_PnL As New EtsMmReportsLib.MmRpPnLColl
Private m_ExercisedStock As New EtsMmReportsLib.MmRpExercisedStockColl
Private m_RiskMatrix As New EtsMmReportsLib.MmRpRiskMatrixColl
Private m_PosWithEarlyExercise As New EtsMmReportsLib.MmRpPosWithEarlyExerciseColl

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1
Public Property Get Filter(ByVal nFilter As Long) As Long
    Filter = m_nFilter(nFilter)
End Property

Public Function Init() As Boolean
    On Error GoTo EH
    
    LoadSettings
    m_sCurrentOriginalText = ""
    
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    m_bKeyDown(GT_REPORTS_FILTER) = False
    m_bKeyDown(GT_REPORTS_RESULTS) = False
    
    m_GridLock(GT_REPORTS_FILTER).Init fgFlt
    m_GridLock(GT_REPORTS_RESULTS).Init fgRes
    
    InitColumns
    InitGrids
    InitPrinter
    InitCombos
    
    FormatFltGrid
    FormatFltColumns
    
    InitFltData
    
    FormatResGrid
'    FormatResColumns
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequestSupported = False ' g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    Set m_Und = New EtsMmReportsLib.MmRpUndColl
    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    
    Init = True
    
    AdjustCaption
    AdjustState
    
    fgFlt.Select 1, PFC_REPORT
    
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to init reports view."
End Function
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    Dim i&
    
    m_nFilter(PFC_UNDERLYING) = aFilters.Data(MFC_SYMBOL)
    m_nFilter(PFC_GROUP) = aFilters.Data(MFC_GROUPS)
    m_nFilter(PFC_TRADER_GROUP) = aFilters.Data(MFC_TRADER_GROUP)
    m_nFilter(PFC_TRADER) = aFilters.Data(MFC_TRADER)
    m_nFilter(PFC_STRATEGY) = aFilters.Data(MFC_STRATEGY)
    m_nFilter(PFC_TYPE) = aFilters.Data(MFC_TRADES)
   
    For i = PFC_REPORT To PFC_LAST_COLUMN
        m_nFilterPrev(i) = BAD_LONG_VALUE
    Next
    m_dMaxStrikeDifPrev = BAD_DOUBLE_VALUE
    
    FilterUpdate
    FilterUpdateChangedStatus
    AdjustState
End Sub
Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Dim i&
    
    Select Case nType
        Case TYPE_UNDERLYING
            m_nFilter(PFC_UNDERLYING) = nValue
        Case TYPE_GROUP
            m_nFilter(PFC_GROUP) = nValue
        Case TYPE_TRADER_GROUP
            m_nFilter(PFC_TRADER_GROUP) = nValue
        Case TYPE_TRADER
            m_nFilter(PFC_TRADER) = nValue
        Case TYPE_STRATEGY
            m_nFilter(PFC_STRATEGY) = nValue
    End Select
    
    For i = PFC_REPORT To PFC_LAST_COLUMN
        m_nFilterPrev(i) = BAD_LONG_VALUE
    Next
    m_dMaxStrikeDifPrev = BAD_DOUBLE_VALUE
    
    FilterUpdate
    FilterUpdateChangedStatus
    AdjustState
End Sub

Private Sub FilterUpdate()
    On Error Resume Next
    Dim nValue&
    
    With fgFlt
        m_GridLock(GT_REPORTS_FILTER).LockRedraw
        
        nValue = m_nFilter(PFC_REPORT)
        If nValue <= REPORT_NONE Or nValue > REPORT_LAST Then nValue = 0
        m_nFilter(PFC_REPORT) = nValue
        .TextMatrix(1, PFC_REPORT) = nValue
        
        If nValue = REPORT_CLOSE_TO_SPOT Then
            .ColHidden(PFC_MAX_STRIKE_DISTANCE) = False
        Else
            .ColHidden(PFC_MAX_STRIKE_DISTANCE) = True
        End If
        
        nValue = m_nFilter(PFC_UNDERLYING)
        If g_Underlying(nValue) Is Nothing Then nValue = 0
        m_nFilter(PFC_UNDERLYING) = nValue
        .TextMatrix(1, PFC_UNDERLYING) = nValue
        
        .TextMatrix(1, PFC_TYPE) = m_nFilter(PFC_TYPE)

        nValue = m_nFilter(PFC_GROUP)
        If g_UnderlyingGroup(nValue) Is Nothing Then nValue = 0
        m_nFilter(PFC_GROUP) = nValue
        .TextMatrix(1, PFC_GROUP) = nValue
        
        nValue = m_nFilter(PFC_TRADER_GROUP)
        If g_TraderGroup(nValue) Is Nothing Then nValue = 0
        m_nFilter(PFC_TRADER_GROUP) = nValue
        .TextMatrix(1, PFC_TRADER_GROUP) = nValue
        
        nValue = m_nFilter(PFC_TRADER)
        If g_Trader(nValue) Is Nothing Then nValue = 0
        m_nFilter(PFC_TRADER) = nValue
        .TextMatrix(1, PFC_TRADER) = nValue
        
        nValue = m_nFilter(PFC_STRATEGY)
        If nValue <> NO_STRATEGY_ID And g_Strategy(nValue) Is Nothing Then nValue = 0
        m_nFilter(PFC_STRATEGY) = nValue
        .TextMatrix(1, PFC_STRATEGY) = nValue
        
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
    End With
    
    
End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, nValue&
    Dim aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    Dim aTraderGroup As EtsGeneralLib.EtsTraderGroupAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    With fgFlt
        m_GridLock(GT_REPORTS_FILTER).LockRedraw
        
        nValue = 0
        sValue = CStr(nValue)
        sComboList = "#0;Greek By Month|#1;Greek Summary|#2;With Early Exercise|" & _
            "#3;Matrix By Stock|#4;Synthetic|#5;PnL|#6;Risk Matrix|#7;Exercised Stock|" & _
            "#8;Close To Spot"
        .ColComboList(PFC_REPORT) = sComboList
        .TextMatrix(1, PFC_REPORT) = sValue
        m_nFilter(PFC_REPORT) = nValue
        
        nValue = 0
        sValue = CStr(nValue)
        
        'sComboList = g_Params.UnderlyingComboList
        sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
        If Len(sComboList) > 0 Then
            sComboList = "#0;<All>|" & sComboList
        Else
            sComboList = "#0;<All>"
        End If
        
        Set aUnd = g_Underlying(m_nFilter(PFC_UNDERLYING))
        If Not aUnd Is Nothing Then
            nValue = aUnd.ID
            sValue = Trim$(Str$(aUnd.ID))
            Set aUnd = Nothing
        End If
        .ColComboList(PFC_UNDERLYING) = sComboList
        .TextMatrix(1, PFC_UNDERLYING) = sValue
        m_nFilter(PFC_UNDERLYING) = nValue
    
        .ColComboList(PFC_TYPE) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, PFC_TYPE) = "0"
    
        nValue = 0
        sValue = CStr(nValue)
        sComboList = "#0;<All>"
        For Each aUndGroup In g_UnderlyingGroup
            sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
            If aUndGroup.ID = m_nFilter(PFC_GROUP) Then
                nValue = aUndGroup.ID
                sValue = Trim$(Str$(aUndGroup.ID))
            End If
        Next
        .ColComboList(PFC_GROUP) = sComboList
        .TextMatrix(1, PFC_GROUP) = sValue
        m_nFilter(PFC_GROUP) = nValue
    
        nValue = 0
        sValue = CStr(nValue)
        sComboList = "#0;<All>"
        For Each aTraderGroup In g_TraderGroup
            If aTraderGroup.ID <> 0 Then
                sComboList = sComboList & "|#" & Trim$(Str$(aTraderGroup.ID)) & ";" & aTraderGroup.Name
            End If
            If aTraderGroup.ID = m_nFilter(PFC_TRADER_GROUP) Then
                nValue = aTraderGroup.ID
                sValue = Trim$(Str$(nValue))
            End If
        Next
        .ColComboList(PFC_TRADER_GROUP) = sComboList
        .TextMatrix(1, PFC_TRADER_GROUP) = sValue
        m_nFilter(PFC_TRADER_GROUP) = nValue
        
        nValue = 0
        sValue = CStr(nValue)
        sComboList = "#0;<All>"
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
            If aTrader.ID = m_nFilter(PFC_TRADER) Then
                nValue = aTrader.ID
                sValue = Trim$(Str$(nValue))
            End If
        Next
        .ColComboList(PFC_TRADER) = sComboList
        .TextMatrix(1, PFC_TRADER) = sValue
        m_nFilter(PFC_TRADER) = nValue
        
        nValue = 0
        sValue = CStr(nValue)
        sComboList = "#0;<All>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
        'sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
        If m_nFilter(HFC_VALUE) = NO_STRATEGY_ID Then
            nValue = NO_STRATEGY_ID
            sValue = NO_STRATEGY_NAME
        End If
        
        For Each aStrategy In g_Strategy
            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
            If aStrategy.ID = m_nFilter(PFC_STRATEGY) Then
                nValue = aStrategy.ID
                sValue = Trim$(Str$(aStrategy.ID))
            End If
        Next
        .ColComboList(PFC_STRATEGY) = sComboList
        .TextMatrix(1, PFC_STRATEGY) = sValue
        m_nFilter(PFC_STRATEGY) = nValue
    
        m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
    End With
End Sub

Private Function NoFilter() As Boolean
    On Error Resume Next
    NoFilter = Not (m_nFilter(PFC_UNDERLYING) <> 0 _
                    Or m_nFilter(PFC_UNDERLYING) <> 0 _
                    Or m_nFilter(PFC_TYPE) <> 0 _
                    Or m_nFilter(PFC_GROUP) <> 0 _
                    Or m_nFilter(PFC_TRADER_GROUP) <> 0 _
                    Or m_nFilter(PFC_TRADER) <> 0 _
                    Or m_nFilter(PFC_STRATEGY) <> 0)
End Function

Private Function CheckUndFilter(aUnd As EtsGeneralLib.UndAtom) As Boolean
    Dim nValue&
    If Not NoFilter Then
        nValue = m_nFilter(PFC_UNDERLYING)
        If nValue <> 0 Then
            If aUnd.ID = nValue Then
                CheckUndFilter = True
            Else
                CheckUndFilter = False
            End If
        Else
            CheckUndFilter = True
        End If
    Else
        CheckUndFilter = True
    End If
End Function

Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    bMatched = False
    
    
    If aTrd.ContractType = enCtOption Then
        If aTrd.Opt.Expiry < Date Then
            CheckTradeFilter = False
            Exit Function
        End If
    End If
        
    If Not NoFilter Then
        ' futures & futures options trades are not supported yet
        'If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then Exit Function
        Dim iSymValueID&
        iSymValueID = aTrd.UndID
        If aTrd.ContractType = enCtFutOption And Not aTrd.Und Is Nothing Then
            iSymValueID = aTrd.Und.ID
        End If
        'If aTrd.ContractType = enCtFuture Then Exit Function
        nValue = m_nFilter(PFC_UNDERLYING)
        'bMatched = (nValue = 0 Or nValue = aTrd.UndID)
        bMatched = (nValue = 0 Or nValue = iSymValueID)
    
        If bMatched Then
            ' "#0;<All>|#1;Today's|#2;All Except Today's|#3;Simulated Only"
            nValue = m_nFilter(PFC_TYPE)
            bMatched = (nValue = 0)
            If Not bMatched Then
                Select Case nValue
                    Case 1 ' Today's
                        bMatched = (Int(aTrd.TradeDate) = Int(Date) And aTrd.Status <> enTsSimulated)
                            
                    Case 2 ' OpenPos
                        bMatched = (Int(aTrd.TradeDate) < Int(Date) Or aTrd.IsPosition <> 0)
                            
                    Case 3 ' NetPos
                        bMatched = (aTrd.Status <> enTsSimulated)
                            
                    Case 4 ' Simulated
                        bMatched = (aTrd.Status = enTsSimulated)
                            
                    Case 5 ' Today's & Sim
                        bMatched = (Int(aTrd.TradeDate) = Int(Date) Or aTrd.Status = enTsSimulated)
                    
                    Case 6 ' OpenPos & Sim
                        bMatched = (Int(aTrd.TradeDate) < Int(Date) Or aTrd.IsPosition <> 0 Or aTrd.Status = enTsSimulated)
                            
                    Case 7 ' Manual
                        bMatched = (aTrd.Status = enTsManual)
                    
                    Case 8 ' Manual & Sim
                        bMatched = (aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated)
                            
                End Select
            End If
        End If
    
        If bMatched Then
            nValue = m_nFilter(PFC_GROUP)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = Not g_UnderlyingGroup(nValue).Und(aTrd.UndID) Is Nothing
            End If
        End If
    
        If bMatched Then
            nValue = m_nFilter(PFC_TRADER_GROUP)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = Not g_TraderGroup(nValue).Trader(aTrd.Trader.ID) Is Nothing
            End If
        End If
        
        If bMatched Then
            nValue = m_nFilter(PFC_TRADER)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = (aTrd.Trader.ID = nValue)
            End If
        End If
    
        If bMatched Then
            nValue = m_nFilter(PFC_STRATEGY)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = (aTrd.strategyID = IIf(nValue > 0, nValue, 0))
            End If
        End If
        
        CheckTradeFilter = bMatched
    Else
        CheckTradeFilter = True
    End If
End Function

Private Function GetUnderlyingKey(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As String

    GetUnderlyingKey = CStr(aTrd.Und.ID)
    'If m_nFilter(PFC_REPORT) <> REPORT_CLOSE_TO_SPOT Then
    '    If IsItGrouppedReport(False) = False Then
    '        Exit Function
    '    End If
    'End If
    GetUnderlyingKey = GetUnderlyingKey + "_"
    If Not aTrd.Strategy Is Nothing Then
        GetUnderlyingKey = GetUnderlyingKey + CStr(aTrd.Strategy.ID)
    Else
        GetUnderlyingKey = GetUnderlyingKey + " "
    End If
    
    'GetUnderlyingKey = GetUnderlyingKey + "_"
    'GetUnderlyingKey = GetUnderlyingKey + CStr(aTrd.TradeID)
End Function
Private Function GetUnderlyingKeyByStrategy(ByVal UnderlyingID, ByRef strategyID) As String
  
    GetUnderlyingKeyByStrategy = CStr(UnderlyingID)
    'If IsItGrouppedReport(False) = False Then
    '    Exit Function
    'End If
    GetUnderlyingKeyByStrategy = GetUnderlyingKeyByStrategy + "_"
    If strategyID <> NO_STRATEGY_ID Then
        GetUnderlyingKeyByStrategy = GetUnderlyingKeyByStrategy + CStr(strategyID)
    Else
        GetUnderlyingKeyByStrategy = GetUnderlyingKeyByStrategy + " "
    End If
    
    'GetUnderlyingKey = GetUnderlyingKey + "_"
    'GetUnderlyingKey = GetUnderlyingKey + CStr(aTrd.tradeID)
End Function

Private Function AddTrade2Report(aTrd As EtsGeneralLib.MmTradeInfoAtom, bTradeFake As Boolean) As Boolean
    Dim aUnd As EtsMmReportsLib.MmRpUndAtom, aOpt As EtsMmReportsLib.MmRpOptAtom
    Dim aOptRoot As EtsMmReportsLib.MmRpOptRootAtom
    Dim nKey&, sKey$, aGUnd As EtsGeneralLib.UndAtom
    Dim fIV#, fHV#, fVolOfVol#, fVega#, dtUpdateDate As Date
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom, aSynthUnd As EtsMmReportsLib.MmRpUndAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim aFut As EtsMmReportsLib.MmRpFutAtom
    
    sKey = GetUnderlyingKey(aTrd)
    nKey = aTrd.UndID
        
    Set aUnd = m_Und(sKey)

    If aUnd Is Nothing Then
        
        Set aUnd = m_Und.Add(sKey)
        aUnd.ID = aTrd.Und.ID
        
        aUnd.Symbol = aTrd.Und.Symbol
        
        Set aUnd.price = Nothing
        Set aUnd.price = m_Und.GetPriceProperty(aUnd.Symbol)
        If aUnd.price Is Nothing Then
            Set aUnd.price = New EtsMmReportsLib.MMRpPrice
        End If
        

'        If Not aTrd.Trader Is Nothing Then
'            aUnd.Account = aTrd.Trader.Name
'        End If

        If Not aTrd.Strategy Is Nothing Then
            aUnd.Strategy = aTrd.Strategy.Name
        Else
            aUnd.Strategy = ""
        End If
                
                aUnd.UndType = aTrd.Und.UndType
                aUnd.Symbol = aTrd.Und.Symbol
        
                If Not aTrd.Trader Is Nothing Then
                    aUnd.Trader = aTrd.Trader.Name
                End If
                If Not aTrd.Strategy Is Nothing Then
                        aUnd.Strategy = aTrd.Strategy.Name
                End If
                    
                If aUnd.UndType = enCtStock Then
                    Set aUnd.Dividend = aTrd.Und.Dividend
                Else
                    Set aUnd.Dividend = aTrd.Und.Dividend
                    Set aUnd.BasketIndex = g_Index(aUnd.ID)
                        
                    If (Not aUnd.Dividend Is Nothing) Then
                        If (aUnd.Dividend.CustomDivs Is Nothing) Then
                            If (Not g_Main.UnderlyingAll(aUnd.ID) Is Nothing) Then
                                If (Not g_Main.UnderlyingAll(aUnd.ID).Dividend Is Nothing) Then
                                    Set aUnd.Dividend.CustomDivs = g_Main.UnderlyingAll(aUnd.ID).Dividend.CustomDivs
                                End If
                            End If
                        End If
                    End If
                    aUnd.Yield = aTrd.Und.Yield
                End If

                ' setup active future for underlying
                If Not aTrd.Und.ActiveFuture Is Nothing Then
                    Set aUnd.ActiveFuture = New EtsMmReportsLib.MmRpFutAtom
                    aUnd.ActiveFuture.ID = aTrd.Und.ActiveFuture.ID
                    aUnd.ActiveFuture.Symbol = aTrd.Und.ActiveFuture.Symbol
                    aUnd.ActiveFuture.Maturity = aTrd.Und.ActiveFuture.MaturityDate
                    aUnd.ActiveFuture.IsAmerican = aTrd.Und.ActiveFuture.IsAmerican
                    aUnd.ActiveFuture.FutureBasis = aTrd.Und.ActiveFuture.FutureBasis
                    Set aUnd.ActiveFuture.UndPriceProfile = aTrd.Und.ActiveFuture.UndPriceProfile
                    Set aUnd.ActiveFuture.OptPriceProfile = aTrd.Und.ActiveFuture.OptPriceProfile
                    aUnd.ActiveFuture.FutRootID = aTrd.Und.ActiveFuture.FutRootID
                    aUnd.ActiveFuture.FutRootSymbol = aTrd.Und.ActiveFuture.Symbol
                    aUnd.ActiveFuture.UndID = aTrd.Und.ID
                    aUnd.ActiveFuture.CalcGreeks = True
                    Set aUnd.ActiveFuture.price = Nothing
                    Set aUnd.ActiveFuture.price = m_Und.GetPricePropertyFut(aTrd.Und.Symbol, aUnd.ActiveFuture.Symbol)
                    If aUnd.ActiveFuture.price Is Nothing Then
                        Set aUnd.ActiveFuture.price = New EtsMmReportsLib.MMRpPrice
                    End If
            
                    aUnd.ActiveFuture.price.Bid = BAD_DOUBLE_VALUE
                    aUnd.ActiveFuture.price.Ask = BAD_DOUBLE_VALUE
                    aUnd.ActiveFuture.price.Last = BAD_DOUBLE_VALUE
                    aUnd.ActiveFuture.price.Close = aTrd.Und.ActiveFuture.PriceClose
                    aUnd.ActiveFuture.price.TheoClose = aTrd.Und.ActiveFuture.PriceTheoclose
                    aUnd.ActiveFuture.ContractName = aTrd.Und.ActiveFuture.ContractName
                    aUnd.ActiveFuture.OptLotSize = aTrd.Und.FutRoots(aUnd.ActiveFuture.FutRootID).OptLotSize
                    aUnd.ActiveFuture.FutLotSize = aTrd.Und.FutRoots(aUnd.ActiveFuture.FutRootID).FutLotSize
                    sKey = CStr(enCtFuture) & "_" & aUnd.ActiveFuture.Symbol
                    If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aUnd.ActiveFuture
                End If
        
                aUnd.IsAmerican = aTrd.Und.IsAmerican
                aUnd.IsHTB = aTrd.Und.IsHTB
                aUnd.Skew = aTrd.Und.Skew
                aUnd.Kurt = aTrd.Und.Kurt
                aUnd.Prev10TrdAvgVol = aTrd.Und.Volume10Day
                aUnd.Prev5ExpAvgVol = aTrd.Und.Volume5Expiration
                aUnd.HasSynthetic = aTrd.Und.HaveSyntheticRoots
                Set aUnd.SyntheticRoots = aTrd.Und.SyntheticRoots
                Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
                Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile

                aUnd.price.Close = aTrd.Und.PriceClose
                aUnd.price.TheoClose = aTrd.Und.PriceTheoclose
                aUnd.LotSize = aTrd.Und.LotSize

                gDBW.usp_DataHistoryDH_Get aUnd.ID, aUnd.UndType, fIV, fHV, fVolOfVol, fVega, dtUpdateDate
                If fHV > BAD_DOUBLE_VALUE Then aUnd.HV = fHV

                InitVola aUnd

                sKey = GetColKeyFromUnd(aUnd)
                If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aUnd, Nothing
    Else
            nKey = aTrd.ContractID
            Set aOpt = aUnd.Opt(nKey)
            
            If Not aOpt Is Nothing And aTrd.ContractType = enCtOption Then
                If Not aTrd.Trader Is Nothing Then
                    'aOpt.Account = aTrd.Trader.Name
                End If
                        
                If Not aTrd.Strategy Is Nothing Then
                    'aOpt.Group = aTrd.Strategy.Name
                End If
            End If
    End If
        

        'DoEvents
    If Not m_bDataLoad Then
        AddTrade2Report = False
        Exit Function
    End If

    aUnd.Visible = True
                
    If aTrd.ContractType = enCtFuture Then
        
        Set aFut = aUnd.Fut.Item(aTrd.FutID)
        If Not aTrd.Fut Is Nothing Then
            
            If aFut Is Nothing And Not aTrd.Fut Is Nothing And aTrd.Fut.MaturityDate >= Now Then
                Set aFut = New EtsMmReportsLib.MmRpFutAtom
                'init future
                aFut.ID = aTrd.FutID
                aFut.Symbol = aTrd.Symbol
                
                aFut.Maturity = aTrd.Fut.MaturityDate
                aFut.IsAmerican = aTrd.Fut.IsAmerican
                Set aFut.UndPriceProfile = aTrd.Fut.UndPriceProfile
                Set aFut.OptPriceProfile = aTrd.Fut.OptPriceProfile
                aFut.FutRootID = aTrd.Fut.FutRootID
                aFut.FutRootSymbol = aTrd.FutRoot.Symbol
                aFut.UndID = aTrd.UndID
                aFut.CalcGreeks = True
                Set aFut.price = Nothing
                Set aFut.price = m_Und.GetPricePropertyFut(aTrd.Und.Symbol, aFut.Symbol)
                
                If aFut.price Is Nothing Then
                    Set aFut.price = New EtsMmReportsLib.MMRpPrice
                End If
                
                aFut.price.Bid = BAD_DOUBLE_VALUE
                aFut.price.Ask = BAD_DOUBLE_VALUE
                aFut.price.Last = BAD_DOUBLE_VALUE
                aFut.price.Close = aTrd.Fut.PriceClose
                aFut.price.TheoClose = aTrd.Fut.PriceTheoclose
                aFut.ContractName = aTrd.ContractName
                aFut.OptLotSize = aTrd.FutRoot.OptLotSize
                aFut.FutLotSize = aTrd.FutRoot.FutLotSize
                aFut.Ratio = aTrd.Fut.ActiveFutureRatio
                If bTradeFake = False Then aFut.Pos.Add aTrd
                If Not aUnd.ActiveFuture Is Nothing Then Set aFut.ActiveFuture = aUnd.ActiveFuture
                aUnd.Fut.Add aFut.ID, aFut
                
                sKey = CStr(enCtFuture) & "_" & aFut.Symbol
                If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aFut
                
                Set aFut = Nothing
                'aFut.ReplacePriceStatus
                'aFut.NetChange
                'aFut.FutureBasis
                'aFut.ActiveFuturePrice
            End If
        End If
    ElseIf bTradeFake = False And aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtFutOption Then
                    nKey = aTrd.ContractID
                    Set aOpt = aUnd.Opt(nKey)

                    If aOpt Is Nothing Then
                        Set aOpt = aUnd.Opt.Add(nKey)
                        aOpt.ID = aTrd.ContractID
                        If Not aTrd.Opt Is Nothing Then
                            aOpt.Expiry = aTrd.Opt.Expiry
                            aOpt.ExpiryOV = aTrd.Opt.ExpiryOV
                            aOpt.TradingClose = aTrd.Opt.TradingClose
                        Else
                            aOpt.Expiry = aTrd.FutOpt.Expiry
                            aOpt.ExpiryOV = aTrd.FutOpt.ExpiryOV
                            aOpt.TradingClose = aTrd.FutOpt.TradingClose
                        End If
                        aOpt.LotSize = aTrd.LotSize
                        

'                        If Not aTrd.Trader Is Nothing Then
'                            aOpt.Account = aTrd.Trader.Name
'                        End If
                        
                        If Not aTrd.Strategy Is Nothing Then
                            aOpt.Group = aTrd.Strategy.Name
                        End If
                            
                        If Not aTrd.Opt Is Nothing Then
                            aOpt.OptType = aTrd.Opt.OptType
                            aOpt.Strike = aTrd.Opt.Strike
                            aOpt.Symbol = aTrd.Opt.Symbol
                
                            Set aOpt.price = Nothing
                            Set aOpt.price = m_Und.GetPricePropertyOpt(aTrd.Und.Symbol, aOpt.Symbol)
                            If aOpt.price Is Nothing Then
                                Set aOpt.price = New EtsMmReportsLib.MMRpPrice
                            End If
                
                            aOpt.price.Close = aTrd.Opt.PriceClose
                            aOpt.price.TheoClose = aTrd.Opt.PriceTheoclose
                            aOpt.UnderlyingName = aUnd.Symbol
                            aOpt.PriceTheoclose = aTrd.Opt.PriceTheoclose
                            aOpt.RootID = aTrd.OptRootID
                       Else
                            aOpt.OptType = aTrd.FutOpt.OptType
                            aOpt.Strike = aTrd.FutOpt.Strike
                            aOpt.Symbol = aTrd.FutOpt.Symbol
                            Dim aFuture As EtsGeneralLib.EtsFutAtom
                    
                            Set aFuture = aTrd.FutRoot.Futures(aTrd.FutID)
                            If Not aFuture Is Nothing Then
                                 aOpt.UnderlyingName = aFuture.Symbol
                            End If
                    
                            Set aOpt.price = Nothing
                            Set aOpt.price = m_Und.GetPricePropertyOpt(aTrd.Und.Symbol, aOpt.Symbol)
                            If aOpt.price Is Nothing Then
                                  Set aOpt.price = New EtsMmReportsLib.MMRpPrice
                            End If
                            aOpt.price.Close = aTrd.FutOpt.PriceClose
                            aOpt.PriceTheoclose = aTrd.FutOpt.PriceTheoclose
                            aOpt.RootID = aTrd.Fut.FutRootID
                    
                            ' setup future reference for future option
                            Set aFut = aUnd.Fut.Item(aTrd.Fut.ID)
                            If aFut Is Nothing Then
                               ' future for this option not yet loaded
                               Set aFut = New EtsMmReportsLib.MmRpFutAtom
                            'init future
                            aFut.ID = aTrd.FutID
                            aFut.Symbol = aTrd.Fut.Symbol
                            aFut.Maturity = aTrd.Fut.MaturityDate
                            aFut.IsAmerican = aTrd.Fut.IsAmerican
                            Set aFut.UndPriceProfile = aTrd.Fut.UndPriceProfile
                            Set aFut.OptPriceProfile = aTrd.Fut.OptPriceProfile
                            aFut.FutRootID = aTrd.Fut.FutRootID
                            aFut.FutRootSymbol = aTrd.FutRoot.Symbol
                            aFut.UndID = aTrd.UndID
                            aFut.CalcGreeks = True
                            aFut.Ratio = aTrd.Fut.ActiveFutureRatio
                            aFut.FutureBasis = aTrd.Fut.FutureBasis
                            Set aFut.price = Nothing
                            Set aFut.price = m_Und.GetPricePropertyFut(aTrd.Und.Symbol, aFut.Symbol)

                            If aFut.price Is Nothing Then
                                Set aFut.price = New EtsMmReportsLib.MMRpPrice
                            End If
                        
                            aFut.price.Bid = BAD_DOUBLE_VALUE
                            aFut.price.Ask = BAD_DOUBLE_VALUE
                            aFut.price.Last = BAD_DOUBLE_VALUE
                            aFut.price.Close = aTrd.Fut.PriceClose
                            aFut.price.TheoClose = aTrd.Fut.PriceTheoclose
                            aFut.OptLotSize = aTrd.FutRoot.OptLotSize
                            aFut.FutLotSize = aTrd.FutRoot.FutLotSize
                            If Not aUnd.ActiveFuture Is Nothing Then Set aFut.ActiveFuture = aUnd.ActiveFuture
                            aUnd.Fut.Add aFut.ID, aFut
                            
                            sKey = CStr(enCtFuture) & "_" & aFut.Symbol
                            If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aFut


'                            If Not aTrd.Trader Is Nothing Then
'                               aOpt.Account = aTrd.Trader.Name
'                            End If

                            If Not aTrd.Strategy Is Nothing Then
                               aOpt.Group = aTrd.Strategy.Name
                            End If
                    End If
                    Set aOpt.Fut = aFut
                    Set aFut = Nothing
               End If
                        aOpt.ContractType = aTrd.ContractType
                        'If Not aTrd.Opt Is Nothing Then
                        If aOpt.ContractType = enCtFutOption Then
                                aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aOpt.ExpiryOV)
                        Else
                            If aTrd.Opt.Flex = 1 Then
                                aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aOpt.ExpiryOV)
                            Else
                                aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aOpt.ExpiryOV)
                            End If
                        End If
                        'aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aUnd.ExpCalendarID, aOpt.Expiry)

                        If aUnd.HasSynthetic And Not aUnd.SyntheticRoots Is Nothing Then
                            ' load underlyings from synthetic roots
                            Set aSynthRoot = aUnd.SyntheticRoots(aOpt.RootID)

                            If Not aSynthRoot Is Nothing Then
                                For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                                    If aSynthRootComp.UndID <> aUnd.ID Then

                                        Set aSynthUnd = m_Und(aSynthRootComp.UndID)
                                        Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)

                                        If Not aGUnd Is Nothing Then

                                            If aSynthUnd Is Nothing Then
                                                Set aSynthUnd = m_Und.Add(aSynthRootComp.UndID)
                                                aSynthUnd.ID = aSynthRootComp.UndID
                                                
'                                                If Not aTrd.Trader Is Nothing Then
'                                                    aSynthUnd.Account = aTrd.Trader.Name
'                                                End If

                                                aSynthUnd.UndType = aGUnd.UndType
                                                aSynthUnd.Symbol = aGUnd.Symbol

                                                If aSynthUnd.UndType = enCtIndex Then
                                                    Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
                                                    Set aSynthUnd.Dividend = aGUnd.Dividend
                                                    aSynthUnd.Yield = aGUnd.Yield
                                                End If

                                                If aSynthUnd.UndType = enCtStock Then
                                                    Set aSynthUnd.Dividend = aGUnd.Dividend
                                                End If

                                                Set aSynthUnd.price = Nothing
                                                Set aSynthUnd.price = m_Und.GetPriceProperty(aSynthUnd.Symbol)
                                                If aSynthUnd.price Is Nothing Then
                                                    Set aSynthUnd.price = New EtsMmReportsLib.MMRpPrice
                                                End If

                                                aSynthUnd.price.Close = aGUnd.PriceClose
                                                aSynthUnd.price.TheoClose = aGUnd.PriceTheoclose
                                                aSynthUnd.LotSize = aGUnd.LotSize

                                                aSynthUnd.IsAmerican = aGUnd.IsAmerican
                                                aSynthUnd.IsHTB = aGUnd.IsHTB
                                                aSynthUnd.Skew = aGUnd.Skew
                                                aSynthUnd.Kurt = aGUnd.Kurt
                                                aSynthUnd.Prev10TrdAvgVol = aGUnd.Volume10Day
                                                aSynthUnd.Prev5ExpAvgVol = aGUnd.Volume5Expiration
                                                aSynthUnd.HasSynthetic = aGUnd.HaveSyntheticRoots
                                                Set aSynthUnd.SyntheticRoots = aGUnd.SyntheticRoots
                                                Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
                                                Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile

                                                gDBW.usp_DataHistoryDH_Get aSynthUnd.ID, aUnd.UndType, fIV, fHV, fVolOfVol, fVega, dtUpdateDate
                                                If fHV > BAD_DOUBLE_VALUE Then aSynthUnd.HV = fHV

                                                InitVola aSynthUnd

                                                sKey = GetColKeyFromUnd(aSynthUnd)
                                                If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aSynthUnd, Nothing
                                            End If
                                        End If

                                        Set aSynthUnd = Nothing
                                    End If

                                    Set aGUnd = Nothing
                                Next
                                Set aSynthRoot = Nothing
                            End If
                        End If

                        sKey = GetColKeyFromOpt(aOpt)
                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, aOpt
                    End If

                    aOpt.Pos.Add aTrd
                    Set aOpt = Nothing
                Else
                If bTradeFake = False Then
                    aUnd.Pos.Add aTrd
                End If
        End If

        DoEvents
        If Not m_bDataLoad Then
            AddTrade2Report = False
            Exit Function
        End If

        IncProgress pbProgress

        Set aTrd = Nothing
        AddTrade2Report = True
End Function


Private Function PositionsLoad() As Boolean
    On Error GoTo EH
    PositionsLoad = False
    If m_bInProc Then Exit Function
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aUnd As EtsMmReportsLib.MmRpUndAtom, aOpt As EtsMmReportsLib.MmRpOptAtom
    Dim aOptRoot As EtsMmReportsLib.MmRpOptRootAtom
    Dim nKey&, sKey$, nCount&, aGUnd As EtsGeneralLib.UndAtom
    Dim fIV#, fHV#, fVolOfVol#, fVega#, dtUpdateDate As Date
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom, aSynthUnd As EtsMmReportsLib.MmRpUndAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState

    If m_bDataLoad Then
        nCount = g_TradeChannel.Trades.Count

        For Each aTrd In g_TradeChannel.Trades
                If CheckTradeFilter(aTrd) Then
                    If AddTrade2Report(aTrd, False) = False Then
                        Exit For
                    End If
                End If
        Next
        'For Each aTrd In g_TradeChannel.Trades
        '   If CheckTradeFilter(aTrd) Then
        '      nKey = aTrd.UndID
        '       Set aUnd = m_Und(nKey)
        '        If aUnd Is Nothing Then
        '            Set aUnd = m_Und.Add(nKey)
        '            aUnd.ID = aTrd.Und.ID
        '            aUnd.UndType = aTrd.Und.UndType
        '           aUnd.Symbol = aTrd.Und.Symbol
        '            aUnd.Trader = aTrd.Trader.Name
        '            If Not aTrd.Strategy Is Nothing Then
        '                aUnd.Strategy = aTrd.Strategy.Name
        '            End If
        '
        '            If aUnd.UndType = enCtStock Then
        '                Set aUnd.Dividend = aTrd.Und.Dividend
        '            Else
        '                Set aUnd.BasketIndex = g_Index(aUnd.ID)
        '                If Not aUnd.BasketIndex Is Nothing Then
        '                    If Not aUnd.BasketIndex.IsBasket Then Set aUnd.BasketIndex = Nothing
        '                End If
        '
        '               aUnd.Yield = aTrd.Und.Yield
        '            End If
        '
        '            aUnd.IsAmerican = aTrd.Und.IsAmerican
        '            aUnd.IsHTB = aTrd.Und.IsHTB
        '            aUnd.Skew = aTrd.Und.Skew
        '            aUnd.Kurt = aTrd.Und.Kurt
        '            aUnd.Prev10TrdAvgVol = aTrd.Und.Volume10Day
        '            aUnd.Prev5ExpAvgVol = aTrd.Und.Volume5Expiration
        '            aUnd.HasSynthetic = aTrd.Und.HaveSyntheticRoots
        '            aUnd.ExpCalendarID = aTrd.Und.ExpCalendarID
        '            Set aUnd.SyntheticRoots = aTrd.Und.SyntheticRoots
        '            Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
        '            Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
        '
        '            aUnd.PriceClose = aTrd.Und.PriceClose
        '            aUnd.LotSize = aTrd.Und.LotSize
        '
        '            gDBW.usp_DataHistoryDH_Get aUnd.ID, aUnd.UndType, fIV, fHV, fVolOfVol, fVega, dtUpdateDate
        '            If fHV > BAD_DOUBLE_VALUE Then aUnd.HV = fHV
        '
        '            InitVola aUnd
        '
        '            sKey = GetColKeyFromUnd(aUnd)
        '            If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aUnd, Nothing
        '        End If
        '
        '        DoEvents
        '        If Not m_bDataLoad Then Exit For
        '
        '        aUnd.Visible = True
                
        '        If aTrd.ContractType = enCtOption Then
        '            nKey = aTrd.ContractID
        '            Set aOpt = aUnd.Opt(nKey)

        '            If aOpt Is Nothing Then
        '                Set aOpt = aUnd.Opt.Add(nKey)
        '                aOpt.ID = aTrd.ContractID
        '                aOpt.Expiry = aTrd.Opt.Expiry
        '                aOpt.LotSize = aTrd.LotSize
        '                aOpt.OptType = aTrd.Opt.OptType
        '                aOpt.Strike = aTrd.Opt.Strike
        '                aOpt.Symbol = aTrd.Opt.Symbol
        '                aOpt.PriceClose = aTrd.Opt.PriceClose
        '                aOpt.RootID = aTrd.OptRootID
        '                aOpt.PriceTheoClose = aTrd.Opt.PriceTheoClose

        '                aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(aUnd.ExpCalendarID, aOpt.Expiry)

        '                If aUnd.HasSynthetic And Not aUnd.SyntheticRoots Is Nothing Then
                            ' load underlyings from synthetic roots
        '                   Set aSynthRoot = aUnd.SyntheticRoots(aOpt.RootID)

        '                    If Not aSynthRoot Is Nothing Then
        '                        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
        '                            If aSynthRootComp.UndID <> aUnd.ID Then
     
        '                                Set aSynthUnd = m_Und(aSynthRootComp.UndID)
        '                                Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)

        '                                If Not aGUnd Is Nothing Then

        '                                   If aSynthUnd Is Nothing Then

        '                                        Set aSynthUnd = m_Und.Add(aSynthRootComp.UndID)
        '                                        aSynthUnd.ID = aSynthRootComp.UndID

        '                                        aSynthUnd.UndType = aGUnd.UndType
        '                                        aSynthUnd.Symbol = aGUnd.Symbol

        '                                       If aSynthUnd.UndType = enCtIndex Then
        '                                            Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
        '                                            If Not aSynthUnd.BasketIndex Is Nothing Then
        '                                                If Not aSynthUnd.BasketIndex.IsBasket Then Set aSynthUnd.BasketIndex = Nothing
        '                                            End If
        '                                            aSynthUnd.Yield = aGUnd.Yield
        '                                        End If

        '                                        If aSynthUnd.UndType = enCtStock Then
        '                                           Set aSynthUnd.Dividend = aGUnd.Dividend
        '                                        End If

        '                                        aSynthUnd.PriceClose = aGUnd.PriceClose
        '                                        aSynthUnd.LotSize = aGUnd.LotSize

        '                                        aSynthUnd.IsAmerican = aGUnd.IsAmerican
        '                                        aSynthUnd.IsHTB = aGUnd.IsHTB
        '                                        aSynthUnd.Skew = aGUnd.Skew
        '                                        aSynthUnd.Kurt = aGUnd.Kurt
        '                                        aSynthUnd.Prev10TrdAvgVol = aGUnd.Volume10Day
        '                                        aSynthUnd.Prev5ExpAvgVol = aGUnd.Volume5Expiration
        '                                        aSynthUnd.HasSynthetic = aGUnd.HaveSyntheticRoots
        '                                        aSynthUnd.ExpCalendarID = aGUnd.ExpCalendarID
        '                                        Set aSynthUnd.SyntheticRoots = aGUnd.SyntheticRoots
        '                                        Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
        '                                        Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile

        '                                        gDBW.usp_DataHistoryDH_Get aSynthUnd.ID, aUnd.UndType, fIV, fHV, fVolOfVol, fVega, dtUpdateDate
        '                                        If fHV > BAD_DOUBLE_VALUE Then aSynthUnd.HV = fHV

        '                                        InitVola aSynthUnd

        '                                        sKey = GetColKeyFromUnd(aSynthUnd)
        '                                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, aSynthUnd, Nothing
        '                                    End If
        '                                End If

        '                                Set aSynthUnd = Nothing
        '                            End If

        '                            Set aGUnd = Nothing
        '                        Next
        '                        Set aSynthRoot = Nothing
        '                    End If
        '                End If

        '                sKey = GetColKeyFromOpt(aOpt)
        '                If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, aOpt
        '            End If

        '            aOpt.Pos.Add aTrd
        '            Set aOpt = Nothing
        '        Else
        '            aUnd.Pos.Add aTrd
        '        End If

        '        DoEvents
        '        If Not m_bDataLoad Then Exit For

        '        IncProgress pbProgress

        '        Set aTrd = Nothing
        '    End If
       ' Next

        PositionsLoad = m_bDataLoad
    End If

    m_bInProc = False
Ex:
    On Error Resume Next
    m_bDataLoad = False
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load positions."
    GoTo Ex
End Function

Private Function OptionsLoad() As Boolean
    On Error GoTo EH
    Dim aUnd As EtsMmReportsLib.MmRpUndAtom, nID&, nIsCall
    Dim rsOpt As ADODB.Recordset, nRootID&, dStrike#, dtExpire As Date
    Dim aPosWithEarlyExercise As EtsMmReportsLib.MmRpPosWithEarlyExerciseAtom
    Dim aPosWithEarlyExerciseOpt As EtsMmReportsLib.MmRpPosWithEarlyExerciseOptAtom
    Dim aMatrixByStock As EtsMmReportsLib.MmRpMatrixByStockAtom
    Dim aMatrixByStockOpt As EtsMmReportsLib.MmRpMatrixByStockOptAtom
    Dim aSynthetic As EtsMmReportsLib.MmRpSyntheticAtom
    Dim aSyntheticOpt As EtsMmReportsLib.MmRpSyntheticOptAtom
    Dim dtExpiry, dtExpiryOV, dtTradingClose As Date
    
    If m_Grp.Report <> REPORT_POSITION_MATRIX_BY_STOCK And _
        m_Grp.Report <> REPORT_SYNTHETIC And _
        m_Grp.Report <> REPORT_POSITION_WITH_EARLY_EXERCISE Or m_Und.Count <= 0 Then
        
        OptionsLoad = True
        Exit Function
    End If
    
    OptionsLoad = False
    If m_bInProc Then Exit Function
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState

    pbProgress.Max = m_Und.Count
    pbProgress.Value = 0
    pbProgress.Visible = True
    lblProcess.Caption = "Options loading..."
    
    Dim iRepType As Integer
    iRepType = fgFlt.TextMatrix(1, PFC_REPORT)
    Dim bReqAll As Boolean
    bReqAll = True
    If iRepType = 2 And g_Params.EAOptWithPosition Then bReqAll = False
    
    
    If m_bDataLoad Then
        For Each aUnd In m_Und
            If Not aUnd.Visible Then GoTo NEXT_UND
            
            Set rsOpt = gDBW.usp_ReportsUnderlyingOptions_Get(aUnd.ID, Date)
            
            Select Case m_Grp.Report
            Case REPORT_POSITION_WITH_EARLY_EXERCISE
                If (m_PosWithEarlyExercise(aUnd.ID) Is Nothing) Then
                    Set aPosWithEarlyExercise = m_PosWithEarlyExercise.Add(aUnd.ID, aUnd.Symbol)
                End If
                If Not aPosWithEarlyExercise Is Nothing Then
                        
                aPosWithEarlyExercise.ID = aUnd.ID
                aPosWithEarlyExercise.Symbol = aUnd.Symbol
                aPosWithEarlyExercise.UndType = aUnd.UndType
                
                Dim bUnd As EtsMmReportsLib.MmRpUndAtom
                                
                Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
                Dim nStrategyID As Long
    
                nStrategyID = m_nFilter(PFC_STRATEGY)
                If (nStrategyID = 0) Then
                    nStrategyID = NO_STRATEGY_ID
                    For Each aStrategy In g_Strategy
                        If (aStrategy.Name = aUnd.Strategy) Then
                            nStrategyID = aStrategy.ID
                            Exit For
                        End If
                    Next
                End If
                
                Set bUnd = m_Und(GetUnderlyingKeyByStrategy(aUnd.ID, nStrategyID))
                Dim trOptColl As EtsMmReportsLib.MmRpOptColl
                Set trOptColl = New EtsMmReportsLib.MmRpOptColl
                Dim aOpt As EtsMmReportsLib.MmRpOptAtom
                Dim bOpt As EtsMmReportsLib.MmRpOptAtom
                
                If Not bUnd Is Nothing Then
                    For Each aOpt In bUnd.Opt
                        Set bOpt = trOptColl.Add(aOpt.ID, aOpt)
                            If bOpt.price Is Nothing Then
                                Set bOpt.price = m_Und.GetPricePropertyOpt(aUnd.Symbol, bOpt.Symbol)
                                If bOpt.price Is Nothing Then
                                    Set bOpt.price = New EtsMmReportsLib.MMRpPrice
                                End If
                            End If
                    Next
                End If
                
                If Not aPosWithEarlyExercise Is Nothing Then
                    While Not rsOpt.EOF
                        Dim OptionType As EtsGeneralLib.EtsContractTypeEnum, futureID&
                            Dim aFuture As EtsMmReportsLib.MmRpFutAtom
                            OptionType = IIf(ReadLng(rsOpt!IsFutureOption) > 0, enCtFutOption, enCtOption)
                            If Not IsNull(rsOpt!iFutureID.Value) Then
                                futureID& = ReadLng(rsOpt!iFutureID)
                                
                                Set aFuture = bUnd.Fut(futureID)
                                If aFuture Is Nothing Then
                                    Dim aFut As EtsGeneralLib.EtsFutAtom
                                    Set aFut = g_ContractAll(futureID).Fut
                                    If aFut Is Nothing Then GoTo Continue
                                    Set aFuture = New EtsMmReportsLib.MmRpFutAtom
                                    aFuture.ID = aFut.ID
                                    aFuture.Symbol = aFut.Symbol
                                    aFuture.Maturity = aFut.MaturityDate
                                    aFuture.IsAmerican = aFut.IsAmerican
                                    Set aFuture.UndPriceProfile = aFut.UndPriceProfile
                                    Set aFuture.OptPriceProfile = aFut.OptPriceProfile
                                    aFuture.FutRootID = aFut.FutRootID
                                    aFuture.FutRootSymbol = g_ContractAll(futureID).FutRoot.Symbol
                                    aFuture.UndID = bUnd.ID
                                    aFuture.CalcGreeks = True
                                    aFuture.Ratio = aFut.ActiveFutureRatio
                                    Set aFuture.price = Nothing
                                    Set aFuture.price = m_Und.GetPricePropertyFut(bUnd.Symbol, aFuture.Symbol)
                                    If aFuture.price Is Nothing Then
                                        Set aFuture.price = New EtsMmReportsLib.MMRpPrice
                                    End If
                                            
                                    aFuture.price.Bid = BAD_DOUBLE_VALUE
                                    aFuture.price.Ask = BAD_DOUBLE_VALUE
                                    aFuture.price.Last = BAD_DOUBLE_VALUE
                                    aFuture.price.Close = g_ContractAll(futureID).Fut.PriceClose
                                    aFuture.price.TheoClose = g_ContractAll(futureID).Fut.PriceTheoclose
                                    aFuture.OptLotSize = g_ContractAll(futureID).FutRoot.OptLotSize
                                    aFuture.FutLotSize = g_ContractAll(futureID).FutRoot.FutLotSize
                                    If Not bUnd.ActiveFuture Is Nothing Then Set aFuture.ActiveFuture = bUnd.ActiveFuture
                                    bUnd.Fut.Add aFuture.ID, aFuture
                                    Dim sKey$
                                    sKey = CStr(enCtFuture) & "_" & aFuture.Symbol
                                    If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, Nothing, aFuture
                                End If
                            End If
                        dtExpiry = ReadDate(rsOpt!dtExpiry)
                        dtExpiryOV = ReadDate(rsOpt!dtExpiryOV)
                        dtTradingClose = ClipDays(ReadDate(rsOpt!dtTradingClose))
                        nRootID = ReadLng(rsOpt!iOptionRootID)
                        dStrike = ReadDbl(rsOpt!fStrike)
                        If aPosWithEarlyExercise.Opt(nRootID, dStrike, dtExpiry) Is Nothing Then

                            Set aPosWithEarlyExerciseOpt = aPosWithEarlyExercise.Opt.Add(nRootID, dStrike, dtExpiry)
                            With aPosWithEarlyExerciseOpt
                                .RootID = nRootID
                                .StockID = aUnd.ID
                                .Strike = dStrike
                                .Expiry = dtExpiry
                                .RootSymbol = GetOptionRoot(ReadStr(rsOpt!vcSymbol))
                            End With
                            Set aPosWithEarlyExerciseOpt = Nothing
                        End If
                        If Not bUnd Is Nothing Then
                            nID = ReadLng(rsOpt!iOptionID)
                            Set aOpt = aUnd.Opt(nID)
                            If aOpt Is Nothing Then
                                Set aOpt = bUnd.Opt.Add(nID)
                                aOpt.ID = nID
                                aOpt.Expiry = dtExpiry
                                aOpt.ExpiryOV = dtExpiryOV
                                aOpt.TradingClose = dtTradingClose
                                'aOpt.LotSize = aTrd.LotSize
                                aOpt.OptType = ReadLng(rsOpt!tiIsCall) 'aTrd.Opt.OptType
                                aOpt.Strike = dStrike
                                aOpt.Symbol = ReadStr(rsOpt!vcSymbol)
                                'aOpt.PriceClose = aTrd.Opt.PriceClose
                                aOpt.RootID = ReadLng(rsOpt!iOptionRootID)
                                'aOpt.PriceTheoClose = aTrd.Opt.PriceTheoClose
                                aOpt.ContractType = OptionType
                                Set aOpt.Fut = aFuture
                                Set aFuture = Nothing
                                aOpt.VegaWeight = g_ExpCalendar.GetVegaWeight(dtExpiryOV)
                                Set aOpt.price = Nothing
                                Set aOpt.price = m_Und.GetPricePropertyOpt(aUnd.Symbol, aOpt.Symbol)
                                
                                If aOpt.price Is Nothing Then
                                        Set aOpt.price = New EtsMmReportsLib.MMRpPrice
                                End If
                                sKey = GetColKeyFromOpt(aOpt)
                                If bReqAll Then
                                    If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add sKey, Nothing, aOpt
                                End If
                                If aOpt.ContractType = enCtFutOption Then
                                    sKey = GetColKeyFromOpt(aOpt)
                                End If
                            End If
                        End If
Continue:
                        rsOpt.MoveNext
                        DoEvents
                    Wend
                End If
                End If
                Set aPosWithEarlyExercise = Nothing
               
            Case REPORT_POSITION_MATRIX_BY_STOCK
                Set aMatrixByStock = m_MatrixByStock.Add(aUnd.ID, aUnd.Symbol)
                If Not aMatrixByStock Is Nothing Then
                aMatrixByStock.ID = aUnd.ID
                aMatrixByStock.Symbol = aUnd.Symbol
                aMatrixByStock.UndType = aUnd.UndType
                
                If Not aMatrixByStock Is Nothing Then
                    While Not rsOpt.EOF
                        dtExpiry = ReadDate(rsOpt!dtExpiry)
                        nID = ReadLng(rsOpt!iOptionID)
                        If aMatrixByStock.Opt(nID) Is Nothing Then

                            Set aMatrixByStockOpt = aMatrixByStock.Opt.Add(nID)
                            With aMatrixByStockOpt
                                .RootID = ReadLng(rsOpt!iOptionRootID)
                                .StockID = aUnd.ID
                                .Strike = ReadDbl(rsOpt!fStrike)
                                .Expiry = dtExpiry
                                .ID = nID
                                .OptType = IIf(ReadLng(rsOpt!tiIsCall) > 0, enOtCall, enOtPut)
                                .Symbol = ReadStr(rsOpt!vcSymbol)
                                .RootSymbol = GetOptionRoot(.Symbol)
                            End With
                            Set aMatrixByStockOpt = Nothing
                        End If
                        rsOpt.MoveNext
                        DoEvents
                    Wend
                End If
                End If
                Set aMatrixByStock = Nothing
            Case REPORT_SYNTHETIC
                Set aSynthetic = m_Synthetic.Add(aUnd.ID, aUnd.ID, aUnd.Symbol)
                If Not aSynthetic Is Nothing Then
                aSynthetic.ID = aUnd.ID
                aSynthetic.Symbol = aUnd.Symbol
                aSynthetic.UndType = aUnd.UndType
                
                If Not aSynthetic Is Nothing Then
                    While Not rsOpt.EOF
                        dtExpiry = ReadDate(rsOpt!dtExpiry)
                        nID = ReadLng(rsOpt!iOptionID)
                        If aSynthetic.Opt(nID) Is Nothing Then
                            Set aSyntheticOpt = aSynthetic.Opt.Add(nID)
                            With aSyntheticOpt
                                .RootID = ReadLng(rsOpt!iOptionRootID)
                                .StockID = aUnd.ID
                                .Strike = ReadDbl(rsOpt!fStrike)
                                .Expiry = dtExpiry
                                .ID = nID
                                .RootSymbol = GetOptionRoot(ReadStr(rsOpt!vcSymbol))
                            End With
                            Set aMatrixByStockOpt = Nothing
                        End If
                        rsOpt.MoveNext
                        DoEvents
                    Wend
                End If
                End If
                Set aMatrixByStock = Nothing
            End Select
            
            Set rsOpt = Nothing
NEXT_UND:
            Set aUnd = Nothing
            IncProgress pbProgress
            DoEvents
            If Not m_bDataLoad Then Exit For
        Next
        OptionsLoad = m_bDataLoad
    End If

    m_bInProc = False
Ex:
    On Error Resume Next
    m_bDataLoad = False
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load options."
    GoTo Ex
End Function

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_REPORTS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_REPORTS_RESULTS).CopyTo m_gdRes
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .rows = 2
        .Cols = PFC_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarNone
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
    End With
    
    With fgRes
        .rows = 0
        .Cols = 0
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .HighLight = flexHighlightNever
        .FixedCols = 0
        .FixedRows = 0
        .FocusRect = flexFocusSolid
    
        .FontName = "Tahoma"
        .FontBold = False
        .FontItalic = False
        .FontStrikethru = False
        .FontSize = 8
                    
        .BorderStyle = flexBorderFlat
        .GridLinesFixed = flexGridFlat
        .GridColorFixed = RGB(128, 128, 128)
        .BackColorFixed = RGB(224, 224, 224)
                    
        .GridColor = RGB(192, 192, 192)
        
    End With
End Sub

Private Sub InitPrinter()
    On Error Resume Next
    With vsPrinter
        .PhysicalPage = True
        .NavBar = vpnbNone
        .Track = True
        .MarginHeader = .MarginTop - ScaleY(10, vbPoints, vbTwips)
        .MarginFooter = .MarginBottom - ScaleY(10, vbPoints, vbTwips)
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_REPORTS_FILTER).LockRedraw
    
        nCols = PFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = PFC_REPORT To PFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_REPORTS_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .rows - 1
        For i = PFC_REPORT To PFC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).BackColor <> 0, m_gdFlt.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).ForeColor <> 0, m_gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
        Next

        If m_gdFlt.HorizLinesVisible Then
            If m_gdFlt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdFlt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdFlt.GridLinesColor
        
        .BackColorSel = m_gdFlt.BackColorSel
        .ForeColorSel = m_gdFlt.ForeColorSel
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatResGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgRes
        m_GridLock(GT_REPORTS_RESULTS).LockRedraw
    
        .rows = 0
        .Cols = 0
        
        m_GridLock(GT_REPORTS_RESULTS).UnlockRedraw
    End With
End Sub

Private Sub FormatResColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
 
    With fgRes
        m_GridLock(GT_REPORTS_RESULTS).LockRedraw

        aFont.Name = m_gdRes.Font.Name
        aFont.Size = m_gdRes.Font.Size
        aFont.Bold = m_gdRes.Font.Bold
        aFont.Italic = m_gdRes.Font.Italic
        aFont.Strikethrough = m_gdRes.Font.Strikethrough
        aFont.Underline = m_gdRes.Font.Underline

        Set .Font = aFont

        If m_gdRes.HorizLinesVisible Then
            If m_gdRes.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdRes.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdRes.GridLinesColor

        .BackColorSel = m_gdRes.BackColorSel
        .ForeColorSel = m_gdRes.ForeColorSel

        m_GridLock(GT_REPORTS_RESULTS).UnlockRedraw
    End With
End Sub

Private Sub ClearResGrid()
    FormatResGrid
End Sub

Private Sub UpdateResGrid()
    On Error Resume Next
    Dim aGreeksByMonthAtom As EtsMmReportsLib.MmRpGreeksByMonthAtom
    Dim aGreeksByMonthColl As EtsMmReportsLib.MmRpGreeksByMonthColl
    Dim aGreeksSummaryAtom As EtsMmReportsLib.MmRpGreeksSummaryAtom
    Dim aMatrixByStockAtom As EtsMmReportsLib.MmRpMatrixByStockAtom
    Dim aMatrixByStockOptAtom As EtsMmReportsLib.MmRpMatrixByStockOptAtom
    Dim aPnLAtom As EtsMmReportsLib.MmRpPnLAtom
    Dim aSyntheticAtom As EtsMmReportsLib.MmRpSyntheticAtom
    Dim aSyntheticOptAtom As EtsMmReportsLib.MmRpSyntheticOptAtom
    Dim aRiskMatrixAtom As EtsMmReportsLib.MmRpRiskMatrixAtom
    Dim aRiskMatrixShiftAtom As EtsMmReportsLib.MmRpRiskMatrixShiftAtom
    Dim aExercisedStockAtom As EtsMmReportsLib.MmRpExercisedStockAtom
    Dim aPosWithEarlyExerciseAtom As EtsMmReportsLib.MmRpPosWithEarlyExerciseAtom
    Dim aPosWithEarlyExerciseOptAtom As EtsMmReportsLib.MmRpPosWithEarlyExerciseOptAtom
    Dim aColl As New EtsGeneralLib.EtsMmEntityAtomColl, dtExp As Date, aEnt As EtsGeneralLib.EtsMmEntityAtom, i&, nFixedRows&, nFixedCols&
    Dim sKey$, nID&, nHeight, dVal#, sVal$, nCol&, nRow&, iRow&
    Dim dRowDataTmp As Double
    
    
    With vsPrinter
        .Clear
        .StartDoc
        .Header = ""
        .Footer = ""
        nHeight = .PageHeight - .MarginHeader - .MarginFooter
    End With
    
    ClearResGrid
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    lblProcess.Caption = "Formating..."
    
    With fgRes
        Select Case m_nFilter(PFC_REPORT)
        Case REPORT_POSITION_GREEK_BY_MONTH
            nFixedCols = 2
            nFixedRows = 1
        
            .Cols = nFixedCols + 12
            .rows = nFixedRows
        
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .TextMatrix(0, 0) = "Series"
            .TextMatrix(0, 1) = "Symbol"
            .TextMatrix(0, 2) = "cMktVal"
            .TextMatrix(0, 3) = "pMktVal"
            .TextMatrix(0, 4) = "NetMktVal"
            .TextMatrix(0, 5) = "cPoP"
            .TextMatrix(0, 6) = "pPoP"
            .TextMatrix(0, 7) = "NetPoP"
            .TextMatrix(0, 8) = "OptDlt$"
            .TextMatrix(0, 9) = "NetGma$"
            .TextMatrix(0, 10) = "Vga"
            .TextMatrix(0, 11) = "WdtVga"
            .TextMatrix(0, 12) = "Tht"
            .TextMatrix(0, 13) = "Rho"
            
            .ColDataType(0) = flexDTDate
            .ColAlignment(0) = flexAlignLeftTop
            .FixedAlignment(0) = flexAlignCenterCenter
            .ColDataType(1) = flexDTString
            .ColAlignment(1) = flexAlignLeftTop
            .FixedAlignment(1) = flexAlignCenterCenter
            .ColFormat(0) = "MMMYY"
            
            For i = nFixedCols To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
                .ColDataType(i) = flexDTDouble
                .ColFormat(i) = "#,##0.00"
            Next
                
            i = 1
            pbProgress.Max = m_GreeksByMonth.Count
            For Each aGreeksByMonthAtom In m_GreeksByMonth
                    dtExp = GetExpireMonth(aGreeksByMonthAtom.Expiry)
                    sKey = aGreeksByMonthAtom.Symbol & CStr(dtExp)
                    nID = .FindRow(sKey)
                    If nID > -1 Then
                        .TextMatrix(nID, 2) = .ValueMatrix(nID, 2) + aGreeksByMonthAtom.CallMarketValue
                        .TextMatrix(nID, 3) = .ValueMatrix(nID, 3) + aGreeksByMonthAtom.PutMarketValue
                        .TextMatrix(nID, 4) = .ValueMatrix(nID, 4) + aGreeksByMonthAtom.NetMarketValue
                        .TextMatrix(nID, 5) = .ValueMatrix(nID, 5) + aGreeksByMonthAtom.CallPoP
                        .TextMatrix(nID, 6) = .ValueMatrix(nID, 6) + aGreeksByMonthAtom.PutPoP
                        .TextMatrix(nID, 7) = .ValueMatrix(nID, 7) + aGreeksByMonthAtom.NetPoP
                        .TextMatrix(nID, 8) = .ValueMatrix(nID, 8) + aGreeksByMonthAtom.OptionsDelta
                        .TextMatrix(nID, 9) = .ValueMatrix(nID, 9) + aGreeksByMonthAtom.TotalGamma
                        .TextMatrix(nID, 10) = .ValueMatrix(nID, 10) + aGreeksByMonthAtom.TotalVega
                        .TextMatrix(nID, 11) = .ValueMatrix(nID, 11) + aGreeksByMonthAtom.TotalWeightedVega
                        .TextMatrix(nID, 12) = .ValueMatrix(nID, 12) + aGreeksByMonthAtom.TotalTheta
                        .TextMatrix(nID, 13) = .ValueMatrix(nID, 13) + aGreeksByMonthAtom.TotalRho
                    Else
                        .AddItem ""
                        .RowData(i) = sKey
                        .TextMatrix(i, 0) = dtExp
                        .TextMatrix(i, 1) = aGreeksByMonthAtom.Symbol
                        .TextMatrix(i, 2) = aGreeksByMonthAtom.CallMarketValue
                        .TextMatrix(i, 3) = aGreeksByMonthAtom.PutMarketValue
                        .TextMatrix(i, 4) = aGreeksByMonthAtom.NetMarketValue
                        .TextMatrix(i, 5) = aGreeksByMonthAtom.CallPoP
                        .TextMatrix(i, 6) = aGreeksByMonthAtom.PutPoP
                        .TextMatrix(i, 7) = aGreeksByMonthAtom.NetPoP
                        .TextMatrix(i, 8) = aGreeksByMonthAtom.OptionsDelta
                        .TextMatrix(i, 9) = aGreeksByMonthAtom.TotalGamma
                        .TextMatrix(i, 10) = aGreeksByMonthAtom.TotalVega
                        .TextMatrix(i, 11) = aGreeksByMonthAtom.TotalWeightedVega
                        .TextMatrix(i, 12) = aGreeksByMonthAtom.TotalTheta
                        .TextMatrix(i, 13) = aGreeksByMonthAtom.TotalRho
                        i = i + 1
                    End If
                    IncProgress pbProgress
                    Set aGreeksByMonthAtom = Nothing
                    
                    If Not m_bRecalc Then GoTo Ex
            Next
            
            DoEvents
            
            .ColSort(0) = flexSortGenericAscending
            .ColSort(1) = flexSortGenericAscending
            .Select 1, 0, .rows - 1, 1
            .Sort = flexSortUseColSort
                
            .MergeCells = flexMergeRestrictColumns
            .MergeCol(0) = True
            
            .SubtotalPosition = flexSTBelow
            For i = nFixedCols To .Cols - 1
                .Subtotal flexSTSum, 0, i, , , , True, "%s", , True
            Next
                
            For i = nFixedRows To .rows - 1
                If .IsSubtotal(i) Then .TextMatrix(i, 1) = "Total"
            Next
                
            .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
            .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
            .AutoSize 0, .Cols - 1, False, 100
                
            With vsPrinter
                .RenderControl = fgRes.hWnd
            End With
        Case REPORT_POSITION_GREEK_SUMMARY
            nFixedCols = 1
            nFixedRows = 1
            
            .Cols = nFixedCols + 14
            .rows = nFixedRows
                
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .MergeCells = flexMergeNever
            .TextMatrix(0, 0) = "Symbol"
            .TextMatrix(0, 1) = "cMktVal"
            .TextMatrix(0, 2) = "pMktVal"
            .TextMatrix(0, 3) = "NetMktVal"
            .TextMatrix(0, 4) = "cPoP"
            .TextMatrix(0, 5) = "pPoP"
            .TextMatrix(0, 6) = "NetPoP"
            .TextMatrix(0, 7) = "OptDlt$"
            .TextMatrix(0, 8) = "StkPos"
            .TextMatrix(0, 9) = "NetDlt$"
            .TextMatrix(0, 10) = "NetGma$"
            .TextMatrix(0, 11) = "Vga"
            .TextMatrix(0, 12) = "WdtVga"
            .TextMatrix(0, 13) = "Tht"
            .TextMatrix(0, 14) = "Rho"
            
            .ColDataType(0) = flexDTString
            .ColAlignment(0) = flexAlignLeftTop
            .FixedAlignment(0) = flexAlignCenterCenter
            
            For i = nFixedCols To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
                .ColDataType(i) = flexDTDouble
                .ColFormat(i) = "#,##0.00"
            Next
                
            i = 1
            pbProgress.Max = m_GreeksSummary.Count
            For Each aGreeksSummaryAtom In m_GreeksSummary
                    .AddItem ""
                    .TextMatrix(i, 0) = aGreeksSummaryAtom.Symbol
                    .TextMatrix(i, 1) = aGreeksSummaryAtom.CallMarketValue
                    .TextMatrix(i, 2) = aGreeksSummaryAtom.PutMarketValue
                    .TextMatrix(i, 3) = aGreeksSummaryAtom.NetMarketValue
                    .TextMatrix(i, 4) = aGreeksSummaryAtom.CallPoP
                    .TextMatrix(i, 5) = aGreeksSummaryAtom.PutPoP
                    .TextMatrix(i, 6) = aGreeksSummaryAtom.NetPoP
                    .TextMatrix(i, 7) = aGreeksSummaryAtom.OptionsDelta
                    .TextMatrix(i, 8) = aGreeksSummaryAtom.UndPosInShares
                    .TextMatrix(i, 9) = aGreeksSummaryAtom.NetPos
                    .TextMatrix(i, 10) = aGreeksSummaryAtom.TotalGamma
                    .TextMatrix(i, 11) = aGreeksSummaryAtom.TotalVega
                    .TextMatrix(i, 12) = aGreeksSummaryAtom.TotalWeightedVega
                    .TextMatrix(i, 13) = aGreeksSummaryAtom.TotalTheta
                    .TextMatrix(i, 14) = aGreeksSummaryAtom.TotalRho
                    i = i + 1
                    IncProgress pbProgress
                    Set aGreeksSummaryAtom = Nothing
                    
                    If Not m_bRecalc Then GoTo Ex
            Next
            DoEvents
            
            .SubtotalPosition = flexSTBelow
            For i = nFixedCols To .Cols - 1
                .Subtotal flexSTSum, -1, i, , , , True, "Total", , True
            Next
               
            .ColSort(0) = flexSortGenericAscending
            .Select 1, 0, .rows - 1
            .Sort = flexSortUseColSort
                
            .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
            .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
            .AutoSize 0, .Cols - 1, False, 100
                
            With vsPrinter
                .RenderControl = fgRes.hWnd
            End With
        Case REPORT_POSITION_WITH_EARLY_EXERCISE
            pbProgress.Max = m_PosWithEarlyExercise.Count
            For Each aPosWithEarlyExerciseAtom In m_PosWithEarlyExercise
                With vsPrinter
                    If .CurrentY > nHeight Then .NewPage
                    
                    .FontName = "Tahoma"
                    .FontBold = False
                    .FontSize = 8
                    .TextAlign = taLeftTop
                    .Text = "Symbol" & String$(2, vbTab) & aPosWithEarlyExerciseAtom.Symbol & vbCrLf
                    .Text = "Stock Price" & vbTab & Format$(aPosWithEarlyExerciseAtom.price, "#,##0.00") & String$(2, vbTab)
                    .Text = "Stock Position" & vbTab & Format$(aPosWithEarlyExerciseAtom.PosInShares, "#,##0.00") & vbCrLf
                    .Text = String$(4, vbTab) & "Net Delta" & String$(2, vbTab) & aPosWithEarlyExerciseAtom.NetDelta & String$(2, vbCrLf)
                End With
                
                If aPosWithEarlyExerciseAtom.Opt.Count <= 0 Then GoTo NEXT_POSITION_WITH_EARLY_EXERCISE
                
                nFixedCols = 3
                nFixedRows = 1
            
                .Cols = nFixedCols + 4 + 4
                .rows = nFixedRows
                
                .FixedRows = nFixedRows
                .FixedCols = 0
                .FrozenCols = nFixedCols
                .MergeCells = flexMergeRestrictColumns
                .MergeCol(0) = True
                .TextMatrix(0, 0) = "Expiration"
                '.TextMatrix(1, 0) = "ation"
                .TextMatrix(0, 1) = "Root"
                .TextMatrix(0, 2) = "Strike"
                .TextMatrix(0, 3) = "cQty"
                .TextMatrix(0, 4) = "pQty"
                .TextMatrix(0, 5) = "cEarly" ' Exercise"
                '.TextMatrix(1, 5) = "Exercise"
                .TextMatrix(0, 6) = "pEarly" ' Exercise"
                '.TextMatrix(1, 6) = "Exercise"
                .TextMatrix(0, 7) = "Div"
                .TextMatrix(0, 8) = "Interest"
                .TextMatrix(0, 9) = "cOTM"
                .TextMatrix(0, 10) = "pOTM"
            
                .ColDataType(0) = flexDTDate
                .ColAlignment(0) = flexAlignCenterTop
                .ColFormat(0) = "MMMYY"
            
                .ColDataType(1) = flexDTString
                .ColAlignment(1) = flexAlignLeftTop
            
                .ColDataType(2) = flexDTDouble
                .ColDataType(3) = flexDTLong
                .ColDataType(4) = flexDTLong
                .ColDataType(5) = flexDTBoolean
                .ColDataType(6) = flexDTBoolean
                
                .ColDataType(7) = flexDTLong
                .ColDataType(8) = flexDTLong
                .ColDataType(9) = flexDTLong
                .ColDataType(10) = flexDTLong
                
                .ColFormat(7) = "#,##0.00"
                .ColFormat(8) = "#,##0.00"
                .ColFormat(9) = "#,##0.00"
                .ColFormat(10) = "#,##0.00"
                
                .ColFormat(2) = "#,##0.00"
                .ColFormat(5) = "Yes;No"
                .ColFormat(6) = "Yes;No"
            
                .ColAlignment(5) = flexAlignCenterCenter
                .ColAlignment(6) = flexAlignCenterCenter
                
                For i = 0 To .Cols - 1
                    .FixedAlignment(i) = flexAlignCenterCenter
                Next
                
                i = nFixedRows
                
                For Each aPosWithEarlyExerciseOptAtom In aPosWithEarlyExerciseAtom.Opt
                    If g_Params.EAOptWithPosition = False Or aPosWithEarlyExerciseOptAtom.CallPos Or aPosWithEarlyExerciseOptAtom.PutPos Then
                        'If aPosWithEarlyExerciseOptAtom.AreAnyEarlyExerc = True Then
                        '    If g_Params.EAOnlyExerc = False Or aPosWithEarlyExerciseOptAtom.CallEarlyExercise And aPosWithEarlyExerciseOptAtom.CallPos _
                        '    Or aPosWithEarlyExerciseOptAtom.PutEarlyExercise And aPosWithEarlyExerciseOptAtom.PutPos Then
                                .AddItem ""
                                .TextMatrix(i, 0) = GetExpireMonth(aPosWithEarlyExerciseOptAtom.Expiry)
                                .TextMatrix(i, 1) = aPosWithEarlyExerciseOptAtom.RootSymbol
                                .TextMatrix(i, 2) = aPosWithEarlyExerciseOptAtom.Strike
                                .TextMatrix(i, 3) = aPosWithEarlyExerciseOptAtom.CallPos
                                .TextMatrix(i, 4) = aPosWithEarlyExerciseOptAtom.PutPos
                                .TextMatrix(i, 5) = aPosWithEarlyExerciseOptAtom.CallEarlyExercise
                                .TextMatrix(i, 6) = aPosWithEarlyExerciseOptAtom.PutEarlyExercise
                         
                                .TextMatrix(i, 7) = aPosWithEarlyExerciseOptAtom.Dividents
                                .TextMatrix(i, 8) = aPosWithEarlyExerciseOptAtom.Interest
                        
                                If aPosWithEarlyExerciseOptAtom.pOTM >= 0 Then
                                    .TextMatrix(i, 9) = aPosWithEarlyExerciseOptAtom.pOTM
                                End If
                                If aPosWithEarlyExerciseOptAtom.cOTM >= 0 Then
                                    .TextMatrix(i, 10) = aPosWithEarlyExerciseOptAtom.cOTM
                                End If
                                Set aPosWithEarlyExerciseOptAtom = Nothing
                                i = i + 1
                        'End If
                      'End If
                    End If
                Next
                                
                .ColSort(0) = flexSortGenericAscending
                .ColSort(2) = flexSortGenericAscending
                .Select 1, 0, .rows - 1, 2
                .Sort = flexSortUseColSort
                
                .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
                .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
                .AutoSize 0, .Cols - 1, False, 100
                
                vsPrinter.RenderControl = fgRes.hWnd
                
NEXT_POSITION_WITH_EARLY_EXERCISE:
                vsPrinter.Text = String$(2, vbCrLf)
                
                IncProgress pbProgress
                Set aPosWithEarlyExerciseAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
        Case REPORT_POSITION_MATRIX_BY_STOCK
            pbProgress.Max = m_MatrixByStock.Count
            For Each aMatrixByStockAtom In m_MatrixByStock
                If aMatrixByStockAtom.Opt.Count <= 0 Then GoTo NEXT_MATRIX_BY_STOCK
                aColl.Clear
                For Each aMatrixByStockOptAtom In aMatrixByStockAtom.Opt
                    dtExp = GetExpireMonth(aMatrixByStockOptAtom.Expiry)
                    nID = CLng(dtExp)
                    sKey = CStr(nID)
                    If aColl.Item(sKey) Is Nothing Then
                        Set aEnt = aColl.Add(sKey)
                        aEnt.ID = nID
                        aEnt.Name = Format$(dtExp, "YYYY-MM")
                        aEnt.Data3 = dtExp
                        Set aEnt = Nothing
                    End If
                    Set aMatrixByStockOptAtom = Nothing
                Next
                
                nFixedRows = 1
                nFixedCols = 3
                
                .Cols = nFixedCols + aColl.Count
                .rows = nFixedRows
                
                .FixedRows = nFixedRows
                .FixedCols = 0
                .FrozenCols = nFixedCols
                .TextMatrix(0, 0) = "Type"
                .TextMatrix(0, 1) = "Strike"
                .TextMatrix(0, 2) = "Root"
                .MergeCells = flexMergeRestrictColumns
                .MergeCol(0) = True
                .ColDataType(1) = flexDTDouble
                .ColFormat(1) = "#,##0.00"
                .ColAlignment(0) = flexAlignLeftTop
                .FixedAlignment(0) = flexAlignCenterCenter
                .FixedAlignment(1) = flexAlignCenterCenter
                .FixedAlignment(2) = flexAlignCenterCenter
                
                SortEntCollByName aColl
                
                i = nFixedCols
                For Each aEnt In aColl
                    dtExp = aEnt.Data3
                    .TextMatrix(0, i) = Format$(dtExp, "MMMYY")
                    .ColKey(i) = CStr(CLng(dtExp))
                    .ColAlignment(i) = flexAlignCenterCenter
                    .ColDataType(i) = flexDTDouble
                    .ColFormat(i) = "#,##0.00"
                    i = i + 1
                    Set aEnt = Nothing
                Next
                
                i = nFixedRows
                For Each aMatrixByStockOptAtom In aMatrixByStockAtom.Opt
                    sKey = aMatrixByStockOptAtom.RootSymbol & CStr(aMatrixByStockOptAtom.Strike) & "_" & CStr(aMatrixByStockOptAtom.OptType)
                    dtExp = GetExpireMonth(aMatrixByStockOptAtom.Expiry)
                    nID = .FindRow(sKey)
                    If nID > -1 Then
                        .TextMatrix(nID, .ColIndex(CStr(CLng(dtExp)))) = aMatrixByStockOptAtom.Pos
                    Else
                        .AddItem ""
                        .RowData(i) = sKey
                        .Cell(flexcpText, i, nFixedCols, i, .Cols - 1) = 0
                        .TextMatrix(i, 0) = IIf(aMatrixByStockOptAtom.OptType = enOtCall, "CALLS", "PUTS")
                        .TextMatrix(i, 1) = aMatrixByStockOptAtom.Strike
                        .TextMatrix(i, 2) = aMatrixByStockOptAtom.RootSymbol
                        .TextMatrix(i, .ColIndex(CStr(CLng(dtExp)))) = aMatrixByStockOptAtom.Pos
                        i = i + 1
                    End If
                    Set aMatrixByStockOptAtom = Nothing
                Next
                                
                .ColSort(0) = flexSortGenericAscending
                .ColSort(1) = flexSortGenericAscending
                .ColSort(2) = flexSortGenericAscending
                .Select 1, 0, .rows - 1, 2
                .Sort = flexSortUseColSort
                
                .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
                .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
                .AutoSize 0, .Cols - 1, False, 200
                
                With vsPrinter
                    If .CurrentY > nHeight Then .NewPage
                    
                    .FontName = "Tahoma"
                    .FontBold = False
                    .FontSize = 8
                    .TextAlign = taLeftTop
                    .Text = "Symbol" & String$(2, vbTab) & aMatrixByStockAtom.Symbol & vbCrLf
                    .Text = "Stock Price" & vbTab & Format$(aMatrixByStockAtom.price, "#,##0.00") & String$(2, vbCrLf)
                    .RenderControl = fgRes.hWnd
                    .Text = vbCrLf
                End With
NEXT_MATRIX_BY_STOCK:
                IncProgress pbProgress
                Set aMatrixByStockAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
        Case REPORT_SYNTHETIC
            pbProgress.Max = m_Synthetic.Count
            For Each aSyntheticAtom In m_Synthetic
                With vsPrinter
                    If .CurrentY > nHeight Then .NewPage
                    
                    .FontName = "Tahoma"
                    .FontBold = False
                    .FontSize = 8
                    .TextAlign = taLeftTop
                    .Text = "Symbol" & String$(2, vbTab) & aSyntheticAtom.Symbol & String$(2, vbTab)
                    .Text = "Stock Position" & vbTab & Format$(aSyntheticAtom.Pos, "#,##0.00") & String$(2, vbTab)
                    .Text = "Net Delta" & String$(2, vbTab) & Format$(aSyntheticAtom.NetDelta, "#,##0.00") & vbCrLf
                    .Text = "Stock Price" & vbTab & Format$(aSyntheticAtom.price, "#,##0.00") & String$(2, vbTab)
                    .Text = "Synthetic Stock" & vbTab & Format$(aSyntheticAtom.Synthetic, "#,##0.00") & String$(2, vbTab)
                    .Text = "Net Stock" & String$(2, vbTab) & Format$(aSyntheticAtom.Net, "#,##0.00") & String$(2, vbCrLf)
                End With
                
                If aSyntheticAtom.Opt.Count <= 0 Then GoTo NEXT_SYNTHETIC
                
                aColl.Clear
                For Each aSyntheticOptAtom In aSyntheticAtom.Opt
                    dtExp = GetExpireMonth(aSyntheticOptAtom.Expiry)
                    nID = CLng(dtExp)
                    sKey = CStr(nID)
                    If aColl(sKey) Is Nothing Then
                        Set aEnt = aColl.Add(sKey)
                        aEnt.ID = nID
                        aEnt.Name = Format$(dtExp, "YYYY-MM")
                        aEnt.Data3 = dtExp
                        Set aEnt = Nothing
                    End If
                    Set aSyntheticOptAtom = Nothing
                Next

                nFixedRows = 1
                nFixedCols = 2

                ' + 5 cols
                .Cols = nFixedCols + aColl.Count + 5
                .rows = nFixedRows

                .FixedRows = nFixedRows
                .FixedCols = 0
                .FrozenCols = nFixedCols
                .TextMatrix(0, 0) = "Strike"
                .TextMatrix(0, 1) = "Root"
                .MergeCells = flexMergeRestrictColumns
                .MergeCol(0) = True
                .ColDataType(0) = flexDTDouble
                .ColAlignment(0) = flexAlignCenterCenter
                .ColFormat(0) = "#,##0.00"
                .ColDataType(1) = flexDTString
                .ColAlignment(1) = flexAlignLeftTop
                .FixedAlignment(1) = flexAlignCenterCenter

                SortEntCollByName aColl

                i = nFixedCols
                For Each aEnt In aColl
                    dtExp = aEnt.Data3
                    .TextMatrix(0, i) = Format$(dtExp, "MMMYY")
                    .ColKey(i) = CStr(CLng(dtExp))
                    .ColAlignment(i) = flexAlignCenterCenter
                    .ColDataType(i) = flexDTDouble
                    .ColFormat(i) = "#,##0.00"
                    i = i + 1
                    Set aEnt = Nothing
                Next
                
                ' 5 cols
                .TextMatrix(0, i) = "Net"
                .TextMatrix(0, i + 1) = "OTM Delta"
                .TextMatrix(0, i + 2) = "Gamma"
                .TextMatrix(0, i + 3) = "Vega"
                .TextMatrix(0, i + 4) = "Theta"

                For i = nFixedCols To .Cols - 1
                    .FixedAlignment(i) = flexAlignCenterCenter
                    .ColDataType(i) = flexDTDouble
                    .ColFormat(i) = "#,##0.00"
                Next
                
                i = nFixedRows
                For Each aSyntheticOptAtom In aSyntheticAtom.Opt
                    sKey = Format$(aSyntheticOptAtom.Strike, "#,##0.00") & aSyntheticOptAtom.RootSymbol
                    dtExp = GetExpireMonth(aSyntheticOptAtom.Expiry)
                    nRow = .FindRow(sKey)
                    nCol = .ColIndex(CStr(CLng(dtExp)))
                    If nRow > -1 Then
                        dVal = aSyntheticOptAtom.NetPos
                        dVal = dVal + .ValueMatrix(nRow, nCol)
                        .TextMatrix(nRow, nCol) = dVal
                    Else
                        .AddItem ""
                        .RowData(i) = sKey
                        .Cell(flexcpText, i, nFixedCols, i, .Cols - 1) = 0
                        .TextMatrix(i, 0) = aSyntheticOptAtom.Strike
                        .TextMatrix(i, 1) = aSyntheticOptAtom.RootSymbol
                        .TextMatrix(i, nCol) = aSyntheticOptAtom.NetPos
                        nRow = i
                        i = i + 1
                    End If
                    
                    ' update 5 cols
                    nCol = nFixedCols + aColl.Count
                    dVal = aSyntheticOptAtom.NetPos
                    dVal = dVal + .ValueMatrix(nRow, nCol)
                    .TextMatrix(nRow, nCol) = dVal
                    nCol = nCol + 1
                    dVal = aSyntheticOptAtom.OTMDelta
                    dVal = dVal + .ValueMatrix(nRow, nCol)
                    .TextMatrix(nRow, nCol) = dVal
                    nCol = nCol + 1
                    dVal = aSyntheticOptAtom.Gamma
                    dVal = dVal + .ValueMatrix(nRow, nCol)
                    .TextMatrix(nRow, nCol) = dVal
                    
'                    nCol = nCol + 1
'                    dVal = aSyntheticOptAtom.NetGamma
'                    dVal = dVal + .ValueMatrix(nRow, nCol)
'                    .TextMatrix(nRow, nCol) = dVal
                    
                    nCol = nCol + 1
                    dVal = aSyntheticOptAtom.Vega
                    dVal = dVal + .ValueMatrix(nRow, nCol)
                    .TextMatrix(nRow, nCol) = dVal
                    nCol = nCol + 1
                    dVal = aSyntheticOptAtom.Theta
                    dVal = dVal + .ValueMatrix(nRow, nCol)
                    .TextMatrix(nRow, nCol) = dVal
                    
                    ' insert option id to coll
                    nID = CLng(dtExp)
                    Set aEnt = aColl(CStr(nID))
                    If Not aEnt Is Nothing Then
                        aEnt.dValue1 = aEnt.dValue1 + aSyntheticOptAtom.Delta
                        aEnt.dValue2 = aEnt.dValue2 + aSyntheticOptAtom.Gamma
                        aEnt.dValue3 = aEnt.dValue3 + aSyntheticOptAtom.Vega
                        aEnt.dValue4 = aEnt.dValue4 + aSyntheticOptAtom.Theta
                    Else
                        Debug.Assert False
                    End If
                    
                    Set aEnt = Nothing
                    Set aSyntheticOptAtom = Nothing
                Next
                
                .ColSort(0) = flexSortGenericAscending
                .ColSort(1) = flexSortGenericAscending
                .Select 1, 0, .rows - 1, 1
                .Sort = flexSortUseColSort

                ' Net bottom row
                .SubtotalPosition = flexSTBelow
                For i = nFixedCols To .Cols - 1
                    .Subtotal flexSTSum, -1, i, , , , True, " ", , True
                Next
                .TextMatrix(.rows - 1, 1) = "Net"
               
                ' Delta, Gamma, Vega, Theta bottom rows
                .AddItem ""
                nRow = .rows - 1
                nCol = nFixedCols
                dVal = 0
                .TextMatrix(nRow, 1) = "Delta"
                For Each aEnt In aColl
                    dVal = dVal + aEnt.dValue1
                    .TextMatrix(nRow, nCol) = aEnt.dValue1
                    nCol = nCol + 1
                    Set aEnt = Nothing
                Next
                .TextMatrix(nRow, nCol) = dVal
                
                .AddItem ""
                nRow = .rows - 1
                nCol = nFixedCols
                dVal = 0
                .TextMatrix(nRow, 1) = "Gma$1"
                For Each aEnt In aColl
                    dVal = dVal + aEnt.dValue2
                    .TextMatrix(nRow, nCol) = aEnt.dValue2
                    nCol = nCol + 1
                    Set aEnt = Nothing
                Next
                .TextMatrix(nRow, nCol) = dVal
                
                .AddItem ""
                nRow = .rows - 1
                nCol = nFixedCols
                dVal = 0
                .TextMatrix(nRow, 1) = "Vega"
                For Each aEnt In aColl
                    dVal = dVal + aEnt.dValue3
                    .TextMatrix(nRow, nCol) = aEnt.dValue3
                    nCol = nCol + 1
                    Set aEnt = Nothing
                Next
                .TextMatrix(nRow, nCol) = dVal
                
                .AddItem ""
                nRow = .rows - 1
                nCol = nFixedCols
                dVal = 0
                .TextMatrix(nRow, 1) = "Theta"
                For Each aEnt In aColl
                    dVal = dVal + aEnt.dValue4
                    .TextMatrix(nRow, nCol) = aEnt.dValue4
                    nCol = nCol + 1
                    Set aEnt = Nothing
                Next
                .TextMatrix(nRow, nCol) = dVal
                
                .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
                .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
                .Cell(flexcpFontBold, nFixedRows, nFixedCols + aColl.Count, .rows - 1, .Cols - 1) = True
                .Cell(flexcpFontBold, .rows - 5, 0, .rows - 1, .Cols - 1) = True
                .AutoSize 0, .Cols - 1, False, 200
                
                vsPrinter.RenderControl = fgRes.hWnd
NEXT_SYNTHETIC:
                vsPrinter.Text = String$(2, vbCrLf)
                
                IncProgress pbProgress
                Set aMatrixByStockAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
        Case REPORT_PNL
            nFixedCols = 1
            nFixedRows = 1
            
            .Cols = nFixedCols + 12
            .rows = nFixedRows
                
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .MergeCells = flexMergeNever
            .TextMatrix(0, 0) = "Symbol"
            .TextMatrix(0, 1) = "cMktVal"
            .TextMatrix(0, 2) = "pMktVal"
            .TextMatrix(0, 3) = "NetMktVal"
            .TextMatrix(0, 4) = "cPoP"
            .TextMatrix(0, 5) = "pPoP"
            .TextMatrix(0, 6) = "NetPoP"
            .TextMatrix(0, 7) = "StkMTM" & vbCrLf & "P&L-Daily"
            .TextMatrix(0, 8) = "OptMTM" & vbCrLf & "P&L-Daily"
            .TextMatrix(0, 9) = "OptTheo" & vbCrLf & "P&L-Daily"
            .TextMatrix(0, 10) = "StkMTM-LTD" & vbCrLf & "P&L"
            .TextMatrix(0, 11) = "OptMTM-LTD" & vbCrLf & "P&L"
            .TextMatrix(0, 12) = "OptTheo-LTD" & vbCrLf & "P&L"
            
            .ColDataType(0) = flexDTString
            .ColAlignment(0) = flexAlignLeftTop
            .RowHeight(0) = 2 * .RowHeight(0)
            
            For i = 0 To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
                .ColDataType(i) = flexDTDouble
                .ColFormat(i) = "#,##0.00"
            Next
                
            i = 1
            pbProgress.Max = m_PnL.Count
            For Each aPnLAtom In m_PnL
                
                .AddItem ""
                .TextMatrix(i, 0) = aPnLAtom.Symbol
                .TextMatrix(i, 1) = aPnLAtom.CallMarketValue
                .TextMatrix(i, 2) = aPnLAtom.PutMarketValue
                .TextMatrix(i, 3) = aPnLAtom.NetMarketValue
                .TextMatrix(i, 4) = aPnLAtom.CallPoP
                .TextMatrix(i, 5) = aPnLAtom.PutPoP
                .TextMatrix(i, 6) = aPnLAtom.NetPoP
                .TextMatrix(i, 7) = aPnLAtom.UndMTMPnLDaily
                .TextMatrix(i, 8) = aPnLAtom.OptionsMTMPnLDaily
                .TextMatrix(i, 9) = aPnLAtom.OptionsTheoPnLDaily
                .TextMatrix(i, 10) = aPnLAtom.UndMTMLTDPnL
                .TextMatrix(i, 11) = aPnLAtom.OptionsMTMLTDPnL
                .TextMatrix(i, 12) = aPnLAtom.OptionsTheoLTDPnL
                i = i + 1
                IncProgress pbProgress
                Set aPnLAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
            
            .SubtotalPosition = flexSTBelow
            For i = nFixedCols To .Cols - 1
                .Subtotal flexSTSum, -1, i, , , , True, "Total", , True
            Next
               
            .ColSort(0) = flexSortGenericAscending
            .Select 1, 0, .rows - 1, 0
            .Sort = flexSortUseColSort
                
            .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
            .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
            .AutoSize 0, .Cols - 1, False, 200
                
            With vsPrinter
                .RenderControl = fgRes.hWnd
            End With
        Case REPORT_RISK_MATRIX
            nFixedCols = 2
            nFixedRows = 1
            
            .Cols = nFixedCols + 11
            .rows = nFixedRows
                
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .TextMatrix(0, 0) = "MktMove"
            .TextMatrix(0, 1) = "Shift"
            .TextMatrix(0, 2) = "StkPrice"
            .TextMatrix(0, 3) = "TheoMktVal"
            .TextMatrix(0, 4) = "TheoMktVal" & vbCrLf & "Change"
            .TextMatrix(0, 5) = "NetTheo" & vbCrLf & "PoP"
            .TextMatrix(0, 6) = "OptTheo" & vbCrLf & "P&L-Daily"
            .TextMatrix(0, 7) = "Delta"
            .TextMatrix(0, 8) = "Gma$1"
            .TextMatrix(0, 9) = "NetGma$"
            .TextMatrix(0, 10) = "Vega"
            .TextMatrix(0, 11) = "Theta"
            .TextMatrix(0, 12) = "_MktMove"
            .ColHidden(11) = True
            
            .RowHeight(0) = 2 * .RowHeight(0)
            .ColDataType(0) = flexDTString
            .ColAlignment(0) = flexAlignRightCenter
            
            For i = 1 To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
                .ColDataType(i) = flexDTDouble
                .ColFormat(i) = "#,##0.00"
            Next
            
            Dim bFirstSymbol As Boolean: bFirstSymbol = True
            
            pbProgress.Max = m_RiskMatrix.Count
            For Each aRiskMatrixAtom In m_RiskMatrix
                With vsPrinter
                    If .CurrentY > nHeight - 500 Or Not bFirstSymbol Then
                        .NewPage
                    End If
                    bFirstSymbol = False
                    
                    .FontName = "Tahoma"
                    .FontBold = False
                    .FontSize = 8
                    .TextAlign = taLeftTop
                    If aRiskMatrixAtom.ID <> -1 Then
                        .Text = "Symbol" & String$(2, vbTab) & aRiskMatrixAtom.Symbol & vbCrLf
                        .Text = "Stock Price" & vbTab & Format$(aRiskMatrixAtom.price, "#,##0.00") & String$(2, vbTab)
                        .Text = "30d HV" & vbTab & Format$(aRiskMatrixAtom.HV, "#,##0.00") & "%" & String$(2, vbTab)
                        .Text = "1 STD" & vbTab & Format$(aRiskMatrixAtom.STD, "#,##0.00") & "%" & String$(2, vbCrLf)
                    Else
                        .Text = "Totals" & String$(2, vbCrLf)
                    End If
                End With
                
                .ColHidden(1) = True
                .ColHidden(2) = Not aRiskMatrixAtom.ID <> -1
                
                .rows = nFixedRows
                .ColFormat(0) = "%"
                
                i = 1
                For Each aRiskMatrixShiftAtom In aRiskMatrixAtom.Shift
                    If aRiskMatrixShiftAtom.GroupID = 0 Then
                        .AddItem ""
                        .TextMatrix(i, 0) = aRiskMatrixShiftAtom.MarketMove
'                        .TextMatrix(i, 1) = aRiskMatrixShiftAtom.Shift
                        .TextMatrix(i, 2) = IIf(aRiskMatrixShiftAtom.price >= 0, aRiskMatrixShiftAtom.price, "--")
                        .TextMatrix(i, 3) = IIf(aRiskMatrixShiftAtom.MarketValue > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.MarketValue, "--")
                        .TextMatrix(i, 4) = IIf(aRiskMatrixShiftAtom.MarketValueChange > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.MarketValueChange, "--")
                        .TextMatrix(i, 5) = aRiskMatrixShiftAtom.PoP
                        .TextMatrix(i, 6) = aRiskMatrixShiftAtom.OptionsTheoPnLDaily
                        .TextMatrix(i, 7) = aRiskMatrixShiftAtom.Delta
                        .TextMatrix(i, 8) = aRiskMatrixShiftAtom.Gamma
                        .TextMatrix(i, 9) = IIf(aRiskMatrixShiftAtom.NetGamma > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.NetGamma, "--")
                        .TextMatrix(i, 10) = aRiskMatrixShiftAtom.Vega
                        .TextMatrix(i, 11) = aRiskMatrixShiftAtom.Theta
                        .TextMatrix(i, 12) = aRiskMatrixShiftAtom.MarketMove
                        i = i + 1
                        Set aRiskMatrixShiftAtom = Nothing
                    End If
                Next
            
                .ColSort(12) = flexSortGenericAscending
                .Select 1, 12, .rows - 1, 12
                .Sort = flexSortUseColSort
            
                .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
                .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
                .AutoSize 0, .Cols - 1, False, 50
                
                With vsPrinter
                    .RenderControl = fgRes.hWnd
                    .Text = vbCrLf
                End With
                                
                If aRiskMatrixAtom.ID = -1 Then GoTo REPORT_RISK_MATRIX_NEXT
                
' STD Risk Matrix
                .ColHidden(1) = False
                
                .rows = nFixedRows
                .ColFormat(0) = "###"
                
                i = 1
                For Each aRiskMatrixShiftAtom In aRiskMatrixAtom.Shift
                    If aRiskMatrixShiftAtom.GroupID = 1 Then
                        .AddItem ""
                        .TextMatrix(i, 0) = CStr(aRiskMatrixShiftAtom.MarketMove) & " STD"
                        .TextMatrix(i, 1) = aRiskMatrixShiftAtom.Shift
                        .TextMatrix(i, 2) = IIf(aRiskMatrixShiftAtom.price > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.price, "--")
                        .TextMatrix(i, 3) = IIf(aRiskMatrixShiftAtom.MarketValue > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.MarketValue, "--")
                        .TextMatrix(i, 4) = IIf(aRiskMatrixShiftAtom.MarketValueChange > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.MarketValueChange, "--")
                        .TextMatrix(i, 5) = aRiskMatrixShiftAtom.PoP
                        .TextMatrix(i, 6) = aRiskMatrixShiftAtom.OptionsTheoPnLDaily
                        .TextMatrix(i, 7) = aRiskMatrixShiftAtom.Delta
                        .TextMatrix(i, 8) = IIf(aRiskMatrixShiftAtom.Gamma > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.Gamma, "--")
                        .TextMatrix(i, 9) = IIf(aRiskMatrixShiftAtom.NetGamma > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.NetGamma, "--")
                        .TextMatrix(i, 10) = IIf(aRiskMatrixShiftAtom.Vega, aRiskMatrixShiftAtom.Vega, "--")
                        .TextMatrix(i, 11) = IIf(aRiskMatrixShiftAtom.Theta, aRiskMatrixShiftAtom.Theta, "--")
                        .TextMatrix(i, 12) = IIf(aRiskMatrixShiftAtom.MarketMove > BAD_DOUBLE_VALUE, aRiskMatrixShiftAtom.MarketMove, "--")
                        i = i + 1
                        Set aRiskMatrixShiftAtom = Nothing
                    End If
                Next
            
                If .rows > 1 Then
                    .ColSort(0) = flexSortGenericAscending
                    .Select 1, 12, .rows - 1, 12
                    .Sort = flexSortUseColSort
            
                    .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
                    .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
                    .AutoSize 0, .Cols - 1, False, 50
                
                    With vsPrinter
                        If .CurrentY > nHeight Then .NewPage
                        .RenderControl = fgRes.hWnd
                        .Text = vbCrLf
                    End With
                End If
                                
REPORT_RISK_MATRIX_NEXT:
                vsPrinter.Text = vbCrLf
                IncProgress pbProgress
                Set aRiskMatrixAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
            
        Case REPORT_CLOSE_TO_SPOT
            nFixedCols = 8
            nFixedRows = 1
            Dim dDiff As Double
            dDiff = CDbl(fgFlt.TextMatrix(1, PFC_MAX_STRIKE_DISTANCE))
            
            .Cols = nFixedCols
            .rows = nFixedRows
                
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .TextMatrix(0, 0) = "Account"
            .ColDataType(0) = flexDTString
            .ColFormat(0) = ""
                
            .TextMatrix(0, 1) = "Strategy"
            .ColDataType(1) = flexDTString
            .ColFormat(1) = ""
            
            .TextMatrix(0, 2) = "Underlying"
            .ColDataType(2) = flexDTString
            .ColFormat(2) = ""
            
            .TextMatrix(0, 3) = "Option"
            .ColDataType(3) = flexDTString
            .ColFormat(3) = ""
            
            .TextMatrix(0, 4) = "Strike"
            .ColDataType(4) = flexDTDouble
            .ColFormat(4) = "##.####"
            
            .TextMatrix(0, 5) = "Expiry"
            .ColDataType(7) = flexDTDate
            .ColFormat(7) = "MMM-YY"
            
            .TextMatrix(0, 6) = "Last Und Price"
            .ColDataType(6) = flexDTDouble
            .ColFormat(6) = "#,###.#0"
            
            .TextMatrix(0, 7) = "Diff"
            .ColDataType(7) = flexDTDouble
            .ColFormat(7) = "#,###.#0"
                
            .ColHidden(8) = True
            .ColHidden(9) = True
            
            .RowHeight(0) = 2 * .RowHeight(0)
            .ColDataType(0) = flexDTString
            .ColAlignment(0) = flexAlignRightCenter
            
            For i = 0 To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
            Next
            
            'Dim bFirstSymbol_ As Boolean: bFirstSymbol_ = True
            
            pbProgress.Max = m_RiskMatrix.Count
            Dim aUnd As EtsMmReportsLib.MmRpUndAtom
            
            i = 1
            For Each aUnd In m_Und
                With vsPrinter
                    If .CurrentY > nHeight - 500 Then 'Or Not bFirstSymbol_ Then
                        .NewPage
                    End If
                    'bFirstSymbol_ = False
                    
                    .FontName = "Tahoma"
                    .FontBold = False
                    .FontSize = 8
                    .TextAlign = taLeftTop
                End With
                
                Dim bAdd As Boolean
                bAdd = False
                
                Dim bDiffCurrent  As Double
                Dim aOpt As EtsMmReportsLib.MmRpOptAtom
                Dim dStrike As Double
                
                For Each aOpt In aUnd.Opt
                    dStrike = aOpt.Strike
                    
                    Dim dPriceClc As Double
                    
                    If aOpt.ContractType = enCtFutOption Then
                        Dim dummy As EtsReplacePriceStatusEnum
                        dPriceClc = aOpt.Fut.UndPriceProfile.GetUndPriceMid(aOpt.Fut.price.Bid, aOpt.Fut.price.Ask, aOpt.Fut.price.Last, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, dummy)
                        '.GetFuturePrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, dummy, bdummy)
                    Else
                        dPriceClc = aUnd.price.Last
                    End If

                    bDiffCurrent = Abs(dPriceClc - dStrike)
                    'bDiffCurrent = Abs(aUnd.PriceLast - dStrike)
                    If bDiffCurrent <= dDiff And aUnd.Opt.Count > 0 Then
                        bAdd = True
                    End If
                    
                    If bAdd Then
                        .AddItem ""
                        
                        .TextMatrix(i, 0) = aUnd.Trader
                        .TextMatrix(i, 1) = aOpt.Group ' aUnd.Strategy
                        
                        .TextMatrix(i, 2) = aUnd.Symbol
                        
                        .TextMatrix(i, 6) = aUnd.price.Last
                        .TextMatrix(i, 7) = bDiffCurrent
    
                        'For Each aOpt In aUnd.Opt
                        .TextMatrix(i, 3) = aOpt.Symbol
                        .TextMatrix(i, 4) = aOpt.Strike
                        .TextMatrix(i, 5) = aOpt.Expiry
                        'Next
                        i = i + 1
                    End If
                Next
                aUnd = Nothing
REPORT_CLOSE_TO_SPOT:
                'vsPrinter.Text = vbCrLf
                IncProgress pbProgress
'                Set aRiskMatrixAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
            .AutoSize 0, .Cols - 1, False, 50
            With vsPrinter
                .RenderControl = fgRes.hWnd
                .Text = vbCrLf
            End With
        Case REPORT_EXERCISED_STOCK_POSITION
            nFixedCols = 2
            nFixedRows = 1
            
            .Cols = nFixedCols + 3
            .rows = nFixedRows
                
            .FixedRows = nFixedRows
            .FixedCols = 0
            .FrozenCols = nFixedCols
            .TextMatrix(0, 0) = "Symbol"
            .TextMatrix(0, 1) = "Series"
            .TextMatrix(0, 2) = "Exercised Stock" & vbCrLf & "Position"
            .TextMatrix(0, 3) = "5 Prev Expiration Dates" & vbCrLf & "Average Volume"
            .TextMatrix(0, 4) = "10 Prev Trade Days" & vbCrLf & "Average Volume"
            
            .ColDataType(0) = flexDTString
            .ColAlignment(0) = flexAlignLeftTop
            .FixedAlignment(0) = flexAlignCenterCenter
            .ColDataType(1) = flexDTDate
            .ColAlignment(1) = flexAlignLeftTop
            .ColFormat(1) = "MMMYY"
            .FixedAlignment(1) = flexAlignCenterCenter
            .RowHeight(0) = 2 * .RowHeight(0)
            
            For i = nFixedCols To .Cols - 1
                .FixedAlignment(i) = flexAlignCenterCenter
                .ColDataType(i) = flexDTDouble
                .ColFormat(i) = "#,##0.00"
            Next
                
            i = 1
            pbProgress.Max = m_ExercisedStock.Count
            For Each aExercisedStockAtom In m_ExercisedStock
                dtExp = GetExpireMonth(aExercisedStockAtom.Expiry)
                sKey = aExercisedStockAtom.Symbol & CStr(dtExp)
                nID = .FindRow(sKey)
                If nID > -1 Then
                    .TextMatrix(nID, 2) = .ValueMatrix(nID, 2) + aExercisedStockAtom.Pos
                Else
                    .AddItem ""
                    .RowData(i) = sKey
                    .TextMatrix(i, 0) = aExercisedStockAtom.Symbol
                    .TextMatrix(i, 1) = dtExp
                    .TextMatrix(i, 2) = aExercisedStockAtom.Pos
                    .TextMatrix(i, 3) = aExercisedStockAtom.Prev5ExpAvgVol
                    .TextMatrix(i, 4) = aExercisedStockAtom.Prev10TrdAvgVol
                    i = i + 1
                End If
                IncProgress pbProgress
                Set aExercisedStockAtom = Nothing
                DoEvents
                If Not m_bRecalc Then GoTo Ex
            Next
            
            .MergeCells = flexMergeRestrictColumns
            .MergeCol(0) = True
            
            .ColSort(0) = flexSortGenericAscending
            .ColSort(1) = flexSortGenericAscending
            .Select 1, 0, .rows - 1, 1
            .Sort = flexSortUseColSort
                
            .Cell(flexcpBackColor, .FixedRows, .FixedCols, .rows - 1, .Cols - 1) = RGB(255, 255, 255)
            .Cell(flexcpForeColor, 0, 0, .rows - 1, .Cols - 1) = RGB(1, 1, 1)
            .AutoSize 0, .Cols - 1, False, 50
                
            With vsPrinter
                .RenderControl = fgRes.hWnd
            End With
        End Select
    End With
        
Ex:
    With vsPrinter
        .EndDoc
        .FontName = "Tahoma"
        .FontBold = False
        .FontSize = 8

        For i = 1 To .PageCount
            .StartOverlay i

            .TextAlign = taLeftTop
            .CurrentX = .MarginLeft
            .CurrentY = .MarginHeader
            .Text = AppTitle()

            .TextAlign = taRightTop
            .Text = Format$(Now, "General Date")

            .CurrentX = .MarginLeft
            .CurrentY = .PageHeight - .MarginBottom
            .TextAlign = taRightTop
            .Paragraph = "Page " & i & " of " & .PageCount

            .EndOverlay
        Next
    End With

    Set aColl = Nothing
End Sub

Private Function GetExpireMonth(dtExpire As Date) As Date
    GetExpireMonth = DateSerial(Year(dtExpire), Month(dtExpire), 1)
End Function

Private Sub SetReportName()
    On Error Resume Next
    Dim sName$
    
    Select Case m_nFilter(PFC_REPORT)
    Case REPORT_POSITION_GREEK_BY_MONTH
        sName = "Position Greek By Month"
    Case REPORT_POSITION_GREEK_SUMMARY
        sName = "Position Greek Summary"
    Case REPORT_POSITION_WITH_EARLY_EXERCISE
        sName = "Positions With Early Exercise"
    Case REPORT_POSITION_MATRIX_BY_STOCK
        sName = "Position Matrix By Stock"
    Case REPORT_SYNTHETIC
        sName = "Synthetic Open/Day Positions"
    Case REPORT_PNL
        sName = "P&L"
    Case REPORT_RISK_MATRIX
        sName = "Risk Matrix"
    Case REPORT_EXERCISED_STOCK_POSITION
        sName = "Exercised Stock Position"
    Case REPORT_CLOSE_TO_SPOT
        sName = "Close To Spot"
    End Select
    
    With vsPrinter
        .FontName = "Tahoma"
        .FontBold = True
        .FontSize = 12
        .TextAlign = taCenterTop
        .Paragraph = sName
    End With
End Sub

Private Sub SetReportFilter()
    With vsPrinter
        .FontName = "Tahoma"
        .FontBold = False
        .FontSize = 8
        .TextAlign = taLeftTop
        If fgFlt.ColHidden(PFC_MAX_STRIKE_DISTANCE) = True Then
           .Text = fgFlt.TextMatrix(0, PFC_UNDERLYING) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_UNDERLYING) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TYPE) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TYPE) & vbTab & _
                fgFlt.TextMatrix(0, PFC_GROUP) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_GROUP) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TRADER_GROUP) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TRADER_GROUP) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TRADER) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TRADER) & vbTab & _
                fgFlt.TextMatrix(0, PFC_STRATEGY) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_STRATEGY) & vbCrLf
        Else
            .Text = fgFlt.TextMatrix(0, PFC_UNDERLYING) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_UNDERLYING) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TYPE) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TYPE) & vbTab & _
                fgFlt.TextMatrix(0, PFC_GROUP) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_GROUP) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TRADER_GROUP) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TRADER_GROUP) & vbTab & _
                fgFlt.TextMatrix(0, PFC_TRADER) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_TRADER) & vbTab & _
                fgFlt.TextMatrix(0, PFC_STRATEGY) & ": " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_STRATEGY) & vbCrLf & _
                fgFlt.TextMatrix(0, PFC_MAX_STRIKE_DISTANCE) & " : " & fgFlt.Cell(flexcpTextDisplay, 1, PFC_MAX_STRIKE_DISTANCE) & vbCrLf
        End If
    End With
End Sub

Private Sub SetReportDate()
    On Error Resume Next
    With vsPrinter
        .TextAlign = taRightTop
        .TextColor = &H8000000C
        .Text = "Date" & vbTab & Format$(Date, "Short Date") & vbCrLf
    End With
End Sub

Private Sub aParams_PriceProfilesChange()
    SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    SetRefreshHint True
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    
    On Error Resume Next
    bUpdate = False
    
    For Each aFilterAtom In collUpdUnd
        If Not m_Und(aFilterAtom.ID) Is Nothing Then
            bUpdate = True
            Set aFilterAtom = Nothing
            Exit For
        End If
        Set aFilterAtom = Nothing
    Next
    If bUpdate Then SetRefreshHint True
End Sub

Private Sub aParams_UndPriceToleranceChange()
    SetRefreshHint True
End Sub

Private Sub cbxZoom_Click()
    On Error Resume Next
    If cbxZoom.ListIndex >= 0 Then
        If cbxZoom.ItemData(cbxZoom.ListIndex) <> zmPercentage Then
            vsPrinter.ZoomMode = cbxZoom.ItemData(cbxZoom.ListIndex)
        Else
            vsPrinter.Zoom = val(cbxZoom.List(cbxZoom.ListIndex))
        End If
        
        SaveSettings
    End If
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = CLng(sValue)
            If Err.Number <> 0 Then nValue = val(sValue)
            If Err.Number = 0 Then
                m_nFilter(Col) = nValue
                .AutoSize 0, .Cols - 1, , 100
            End If

            FilterUpdate
            FilterUpdateChangedStatus
            AdjustCaption
            AdjustState
        End If
    End With
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_REPORTS_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_REPORTS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_REPORTS_FILTER) Then
        m_bKeyDown(GT_REPORTS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_REPORTS_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
        End Select
    End If
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = IsDblClickHandled Or m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    'mnuOrderNewStock                           "New Stock Order..."
    'mnuOrderNewOption                          "New Option Order..."
    'mnuCtxRefresh                              "Refresh"
    'mnuCtxCopy                                 "Copy Grid"
    'mnuCtxGridLayout                           "Grid Layout..."

    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxOrderNewStock.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewOption.Visible = g_Params.OrdersVisible
   
    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    
'    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.EnableOrders
'    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.EnableOrders
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    PopupMenu mnuCtx
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_REPORTS_FILTER
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
    SetGridLayout enGridType, gdGrid
End Sub

Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    SetGridLayout enGridType, gdGrid
    Screen.MousePointer = vbDefault
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    
    Select Case enGridType
        Case GT_REPORTS_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_REPORTS_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
    End Select
    
    UserControl_Resize
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
    
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bDataLoad Then
        m_bDataLoad = False
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bRecalc Then
        m_bRecalc = False
    
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
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_REPORTS_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_REPORTS_FILTER
            CustomizeFltGridLayout
    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, ByRef dPrice#)
    On Error Resume Next

    Set aUnd = g_Underlying(m_nFilter(PFC_UNDERLYING))
            
    If bIsStock And Not aUnd Is Nothing Then
        dPrice = m_Und(aUnd.ID).price.Bid
        If dPrice <= 0# Then dPrice = m_Und(aUnd.ID).price.Last
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#

    m_bInProc = True
    FillDataForOrderFromCurrentSelection bIsStock, aUnd, dPrice
    m_bInProc = False

    On Error GoTo EH
    If bIsStock Then
        frmOrderEntry.NewStockOrder aUnd, True, 100, dPrice
    Else
        frmOrderEntry.NewOptionOrder aUnd, Nothing, True, 10, dPrice
    End If

    Set aUnd = Nothing
    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Reports View: Fail to create new order."
    ResetMenuData
    Set aUnd = Nothing
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
    OrderNew False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    OrderNew True
End Sub

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, aUnd, dPrice
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, Nothing, False, 10&, 0#
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Reports View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxRefresh_Click()
    Refresh
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
    
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc Then Exit Sub
    
    ClearViewAndData
    SetRefreshHint False

    CopyFilters True
    FilterUpdateChangedStatus
    
    m_Grp.ID = m_nFilter(PFC_UNDERLYING)
    m_Grp.GroupType = TYPE_UNDERLYING
    m_Grp.Report = m_nFilter(PFC_REPORT)
    
    If m_Grp.Report <> REPORT_NONE And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
        m_bLastQuoteReqNow = False
    End If

    If m_Grp.Report <> REPORT_NONE Then
        lblStatus.Visible = False
        imgStop.Visible = True
        imgStopDis.Visible = False
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = g_TradeChannel.Trades.Count
        pbProgress.Visible = True
        lblProcess.Caption = "Data loading..."
        lblProcess.Visible = True
        lblProcess.Refresh

        If PositionsLoad And OptionsLoad Then
            AdjustCaption

            lblProcess.Caption = "Last quotes request..."
            lblProcess.Refresh

            RequestLastQuotes
        Else
            ClearViewAndData
'            FilterUpdateAll
            AdjustCaption
            AdjustState

            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        AdjustCaption
    End If

    Screen.MousePointer = vbDefault
End Sub

Private Sub UserControl_Initialize()
    m_bInProc = False
    m_bShutDown = False
    m_bRecalc = False
    m_bLastQuoteReqNow = False
    m_bDataLoad = False
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
        
    With fgFlt
        .Top = 0
        .Left = 0
        .Height = .RowHeight(0) + (.rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
        .Refresh
    End With
    With picNav
        .Top = fgFlt.Height + 50
        .Left = 0
        .Width = ScaleWidth
    End With
'    With fgRes
'        .Left = 0
'        .Top = fgFlt.Height + GRID_VERT_SPACE
'        .Width = ScaleWidth
'        .Height = ScaleHeight - .Top
'        .Refresh
'    End With
    With vsPrinter
        .Left = 0
        .Top = picNav.Top + picNav.Height
        .Width = ScaleWidth
        .Height = ScaleHeight - .Top
        .Refresh
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
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If
        Set PriceProvider = Nothing
    End If
    
    ClearViewAndData
    
    Set m_GroupPriceProvider = Nothing
    Set m_Und = Nothing
    
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set frmLayout = Nothing
    Set aParams = Nothing
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Private Sub AdjustState()
    On Error Resume Next
    Dim bEnabled As Boolean, aCtrl As Control
    bEnabled = Not (m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc)
    
    cbxZoom.Enabled = bEnabled
'    lblZoom.Enabled = bEnabled
'    lblPage.Enabled = bEnabled
    
    lblPage.Caption = "Page " & vsPrinter.PreviewPage & " of " & vsPrinter.PageCount

    btnNav(1).Enabled = (bEnabled And vsPrinter.PageCount > 0 And vsPrinter.PreviewPage > 1)
    btnNav(2).Enabled = btnNav(1).Enabled
    btnNav(3).Enabled = (bEnabled And vsPrinter.PageCount > vsPrinter.PreviewPage)
    btnNav(4).Enabled = btnNav(3).Enabled

    mnuCtxRefresh.Enabled = bEnabled

    RaiseEvent OnStateChange
End Sub

Private Sub btnNav_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
        Case 1 ' First
            vsPrinter.PreviewPage = 1
        Case 2 ' Prev
            vsPrinter.PreviewPage = vsPrinter.PreviewPage - 1
        Case 3 ' Next
            vsPrinter.PreviewPage = vsPrinter.PreviewPage + 1
        Case 4 ' Last
            vsPrinter.PreviewPage = vsPrinter.PageCount
    End Select
    
    AdjustState
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = fgFlt.Cell(flexcpTextDisplay, 1, PFC_REPORT) & " - Reports"
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = fgFlt.Cell(flexcpTextDisplay, 1, PFC_REPORT)
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_bRecalc
End Property

Public Property Get CanPrint() As Boolean
    CanPrint = Not (vsPrinter.ReadyState = vpstEmpty)
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_REPORTS_FILTER, PFC_REPORT, PFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Function Group() As clsRpGrpAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Private Sub ClearViewAndData()
    On Error Resume Next
    
    ClearResGrid
    vsPrinter.Clear
    
    m_Grp.Clear
    m_Und.Clear
    m_QuoteRequests.Clear
    m_GreeksByMonth.Clear
    m_GreeksSummary.Clear
    m_MatrixByStock.Clear
    m_Synthetic.Clear
    m_PnL.Clear
    m_RiskMatrix.Clear
    m_ExercisedStock.Clear
    m_PosWithEarlyExercise.Clear
End Sub

Public Sub Refresh()
    On Error Resume Next
    With fgFlt
        ' exit from edit mode
        If .EditWindow <> 0 Then .Select .Row, .Col
    End With
    
    tmrShow.Enabled = True
End Sub

Public Sub PageSetup()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    m_bInProc = True
    AdjustState
    
    If vsPrinter.NDevices > 0 Then
        If vsPrinter.PrintDialog(pdPageSetup) Then
            m_bInProc = True
            m_bRecalc = True

            pbProgress.Visible = True
            lblProcess.Visible = True
            imgStop.Visible = True
            
            UpdateResGrid
            
            imgStop.Visible = False
            pbProgress.Visible = False
            lblProcess.Visible = False
            
            m_bRecalc = False
            m_bInProc = False
            
            SaveSettings
        End If
    Else
        gCmn.CantAccessPrinterErrorMsgBox
    End If
    
    m_bInProc = False
    AdjustState
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
    If m_nFilter(PFC_UNDERLYING) <> nNewUndID Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then
                With fgFlt
                    m_GridLock(GT_REPORTS_FILTER).LockRedraw
                    
                    sValue = "0"
                    nValue = 0
                    
                    sComboList = g_Params.UnderlyingComboList
                    'sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
                    If Len(sComboList) > 0 Then
                        sComboList = "#0;<All>|" & sComboList
                    Else
                        sComboList = "#0;<All>"
                    End If
                    
                    Set aUnd = g_Underlying(m_nFilter(PFC_UNDERLYING))
                    If Not aUnd Is Nothing Then
                        nValue = aUnd.ID
                        sValue = Trim$(Str$(aUnd.ID))
                        Set aUnd = Nothing
                    End If
                    .ColComboList(PFC_UNDERLYING) = sComboList
                    .TextMatrix(1, PFC_UNDERLYING) = sValue
                    m_nFilter(PFC_UNDERLYING) = nValue
                    
                    m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
                End With
            End If
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom, sComboList$, sValue$, nValue&
    If m_nFilter(PFC_GROUP) <> nNewUndGroupID Then
        Set aUndGroup = g_UnderlyingGroup(nNewUndGroupID)
        If Not aUndGroup Is Nothing Then
            With fgFlt
                m_GridLock(GT_REPORTS_FILTER).LockRedraw
                
                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(PFC_GROUP) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(aUndGroup.ID))
                    End If
                Next
                .ColComboList(PFC_GROUP) = sComboList
                .TextMatrix(1, PFC_GROUP) = sValue
                m_nFilter(PFC_GROUP) = nValue
                
                m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, sComboList$, sValue$, nValue&
    If m_nFilter(PFC_STRATEGY) <> nStrategyID Then
        Set aStrategy = g_Strategy(nStrategyID)
        If Not aStrategy Is Nothing Then
            With fgFlt
                m_GridLock(GT_REPORTS_FILTER).LockRedraw
                
                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_nFilter(PFC_STRATEGY) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(aStrategy.ID))
                    End If
                Next
                .ColComboList(PFC_STRATEGY) = sComboList
                .TextMatrix(1, PFC_STRATEGY) = sValue
                m_nFilter(PFC_STRATEGY) = nValue
                
                m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub PrintView()
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    If m_bInProc Then Exit Sub
    
    m_bInProc = True
    AdjustState
    
    vsPrinter.PrintDoc True
    
    m_bInProc = False
    AdjustState
    Screen.MousePointer = vbDefault
End Sub

Private Function GetColKeyFromUnd(aUnd As EtsMmReportsLib.MmRpUndAtom) As String
    On Error Resume Next
    GetColKeyFromUnd = ""
    GetColKeyFromUnd = CStr(IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex)) & "_" & aUnd.Symbol
End Function

Private Function GetColKeyFromOpt(aOpt As EtsMmReportsLib.MmRpOptAtom) As String
    On Error Resume Next
    GetColKeyFromOpt = ""
    GetColKeyFromOpt = IIf(aOpt.ContractType = enCtOption, enCtOption, enCtFutOption) & "_" & aOpt.Symbol
End Function

Private Function GetColKeyFromQUP(ByRef aParams As PRICEPROVIDERSLib.QuoteUpdateParams) As String
    On Error Resume Next
    Dim instrID$
    
    GetColKeyFromQUP = ""
    'GetColKeyFromQUP = CStr(IIf(aParams.Type = enOPT, enCtOption, IIf(aParams.Type = enSTK, enCtStock, enCtIndex))) & "_" & aParams.Symbol
     Select Case aParams.Type
        Case enOPT:
            instrID = CStr(enCtOption)
            
        Case enSTK:
            instrID = CStr(enCtStock)
            
        Case enFUT:
            instrID = CStr(enCtFuture)
            
        Case enFOP
            instrID = CStr(enCtFutOption)
            
        Case enIDX:
            instrID = CStr(enCtIndex)
    End Select
    GetColKeyFromQUP = instrID & "_" & aParams.Symbol
'    GetColKeyFromQUP = CStr( IIf( aParams_
'                                IIf( _
'                                        aParams.Type = enOPT, enCtOption, _
'                                            IIf( _
'                                                    aParams.Type = enSTK, enCtStock, enCtIndex _
'                                                ) _
'                                    ) _
'                            ) & "_" & aParams.Symbol
End Function

Private Sub InitVola(ByRef aUnd As EtsMmReportsLib.MmRpUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.UndType = enCtStock, enCtStock, enCtIndex), g_VolaSource
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim aReq As clsRpRequestAtom, bFinished As Boolean
    Dim aRequest As PRICEPROVIDERSLib.QuoteUpdateParams
    
    aRequest = Request
    bFinished = False
    
    If ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If aRequest.Type <> enGrSTK And aRequest.Type <> enGrIDX Then
                
                Set aReq = m_QuoteRequests(GetColKeyFromQUP(aRequest))
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, aRequest.Symbol & ": " & Description
                
                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                    IncProgress pbProgress
                    
                    If m_bGroupRequestSupported And aRequest.Type <> enOPT Then
                        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    End If
                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                                Or m_bGroupRequestSupported And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
                End If
            Else
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequestSupported Then
                    m_GroupPriceProvider.CancelGroup
                End If
                
                CalcReport
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
                
                AdjustState
            End If
        Else
 '           Debug.Assert False
        End If
    
    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim aReq As clsRpRequestAtom
    Dim aUnd As EtsMmReportsLib.MmRpUndAtom, aOpt As EtsMmReportsLib.MmRpOptAtom, aFut As EtsMmReportsLib.MmRpFutAtom
    Dim dPriceBid#, dPriceAsk#, dPriceLast#
    Dim bFinished As Boolean
    bFinished = False
    
    If Params.Type <> enGrSTK And Params.Type <> enGrIDX Then
        Set aReq = m_QuoteRequests(GetColKeyFromQUP(Params))
        If Not aReq Is Nothing Then
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
            
            Set aOpt = aReq.Opt
            Set aUnd = aReq.Und
            Set aFut = aReq.Fut
            
            If Not aOpt Is Nothing Then
                aOpt.price.Bid = dPriceBid
                aOpt.price.Ask = dPriceAsk
                aOpt.price.Last = dPriceLast
            ElseIf Not aUnd Is Nothing Then
                aUnd.price.Bid = dPriceBid
                aUnd.price.Ask = dPriceAsk
                aUnd.price.Last = dPriceLast
            ElseIf Not aFut Is Nothing Then
                aFut.price.Bid = dPriceBid
                aFut.price.Ask = dPriceAsk
                aFut.price.Last = dPriceLast
            End If
            
            
            Set aOpt = Nothing
            Set aUnd = Nothing
                    
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            Set aReq = Nothing
                
            If m_bGroupRequestSupported And Params.Type <> enOPT Then
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
            End If
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                        Or m_bGroupRequestSupported And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)

        End If
    Else
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If
    
    If bFinished Then
        m_bLastQuoteReqNow = False
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequestSupported Then
            m_GroupPriceProvider.CancelGroup
        End If

        CalcReport

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
    
        AdjustState
    End If
End Sub

Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aReq As clsRpRequestAtom
    Dim nCount&
    Dim qup As QuoteUpdateParams
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        
        nCount = m_QuoteRequests.Count
        If nCount > 0 Then
            m_bInProc = True
            m_bLastQuoteReqNow = True
            AdjustState

            m_nLastQuoteReqCount = nCount
            m_nLastQuoteReqDone = 0&

            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_nLastQuoteReqCount
            pbProgress.Visible = True

            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Last quotes request..."
            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents

            If Not m_bGroupRequestSupported Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                qup.Exchange = ""
                qup.Symbol = ""
                qup.Type = enMStart
                PriceProvider.RequestLastQuote qup
                

                For Each aReq In m_QuoteRequests
                    If Not m_bLastQuoteReqNow Then Exit For
                    PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                    DoEvents
                Next
                
                qup.Type = enMStop
                PriceProvider.RequestLastQuote qup

            Else
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                
                For Each aReq In m_QuoteRequests
                    If Not m_bLastQuoteReqNow Then Exit For
                        
                    aParam = aReq.GetQuoteUpdateParam
                    If aParam.Type = enSTK Or aParam.Type = enIDX Then
                        PriceProvider.RequestLastQuote aParam
                        m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                        DoEvents
                    
                        If Not m_bLastQuoteReqNow Then Exit For
                        
                        If aReq.Und.Opt.Count > 0 Then
                            aParam.Type = IIf(aParam.Type = enSTK, enGrSTK, enGrIDX)
                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                            DoEvents
                        End If
                    End If
                Next
            End If

            m_bInProc = False
            AdjustState
        Else
            CalcReport

            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            lblProcess.Caption = ""
            imgStop.Visible = False
            imgStopDis.Visible = False
        End If
    Else
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
    
    PriceProvider.CancelLastQuote
    If m_bGroupRequestSupported Then
        m_GroupPriceProvider.CancelGroup
    End If
End Sub

Private Sub CalcReport()
    Dim aUnd As EtsMmReportsLib.MmRpUndAtom, nID&
    Dim RateValues() As Double, RateDates() As Double
    Dim HTBRates() As Double, HTBDates() As Double
    Dim dTolerance#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim bUseTheoVolatility As Boolean, bUseTheoNoBid As Boolean, bUseTheoBadMarketVola As Boolean
    Dim aGreeksSummary As EtsMmReportsLib.MmRpGreeksSummaryAtom, aGreeksSummaryColl As EtsMmReportsLib.MmRpGreeksSummaryColl
    Dim aGreeksByMonth As EtsMmReportsLib.MmRpGreeksByMonthAtom, aGreeksByMonthColl As EtsMmReportsLib.MmRpGreeksByMonthColl
    Dim aPnL As EtsMmReportsLib.MmRpPnLAtom, aPnLColl As EtsMmReportsLib.MmRpPnLColl
    Dim aSynthetic As EtsMmReportsLib.MmRpSyntheticAtom, aSyntheticColl As EtsMmReportsLib.MmRpSyntheticColl
    Dim aRiskMatrix As EtsMmReportsLib.MmRpRiskMatrixAtom, aRiskMatrixColl As EtsMmReportsLib.MmRpRiskMatrixColl
    Dim aExercisedStock As EtsMmReportsLib.MmRpExercisedStockAtom, aExercisedStockColl As EtsMmReportsLib.MmRpExercisedStockColl
    'Dim aStressReportColl As EtsMmReportsLib.MmRpStressReportColl
    
    On Error Resume Next
    m_bInProc = True
    m_bRecalc = True
    dTolerance = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    bUseTheoVolatility = g_Params.UseTheoVolatility
    bUseTheoNoBid = g_Params.UseTheoNoBid
    bUseTheoBadMarketVola = g_Params.UseTheoBadMarketVola
        
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = m_Und.Count
    lblProcess.Caption = "Calculating..."
    
    On Error GoTo EH
    Select Case m_Grp.Report
    Case REPORT_POSITION_GREEK_BY_MONTH
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_POSITION_GREEK_BY_MONTH
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                Set aGreeksByMonthColl = aUnd.CalcGreeksByMonth(g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, g_Main.CalculationParametrs)
            If aGreeksByMonthColl Is Nothing Then GoTo NEXT_REPORT_POSITION_GREEK_BY_MONTH
            For Each aGreeksByMonth In aGreeksByMonthColl
                nID = m_GreeksByMonth.Count
                'If m_ReportCollapseStyle <> EnRptGrType_Stocks Then
                '    aGreeksByMonth.Account = aUnd.Account
                '    aGreeksByMonth.StrategyName = aUnd.Strategy
                'End If
            If Year(aGreeksByMonth.Expiry) >= Year(DateTime.Now) - 3 Then
                m_GreeksByMonth.Add nID, aGreeksByMonth
                End If
                Set aGreeksByMonth = Nothing
            Next
            Set aGreeksByMonthColl = Nothing
NEXT_REPORT_POSITION_GREEK_BY_MONTH:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_POSITION_GREEK_SUMMARY
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_POSITION_GREEK_SUMMARY
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                Set aGreeksSummaryColl = aUnd.CalcGreeksSummary(g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, g_Main.CalculationParametrs)
            If aGreeksSummaryColl Is Nothing Then GoTo NEXT_REPORT_POSITION_GREEK_SUMMARY
            For Each aGreeksSummary In aGreeksSummaryColl
                nID = m_GreeksSummary.Count
                
                'aGreeksSummary.Account = aUnd.Account
                'aGreeksSummary.HedgeGroup = aUnd.StrategyName
                
                m_GreeksSummary.Add nID, aGreeksSummary
                Set aGreeksSummary = Nothing
            Next
            Set aGreeksSummaryColl = Nothing
NEXT_REPORT_POSITION_GREEK_SUMMARY:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_POSITION_WITH_EARLY_EXERCISE
        For Each aUnd In m_Und
            If Not aUnd.Visible Then GoTo NEXT_REPORT_POSITION_WITH_EARLY_EXERCISE
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                aUnd.CalcPosWithEarlyExercise g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, m_PosWithEarlyExercise(aUnd.ID), _
                    g_Params.EAOptWithPosition, g_Params.EAUpcomingDivs, g_Params.EADaysToDiv, g_Main.CalculationParametrs
                
NEXT_REPORT_POSITION_WITH_EARLY_EXERCISE:
            IncProgress pbProgress
            DoEvents
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_POSITION_MATRIX_BY_STOCK
        For Each aUnd In m_Und
            If Not aUnd.Visible Then GoTo NEXT_REPORT_POSITION_MATRIX_BY_STOCK
                aUnd.CalcMatrixByStock dTolerance, enRoundingRule, m_MatrixByStock(aUnd.ID), g_Main.CalculationParametrs
            
NEXT_REPORT_POSITION_MATRIX_BY_STOCK:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_SYNTHETIC
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_SYNTHETIC
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                aUnd.CalcSynthetics g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, m_Synthetic, g_Main.CalculationParametrs

NEXT_REPORT_SYNTHETIC:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_PNL
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_PNL
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                Set aPnLColl = aUnd.CalcPnLs(g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, g_Main.CalculationParametrs)
            If aPnLColl Is Nothing Then GoTo NEXT_REPORT_PNL
            For Each aPnL In aPnLColl
                nID = m_PnL.Count
                'aPnL.Account = aUnd.Account
                'aPnL.HedgeGroup = aUnd.StrategyName
                
                m_PnL.Add nID, aPnL
                Set aPnL = Nothing
            Next
            Set aPnLColl = Nothing
NEXT_REPORT_PNL:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_RISK_MATRIX
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_RISK_MATRIX
            GetRates aUnd, RateValues, RateDates, HTBRates, HTBDates
                Set aRiskMatrixColl = aUnd.CalcRiskMatrix(g_Params.CalcModel, _
                RateValues, RateDates, HTBRates, HTBDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
                    m_Und, dTolerance, enRoundingRule, g_Main.CalculationParametrs)
                    
            If aRiskMatrixColl Is Nothing Then GoTo NEXT_REPORT_RISK_MATRIX
            For Each aRiskMatrix In aRiskMatrixColl
                nID = m_RiskMatrix.Count
                m_RiskMatrix.Add nID, aRiskMatrix.Symbol, aRiskMatrix
                Set aRiskMatrix = Nothing
            Next
            Set aRiskMatrixColl = Nothing
NEXT_REPORT_RISK_MATRIX:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
        For Each aUnd In m_Und
            aUnd.CalcRiskMatrixTotals m_RiskMatrix
            Exit For
        Next
    Case REPORT_EXERCISED_STOCK_POSITION
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_EXERCISED_STOCK_POSITION
                Set aExercisedStockColl = aUnd.CalcExercisedStocks(m_Und, dTolerance, enRoundingRule, g_Main.CalculationParametrs)
            If aExercisedStockColl Is Nothing Then GoTo NEXT_REPORT_EXERCISED_STOCK_POSITION
            For Each aExercisedStock In aExercisedStockColl
                nID = m_ExercisedStock.Count
                m_ExercisedStock.Add nID, aExercisedStock
                Set aExercisedStock = Nothing
            Next
            Set aExercisedStockColl = Nothing
NEXT_REPORT_EXERCISED_STOCK_POSITION:
            IncProgress pbProgress
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
    Case REPORT_CLOSE_TO_SPOT
        For Each aUnd In m_Und
            If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_CLOSE_TO_SPOT
            
NEXT_REPORT_CLOSE_TO_SPOT:
            Set aUnd = Nothing
            If Not m_bRecalc Then Exit For
        Next
'    Case REPORT_GAMMA
'        If m_nFilter(PFC_STEP_COUNT) = 0 Or m_nFilter(PFC_STEP_PERCENT) = 0 Then
'            m_bRecalc = False
'        Else
'            Dim nSteps&, dStepPercent#
'            nSteps = m_nFilter(PFC_STEP_COUNT)
'            dStepPercent = fgFlt.ValueMatrix(1, PFC_STEP_PERCENT)
'
'            For Each aUnd In m_Und
'                If Not CheckPos(aUnd) Then GoTo NEXT_REPORT_GAMMA
'                GetRates aUnd, RateValues, RateDates
'                Set aStressReportColl = aUnd.CalcStress(g_Params.CalcModel, _
'                    RateValues, RateDates, bUseTheoVolatility, bUseTheoNoBid, bUseTheoBadMarketVola, _
'                    m_Und, dTolerance, enRoundingRule, nSteps, dStepPercent)
'                If aStressReportColl Is Nothing Then GoTo NEXT_REPORT_GAMMA
'                For Each aGreeksByMonthColl In aStressReportColl
'                    nID = m_GammaStressByMonth.Count
'                    If Year(aGreeksByMonth.Expiry) >= Year(DateTime.Now) - 3 Then
'                    m_GammaStressByMonth.Add nID, aGreeksByMonthColl
'                    End If
'                    Set aGreeksByMonthColl = Nothing
'                Next
'                Set aStressReportColl = Nothing
'NEXT_REPORT_GAMMA:
'                IncProgress pbProgress
'                Set aUnd = Nothing
'                If Not m_bRecalc Then Exit For
'            Next
'        End If
    Case REPORT_NONE
        Debug.Assert False
    End Select
    
    If m_bRecalc Then
        UpdateResGrid
        m_bRecalc = False
    End If
    
    m_bInProc = False
    Exit Sub
EH:
    If Not aUnd Is Nothing Then
        LogEvent EVENT_WARNING, aUnd.Symbol & ": Failed to calculate report"
        Resume Next
    End If
    
    LogEvent EVENT_ERROR, "Failed to run report: " & Err.Description
    m_bInProc = False
    m_bRecalc = False
End Sub

Private Function CheckPos(aUnd As EtsMmReportsLib.MmRpUndAtom) As Boolean
    On Error Resume Next
    CheckPos = False
    With aUnd.Pos
        CheckPos = .PosDailyPrevDateBuy <> 0 Or .PosDailyPrevDateSell <> 0 Or _
            .PosDailyTodayBuy <> 0 Or .PosDailyTodaySell Or _
            .QtyDailyPrevDateBuy <> 0 Or .QtyDailyPrevDateSell <> 0 Or _
            .QtyDailyTodayBuy <> 0 Or .QtyDailyTodaySell
    End With
    CheckPos = CheckPos Or aUnd.Opt.Count > 0
End Function

Private Sub GetRates(aUnd As EtsMmReportsLib.MmRpUndAtom, RateValues() As Double, RateDates() As Double, _
                                                          HTBRates() As Double, HTBDates() As Double)
    Dim bUseMidRates As Boolean, cPosThreshold@, dPos#
    Dim lRatesCount As Long
    
    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    Else
        bUseMidRates = True
    End If
    
    If (g_Main.Rates.Count = 0) Then
        lRatesCount = 0
    Else
        lRatesCount = g_Main.Rates.Count - 1
    End If
    
    ReDim RateValues(0 To lRatesCount)
    ReDim RateDates(0 To lRatesCount)
    
    Dim Idx As Long: Idx = 0
    Dim aRateAtom As EtsGeneralLib.EtsRateAtom
    For Each aRateAtom In g_Main.Rates
        If bUseMidRates Then
            RateValues(Idx) = aRateAtom.NeutralRate
        Else
            RateValues(Idx) = IIf(dPos < 0, aRateAtom.ShortRate, aRateAtom.LongRate)
        End If
        
        RateDates(Idx) = CDbl(aRateAtom.Period) / 365#
        Idx = Idx + 1
    Next
    
    ReDim HTBRates(0 To 0)
    ReDim HTBDates(0 To 0)
    
    HTBRates(0) = BAD_DOUBLE_VALUE
    
    Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
    If bIsHTBRatesExist Then
        
        If (g_UnderlyingAll(aUnd.ID).HTBRates.Count = 0) Then
            lRatesCount = 0
        Else
            lRatesCount = g_UnderlyingAll(aUnd.ID).HTBRates.Count - 1
        End If
        
        ReDim HTBRates(0 To lRatesCount)
        ReDim HTBDates(0 To lRatesCount)
        
        Idx = 0
        For Each aRateAtom In g_UnderlyingAll(aUnd.ID).HTBRates
            HTBRates(Idx) = IIf(dPos < 0, aRateAtom.ShortRate, aRateAtom.LongRate)
            HTBDates(Idx) = CDbl(aRateAtom.Period) / 365#
            
            Idx = Idx + 1
        Next
   End If
End Sub

Private Sub InitCombos()
    On Error Resume Next
    Dim i&
    
    With cbxZoom
        .Clear
        
        .AddItem "Thumbnail", 0
        .ItemData(0) = zmThumbnail
        
        .AddItem "Two Pages", 1
        .ItemData(1) = zmTwoPages
        
        .AddItem "Whole Page", 2
        .ItemData(2) = zmWholePage
        
        .AddItem "Page Width", 3
        .ItemData(3) = zmPageWidth
        
        .AddItem "Stretch", 4
        .ItemData(4) = zmStretch
    
        .AddItem "25%", 5
        .ItemData(5) = zmPercentage
        
        .AddItem "50%", 6
        .ItemData(6) = zmPercentage
    
        .AddItem "75%", 7
        .ItemData(7) = zmPercentage
    
        .AddItem "100%", 8
        .ItemData(8) = zmPercentage
    
        .AddItem "150%", 9
        .ItemData(9) = zmPercentage
    
        .AddItem "200%", 10
        .ItemData(10) = zmPercentage
    
        .AddItem "400%", 11
        .ItemData(11) = zmPercentage
        
        For i = 0 To .ListCount - 1
            If .ItemData(i) = vsPrinter.ZoomMode Then
                If vsPrinter.ZoomMode <> zmPercentage Then
                    .ListIndex = i
                    Exit For
                ElseIf val(.List(i)) = Round(vsPrinter.Zoom) Then
                    .ListIndex = i
                    Exit For
                End If
            End If
        Next
        If .ListIndex < 0 Then .ListIndex = 2
    End With
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    With vsPrinter
        .ZoomMode = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "PrintZoomMode", .ZoomMode)
        If .ZoomMode = zmPercentage Then
            .Zoom = val(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PrintZoom", Trim$(Str$(.Zoom))))
        End If
    
        .PaperSize = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "PrintPaperSize", .PaperSize)
        .Orientation = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "PrintOrientation", .Orientation)
        .MarginLeft = val(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PrintMarginLeft", Trim$(Str$(.MarginLeft))))
        .MarginRight = val(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PrintMarginRight", Trim$(Str$(.MarginRight))))
        .MarginTop = val(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PrintMarginTop", Trim$(Str$(.MarginTop))))
        .MarginBottom = val(g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PrintMarginBottom", Trim$(Str$(.MarginBottom))))
    End With
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    With vsPrinter
        g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "PrintZoomMode", .ZoomMode
        g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "PrintZoom", Trim$(Str$(.Zoom))
        
        g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "PrintPaperSize", .PaperSize
        g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "PrintOrientation", .Orientation
        g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "PrintMarginLeft", Trim$(Str$(.MarginLeft))
        g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "PrintMarginRight", Trim$(Str$(.MarginRight))
        g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "PrintMarginTop", Trim$(Str$(.MarginTop))
        g_aUserXMLParams.SetStringValue APP_XML_KEY & "Settings", "PrintMarginBottom", Trim$(Str$(.MarginBottom))
    End With
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&, sSection$
    
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height

    ' common info
    For i = PFC_REPORT To PFC_LAST_COLUMN
        aStorage.SetLongValue "ReportsFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next
    
    With vsPrinter
        sSection = "Settings" & sKey
        aStorage.SetLongValue sSection, "PrintZoomMode", .ZoomMode
        aStorage.SetLongValue sSection, "PrintZoom", Trim$(Str$(.Zoom))
        
        aStorage.SetLongValue sSection, "PrintPaperSize", .PaperSize
        aStorage.SetLongValue sSection, "PrintOrientation", .Orientation
        aStorage.SetLongValue sSection, "PrintMarginLeft", Trim$(Str$(.MarginLeft))
        aStorage.SetLongValue sSection, "PrintMarginRight", Trim$(Str$(.MarginRight))
        aStorage.SetLongValue sSection, "PrintMarginTop", Trim$(Str$(.MarginTop))
        aStorage.SetLongValue sSection, "PrintMarginBottom", Trim$(Str$(.MarginBottom))
    End With
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub SaveToPDF(sFilePath As String)
    On Error GoTo EH
    vsPdfCtrl.ConvertDocument vsPrinter, sFilePath
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub
Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&, sSection$
    
    If Len(sKey) > 0 Then sKey = "." & sKey
        
    ' common info
    For i = PFC_REPORT To PFC_LAST_COLUMN
        m_nFilter(i) = aStorage.GetLongValue("ReportsFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next
    
    m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
    m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
    m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
    m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
    
    FilterUpdate
    
    With vsPrinter
        sSection = "Settings" & sKey
        .ZoomMode = aStorage.GetLongValue(sSection, "PrintZoomMode", .ZoomMode)
        .Zoom = val(aStorage.GetLongValue(sSection, "PrintZoom", Trim$(Str$(.Zoom))))
    
        .PaperSize = aStorage.GetLongValue(sSection, "PrintPaperSize", .PaperSize)
        .Orientation = aStorage.GetLongValue(sSection, "PrintOrientation", .Orientation)
        .MarginLeft = val(aStorage.GetLongValue(sSection, "PrintMarginLeft", Trim$(Str$(.MarginLeft))))
        .MarginRight = val(aStorage.GetLongValue(sSection, "PrintMarginRight", Trim$(Str$(.MarginRight))))
        .MarginTop = val(aStorage.GetLongValue(sSection, "PrintMarginTop", Trim$(Str$(.MarginTop))))
        .MarginBottom = val(aStorage.GetLongValue(sSection, "PrintMarginBottom", Trim$(Str$(.MarginBottom))))
    End With
    
    With cbxZoom
        For i = 0 To .ListCount - 1
            If .ItemData(i) = vsPrinter.ZoomMode Then
                If vsPrinter.ZoomMode <> zmPercentage Then
                    .ListIndex = i
                    Exit For
                ElseIf val(.List(i)) = Round(vsPrinter.Zoom) Then
                    .ListIndex = i
                    Exit For
                End If
            End If
        Next
        If .ListIndex < 0 Then .ListIndex = 2
    End With
    
    Refresh
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Private Sub vsPrinter_AfterUserPage()
    AdjustState
End Sub

Private Sub vsPrinter_Error()
    ShowPrinterErrorMsgBox
End Sub

Private Sub vsPrinter_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyOrder(KeyCode, Shift) Then
        m_bKeyDown(GT_REPORTS_FILTER) = True
    End If
End Sub

Private Sub vsPrinter_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_REPORTS_FILTER) Then
        m_bKeyDown(GT_REPORTS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_REPORTS_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
        End Select
    End If
End Sub

Private Sub vsPrinter_NewPage()
    On Error Resume Next
    SetReportName
    vsPrinter.Text = vbCrLf & vbCrLf
    SetReportFilter
    vsPrinter.Text = vbCrLf
End Sub

Private Sub CopyFilters(ByVal bToPrev As Boolean)
    On Error Resume Next
    Dim i&
    
    If bToPrev Then
        For i = PFC_REPORT To PFC_LAST_COLUMN
            m_nFilterPrev(i) = m_nFilter(i)
        Next
        m_dMaxStrikeDifPrev = CDbl(fgFlt.TextMatrix(1, PFC_MAX_STRIKE_DISTANCE))
    Else
        For i = PFC_REPORT To PFC_LAST_COLUMN
            If m_nFilterPrev(i) <> BAD_LONG_VALUE Then m_nFilter(i) = m_nFilterPrev(i)
        Next
        fgFlt.TextMatrix(1, PFC_MAX_STRIKE_DISTANCE) = m_dMaxStrikeDifPrev
    End If
End Sub

Private Sub FilterUpdateChangedStatus()
    On Error Resume Next
    Dim i&, bSetHint As Boolean
    
    bSetHint = False
    
    With fgFlt
        m_GridLock(GT_REPORTS_FILTER).LockRedraw
        
        For i = PFC_REPORT To PFC_LAST_COLUMN
            If m_nFilter(i) <> m_nFilterPrev(i) Then
                .Cell(flexcpFontBold, 1, i) = True
                bSetHint = True
            Else
                .Cell(flexcpFontBold, 1, i) = False
            End If
        Next
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_REPORTS_FILTER).UnlockRedraw
        SetRefreshHint bSetHint
    End With
End Sub

Public Sub FilterRevert()
    On Error Resume Next
    CopyFilters False
    FilterUpdate
    FilterUpdateChangedStatus
    AdjustState
End Sub

Public Function FilterRevertEnabled() As Boolean
    On Error Resume Next
    Dim i&, bCanRevert As Boolean: bCanRevert = False
    
    If Not m_bInProc Then
        bCanRevert = False
        For i = PFC_REPORT To PFC_LAST_COLUMN
            If m_nFilter(i) <> m_nFilterPrev(i) And m_nFilterPrev(i) <> BAD_LONG_VALUE Then
                bCanRevert = True
                Exit For
            End If
        Next
    End If
    
    FilterRevertEnabled = bCanRevert
End Function

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub ShowPrinterErrorMsgBox()
Dim iError As Integer
    With vsPrinter
        Select Case .Error
        Case 0
            iError = 0
        Case vperControlIncapable
            iError = CLng(vperControlIncapable)
        Case vperCantAccessPrinter
            gCmn.CantAccessPrinterErrorMsgBox
        Case Else
            gCmn.ErrorMsgBox m_frmOwner, .ErrorDescription
        End Select
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Printer Error:" & CStr(.Error) & " " & .ErrorDescription, m_frmOwner.GetCaption
    End With
    
End Sub

Private Function GetOptionRoot(ByVal Symbol As String) As String
   ' GetOptionRoot = IIf(Len(Symbol) <= 2, Symbol, Mid$(Symbol, 1, Len(Symbol) - 2))
    If InStr(1, Symbol, "/") = 0 Then
    GetOptionRoot = IIf(Len(Symbol) <= 2, Symbol, Mid$(Symbol, 1, Len(Symbol) - 2))
    Else
        GetOptionRoot = Left(Symbol, InStr(1, Symbol, "/") + 2)
    End If
End Function

Private Sub HandleGridDblClick()
    On Error Resume Next
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
        End If
    End If
End Sub





