VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlGreeksView 
   ClientHeight    =   7440
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   10515
   KeyPreview      =   -1  'True
   ScaleHeight     =   7440
   ScaleWidth      =   10515
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1800
      Top             =   4440
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
      Height          =   2055
      Left            =   0
      TabIndex        =   1
      Top             =   780
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator1 
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
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator3 
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
      Begin VB.Menu mnuCtxSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxAscending 
         Caption         =   "Sort Ascending"
      End
      Begin VB.Menu mnuCtxDescending 
         Caption         =   "Sort Descending"
      End
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxEditCV 
         Caption         =   "Edit Custom Variable..."
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
      Begin VB.Menu mnuCtxGridLayoutParameters 
         Caption         =   "Grid Parameters ..."
      End
   End
End
Attribute VB_Name = "ctlGreeksView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const ROW_TMVAL_TOTAL& = 1&
Private Const ROW_DELTA_TOTAL& = 2&
Private Const ROW_DELTA1_TOTAL& = 3&
Private Const ROW_GAMMA_TOTAL& = 4&
Private Const ROW_GAMMA1_TOTAL& = 5&
Private Const ROW_VEGA_TOTAL& = 6&

Private Const MIN_MONEYNESS As Long = -60
Private Const MAX_MONEYNESS As Long = 160

Public Event OnSetCaption()
Public Event OnStateChange()

Private m_gdFlt As New clsGridDef
Private m_gdUnd As New clsGridDef
Private WithEvents geUnd As clsGridEx
Attribute geUnd.VB_VarHelpID = -1
Private m_iSeriesOrder As SortSettings

Private m_gdUndRows As clsGridDef

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
'Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
Private VolaSource As VolatilitySourcesLib.VolatilitySource
Private TradeChannel As clsTradeChannel

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label
Public lblValue As VB.Label
Public WithEvents imgStop As VB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As VB.Image

Private m_Grp As EtsMmRisksLib.MmRvGrpAtom
Private m_Und As EtsMmRisksLib.MmRvUndColl

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bRecalc As Boolean
Private m_bLastQuoteReqNow As Boolean

Private m_QuoteRequests As New clsGvRequestColl
Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Public m_frmOwner As Form

Private m_sCurrentOriginalText As String
Private m_nFilter(GFC_GROUP To GFC_COLUMN_COUNT) As Long

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_nParamsCount As Long

''''''''''''''''''''''''''''''''''''''''''''''''''''''''
' Custom variables
Private m_aContext As CVLib.cvRTContext
Private m_nContextID As Long

Private m_nTotalsLevelID As Long
Private m_nUndLevelID As Long
Private m_nPosLevelID As Long
Private m_nGroupTotalsLevelID As Long
Private m_nGroupUndLevelID As Long

Private m_bPosDataProviderInit As Boolean
Private m_bUndDataProviderInit As Boolean

' collections: key - idx of the grid column; value - ID of the custom variable
Private m_cvIDs As Collection
''''''''''''''''''''''''''''''''''''''''''''''''''''''''

' array with sorted underlyings
Private m_SortedUnd() As NodeSortItemType
Private m_nSortRow As Long
Private m_nSortCol As Long
Private m_bSortByStrKey As Boolean
Private m_iParamSort As SortSettings



Private m_bKeyDown(GT_GREEKS_FILTER To GT_GREEKS_UNDERLYING) As Boolean
Private m_GridLock(GT_GREEKS_FILTER To GT_GREEKS_UNDERLYING) As New clsGridLock

'Private m_bGroupRequest As Boolean
Private m_bShutDown As Boolean
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Public Function Init() As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    
'    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
'    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
'    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
'    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_GREEKS_FILTER) = False
    m_bKeyDown(GT_GREEKS_UNDERLYING) = False
    
    m_GridLock(GT_GREEKS_FILTER).Init fgFlt
    m_GridLock(GT_GREEKS_UNDERLYING).Init fgUnd
    
    m_iSeriesOrder = flexSortGenericAscending
    m_iParamSort = flexSortGenericAscending
    m_nSortCol = GUC_SYMBOL
    m_bSortByStrKey = True
    
    m_bPosDataProviderInit = False
    m_bUndDataProviderInit = False
    
    Set m_gdUndRows = New clsGridDef
    
    InitCVContext
    InitGreeksColumnsWithCV
    
    InitColumns
    InitGrids
    
    Set geUnd = New clsGridEx
    geUnd.Init fgUnd
    
    FormatFltGrid
    FormatFltColumns
    
    Set m_Grp = New EtsMmRisksLib.MmRvGrpAtom
    Set m_Und = New EtsMmRisksLib.MmRvUndColl
    
    Set m_Grp.DataGroupingTotal = New EtsMmRisksLib.MmRvGroupingTotalColl
    
    InitFltData
    
    Set VolaSource = g_VolaSource
    Set TradeChannel = g_TradeChannel
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    fgFlt.Col = GFC_GROUP
    fgFlt.Row = 1
    
    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    
    Init = True
    
    AdjustCaption
    AdjustState
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize greeks view."
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    m_nFilter(GFC_GROUP) = nType
    m_nFilter(GFC_VALUE) = nValue
    FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdateAll()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
        
        If m_nFilter(GFC_GROUP) <> TYPE_UNDERLYING _
            And m_nFilter(GFC_GROUP) <> TYPE_GROUP _
            And m_nFilter(GFC_GROUP) <> TYPE_TRADER _
            And m_nFilter(GFC_GROUP) <> TYPE_STRATEGY _
            And m_nFilter(GFC_GROUP) <> TYPE_ALL Then
        
            m_nFilter(GFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(GFC_VALUE) = 0
        End If
        
        If m_nFilter(GFC_GROUP) = TYPE_ALL Then
            m_nFilter(GFC_VALUE) = -1
        End If
        
        .TextMatrix(1, GFC_GROUP) = m_nFilter(GFC_GROUP)
        
        FilterUpdateValue False

        .TextMatrix(1, GFC_TYPE) = m_nFilter(GFC_TYPE)
        
        FilterUpdateModel False
        
        .TextMatrix(1, GFC_MODE) = m_nFilter(GFC_MODE)

        .AutoSize 0, .Cols - 1, False, 100
        
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateValue(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim sComboList$, sValue$, aEnt As EtsGeneralLib.EtsMmEntityAtom, aUnd As EtsGeneralLib.UndAtom
    Dim nValue&, collEnt As EtsGeneralLib.EtsMmEntityAtom
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = ""
        
        Select Case m_nFilter(GFC_GROUP)
            Case TYPE_ALL
                sValue = "-1"
                nValue = -1
                sComboList = "#-1;<All>"
            
            Case TYPE_UNDERLYING
                sComboList = g_Params.UnderlyingComboList
                
                Set aUnd = g_Underlying(m_nFilter(GFC_VALUE))
                If Not aUnd Is Nothing Then
                    nValue = aUnd.ID
                    sValue = Trim$(Str$(nValue))
                    Set aUnd = Nothing
                End If
                
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
            
            Case TYPE_TRADER
                Dim aTrader As EtsGeneralLib.EtsTraderAtom
                
                For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = m_nFilter(GFC_VALUE) Then
                        nValue = aTrader.ID
                        sValue = Trim$(nValue)
                    End If
                Next
            
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
                
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(GFC_VALUE) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
            
            Case TYPE_STRATEGY
                Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
                
                sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
                If m_nFilter(GFC_VALUE) = NO_STRATEGY_ID Then
                    nValue = NO_STRATEGY_ID
                    sValue = NO_STRATEGY_NAME
                End If
                
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_nFilter(GFC_VALUE) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
                
        End Select
        
        If Len(sComboList) > 0 Then
            If nValue = 0 Then
                sComboList = "#0;<Select>" & sComboList
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(GFC_VALUE) = sComboList
        .TextMatrix(1, GFC_VALUE) = sValue
        m_nFilter(GFC_VALUE) = nValue
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, False, 100
        End If
        
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
    
        .TextMatrix(1, GFC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, False, 100
        End If
        
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_GREEKS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_GREEKS_UNDERLYING).CopyTo m_gdUnd
    g_DefGrids(GT_GREEKS_UND_ROWS).CopyTo m_gdUndRows
    
    m_nParamsCount = m_gdUndRows.GetVisibleCount
End Sub

Private Sub InitFltData()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
        
        .ColComboList(GFC_GROUP) = "#" & CStr(TYPE_UNDERLYING) & ";Underlying" & _
                                    "|#" & CStr(TYPE_GROUP) & ";Group" & _
                                    "|#" & CStr(TYPE_TRADER) & ";Trader" & _
                                    "|#" & CStr(TYPE_STRATEGY) & ";Strategy" & _
                                    "|#" & CStr(TYPE_ALL) & ";<All>"
        .TextMatrix(1, GFC_GROUP) = CStr(TYPE_UNDERLYING)
        m_nFilter(GFC_GROUP) = TYPE_UNDERLYING
        
        .ColComboList(GFC_VALUE) = "#0;<None>"
        .TextMatrix(1, GFC_VALUE) = "0"
        m_nFilter(GFC_VALUE) = 0
    
        .ColComboList(GFC_TYPE) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, GFC_TYPE) = "0"
        
        .ColComboList(GFC_MODE) = "#1;Expirations|#2;Moneyness"
        m_nFilter(GFC_MODE) = 1
        .TextMatrix(1, GFC_MODE) = m_nFilter(GFC_MODE)
    
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = GFC_COLUMN_COUNT
        
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
    
    With fgUnd
        .Rows = 1
        .Cols = 2
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = False
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSort
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
        .MergeCells = flexMergeFree
        .MergeCompare = flexMCExact
        .TabBehavior = flexTabControls
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
    
        nCols = GFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = GFC_GROUP To GFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_GREEKS_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = GFC_GROUP To GFC_LAST_COLUMN
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
        
        .AutoSize 0, .Cols - 1, False, 100
        
        m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&, aExp As EtsMmRisksLib.MmRvGroupingTotalAtom
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
    
        nCols = GUC_COLUMN_COUNT - 3 + m_Grp.DataGroupingTotal.Count
        .Cols = nCols + 1
        
        .ColHidden(GUC_KEY) = True
        .ColKey(GUC_KEY) = GUC_NONE
        .ColDataType(GUC_KEY) = flexDTString
        
        For i = GUC_SYMBOL To nCols
            .TextMatrix(0, i) = m_gdUnd.Col(i).Caption
            .ColFormat(i) = m_gdUnd.Col(i).Format
            .ColDataType(i) = m_gdUnd.Col(i).DataType
            .ColKey(i) = i
            .ColSort(i) = flexSortNone
        Next
        
        .TextMatrix(0, nCols) = m_gdUnd.Col(GUC_SYM_TOTAL).Caption
        .ColFormat(nCols) = m_gdUnd.Col(GUC_SYM_TOTAL).Format
        .ColDataType(nCols) = m_gdUnd.Col(GUC_SYM_TOTAL).DataType
        .ColKey(nCols) = GUC_SYM_TOTAL
        .ColSort(nCols) = flexSortNone
        
        i = GUC_VALUE + 1
        For Each aExp In m_Grp.DataGroupingTotal
            nIdx = IIf((i - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
            
            Select Case m_Grp.GroupingType
                Case enGroupingExpiry
                    .TextMatrix(0, i) = Format$(aExp.ExpiryMonth, "MMM YY")
                Case enGroupingMoneyness
                    Select Case aExp.Moneyness
                        Case MIN_MONEYNESS
                            .TextMatrix(0, i) = "< " & CStr(MIN_MONEYNESS + 10) & " %"
                        Case MAX_MONEYNESS
                            .TextMatrix(0, i) = "> " & CStr(MAX_MONEYNESS - 10) & " %"
                        Case Else
                            .TextMatrix(0, i) = Format$(aExp.Moneyness / 100, "0 %")
                    End Select
            End Select
            
            .ColFormat(i) = m_gdUnd.Col(nIdx).Format
            .ColDataType(i) = m_gdUnd.Col(nIdx).DataType
            
            .ColKey(i) = aExp.ExpiryMonth
            .ColSort(i) = flexSortNone
            i = i + 1
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols) = flexAlignCenterCenter
        
        .Row = 1
        .Col = 1
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub FormatUndColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont, aExp As EtsMmRisksLib.MmRvGroupingTotalAtom

    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw

        aFont.Name = m_gdUnd.Font.Name
        aFont.Size = m_gdUnd.Font.Size
        aFont.Bold = m_gdUnd.Font.Bold
        aFont.Italic = m_gdUnd.Font.Italic
        aFont.Strikethrough = m_gdUnd.Font.Strikethrough
        aFont.Underline = m_gdUnd.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = GUC_SYMBOL To GUC_VALUE
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(i).BackColor <> 0, m_gdUnd.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(i).ForeColor <> 0, m_gdUnd.Col(i).ForeColor, RGB(1, 1, 1))
        Next
        .Cell(flexcpBackColor, 1, nCols, nRows, nCols) = IIf(m_gdUnd.Col(GUC_SYM_TOTAL).BackColor <> 0, m_gdUnd.Col(GUC_SYM_TOTAL).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, nCols, nRows, nCols) = IIf(m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor <> 0, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor, RGB(1, 1, 1))
        
        i = GUC_VALUE + 1
        For Each aExp In m_Grp.DataGroupingTotal
            nIdx = IIf((i - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).BackColor <> 0, m_gdUnd.Col(nIdx).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).ForeColor <> 0, m_gdUnd.Col(nIdx).ForeColor, RGB(1, 1, 1))
            i = i + 1
        Next
        
        .Cell(flexcpFontBold, 1, nCols, nRows, nCols) = True
        .Cell(flexcpFontBold, 1, 1, m_nParamsCount, nCols) = True

        If m_gdUnd.HorizLinesVisible Then
            If m_gdUnd.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdUnd.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdUnd.GridLinesColor
        
        .BackColorSel = m_gdUnd.BackColorSel
        .ForeColorSel = m_gdUnd.ForeColorSel

        ShowSortImages
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsUpdateBackColor()
    On Error Resume Next
    Dim clrBackColor As OLE_COLOR, nStartRow&, nEndRow&, nRow&, nRows&, nUndID&, nNextUndID&, i&, nIdx&
    Dim bUseAltColor As Boolean
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        nStartRow = 1&
        bUseAltColor = True
        nRows = .Rows - 1
        nUndID = 0&
        
        For nRow = 1 To nRows
            nNextUndID = 0&
            nNextUndID = .RowData(nRow).Und.ID
            If nNextUndID = 0& Then nUndID = nNextUndID
            
            If nRow = nRows Or nUndID <> nNextUndID Then
                nEndRow = IIf(nRow <> nRows, nRow - 1, nRow)
                nUndID = nNextUndID
                bUseAltColor = Not bUseAltColor
                
                For i = GUC_SYMBOL To GUC_VALUE
                    clrBackColor = m_gdUnd.Col(i).BackColor
                    If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
                    .Cell(flexcpBackColor, nStartRow, i, nEndRow, i) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
                Next
                
                For i = GUC_VALUE + 1 To .Cols - 2
                    nIdx = IIf((i - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
                    clrBackColor = m_gdUnd.Col(nIdx).BackColor
                    If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
                    .Cell(flexcpBackColor, nStartRow, i, nEndRow, i) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
                Next
                
                clrBackColor = m_gdUnd.Col(GUC_SYM_TOTAL).BackColor
                If bUseAltColor Then clrBackColor = GetAlternateColor(clrBackColor)
                .Cell(flexcpBackColor, nStartRow, .Cols - 1, nEndRow, .Cols - 1) = IIf(clrBackColor <> 0, clrBackColor, RGB(1, 1, 1))
                
                nStartRow = nEndRow + 1
            End If
        Next
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    bMatched = False
    
    If Not aTrd Is Nothing Then
        ' futures & futures options trades are not supported yet
        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then Exit Function
        
        If aTrd.ContractType = enCtOption Then
            If aTrd.Opt.ExpiryOV < GetNewYorkTime Then Exit Function
        End If
        
        nValue = m_nFilter(GFC_VALUE)
        
        If nValue <> 0 Then
            Select Case m_nFilter(GFC_GROUP)
                Case TYPE_UNDERLYING
                    bMatched = (aTrd.UndID = nValue)
                    
                Case TYPE_GROUP
                    bMatched = Not g_UnderlyingGroup(nValue).Und(aTrd.UndID) Is Nothing
                    
                Case TYPE_TRADER
                    bMatched = (aTrd.Trader.ID = nValue)
                    
                Case TYPE_STRATEGY
                    bMatched = (aTrd.StrategyID = IIf(nValue > 0, nValue, 0))
                    
                Case TYPE_ALL
                    bMatched = True
                    
            End Select
        
            If bMatched Then
                nValue = m_nFilter(GFC_TYPE)
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
        End If
    End If

    CheckTradeFilter = bMatched
End Function

Private Sub ClearViewAndData(Optional ByVal bClearData As Boolean = True)
    On Error Resume Next
    ShowSortImages , , True
    
    fgUnd.Rows = 1
    fgUnd.Rows = m_nParamsCount + 1
    fgUnd.FrozenRows = 0

    If bClearData Then
        Set m_Grp = Nothing
        Set m_Und = Nothing
    
        Set m_Grp = New EtsMmRisksLib.MmRvGrpAtom
        Set m_Und = New EtsMmRisksLib.MmRvUndColl
    
        Set m_Grp.DataGroupingTotal = New EtsMmRisksLib.MmRvGroupingTotalColl

        m_QuoteRequests.Clear
    End If
    
    FormatUndGrid
    FormatUndColumns
    UnderlyingsShow False
    SetRefreshHint False
End Sub

Private Sub aParams_PriceProfilesChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    Dim aUnd As EtsMmHedgeLib.MmHsUndAtom
    
    If m_Grp.GroupType = 0 Or m_Grp.ID = 0 Then Exit Sub
    
    bUpdate = False
    On Error Resume Next
    
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
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
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
                Select Case Col
                    Case GFC_GROUP
                        m_nFilter(GFC_GROUP) = nValue
                        If m_nFilter(GFC_GROUP) <> TYPE_ALL Then
                            m_nFilter(GFC_VALUE) = 0
                            ClearViewAndData
                            FilterUpdateValue True
                            AdjustCaption
                        Else
                            m_nFilter(GFC_VALUE) = -1
                            FilterUpdateValue True
                            tmrShow.Enabled = True
                        End If
                        
                    Case GFC_VALUE
                        If m_nFilter(GFC_VALUE) = 0 Then
                            sValue = .ColComboList(Col)
                            sValue = Mid$(sValue, InStr(1, sValue, "|") + 1)
                            .ColComboList(Col) = sValue
                            .TextMatrix(Row, Col) = CStr(nValue)
                        End If
                        m_nFilter(GFC_VALUE) = nValue
                        .AutoSize 0, .Cols - 1, False, 100
                        tmrShow.Enabled = True
                        
                    Case GFC_TYPE
                        m_nFilter(GFC_TYPE) = nValue
                        .AutoSize 0, .Cols - 1, False, 100
                        tmrShow.Enabled = True
                        
                    Case GFC_MODE
                        m_nFilter(GFC_MODE) = nValue
                        .AutoSize 0, .Cols - 1, False, 100
                        tmrShow.Enabled = True
                        
                End Select
            Else
                .TextMatrix(1, Col) = m_nFilter(Col)
            End If
        End If
    End With
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_GREEKS_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_GREEKS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_GREEKS_FILTER) Then
        m_bKeyDown(GT_GREEKS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_GREEKS_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
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

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_GREEKS_FILTER
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgFlt
        If Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= GFC_GROUP And Col <= GFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            
            If Not Cancel Then Cancel = (Col = GFC_VALUE And m_nFilter(GFC_GROUP) = TYPE_ALL)
            If Not Cancel Then m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End If
    End With
End Sub

Private Sub fgUnd_DblClick()
    On Error Resume Next
    With fgUnd
        m_enMenuGrid = GT_GREEKS_UNDERLYING
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_GREEKS_UNDERLYING) = True
    End If
End Sub

Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_GREEKS_UNDERLYING) Then
        m_bKeyDown(GT_GREEKS_UNDERLYING) = False
        With fgUnd
            m_enMenuGrid = GT_GREEKS_UNDERLYING
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                mnuCtxTradeNew_Click
            
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

Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgUnd
            m_enMenuGrid = GT_GREEKS_UNDERLYING
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                .Row = m_nMenuGridRow
            End If
            If m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
                .Col = m_nMenuGridCol
            End If
            
            ShowPopup
        End With
    End If
End Sub

Private Sub frmLayout_OnGridChanged(ByVal enGridType As GridTypeEnum, ByVal nVarID As Long)
    On Error Resume Next
    SaveGridDefAsDefault m_gdUndRows, GT_GREEKS_UND_ROWS
    
    InitCVContext
    InitGreeksColumnsWithCV
End Sub

Private Sub geUnd_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    If Col = GUC_SYMBOL Then
        Order = flexSortCustom
    Else
        Order = flexSortNone
    End If
End Sub

Private Sub geUnd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    If Col = GUC_SYMBOL Then
        m_iSeriesOrder = IIf(m_iSeriesOrder = flexSortGenericAscending, flexSortGenericDescending, flexSortGenericAscending)
        Order = m_iSeriesOrder
        m_nSortCol = GUC_SYMBOL
        
        With fgUnd
            If .Rows > m_nParamsCount + 1 Then
                Dim nRow&, nCol&
                
                nRow = .Row
                nCol = .Col
                
                m_bSortByStrKey = True
                Erase m_SortedUnd
                ReDim m_SortedUnd(0 To m_Und.Count - 1)
                
                Dim aUnd As EtsMmRisksLib.MmRvUndAtom
                Dim Idx As Long
                
                For Each aUnd In m_Und
                    m_SortedUnd(Idx).StrKey = aUnd.Symbol
                    m_SortedUnd(Idx).ID = aUnd.ID
                    
                    Idx = Idx + 1
                Next
                
                UnderlyingsShow True
                UnderlyingsUpdateBadStatus
                TotalsUpdateBadStatus
                
                .Row = nRow
                .Col = nCol
            End If
        End With
    Else
        Order = flexSortCustom
    End If
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
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
        
    ElseIf m_bInProc Then
        m_bInProc = False
        AdjustState
    End If
End Sub

Private Sub mnuCtxAscending_Click()
    On Error Resume Next
    SortByParameter True
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_GREEKS_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_GREEKS_UNDERLYING
            g_ClipMgr.CopyGridToClipboard fgUnd

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, ByRef dPrice#)
    On Error Resume Next
    Dim aRowData As clsGvRowData
    
    Select Case True
        Case m_enMenuGrid = GT_GREEKS_UNDERLYING And m_nMenuGridRow > 6
            Do While aUnd Is Nothing And m_nMenuGridRow > 4
                Set aRowData = fgUnd.RowData(m_nMenuGridRow)
                
                If Not aRowData Is Nothing Then
                    If Not aRowData.Und Is Nothing Then
                        Set aUnd = g_Underlying(aRowData.Und.ID)
                    Else
                        Set aUnd = g_Underlying(aRowData.SynthUndID)
                    End If
                
                    Set aRowData = Nothing
                End If
                
                m_nMenuGridRow = m_nMenuGridRow - 1
            Loop
        
        Case m_enMenuGrid = GT_GREEKS_FILTER And m_nFilter(GFC_GROUP) = TYPE_UNDERLYING
            Set aUnd = g_Underlying(m_nFilter(GFC_VALUE))
            
    End Select

    If bIsStock And Not aUnd Is Nothing Then
        dPrice = m_Und(aUnd.ID).PriceBid
        If dPrice <= 0# Then dPrice = m_Und(aUnd.ID).PriceLast
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, aUnd, dPrice
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, True, 100, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, Nothing, True, 10, dPrice
'    End If
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Greeks View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxDescending_Click()
    On Error Resume Next
    SortByParameter False
End Sub

Private Sub SortByParameter(ByVal bAsc As Boolean)
    On Error Resume Next
    Dim aRowData As clsGvRowData
    Dim nUndRow As Long, nParamRow As Long
    Dim Idx As Long
    
    Erase m_SortedUnd
    ReDim m_SortedUnd(0 To m_Und.Count - 1)
    
    m_iParamSort = IIf(bAsc, flexSortGenericAscending, flexSortGenericDescending)
    If fgUnd.ColDataType(m_nMenuGridCol) = flexDTDouble Or fgUnd.ColDataType(m_nMenuGridCol) = flexDTLong Then
        m_bSortByStrKey = False
    Else
        m_bSortByStrKey = True
    End If
    
    nUndRow = m_nParamsCount + 1
    nParamRow = nUndRow + ((m_nMenuGridRow - 1) Mod m_nParamsCount)
    
    ShowSortImages nParamRow, m_nMenuGridCol
    
    For Idx = 0 To UBound(m_SortedUnd)
        Set aRowData = fgUnd.RowData(nUndRow)
        
        m_SortedUnd(Idx).ID = aRowData.Und.ID
        If m_bSortByStrKey Then
            m_SortedUnd(Idx).StrKey = fgUnd.TextMatrix(nParamRow, m_nMenuGridCol)
        Else
            m_SortedUnd(Idx).NumKey = fgUnd.TextMatrix(nParamRow, m_nMenuGridCol)
        End If
        
        nParamRow = nParamRow + m_nParamsCount
        nUndRow = nUndRow + m_nParamsCount
        
        Set aRowData = Nothing
    Next
    
    UnderlyingsShow True
End Sub

Private Sub mnuCtxEditCV_Click()
    On Error Resume Next
    Dim sVarCaption As String: sVarCaption = STR_NA
    
    Dim aRowData As clsGvRowData
    Set aRowData = fgUnd.RowData(m_nMenuGridRow)
    If Not aRowData Is Nothing Then
        If aRowData.ParamIdx <> BAD_LONG_VALUE Then
            sVarCaption = m_gdUndRows.Col(aRowData.ParamIdx).Caption
        End If
    End If
    
    
    Dim frmVarEditor As frmCustomVariableEditor
    Set frmVarEditor = New frmCustomVariableEditor

    frmVarEditor.EditVariable ContextID, PosLevelID, BAD_LONG_VALUE, m_frmOwner, , , sVarCaption
    
    Set frmVarEditor = Nothing
    
End Sub

Private Sub mnuCtxGridLayoutParameters_Click()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_GREEKS_UND_ROWS, GUR_TIME_VALUE, g_GUR_LAST_ROW, m_gdUndRows, m_frmOwner
    End If
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
'    gCmn.ErrorMsgBox m_frmOwner, "Greeks View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error GoTo EH
    If m_bInProc Then Exit Sub
    Dim nUndID&, nID&, nColIdx&, bCheckSynth As Boolean
    Dim aRowData As clsGvRowData

    m_bInProc = True
    nUndID = 0
    nID = 0
    
    If m_enMenuGrid = GT_GREEKS_UNDERLYING Then
        If m_nMenuGridRow > m_nParamsCount And m_nMenuGridRow < m_nMenuGridRows Then
            On Error Resume Next
            bCheckSynth = True
            
            Do While nUndID = 0 And m_nMenuGridRow > 4
                Set aRowData = fgUnd.RowData(m_nMenuGridRow)
                
                If Not aRowData Is Nothing Then
                    If Not aRowData.Und Is Nothing Then
                        nUndID = aRowData.Und.ID
                    Else
                        nUndID = IIf(bCheckSynth, aRowData.SynthUndID, 0)
                    End If
                    nID = nUndID
                    Set aRowData = Nothing
                Else
                    bCheckSynth = False
                End If
                
                m_nMenuGridRow = m_nMenuGridRow - 1
            Loop
            On Error GoTo EH
        End If
        
    ElseIf m_enMenuGrid = GT_GREEKS_FILTER Then
        If m_nFilter(GFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_nFilter(GFC_VALUE)
            nID = nUndID
        End If
    End If

    m_bInProc = False
    If nUndID <> 0 Then frmTradeEditor.NewTrade nID, nUndID, 1, m_frmOwner
    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_GREEKS_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Greeks Filter", "Greeks Filter", fgFlt
'
'        Case GT_GREEKS_UNDERLYING
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Greeks", "Greeks", fgUnd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_GREEKS_FILTER
            CustomizeFltGridLayout

        Case GT_GREEKS_UNDERLYING
            CustomizeUndGridLayout
        
    End Select
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    'mnuCtxTradeNew                 "New Trade..."
    'mnuCtxOrderNewStock            "New Stock Order..."
    'mnuCtxOrderNewOption           "New Option Order..."
    'mnuCtxRefresh                  "Refresh"
    'mnuCtxCopy                     "Copy Grid"
    'mnuCtxPrint                    "Print Grid..."
    'mnuCtxGridLayout               "Grid Layout..."
    
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    Select Case m_enMenuGrid
        Case GT_GREEKS_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nFilter(GFC_GROUP) = TYPE_UNDERLYING And m_nFilter(GFC_VALUE) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            
            mnuCtxGridLayoutParameters.Enabled = False
            
            mnuCtxAscending.Enabled = False
            mnuCtxAscending.Caption = "Sort Ascending"
            
            mnuCtxDescending.Enabled = False
            mnuCtxDescending.Caption = "Sort Descending"
            
            PopupMenu mnuCtx
        
        Case GT_GREEKS_UNDERLYING
            mnuCtxTradeNew.Enabled = (Not m_bInProc _
                                    And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                    And m_nMenuGridRow > m_nParamsCount And m_nMenuGridRow < m_nMenuGridRows)

            mnuCtxGridLayoutParameters.Enabled = True
            
            ''''' Sorting '''''
            Dim aRowData As clsGvRowData
            Dim sParamCaption As String: sParamCaption = STR_NA
            Dim bIsSortEnabled As Boolean
            
            Set aRowData = fgUnd.RowData(m_nMenuGridRow)
            If Not aRowData Is Nothing Then
                If aRowData.ParamIdx <> BAD_LONG_VALUE Then sParamCaption = m_gdUndRows.Col(aRowData.ParamIdx).Caption
            End If
            
            bIsSortEnabled = CBool(sParamCaption <> STR_NA And m_Und.Count > 0 And m_nMenuGridCol > 2)
            
            mnuCtxAscending.Enabled = bIsSortEnabled
            mnuCtxAscending.Caption = "Sort " & IIf(bIsSortEnabled, sParamCaption & " ", "") & "Ascending"
            
            mnuCtxDescending.Enabled = bIsSortEnabled
            mnuCtxDescending.Caption = "Sort " & IIf(bIsSortEnabled, sParamCaption & " ", "") & "Descending"
            '''''''''''''''''''
            
            PopupMenu mnuCtx
    End Select
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
    
    If m_bDataLoad Then Exit Sub
    
    Screen.MousePointer = vbArrow
    DoEvents
    
    If m_Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
        m_bLastQuoteReqNow = False
        m_QuoteRequests.Clear
    End If
    
    ClearViewAndData
    AdjustCaption
    
    If m_nFilter(GFC_GROUP) <> 0 And m_nFilter(GFC_VALUE) <> 0 Then
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
        
        If UnderlyingsLoad Then
            AdjustCaption
                    
            lblProcess.Caption = "Last quotes request..."
            lblProcess.Refresh
            
            RequestLastQuotes
            SetRefreshHint False
        Else
            If m_nFilter(GFC_GROUP) = TYPE_ALL Then m_nFilter(GFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(GFC_VALUE) = 0
            ClearViewAndData
            FilterUpdateAll
            AdjustCaption
            AdjustState
            
            imgStop.Visible = False
            imgStopDis.Visible = False
            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
        End If
    
    End If
    
    Screen.MousePointer = vbDefault
End Sub

Private Function UnderlyingsLoad() As Boolean
    On Error GoTo EH
    UnderlyingsLoad = False
    If m_bInProc Then Exit Function
    
    Dim aGrpDataProvider As CVLib.IcvDataProvider
    Dim aUndDataProvider As CVLib.IcvDataProvider
    Dim aPosDataProvider As CVLib.IcvDataProvider
    
    Dim lUnsupported As Long
    Dim infoSysVars() As CVLib.cvInfo
    Dim infoSysFuncs() As CVLib.cvInfo
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, nBS&, sKey$, nQty&, nQtyInShares&
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aGUnd As EtsGeneralLib.UndAtom
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom, aReq As clsGvRequestAtom
    
    m_bInProc = True
    m_bDataLoad = True
    AdjustState
    
    m_Grp.ID = m_nFilter(GFC_VALUE)
    m_Grp.GroupType = m_nFilter(GFC_GROUP)
    m_Grp.GroupingType = m_nFilter(GFC_MODE)
    
    If m_Grp.ID <> 0 And m_Grp.GroupType <> 0 Then
        Select Case m_Grp.GroupType
            Case TYPE_UNDERLYING
                Set aGUnd = g_Underlying(m_Grp.ID)
                If Not aGUnd Is Nothing Then
                    m_Grp.Name = aGUnd.Symbol
                    Set aGUnd = Nothing
                Else
                    m_bDataLoad = False
                End If
                
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom

                Set aUndGroup = g_UnderlyingGroup(m_Grp.ID)
                If Not aUndGroup Is Nothing Then
                    m_Grp.Name = aUndGroup.Name
                    Set aUndGroup = Nothing
                Else
                    m_bDataLoad = False
                End If
            
            Case TYPE_TRADER
                Dim aTrader As EtsGeneralLib.EtsTraderAtom
                
                Set aTrader = g_Trader(m_Grp.ID)
                If Not aTrader Is Nothing Then
                    m_Grp.Name = aTrader.Name
                    Set aTrader = Nothing
                Else
                    m_bDataLoad = False
                End If
            
            Case TYPE_STRATEGY
                Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
                
                If m_Grp.ID > 0 Then
                    Set aStrategy = g_Strategy(m_Grp.ID)
                    If Not aStrategy Is Nothing Then
                        m_Grp.Name = aStrategy.Name
                        Set aStrategy = Nothing
                    Else
                        m_bDataLoad = False
                    End If
                Else
                    m_Grp.Name = NO_STRATEGY_NAME
                End If
            
            Case TYPE_ALL
                m_Grp.Name = "<All>"
            
            Case Else
                m_bDataLoad = False
        End Select
        
        If m_bDataLoad Then
        
            Set aGrpDataProvider = m_Grp
        
            If m_aContext.Halted = False Then
                m_aContext.GetSystemEnvironment m_nTotalsLevelID, infoSysVars, infoSysFuncs
                lUnsupported = aGrpDataProvider.Check(infoSysVars, infoSysFuncs)
                If lUnsupported > 0 Then
                    m_aContext.Halt
                    gCmn.ErrorMsgBox m_frmOwner, "Custom variables functionality disabled due incorrect version of data provider"
                End If
            End If
            
            If m_aContext.Halted = False Then
                Set m_Grp.cvRTContext = m_aContext
                m_Grp.cvPoolID = m_aContext.CreateCVPool(aGrpDataProvider, m_nTotalsLevelID)
            End If
        
        
            For Each aTrd In g_TradeChannel.Trades.FilterTradesCV(-1, 9, 0, Nothing, False, g_Params.DisplayZeroPositions)
                If CheckTradeFilter(aTrd) Then
                    Set aUnd = m_Und(aTrd.UndID)
                    If aUnd Is Nothing Then
                        Set aUnd = New EtsMmRisksLib.MmRvUndAtom
                        Set aUnd.DataGrouping = New EtsMmRisksLib.MmRvGroupingColl
                        
                        aUnd.ID = aTrd.Und.ID
                        aUnd.ContractType = aTrd.Und.UndType
                        aUnd.Symbol = aTrd.Und.Symbol
                        aUnd.GroupingType = m_nFilter(GFC_MODE)
                        
                        Set aUnd.Dividend = aTrd.Und.Dividend
                        
                        If aUnd.ContractType = enCtIndex Then
                            Set aUnd.BasketIndex = g_Index(aUnd.ID)
                            aUnd.Yield = aTrd.Und.Yield
                        End If
                        
                        aUnd.IsAmerican = aTrd.Und.IsAmerican
                        aUnd.IsHTB = aTrd.Und.IsHTB
                        aUnd.Skew = aTrd.Und.Skew
                        aUnd.Kurt = aTrd.Und.Kurt
                        
                        aUnd.HasSynthetic = aTrd.Und.HaveSyntheticRoots
                        Set aUnd.SynthRoots = aTrd.Und.SyntheticRoots
                        Set aUnd.UndPriceProfile = aTrd.Und.UndPriceProfile
                        Set aUnd.OptPriceProfile = aTrd.Und.OptPriceProfile
                        
                        InitVola aUnd
                        
                        sKey = CStr(aUnd.ContractType) & "_" & aUnd.Symbol
                        If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add aUnd, Nothing, sKey
                        
                        m_Und.Add aUnd.ID, aUnd.Symbol, aUnd
                    End If
                    
                    
                    Set aUndDataProvider = aUnd
                    Set aGrpDataProvider = m_Grp
                    
                    If m_bUndDataProviderInit = False And m_aContext.Halted = False Then
                        m_aContext.GetSystemEnvironment m_nUndLevelID, infoSysVars, infoSysFuncs
                        lUnsupported = aUndDataProvider.Check(infoSysVars, infoSysFuncs)
                        If lUnsupported > 0 Then
                            m_aContext.Halt
                            gCmn.ErrorMsgBox Nothing, "Custom variables functionality disabled due incorrect version of data provider"
                        End If
                        m_bUndDataProviderInit = True
                    End If
                    
                    If m_aContext.Halted = False And aUnd.cvPoolID = BAD_LONG_VALUE Then
                        Set aUnd.cvRTContext = m_aContext
                        aUnd.cvPoolID = m_aContext.CreateCVPool(aUndDataProvider, m_nUndLevelID)
                    End If
                    
                    
                    DoEvents
                    If Not m_bDataLoad Then Exit For
                    
                    nBS = IIf(aTrd.IsBuy, 1, -1)
                    nQty = aTrd.Quantity * nBS
                    nQtyInShares = nQty * aTrd.LotSize
                    
                    If aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtIndex Or aTrd.ContractType = enCtStock Then
                        Set aPos = aUnd.Pos(aTrd.ContractID)
                        If aPos Is Nothing Then
                            Set aPos = New EtsMmRisksLib.MmRvPosAtom
                            aPos.ID = aTrd.ContractID
                            
                            aPos.ContractType = aTrd.ContractType
                            aPos.Symbol = aTrd.Symbol
                            If aPos.ContractType = enCtOption Then
                                aPos.PriceClose = aTrd.Opt.PriceClose
                                aPos.LotSize = aTrd.LotSize
                                aPos.OptType = IIf(aTrd.Opt.OptType <> enOtPut, enOtCall, enOtPut)
                                aPos.Expiry = aTrd.Opt.Expiry
                                aPos.ExpiryOV = aTrd.Opt.ExpiryOV
                                aPos.TradingClose = aTrd.Opt.TradingClose
                                aPos.Strike = aTrd.Opt.Strike
                                aPos.PriceTheo = aTrd.Opt.PriceTheoClose
                                aPos.OptionRootID = aTrd.OptRootID
                                aPos.IsFlex = IIf(aTrd.Opt.Flex = 1, True, False)
                            Else
                                aPos.LotSize = aTrd.LotSize
                                aPos.PriceClose = aTrd.Und.PriceClose
                            End If
                            aPos.Visible = True
                            
                            aUnd.Pos.Add aPos.ID, aPos.Symbol, aPos
                        
'                            If aUnd.HasSynthetic And Not aUnd.SynthRoots Is Nothing Then
                            If aUnd.HasSynthetic And Not aUnd.SynthRoots Is Nothing And aPos.ContractType = enCtOption Then
                                Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
                                
                                If Not aSynthRoot Is Nothing Then
                                    aPos.IsSynthetic = True
                                    If aPos.SynthGreeks Is Nothing Then Set aPos.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl
                                    If aUnd.SynthGreeks Is Nothing Then Set aUnd.SynthGreeks = New EtsMmRisksLib.MmRvSynthGreeksColl
                                    
                                    If aSynthRoot.CashValue > 0# Then
                                        If m_Und(USD_ID) Is Nothing Then
                                            Set aSynthUnd = New EtsMmRisksLib.MmRvUndAtom
                                            
                                            aSynthUnd.ID = USD_ID
                                            aSynthUnd.ContractType = enCtStock
                                            aSynthUnd.Symbol = USD_SYMBOL
                                            
'                                            aSynthUnd.Pos = BAD_DOUBLE_VALUE
                                            
                                            aSynthUnd.PriceAsk = 1#
                                            aSynthUnd.PriceBid = 1#
                                            aSynthUnd.PriceLast = 1#
                                                
                                            Set aSynthUnd.UndPriceProfile = aUnd.UndPriceProfile
                                            Set aSynthUnd.OptPriceProfile = aUnd.OptPriceProfile
                                            
                                            Set aSynthUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                                            
                                            m_Und.Add aSynthUnd.ID, aSynthUnd.Symbol, aSynthUnd
                                            Set aSynthUnd = Nothing
                                        End If

                                        Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                        aSynthGreek.SynthUndID = USD_ID
                                        aSynthGreek.SynthUndSymbol = USD_SYMBOL
                                        aPos.SynthGreeks.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                                        Set aSynthGreek = Nothing
                                        
                                        Set aSynthGreek = aUnd.SynthGreeks(USD_ID)
                                        If aSynthGreek Is Nothing Then
                                            Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                            aSynthGreek.SynthUndID = USD_ID
                                            aSynthGreek.SynthUndSymbol = USD_SYMBOL
                                            aUnd.SynthGreeks.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                                        End If
                                        Set aSynthGreek = Nothing
                                    End If
                                    
                                    For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                                        
                                        If aSynthRootComp.UndID <> aUnd.ID Then
                                            Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)
                                            If Not aGUnd Is Nothing Then
                                            
                                                Set aSynthUnd = m_Und(aSynthRootComp.UndID)
                                                If aSynthUnd Is Nothing Then
                                                    Set aSynthUnd = New EtsMmRisksLib.MmRvUndAtom
                                                    Set aSynthUnd.DataGrouping = New EtsMmRisksLib.MmRvGroupingColl
                                                    aSynthUnd.ID = aSynthRootComp.UndID
                                                
                                                    aSynthUnd.ContractType = aGUnd.UndType
                                                    aSynthUnd.Symbol = aGUnd.Symbol
                                                    aSynthUnd.IsAmerican = aGUnd.IsAmerican
                                                    aSynthUnd.IsHTB = aGUnd.IsHTB
                                                    aSynthUnd.Skew = aGUnd.Skew
                                                    aSynthUnd.Kurt = aGUnd.Kurt
                                                    aSynthUnd.HasSynthetic = aGUnd.HaveSyntheticRoots
                                                    
                                                    aSynthUnd.GroupingType = m_nFilter(GFC_MODE)
                                                    
                                                    Set aSynthUnd.SynthRoots = aGUnd.SyntheticRoots
                                                    
                                                    Set aSynthUnd.Dividend = aGUnd.Dividend
                                                    
                                                    If aSynthUnd.ContractType = enCtIndex Then
                                                        Set aSynthUnd.BasketIndex = g_Index(aSynthUnd.ID)
                                                        aSynthUnd.Yield = aGUnd.Yield
                                                    End If
                                                    
                                                    Set aSynthUnd.UndPriceProfile = aGUnd.UndPriceProfile
                                                    Set aSynthUnd.OptPriceProfile = aGUnd.OptPriceProfile
                                                    
'                                                    aSynthUnd.Pos = BAD_DOUBLE_VALUE
                                                    InitVola aSynthUnd
                                                    
                                                    sKey = CStr(aSynthUnd.ContractType) & "_" & aSynthUnd.Symbol
                                                    If m_QuoteRequests(sKey) Is Nothing Then m_QuoteRequests.Add aSynthUnd, Nothing, sKey
                                                    
                                                    m_Und.Add aSynthUnd.ID, aSynthUnd.Symbol, aSynthUnd
                                                    
                                                    Set aGUnd = Nothing
                                                End If
                                                                                    
                                                If aSynthUnd.SynthPos Is Nothing Then Set aSynthUnd.SynthPos = New EtsMmRisksLib.MmRvPosColl
                                                aSynthUnd.SynthPos.Add aPos.ID, aPos.Symbol, aPos
                                                
                                                Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                                aSynthGreek.SynthUndID = aSynthRootComp.UndID
                                                aSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                                                aPos.SynthGreeks.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                                                Set aSynthGreek = Nothing
                                                
                                                Set aSynthGreek = aUnd.SynthGreeks(aSynthRootComp.UndID)
                                                If aSynthGreek Is Nothing Then
                                                    Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                                    aSynthGreek.SynthUndID = aSynthRootComp.UndID
                                                    aSynthGreek.SynthUndSymbol = aSynthUnd.Symbol
                                                    aUnd.SynthGreeks.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                                                End If
                                                Set aSynthGreek = Nothing
                                            
                                            End If
                                        End If
                                    Next
                                    Set aSynthUnd = Nothing
                                    Set aSynthRoot = Nothing
                                End If
                            End If
                            
                            aPos.Qty = 0
                            aPos.QtyInShares = 0
                        
                            sKey = CStr(aPos.ContractType) & "_" & aPos.Symbol
                            Set aReq = m_QuoteRequests(sKey)
                            If aReq Is Nothing Then
                                m_QuoteRequests.Add aUnd, aPos, sKey
                            Else
                                If aReq.Pos Is Nothing Then Set aReq.Pos = aPos
                                Set aReq = Nothing
                            End If
                        End If
                                                
                        Set aPosDataProvider = aPos
                        Set aUndDataProvider = aUnd
                        Set aGrpDataProvider = m_Grp
                        
                        If m_bPosDataProviderInit = False And m_aContext.Halted = False Then
                            m_aContext.GetSystemEnvironment m_nPosLevelID, infoSysVars, infoSysFuncs
                            lUnsupported = aPosDataProvider.Check(infoSysVars, infoSysFuncs)
                            If lUnsupported > 0 Then
                                m_aContext.Halt
                                gCmn.ErrorMsgBox Nothing, "Custom variables functionality disabled due incorrect version of data provider"
                            End If
                            m_bPosDataProviderInit = True
                        End If
                        
                        If m_aContext.Halted = False Then
                            Set aPos.cvRTContext = m_aContext
                            aPos.cvPoolID = m_aContext.CreateCVPool(aPosDataProvider, m_nPosLevelID)
                            
                            m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_nUndLevelID, aUndDataProvider
                            m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_nTotalsLevelID, aGrpDataProvider
                            m_aContext.BindAggregations aPos.cvPoolID, aUnd.cvPoolID
                            m_aContext.BindAggregations aPos.cvPoolID, m_Grp.cvPoolID
                        End If
                        
                        AddNewTradeToPosition aUnd, aPos, aTrd
                        
                        Set aPos = Nothing
                    Else
'                        If aUnd.Pos = BAD_DOUBLE_VALUE Then aUnd.Pos = 0
'                        aUnd.Pos = aUnd.Pos + aTrd.Price * nQtyInShares
                    End If
                    
                    Set aUnd = Nothing
                End If
                
                DoEvents
                If Not m_bDataLoad Then Exit For
                
                IncProgress pbProgress
                
                Set aTrd = Nothing
            Next
            
            UnderlyingsAdjustRates
            
            UnderlyingsLoad = m_bDataLoad
        End If
    End If

    m_bInProc = False
Ex:
    On Error Resume Next
    Set aPos = Nothing
    Set aTrd = Nothing
    Set aGUnd = Nothing
    Set aUnd = Nothing
    m_bDataLoad = False
    AdjustState
    Exit Function
EH:
    m_bInProc = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings."
    GoTo Ex
End Function

Private Sub UnderlyingsShow(ByVal bReorderOnly As Boolean)
    On Error Resume Next
    Dim i As Long, Idx As Long
    Dim nUndRow&, aUnd As EtsMmRisksLib.MmRvUndAtom, aRowData As clsGvRowData
    Dim iSeriesOrder As SortSettings
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        If Not bReorderOnly Then
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = m_Und.Count
            
            FormatUndGrid
            
            .TextMatrix(ROW_TMVAL_TOTAL, GUC_SYMBOL) = "Total"

            UpdateRowsCaptions 1, False
            
            .Cell(flexcpText, 1, GUC_VALUE + 1, m_nParamsCount, GUC_COLUMN_COUNT - 3 + m_Grp.DataGroupingTotal.Count) = STR_NA
        Else
            .Rows = m_nParamsCount + 1
        End If

        iSeriesOrder = IIf(m_nSortCol = GUC_SYMBOL, m_iSeriesOrder, m_iParamSort)

        If m_Und.Count > 0 Then
            SortNodeArray m_SortedUnd, 0, UBound(m_SortedUnd), m_bSortByStrKey
        
            Idx = IIf(iSeriesOrder = flexSortGenericAscending, 0, UBound(m_SortedUnd))
            
            While Idx >= 0 And Idx <= UBound(m_SortedUnd)
                Set aUnd = m_Und(m_SortedUnd(Idx).ID)
                If Not aUnd Is Nothing Then
                    If Not aUnd.DataGrouping Is Nothing Then
                        If aUnd.DataGrouping.Count > 0 Then
                            Set aRowData = New clsGvRowData
                            Set aRowData.Und = aUnd
                    
                            nUndRow = .Rows
        
                            .AddItem "", nUndRow
                            .RowData(nUndRow) = aRowData
                            .TextMatrix(nUndRow, GUC_KEY) = CStr(aUnd.ID)
                        
                            UnderlyingUpdate nUndRow, True, aRowData
        
                            IncProgress pbProgress
                            DoEvents
                        End If
                    End If
                End If
                
                Idx = IIf(iSeriesOrder = flexSortGenericAscending, Idx + 1, Idx - 1)
            Wend
        End If
        
        If Not bReorderOnly Then
            TotalsUpdate False
        End If
        
        FormatUndColumns
        UnderlyingsUpdateBackColor
        
        If Not bReorderOnly Then
            geUnd.ShowSortImage GUC_SYMBOL, IIf(m_iSeriesOrder = flexSortGenericAscending, 1, -1)
            .AutoSize 1, .Cols - 1, False, 100
        End If
        
        .MergeCol(GUC_SYMBOL) = True
        If Not bReorderOnly Then
            .Row = 1
            .Col = 1
        End If
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub TotalsUpdate(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim aExpTot As EtsMmRisksLib.MmRvGroupingTotalAtom, nCol&, nIdx&
    Dim i As Long, nRowIdx As Long, nRow As Long
    Dim cvID As Long, vtVal As Variant
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        nCol = GUC_VALUE + 1
        For Each aExpTot In m_Grp.DataGroupingTotal
            nIdx = IIf((nCol - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
            
            For i = 0 To g_GUR_LAST_ROW
                nRowIdx = m_gdUndRows.Idx(i)
                If nRowIdx = IDX_NONE Then Exit For
                                
                nRow = i + 1
                
                '--------------------------------------------------------------------------
                cvID = BAD_LONG_VALUE
                cvID = m_cvIDs(CStr(nRowIdx))
                If cvID <> BAD_LONG_VALUE Then
                    vtVal = BAD_DOUBLE_VALUE
                    vtVal = m_aContext.GetCVValue(aExpTot.cvPoolID, cvID)
                    .TextMatrix(nRow, nCol) = IIf(vtVal > BAD_DOUBLE_VALUE, vtVal, STR_NA)
                End If
                '--------------------------------------------------------------------------

                Select Case nRowIdx
                    Case GUR_TIME_VALUE
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.TimeValueInShares > BAD_DOUBLE_VALUE, aExpTot.TimeValueInShares, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadTimeValueInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_DELTA_EQ
                        ' OptDlt$
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.DeltaEq > BAD_DOUBLE_VALUE, aExpTot.DeltaEq, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_DELTA_IN_SHARES
                        ' OptDlt
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.DeltaInShares > BAD_DOUBLE_VALUE, aExpTot.DeltaInShares, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_GAMMA_EQ
                        ' Gma$
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.GammaEq > BAD_DOUBLE_VALUE, aExpTot.GammaEq, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadGammaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_GAMMA_IN_SHARES
                        ' Gma$1
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.GammaInShares > BAD_DOUBLE_VALUE, aExpTot.GammaInShares, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadGammaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_VEGA
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.VegaInShares > BAD_DOUBLE_VALUE, aExpTot.VegaInShares, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadVegaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                                                                                
                    Case GUR_NETDELTA_EQ
                        ' OptDlt$
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.NetDeltaEq > BAD_DOUBLE_VALUE, aExpTot.NetDeltaEq, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadNetDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                    Case GUR_NETDELTA_IN_SHARES
                        ' OptDlt
                        ' TODO:
                        .TextMatrix(nRow, nCol) = IIf(aExpTot.NetDeltaInShares > BAD_DOUBLE_VALUE, aExpTot.NetDeltaInShares, STR_NA)
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpTot.BadNetDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                End Select
            Next
            
            nCol = nCol + 1
        Next
        
        nCol = .Cols - 1
        
        For i = 0 To g_GUR_LAST_ROW
            nRowIdx = m_gdUndRows.Idx(i)
            If nRowIdx = IDX_NONE Then Exit For
                
            nRow = i + 1
            
            '--------------------------------------------------------------------------
            cvID = BAD_LONG_VALUE
            cvID = m_cvIDs(CStr(nRowIdx))
            If cvID <> BAD_LONG_VALUE Then
                vtVal = BAD_DOUBLE_VALUE
                vtVal = m_aContext.GetCVValue(m_Grp.cvPoolID, cvID)
                .TextMatrix(nRow, nCol) = IIf(vtVal > BAD_DOUBLE_VALUE, vtVal, STR_NA)
            End If
            '--------------------------------------------------------------------------
            
            Select Case nRowIdx
                Case GUR_TIME_VALUE
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.TimeValue > BAD_DOUBLE_VALUE, m_Grp.TimeValue, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadTimeValue, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_DELTA_EQ
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.OptDelta > BAD_DOUBLE_VALUE, m_Grp.OptDelta, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadOptDelta, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_DELTA_IN_SHARES
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.DeltaInShares > BAD_DOUBLE_VALUE, m_Grp.DeltaInShares, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_GAMMA_EQ
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.GammaEq > BAD_DOUBLE_VALUE, m_Grp.GammaEq, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadGammaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_GAMMA_IN_SHARES
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.NetGamma > BAD_DOUBLE_VALUE, m_Grp.NetGamma, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadNetGamma, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_VEGA
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.Vega > BAD_DOUBLE_VALUE, m_Grp.Vega, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadVega, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                
                Case GUR_NETDELTA_EQ
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.DeltaEq > BAD_DOUBLE_VALUE, m_Grp.DeltaEq, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                Case GUR_NETDELTA_IN_SHARES
                    .TextMatrix(nRow, nCol) = IIf(m_Grp.NetDelta > BAD_DOUBLE_VALUE, m_Grp.NetDelta, STR_NA)
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_Grp.BadNetDelta, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                            m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
            End Select
        Next
            
        If bAutosize Then .AutoSize 1, .Cols - 1, False, 100
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub TotalsUpdateBadStatus()
    On Error Resume Next

    Dim aExpTot As EtsMmRisksLib.MmRvGroupingTotalAtom, nCol&, nIdx&
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        nCol = GUC_VALUE + 1
        For Each aExpTot In m_Grp.DataGroupingTotal
            nIdx = IIf((nCol - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
            
            .Cell(flexcpForeColor, ROW_TMVAL_TOTAL, nCol) = IIf(aExpTot.BadTimeValueInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
                                                                    
            .Cell(flexcpForeColor, ROW_DELTA_TOTAL, nCol) = IIf(aExpTot.BadDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
                                                                    
            .Cell(flexcpForeColor, ROW_DELTA1_TOTAL, nCol) = IIf(aExpTot.BadDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
                                                                    
            .Cell(flexcpForeColor, ROW_GAMMA_TOTAL, nCol) = IIf(aExpTot.BadGammaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
            
            .Cell(flexcpForeColor, ROW_GAMMA1_TOTAL, nCol) = IIf(aExpTot.BadGammaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
            
            .Cell(flexcpForeColor, ROW_VEGA_TOTAL, nCol) = IIf(aExpTot.BadVegaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                    m_gdUnd.Col(nIdx).ForeColor)
            
            nCol = nCol + 1
        Next
        
        nCol = .Cols - 1
        .Cell(flexcpForeColor, ROW_TMVAL_TOTAL, nCol) = IIf(m_Grp.BadTimeValue, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                                                                
        .Cell(flexcpForeColor, ROW_DELTA_TOTAL, nCol) = IIf(m_Grp.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                                                                
        .Cell(flexcpForeColor, ROW_DELTA1_TOTAL, nCol) = IIf(m_Grp.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                                                                
        .Cell(flexcpForeColor, ROW_GAMMA_TOTAL, nCol) = IIf(m_Grp.BadGammaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                                                                
        .Cell(flexcpForeColor, ROW_GAMMA1_TOTAL, nCol) = IIf(m_Grp.BadNetGamma, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                                                                
        .Cell(flexcpForeColor, ROW_VEGA_TOTAL, nCol) = IIf(m_Grp.BadVega, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
            
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsCalc()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom

    If m_Und.Count > 0 Then
        lblStatus.Visible = False
        imgStop.Visible = True
        imgStopDis.Visible = False
        lblProcess.Caption = "Calculation..."
        lblProcess.Visible = True
        lblProcess.Refresh

        Dim aIdx As EtsMmRisksLib.MmRvUndAtom
        Set aIdx = New EtsMmRisksLib.MmRvUndAtom
        aIdx.ID = 0

        m_Grp.Calc m_Und, aIdx, False, GM_ALL, True, True, False, _
                    Nothing, _
                    (g_Params.PnLCalcType = PNLCT_LTD), g_Params.RiskFitToImpFlatNoBid, g_Params.RiskMIVCalcType, _
                    g_Params.CalcModel, g_Params.UseTheoVolatility, _
                    g_Params.UseTheoNoBid, g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, _
                    g_Params.PriceRoundingRule, Nothing, False, False, False, g_Main.CalculationParametrs

        Set aIdx = Nothing

    
        imgStop.Visible = False
        imgStopDis.Visible = False
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
    End If
End Sub

Private Sub UnderlyingsUpdateVola()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aExp As EtsMmRisksLib.MmRvGroupingAtom
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, dVola#
    Dim lSurfaceID As Long

    For Each aUnd In m_Und
        For Each aExp In aUnd.DataGrouping
            For Each aPos In aExp.Pos
                dVola = BAD_DOUBLE_VALUE
                lSurfaceID = GetSurfaceByPos(aPos, aUnd)
                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
                aPos.Vola = dVola
            Next
        Next
    Next
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
'Private Sub UnderlyingCalc(ByRef aUnd As EtsMmGreeksLib.MmGaUndAtom)
'    On Error GoTo EH
'    m_Grp.CalcUnderlying aUnd, g_Params.CalcModel, _
'        g_Params.UseTheoVolatility, g_Params.UseTheoNoBid, _
'        g_Params.UseTheoBadMarketVola, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule
'    Exit Sub
'EH:
'    LogEvent EVENT_WARNING, "Fail to calculate greeks for '" & aUnd.Symbol & "': " & Err.Description
'End Sub

'Private Sub UnderlyingsUpdate(ByVal bAutosize As Boolean)
'    On Error Resume Next
'    Dim nRow&, aRowData As clsGvRowData
'
'    With fgUnd
'        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
'
'        For nRow = 7 To .Rows - 1
'            Set aRowData = .RowData(nRow)
'            If Not aRowData Is Nothing Then
'                If Not aRowData.Und Is Nothing Then UnderlyingUpdate nRow, False, aRowData
'                Set aRowData = Nothing
'            End If
'        Next
'
'        If bAutosize Then .AutoSize 1, .Cols - 1, False, 100
'
'        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
'    End With
'End Sub

Private Sub UnderlyingUpdate(ByVal nUndRow As Long, ByVal bAddRows As Boolean, ByRef RowData As clsGvRowData)
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aExpTot As EtsMmRisksLib.MmRvGroupingTotalAtom, aExp As EtsMmRisksLib.MmRvGroupingAtom, nCol&, nIdx&
    Dim aRowData As clsGvRowData, nRow&, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom, aExpSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim aSynthRowData As clsGvRowData
    Dim nKeyValue As Long
    Dim i As Long, nRowIdx As Long
    Dim cvID As Long, vtVal As Variant
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        If Not RowData Is Nothing Then
            Set aRowData = RowData
        Else
            Set aRowData = .RowData(nUndRow)
        End If
        
        If Not aRowData Is Nothing Then
            Set aUnd = aRowData.Und
            If Not aUnd Is Nothing Then
                If bAddRows Then

                    UpdateRowsCaptions nUndRow, True

                End If
                
                nCol = GUC_VALUE + 1
                For Each aExpTot In m_Grp.DataGroupingTotal
                
                    Select Case m_Grp.GroupingType
                        Case enGroupingExpiry
                            nKeyValue = aExpTot.ExpiryMonth
                        Case enGroupingMoneyness
                            nKeyValue = aExpTot.Moneyness
                    End Select
                
                    Set aExp = aUnd.DataGrouping(nKeyValue)
                    nRow = nUndRow
                    nIdx = IIf((nCol - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
                    
                    If Not aExp Is Nothing Then
                    
                        For i = 0 To g_GUR_LAST_ROW
                            nRowIdx = m_gdUndRows.Idx(i)
                            If nRowIdx = IDX_NONE Then Exit For
                            
                            '--------------------------------------------------------------------------
                            cvID = BAD_LONG_VALUE
                            cvID = m_cvIDs(CStr(nRowIdx))
                            If cvID <> BAD_LONG_VALUE Then
                                vtVal = BAD_DOUBLE_VALUE
                                vtVal = m_aContext.GetCVValue(aExp.cvPoolID, cvID)
                                .TextMatrix(nRow, nCol) = IIf(vtVal > BAD_DOUBLE_VALUE, vtVal, STR_NA)
                            End If
                            '--------------------------------------------------------------------------
                            
                            Select Case nRowIdx
                                Case GUR_TIME_VALUE
                                    .TextMatrix(nRow, nCol) = IIf(aExp.TimeValueInShares > BAD_DOUBLE_VALUE, aExp.TimeValueInShares, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadTimeValueInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                        
                                Case GUR_DELTA_EQ
                                    ' OptDlt$
                                    .TextMatrix(nRow, nCol) = IIf(aExp.DeltaEq > BAD_DOUBLE_VALUE, aExp.DeltaEq, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                    
                                    
                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aExpSynthGreek.DeltaInMoney, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDeltaInMoney, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                                    
                                Case GUR_DELTA_IN_SHARES
                                    ' OptDlt
                                    .TextMatrix(nRow, nCol) = IIf(aExp.DeltaInShares > BAD_DOUBLE_VALUE, aExp.DeltaInShares, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                    
                                    
                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aExpSynthGreek.DeltaInShares, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDelta, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                                    
                                Case GUR_GAMMA_EQ
                                    ' Gma$
                                    .TextMatrix(nRow, nCol) = IIf(aExp.GammaEq > BAD_DOUBLE_VALUE, aExp.GammaEq, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadGammaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                    
                                    
                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.GammaInSharesPerc > BAD_DOUBLE_VALUE, aExpSynthGreek.GammaInSharesPerc, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadGammaPerc, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                                    
                                Case GUR_GAMMA_IN_SHARES
                                    ' Gma$1
                                    .TextMatrix(nRow, nCol) = IIf(aExp.GammaInShares > BAD_DOUBLE_VALUE, aExp.GammaInShares, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadGammaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                    
                                    
                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.GammaInShares > BAD_DOUBLE_VALUE, aExpSynthGreek.GammaInShares, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadGamma, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                                    
                                Case GUR_VEGA
                                    .TextMatrix(nRow, nCol) = IIf(aExp.VegaInShares > BAD_DOUBLE_VALUE, aExp.VegaInShares, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadVegaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                                                            
                                Case GUR_NETDELTA_EQ
                                    ' OptDlt$
                                    ' TODO:
                                    .TextMatrix(nRow, nCol) = IIf(aExp.NetDeltaEq > BAD_DOUBLE_VALUE, aExp.NetDeltaEq, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadNetDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)


                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.DeltaInMoney > BAD_DOUBLE_VALUE, aExpSynthGreek.DeltaInMoney, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDeltaInMoney, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                                    
                                Case GUR_NETDELTA_IN_SHARES
                                    ' OptDlt
                                    .TextMatrix(nRow, nCol) = IIf(aExp.NetDeltaInShares > BAD_DOUBLE_VALUE, aExp.NetDeltaInShares, STR_NA)
                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadNetDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                            m_gdUnd.Col(nIdx).ForeColor)


                                    If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                        For Each aSynthGreek In aUnd.SynthGreeks
                                            If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                                Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                                If Not aExpSynthGreek Is Nothing Then
'                                                    .TextMatrix(nRow, nCol) = IIf(aExpSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aExpSynthGreek.DeltaInShares, STR_NA)
'                                                    .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDelta, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                            m_gdUnd.Col(nIdx).ForeColor)
                                                    Set aExpSynthGreek = Nothing
                                                End If
                                            End If
'                                            nRow = nRow + 1
                                        Next
                                    End If
                            End Select
                            
                            nRow = nRow + 1
                        Next
                    End If
                
                    nCol = nCol + 1
                Next
                
                nCol = .Cols - 1
                nRow = nUndRow
                
                For i = 0 To g_GUR_LAST_ROW
                    nRowIdx = m_gdUndRows.Idx(i)
                    If nRowIdx = IDX_NONE Then Exit For
                    
                    '--------------------------------------------------------------------------
                    cvID = BAD_LONG_VALUE
                    cvID = m_cvIDs(CStr(nRowIdx))
                    If cvID <> BAD_LONG_VALUE Then
                        vtVal = BAD_DOUBLE_VALUE
                        vtVal = m_aContext.GetCVValue(aUnd.cvPoolID, cvID)
                        .TextMatrix(nRow, nCol) = IIf(vtVal > BAD_DOUBLE_VALUE, vtVal, STR_NA)
                    End If
                    '--------------------------------------------------------------------------
                    
                    Select Case nRowIdx
                        Case GUR_TIME_VALUE
                            .TextMatrix(nRow, nCol) = IIf(aUnd.TimeValue > BAD_DOUBLE_VALUE, aUnd.TimeValue, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTimeValue, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
                        Case GUR_DELTA_EQ
                            ' OptDlt$
                            .TextMatrix(nRow, nCol) = IIf(aUnd.OptDeltaEq > BAD_DOUBLE_VALUE, aUnd.OptDeltaEq, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadOptDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaEq > BAD_DOUBLE_VALUE, aSynthGreek.DeltaEq, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If
                            
                        Case GUR_DELTA_IN_SHARES
                            ' OptDlt
                            .TextMatrix(nRow, nCol) = IIf(aUnd.OptDelta > BAD_DOUBLE_VALUE, aUnd.OptDelta, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadOptDelta, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)

            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If
            
                        Case GUR_GAMMA_EQ
                            ' Gma$
                            .TextMatrix(nRow, nCol) = IIf(aUnd.NetGamma > BAD_DOUBLE_VALUE, aUnd.NetGamma, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetGamma, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaEq > BAD_DOUBLE_VALUE, aSynthGreek.GammaEq, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGammaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If

                        Case GUR_GAMMA_IN_SHARES
                            ' Gma$1
                            .TextMatrix(nRow, nCol) = IIf(aUnd.Gamma > BAD_DOUBLE_VALUE, aUnd.Gamma, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGamma, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.GammaInShares > BAD_DOUBLE_VALUE, aSynthGreek.GammaInShares, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If
                            
                        Case GUR_VEGA
                            .TextMatrix(nRow, nCol) = IIf(aUnd.Vega > BAD_DOUBLE_VALUE, aUnd.Vega, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVega, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                        
                        Case GUR_NETDELTA_EQ
                            ' OptDlt$
                            .TextMatrix(nRow, nCol) = IIf(aUnd.DeltaEq > BAD_DOUBLE_VALUE, aUnd.DeltaEq, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaEq > BAD_DOUBLE_VALUE, aSynthGreek.DeltaEq, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If
                            
                        Case GUR_NETDELTA_IN_SHARES
                            ' OptDlt
                            .TextMatrix(nRow, nCol) = IIf(aUnd.NetDelta > BAD_DOUBLE_VALUE, aUnd.NetDelta, STR_NA)
                            .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadNetDelta, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                                    m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                            
            
                            If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                                For Each aSynthGreek In aUnd.SynthGreeks
            '                        .TextMatrix(nRow, nCol) = IIf(aSynthGreek.DeltaInShares > BAD_DOUBLE_VALUE, aSynthGreek.DeltaInShares, STR_NA)
            '                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
            '                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
'                                    nRow = nRow + 1
                                Next
                            End If
                            
                    End Select
                    
                    nRow = nRow + 1
                Next
                
                Set aUnd = Nothing
            End If
        End If
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingsUpdateBadStatus()
    On Error Resume Next
    Dim nRow&, aRowData As clsGvRowData
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        For nRow = m_nParamsCount + 1 To .Rows - 1
            Set aRowData = .RowData(nRow)
            If Not aRowData Is Nothing Then
                If Not aRowData.Und Is Nothing Then UnderlyingUpdateBadStatus nRow, aRowData
                Set aRowData = Nothing
            End If
        Next
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingUpdateBadStatus(ByVal nUndRow As Long, ByRef RowData As clsGvRowData)
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aExpTot As EtsMmRisksLib.MmRvGroupingTotalAtom, aExp As EtsMmRisksLib.MmRvGroupingAtom, nCol&, nIdx&
    Dim aRowData As clsGvRowData, nRow&, aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom, aExpSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim nKeyValue As Long
    
    With fgUnd
        m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
        
        If Not RowData Is Nothing Then
            Set aRowData = RowData
        Else
            Set aRowData = .RowData(nUndRow)
        End If
        
        If Not aRowData Is Nothing Then
            Set aUnd = aRowData.Und
            If Not aUnd Is Nothing Then
                
                nCol = GUC_VALUE + 1
                For Each aExpTot In m_Grp.DataGroupingTotal
                
                    Select Case m_Grp.GroupingType
                        Case enGroupingExpiry
                            nKeyValue = aExpTot.ExpiryMonth
                        Case enGroupingMoneyness
                            nKeyValue = aExpTot.Moneyness
                    End Select
                
                    Set aExp = aUnd.DataGrouping(nKeyValue)
                    nRow = nUndRow
                    nIdx = IIf((nCol - GUC_VALUE) Mod 2 <> 0, GUC_EXPIRY_ODD, GUC_EXPIRY_EVEN)
                    
                    If Not aExp Is Nothing Then
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadTimeValueInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                        
                        ' OptDlt$
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                        
                        If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                            For Each aSynthGreek In aUnd.SynthGreeks
                                If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                    Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                    If Not aExpSynthGreek Is Nothing Then
'                                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDeltaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                m_gdUnd.Col(nIdx).ForeColor)
                                        Set aExpSynthGreek = Nothing
                                    End If
                                End If
                                nRow = nRow + 1
                            Next
                        End If
                        
                        ' OptDlt
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                        
                        If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                            For Each aSynthGreek In aUnd.SynthGreeks
                                If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                    Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                    If Not aExpSynthGreek Is Nothing Then
'                                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadDeltaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                m_gdUnd.Col(nIdx).ForeColor)
                                        Set aExpSynthGreek = Nothing
                                    End If
                                End If
                                nRow = nRow + 1
                            Next
                        End If
                        
                        ' Gma$
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadGammaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                        
                        If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                            For Each aSynthGreek In aUnd.SynthGreeks
                                If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                    Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                    If Not aExpSynthGreek Is Nothing Then
'                                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadGammaEq, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                m_gdUnd.Col(nIdx).ForeColor)
                                        Set aExpSynthGreek = Nothing
                                    End If
                                End If
                                nRow = nRow + 1
                            Next
                        End If
                    
                        ' Gma$1
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadGammaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                        
                        If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                            For Each aSynthGreek In aUnd.SynthGreeks
                                If aExp.HasSynth And Not aExp.SynthGreek Is Nothing Then
                                    Set aExpSynthGreek = aExp.SynthGreek(aSynthGreek.SynthUndID)
                                    If Not aExpSynthGreek Is Nothing Then
'                                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExpSynthGreek.BadGammaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
'                                                                                                m_gdUnd.Col(nIdx).ForeColor)
                                        Set aExpSynthGreek = Nothing
                                    End If
                                End If
                                nRow = nRow + 1
                            Next
                        End If
                    
                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aExp.BadVegaInShares, m_gdUnd.Col(nIdx).ForeColorAlt1, _
                                                                                m_gdUnd.Col(nIdx).ForeColor)
                        nRow = nRow + 1
                    End If
                
                    nCol = nCol + 1
                Next
                
                nCol = .Cols - 1
                nRow = nUndRow
'                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadTimeValueInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                ' OptDlt$
                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                        nRow = nRow + 1
                    Next
                End If
                
                ' OptDlt
'                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadDeltaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                        nRow = nRow + 1
                    Next
                End If
                
                ' Gma$
                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGammaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGammaEq, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                        nRow = nRow + 1
                    Next
                End If
            
                ' Gma$1
'                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadGammaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                If aUnd.HasSynthetic And Not aUnd.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aUnd.SynthGreeks
'                        .Cell(flexcpForeColor, nRow, nCol) = IIf(aSynthGreek.BadGammaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                                m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                        nRow = nRow + 1
                    Next
                End If
            
'                .Cell(flexcpForeColor, nRow, nCol) = IIf(aUnd.BadVegaInShares, m_gdUnd.Col(GUC_SYM_TOTAL).ForeColorAlt1, _
'                                                                        m_gdUnd.Col(GUC_SYM_TOTAL).ForeColor)
                nRow = nRow + 1
                
                Set aUnd = Nothing
            End If
        End If
        
        m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim i&, nCount&, aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aReq As clsGvRequestAtom
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
            
            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents
            
            m_nLastQuoteGroupReqCount = 0&
            m_nLastQuoteGroupReqDone = 0&
                
            qup.Exchange = ""
            qup.Exchange = ""
            qup.Type = enMStart
            PriceProvider.RequestLastQuote qup
                
            For Each aReq In m_QuoteRequests
                If Not m_bLastQuoteReqNow Then Exit For
                PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                DoEvents
            Next
            qup.Type = enMStop
            PriceProvider.RequestLastQuote qup
                            
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
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    PriceProvider.CancelLastQuote
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    m_bInProc = False
    AdjustState
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim sKey$, aReq As clsGvRequestAtom, bFinished As Boolean, bCalcGreeks As Boolean
    
    bFinished = False
    bCalcGreeks = False
    
    If ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If Request.Type <> enGrSTK And Request.Type <> enGrIDX Then
                sKey = CStr(IIf(Request.Type = enOPT, enCtOption, IIf(Request.Type = enSTK, enCtStock, enCtIndex))) & "_" & Request.Symbol
                
                Set aReq = m_QuoteRequests(sKey)
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                End If
                
                m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                IncProgress pbProgress
                bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount)
            Else
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                
                PriceProvider.CancelLastQuote
               
                m_bInProc = True
                AdjustState
                
                GroupData
                UnderlyingsUpdateVola
                UnderlyingsCalc
                UnderlyingsShow False
                TotalsUpdate True
                
                m_bInProc = False
                AdjustState
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
            End If
        Else
            Debug.Assert False
        End If
    
    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim sKey$, aReq As clsGvRequestAtom, dPriceBid#, dPriceAsk#, dPriceLast#, nLotSize&
    Dim bFinished As Boolean
    bFinished = False
    
    If Params.Type <> enGrSTK And Params.Type <> enGrIDX Then
        sKey = CStr(IIf(Params.Type = enOPT, enCtOption, IIf(Params.Type = enSTK, enCtStock, enCtIndex))) & "_" & Params.Symbol
        Set aReq = m_QuoteRequests(sKey)
        If Not aReq Is Nothing Then
            dPriceBid = Results.BidPrice
            dPriceAsk = Results.AskPrice
            dPriceLast = Results.LastPrice
            
            If Results.LotSize > 0 Then
                nLotSize = Results.LotSize
            Else
                If Params.Type = enOPT Then
                    nLotSize = 100
                Else
                    nLotSize = 1
                End If
            End If
            
            If Not aReq.Pos Is Nothing Then
                If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Pos.PriceBid = dPriceBid
                If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Pos.PriceAsk = dPriceAsk
                If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Pos.PriceLast = dPriceLast
                
                If aReq.Pos.LotSize <= 0 Then
                    aReq.Pos.LotSize = nLotSize
                End If
            End If
            
            If Params.Type <> enOPT Then
                
                If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Und.PriceBid = dPriceBid
                If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Und.PriceAsk = dPriceAsk
                If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Und.PriceLast = dPriceLast
                
           
                 Debug.Assert (Not aReq.Und.UndPriceProfile Is Nothing)
                 aReq.Und.VolaSrv.UnderlyingPrice = aReq.Und.UndPriceProfile.GetUndPriceMid(aReq.Und.PriceBid, aReq.Und.PriceAsk, aReq.Und.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
            End If
        
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            Set aReq = Nothing
        
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount)
        End If
    Else
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bLastQuoteReqNow = False
        
        PriceProvider.CancelLastQuote
        
        m_bInProc = True
        AdjustState
        
        GroupData
        UnderlyingsUpdateVola
        UnderlyingsCalc
        UnderlyingsShow False
        TotalsUpdate True
        
        m_bInProc = False
        AdjustState
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If
    Set aReq = Nothing
End Sub

Private Sub UnderlyingsAdjustRates()
    On Error Resume Next
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim cPosThreshold@, bUseMidRates As Boolean, dPos#
    
    Dim dtToday As Date: dtToday = GetNewYorkTime
    
    For Each aUnd In m_Und
    
            dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
        
            If GetIrRuleType = enRateBasedOnPosition Then
                cPosThreshold = Abs(GetPosThreshold)
                bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
            Else
                bUseMidRates = True
            End If
            
            Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)

            For Each aPos In aUnd.Pos
                If bUseMidRates Then
                    aPos.Rate = GetNeutralRate(dtToday, aPos.ExpiryOV)
                Else
                    aPos.Rate = IIf(dPos < 0, GetShortRate(dtToday, aPos.ExpiryOV), GetLongRate(dtToday, aPos.ExpiryOV))
                End If
                
                If bIsHTBRatesExist Then
                    aPos.HTBRate = GetHTBRate(aUnd.ID, dtToday, aPos.ExpiryOV)
                Else
                    aPos.HTBRate = BAD_DOUBLE_VALUE
                End If
            Next
        
    Next
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_nFilter(GFC_GROUP) = TYPE_UNDERLYING Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then FilterUpdateValue False
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If m_nFilter(GFC_GROUP) = TYPE_GROUP Then FilterUpdateValue False
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If m_nFilter(GFC_GROUP) = TYPE_STRATEGY Then FilterUpdateValue False
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRefresh.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bLastQuoteReqNow
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = ""
    If m_Grp.ID <> 0 Then sCaption = sCaption & m_Grp.Name & " - "
    
    sCaption = sCaption & "Greeks"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    If m_Grp.ID <> 0 Then
        GetShortCaption = m_Grp.Name
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

Public Sub Refresh()
    On Error Resume Next
    tmrShow.Enabled = True
End Sub

Public Function Group() As EtsMmRisksLib.MmRvGrpAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aRowData As clsGvRowData, nRow&

    CurUnderlyingID = 0
    
    If m_Grp.GroupType = TYPE_UNDERLYING Then
        CurUnderlyingID = m_Grp.ID
    Else
        With fgUnd
            If .Row > 0 Then
                nRow = .Row
                
                Do While CurUnderlyingID = 0 And nRow >= 7
                    Set aRowData = .RowData(nRow)
                    If Not aRowData Is Nothing Then
                        If Not aRowData.Und Is Nothing Then
                            CurUnderlyingID = aRowData.Und.ID
                        End If
                        Set aRowData = Nothing
                    End If
                    nRow = nRow - 1
                Loop
            End If
        End With
    End If
End Function

Private Sub UserControl_Hide()
    On Error Resume Next
    m_bDataLoad = False
    m_bLastQuoteReqNow = False
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub

    With fgFlt
        .Top = 0
        .Left = 0
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
    End With

    With fgUnd
        .Left = 0
        .Top = fgFlt.Height + GRID_VERT_SPACE
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
    m_bLastQuoteReqNow = False
    m_bInProc = False
    
    Set frmLayout = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote

        Set PriceProvider = Nothing
    End If
    
    Set m_Grp = Nothing
    Set m_Und = Nothing
    
    Set geUnd = Nothing
    
    Set m_aContext = Nothing
    Set m_cvIDs = Nothing
    
    Erase m_SortedUnd
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set lblValue = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey

    ' common info
    For i = GFC_GROUP To GFC_LAST_COLUMN
        aStorage.SetLongValue "GreekFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next
    
    m_gdFlt.WriteToStorage "GreekFltGrid" & sKey, aStorage, False
    m_gdUnd.WriteToStorage "GreekUndGrid" & sKey, aStorage
    m_gdUndRows.WriteToStorage "GreekUndRows" & sKey, aStorage
    
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    ' common info
    For i = GFC_GROUP To GFC_LAST_COLUMN
        m_nFilter(i) = aStorage.GetLongValue("GreekFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next
    
    If m_nFilter(GFC_TYPE) < 0 Or m_nFilter(GFC_TYPE) > 6 Then m_nFilter(GFC_TYPE) = 0
    
    m_gdFlt.ReadFromStorage "GreekFltGrid" & sKey, aStorage, False
    m_gdUnd.ReadFromStorage "GreekUndGrid" & sKey, aStorage
    m_gdUndRows.ReadFromStorage "GreekUndRows" & sKey, aStorage
    
    m_nParamsCount = m_gdUndRows.GetVisibleCount
    
    FilterUpdateAll
    tmrShow.Enabled = True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Greeks", "Greeks", fgFlt, fgUnd
'    Screen.MousePointer = vbDefault
End Sub

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_GREEKS_FILTER, GFC_GROUP, GFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeUndGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_GREEKS_UNDERLYING, GUC_SYMBOL, GUC_COLUMN_COUNT, m_gdUnd, m_frmOwner
    End If
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
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
        Case GT_GREEKS_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_GREEKS_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_GREEKS_FILTER).UnlockRedraw
            
        Case GT_GREEKS_UNDERLYING
            gdGrid.CopyTo m_gdUnd
            
            m_GridLock(GT_GREEKS_UNDERLYING).LockRedraw
            FormatUndColumns
            UnderlyingsUpdateBackColor
            TotalsUpdateBadStatus
            UnderlyingsUpdateBadStatus
            fgUnd.AutoSize 1, fgUnd.Cols - 1, False, 100
            m_GridLock(GT_GREEKS_UNDERLYING).UnlockRedraw
            
        Case GT_GREEKS_UND_ROWS
            gdGrid.CopyTo m_gdUndRows
            m_nParamsCount = m_gdUndRows.GetVisibleCount
            
            ClearViewAndData False
    
    End Select
    
    UserControl_Resize
End Sub

Private Sub InitVola(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, aUnd.ContractType, VolaSource
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

Private Sub GroupData()
    On Error Resume Next
    Dim nKeyValue As Long, nExpiryMonth As Long, nRoundMoneyness As Long
    
    Dim aGUnd As EtsGeneralLib.UndAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    
    Dim aUndPosAtom As EtsMmRisksLib.MmRvPosAtom
    Dim aUndPosColl As EtsMmRisksLib.MmRvPosColl
    Set aUndPosColl = New EtsMmRisksLib.MmRvPosColl
    
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim aSynthRootComp As EtsGeneralLib.SynthRootCompAtom
    Dim aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    
    Dim aItem As EtsMmRisksLib.MmRvGroupingAtom, aItemTot As EtsMmRisksLib.MmRvGroupingTotalAtom
    
    Dim aGrpAtomProvider As CVLib.IcvDataProvider
    Dim aGrpTotlProvider As CVLib.IcvDataProvider
    Dim aPosDataProvider As CVLib.IcvDataProvider
    
    Erase m_SortedUnd
    ReDim m_SortedUnd(0 To m_Und.Count - 1)
    Dim Idx As Long
    m_bSortByStrKey = True
    
    For Each aUnd In m_Und
    
        m_SortedUnd(Idx).StrKey = aUnd.Symbol
        m_SortedUnd(Idx).ID = aUnd.ID
        Idx = Idx + 1
    
        For Each aPos In aUnd.Pos
        
            If aPos.ContractType = enCtIndex Or aPos.ContractType = enCtStock Then
                aUndPosColl.Add aPos.ID, aPos.Symbol, aPos
            Else
        
                nExpiryMonth = CLng(DateSerial(Year(aPos.Expiry), Month(aPos.Expiry), Day(aPos.Expiry)))
                nRoundMoneyness = GetRoundMoneyness(aUnd, aPos)
            
                Select Case m_Grp.GroupingType
                    Case enGroupingExpiry
                        nKeyValue = nExpiryMonth
                    
                    Case enGroupingMoneyness
                        nKeyValue = nRoundMoneyness
                End Select
                
                Set aPosDataProvider = aPos
                                    
                Set aItemTot = m_Grp.DataGroupingTotal(nKeyValue)
                If aItemTot Is Nothing Then
                    Set aItemTot = New EtsMmRisksLib.MmRvGroupingTotalAtom
                    aItemTot.ExpiryMonth = nExpiryMonth
                    aItemTot.Moneyness = nRoundMoneyness
    
                    m_Grp.DataGroupingTotal.Add nKeyValue, aItemTot
                    
                    Set aGrpTotlProvider = aItemTot
                    
                    If m_aContext.Halted = False Then
                        Set aItemTot.cvRTContext = m_aContext
                        aItemTot.cvPoolID = m_aContext.CreateCVPool(aGrpTotlProvider, m_nGroupTotalsLevelID)
                    End If
                End If
                
                If m_aContext.Halted = False Then
                    m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_nGroupTotalsLevelID, aGrpTotlProvider
                    m_aContext.BindAggregations aPos.cvPoolID, aItemTot.cvPoolID
                End If
    
                Set aItem = aUnd.DataGrouping(nKeyValue)
                If aItem Is Nothing Then
                    Set aItem = New EtsMmRisksLib.MmRvGroupingAtom
                    aItem.ExpiryMonth = nExpiryMonth
                    aItem.Moneyness = nRoundMoneyness
                    aItem.Expiry = aPos.Expiry
    
                    aUnd.DataGrouping.Add nKeyValue, aItem
                    aItemTot.Grouping.Add aUnd.ID, aItem
                    
                    Set aGrpAtomProvider = aItem
                    
                    If m_aContext.Halted = False Then
                        Set aItem.cvRTContext = m_aContext
                        aItem.cvPoolID = m_aContext.CreateCVPool(aGrpAtomProvider, m_nGroupUndLevelID)
                    End If
                End If
                
                If m_aContext.Halted = False Then
                    m_aContext.RegisterPoolDataProvider aPos.cvPoolID, m_nGroupUndLevelID, aGrpAtomProvider
                    m_aContext.BindAggregations aPos.cvPoolID, aItem.cvPoolID
                End If
                
                aItem.Pos.Add aPos.ID, aPos.Symbol, aPos
                
                If aPos.IsSynthetic Then
                    Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)
                    If Not aSynthRoot Is Nothing Then
                        aItem.HasSynth = True
                        If aItem.SynthGreek Is Nothing Then Set aItem.SynthGreek = New EtsMmRisksLib.MmRvSynthGreeksColl
                        
                        If aSynthRoot.CashValue > 0# Then
                            Set aSynthGreek = aItem.SynthGreek(USD_ID)
                            If aSynthGreek Is Nothing Then
                                Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                aSynthGreek.SynthUndID = USD_ID
                                aSynthGreek.SynthUndSymbol = USD_SYMBOL
                                aItem.SynthGreek.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                            End If
                            Set aSynthGreek = Nothing
                        End If
                        
                        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
                            If aSynthRootComp.UndID <> aUnd.ID Then
                                Set aGUnd = g_UnderlyingAll(aSynthRootComp.UndID)
                                If Not aGUnd Is Nothing Then
                                    Set aSynthGreek = aItem.SynthGreek(aSynthRootComp.UndID)
                                    If aSynthGreek Is Nothing Then
                                        Set aSynthGreek = New EtsMmRisksLib.MmRvSynthGreeksAtom
                                        aSynthGreek.SynthUndID = aSynthRootComp.UndID
                                        aSynthGreek.SynthUndSymbol = aGUnd.Symbol
                                        aItem.SynthGreek.Add aSynthGreek.SynthUndID, aSynthGreek.SynthUndSymbol, aSynthGreek
                                    End If
                                    Set aSynthGreek = Nothing
                                End If
                            End If
                        Next
                        
                    End If
                End If
                
                Set aItem = Nothing
                Set aItemTot = Nothing
                
            End If
            
        Next
        
        ' Add underlyng position to each data grouping
        For Each aItem In aUnd.DataGrouping
            For Each aUndPosAtom In aUndPosColl
                aItem.Pos.Add aUndPosAtom.ID, aUndPosAtom.Symbol, aUndPosAtom
            Next
        Next
        
        aUndPosColl.Clear
        
    Next
    
    Set aUndPosColl = Nothing
    
End Sub

Private Function GetRoundMoneyness(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                                   ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Long
    On Error Resume Next
    Dim dStrike As Double: dStrike = aPos.Strike
    Dim dSpot As Double: dSpot = aUnd.VolaSrv.UnderlyingPrice
    
    GetRoundMoneyness = BAD_LONG_VALUE
    
    GetRoundMoneyness = (dStrike / dSpot - 1) * 100
    GetRoundMoneyness = GetRoundMoneyness / 5
    GetRoundMoneyness = GetRoundMoneyness * 5
    
    If GetRoundMoneyness < MIN_MONEYNESS + 10 Then GetRoundMoneyness = MIN_MONEYNESS
    If GetRoundMoneyness > MAX_MONEYNESS - 10 Then GetRoundMoneyness = MAX_MONEYNESS
End Function

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
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
        
        ElseIf bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
            mnuCtxTradeNew_Click
            FixDblClickHandled
        End If
    End If
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgUnd, , , sHeader, FileSaveType)
    Screen.MousePointer = vbNormal
End Function

Public Function InitCVContext() As Boolean
    On Error GoTo EH
    Set m_aContext = Nothing
    Set m_aContext = New CVLib.cvRTContext

    m_nContextID = gDBW.usp_cvContextID_Get("GreeksView")
    If m_nContextID = 0 Then
        m_aContext.Halt
        gCmn.ErrorMsgBox Nothing, "GreeksView CV context definitions not found. Custom variables functionality disabled"
    Else
        m_aContext.ConnectionString = g_Params.DbConnection
        m_aContext.InitContext m_nContextID

        m_nTotalsLevelID = m_aContext.GetLevelIDByName("Totals")
        m_nUndLevelID = m_aContext.GetLevelIDByName("Underlyings")
        m_nPosLevelID = m_aContext.GetLevelIDByName("Positions")
        m_nGroupTotalsLevelID = m_aContext.GetLevelIDByName("GroupingTotals")
        m_nGroupUndLevelID = m_aContext.GetLevelIDByName("Grouping")

        If m_nTotalsLevelID = 0 Or m_nUndLevelID = 0 Or m_nPosLevelID = 0 Or m_nGroupTotalsLevelID = 0 Or m_nGroupUndLevelID = 0 Then
            m_aContext.Halt
            gCmn.ErrorMsgBox Nothing, "Some required levels definitions within GreeksView not found. Custom variables functionality disabled"
        End If
    End If

    Exit Function
EH:
    Dim sErrDesc$
    sErrDesc = Err.Description
    On Error Resume Next
    Screen.MousePointer = vbDefault
    If Not m_aContext Is Nothing Then m_aContext.Halt
    gCmn.MyMsgBox Nothing, "Fail to initialize custom variable engine" & vbCrLf & sErrDesc, vbCritical
End Function

Public Sub InitGreeksColumnsWithCV(Optional ByVal nNewVarID As Long = BAD_LONG_VALUE)
    On Error Resume Next
    Dim Info As CVLib.cvInfo
    Dim nTotalCount As Long, nUndCount As Long, nGroupTotalCount As Long, nGroupUndCount As Long
    Dim cvIDs() As Long, UndIDs() As Long, GroupTotalIDs() As Long, GroupUndIDs() As Long

    Set m_cvIDs = Nothing
    Set m_cvIDs = New Collection
    
    cvIDs = m_aContext.GetCVIDs(m_nTotalsLevelID)
    nTotalCount = UBound(cvIDs) + 1
    
    UndIDs = m_aContext.GetCVIDs(m_nUndLevelID)
    nUndCount = UBound(UndIDs) + 1
    
    GroupTotalIDs = m_aContext.GetCVIDs(m_nGroupTotalsLevelID)
    nGroupTotalCount = UBound(GroupTotalIDs) + 1
    
    GroupUndIDs = m_aContext.GetCVIDs(m_nGroupUndLevelID)
    nGroupUndCount = UBound(GroupUndIDs) + 1

    ' all variables must be from positions level
    If nTotalCount <> nUndCount Or nTotalCount <> nGroupTotalCount Or nTotalCount <> nGroupUndCount Then
        m_aContext.Halt
        gCmn.ErrorMsgBox m_frmOwner, "Custom variables functionality disabled due incorrect aggregation levels."
        Exit Sub
    End If

    InitGreeksRows nTotalCount
    
    Dim sGridID As String
    Dim i As Long

    For i = 0 To nTotalCount - 1
        Info = m_aContext.GetCVInfo(m_nTotalsLevelID, cvIDs(i))
        sGridID = CStr(AddGreeksRow(Info))
        m_cvIDs.Add cvIDs(i), sGridID
    Next

    LoadGreekRowsDefaults
End Sub

Private Sub UpdateRowsCaptions(ByVal nStartRow As Long, ByVal bIsUndRow As Boolean)
    On Error Resume Next
    Dim aRowData As clsGvRowData
    Dim aSynthRowData As clsGvRowData
    Dim aSynthGreek As EtsMmRisksLib.MmRvSynthGreeksAtom
    Dim i As Long, Idx As Long, nRow As Long
    
    With fgUnd
    
        nRow = nStartRow
    
        If bIsUndRow Then
            Set aRowData = .RowData(nRow)
            .TextMatrix(nStartRow, GUC_SYMBOL) = aRowData.Und.Symbol
        End If
    
        For i = 0 To g_GUR_LAST_ROW
            Idx = m_gdUndRows.Idx(i)
            If Idx = IDX_NONE Then Exit For
            
            If bIsUndRow And i <> 0 Then .AddItem "", nRow
            
            .TextMatrix(nRow, GUC_VALUE) = m_gdUndRows.Col(Idx).Caption
            If i <> 0 Then .TextMatrix(nRow, GUC_SYMBOL) = " "
            
            If bIsUndRow Then
                If aRowData.Und.HasSynthetic And Not aRowData.Und.SynthGreeks Is Nothing Then
                    For Each aSynthGreek In aRowData.Und.SynthGreeks
'                        Set aSynthRowData = New clsGvRowData
'                        aSynthRowData.SynthUndID = aSynthGreek.SynthUndID
'                        nRow = nRow + 1
'                        .AddItem "", nRow
'                        .RowData(nRow) = aSynthRowData
'                        Set aSynthRowData = Nothing
'
'                        .TextMatrix(nRow, GUC_VALUE) = aSynthGreek.SynthUndSymbol
'                        .TextMatrix(nRow, GUC_SYMBOL) = " "
'                        .Cell(flexcpAlignment, nRow, GUC_VALUE) = flexAlignRightCenter
                    Next
                End If
            End If
            
            If aRowData Is Nothing Then
                Set aRowData = New clsGvRowData
                .RowData(nRow) = aRowData
            End If
            aRowData.ParamIdx = Idx
            
            Set aRowData = Nothing
            nRow = nRow + 1
        Next
    End With
    
End Sub

Public Sub ShowSortImages(Optional ByVal nParamRow As Long = -1, _
                          Optional ByVal nParamCol As Long = -1, _
                          Optional ByVal bClear As Boolean = False)
    On Error Resume Next
    Dim Idx As Long
    Dim nRow As Long
    
    If nParamRow = -1 Then
        If bClear Then
            nParamCol = GUC_SYMBOL
        Else
            ' refresh
            nParamRow = m_nSortRow
            nParamCol = m_nSortCol
        End If
    End If
        
    ' clear old images
    Set fgUnd.Cell(flexcpPicture, 0, m_nSortCol) = Nothing
    
    nRow = m_nSortRow
    While nRow < fgUnd.Rows
        Set fgUnd.Cell(flexcpPicture, nRow, GUC_VALUE) = Nothing
        nRow = nRow + m_nParamsCount
    Wend
    
    If nParamCol = GUC_SYMBOL Then
        m_nSortRow = -1
        m_nSortCol = nParamCol
    
        geUnd.AdjustSortImages
        Exit Sub
    End If
    
    ' set new images
    m_nSortRow = nParamRow
    m_nSortCol = nParamCol
    Idx = IIf(m_iParamSort = flexSortGenericAscending, 1, 0)
    
    Set fgUnd.Cell(flexcpPicture, 0, m_nSortCol) = frmMain.imgSort(Idx).Picture
    
    nRow = m_nSortRow
    While nRow < fgUnd.Rows
        Set fgUnd.Cell(flexcpPicture, nRow, GUC_VALUE) = frmMain.imgSort(Idx).Picture
        nRow = nRow + m_nParamsCount
    Wend
    
End Sub

Public Property Get ContextID() As Long
    On Error Resume Next
    ContextID = m_nContextID
End Property

Public Property Get PosLevelID() As Long
    On Error Resume Next
    PosLevelID = m_nPosLevelID
End Property

Public Sub AddNewTradeToPosition(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, _
                                ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nQty&, nQtyInShares&
    
    nQty = aTrd.Quantity * IIf(aTrd.IsBuy, 1, -1)
    nQtyInShares = nQty * aPos.LotSize
    
    If aTrd.IsBuy Then
        If aPos.QtyLTDBuy = BAD_LONG_VALUE Then aPos.QtyLTDBuy = 0
        aPos.QtyLTDBuy = aPos.QtyLTDBuy + nQtyInShares
        If aTrd.IsPosition Then
            If aPos.PriceClose >= DBL_EPSILON Then
                If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
                aPos.PosLTDBuy = aPos.PosLTDBuy + aPos.PriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDBuy = BAD_DOUBLE_VALUE Then aPos.PosLTDBuy = 0
            aPos.PosLTDBuy = aPos.PosLTDBuy + aTrd.Price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateBuy = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateBuy = 0
            aPos.QtyDailyPrevDateBuy = aPos.QtyDailyPrevDateBuy + nQtyInShares
            If aPos.PriceClose >= DBL_EPSILON Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aPos.PriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateBuy = 0
                aPos.PosDailyPrevDateBuy = aPos.PosDailyPrevDateBuy + aTrd.Price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodayBuy = BAD_LONG_VALUE Then aPos.QtyDailyTodayBuy = 0
            aPos.QtyDailyTodayBuy = aPos.QtyDailyTodayBuy + nQtyInShares
            If aTrd.IsPosition Then
                If aPos.PriceClose >= DBL_EPSILON Then
                    If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                    aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aPos.PriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodayBuy = BAD_DOUBLE_VALUE Then aPos.PosDailyTodayBuy = 0
                aPos.PosDailyTodayBuy = aPos.PosDailyTodayBuy + aTrd.Price * nQtyInShares
            End If
        End If
    Else
        If aPos.QtyLTDSell = BAD_LONG_VALUE Then aPos.QtyLTDSell = 0
        aPos.QtyLTDSell = aPos.QtyLTDSell + nQtyInShares
        If aTrd.IsPosition Then
            If aPos.PriceClose >= DBL_EPSILON Then
                If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
                aPos.PosLTDSell = aPos.PosLTDSell + aPos.PriceClose * nQtyInShares
            End If
        Else
            If aPos.PosLTDSell = BAD_DOUBLE_VALUE Then aPos.PosLTDSell = 0
            aPos.PosLTDSell = aPos.PosLTDSell + aTrd.Price * nQtyInShares
        End If
    
        If aTrd.TradeDate < Date Then
            If aPos.QtyDailyPrevDateSell = BAD_LONG_VALUE Then aPos.QtyDailyPrevDateSell = 0
            aPos.QtyDailyPrevDateSell = aPos.QtyDailyPrevDateSell + nQtyInShares
            If aPos.PriceClose >= DBL_EPSILON Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aPos.PriceClose * nQtyInShares
            ElseIf Not aTrd.IsPosition Then
                If aPos.PosDailyPrevDateSell = BAD_DOUBLE_VALUE Then aPos.PosDailyPrevDateSell = 0
                aPos.PosDailyPrevDateSell = aPos.PosDailyPrevDateSell + aTrd.Price * nQtyInShares
            End If
        Else
            If aPos.QtyDailyTodaySell = BAD_LONG_VALUE Then aPos.QtyDailyTodaySell = 0
            aPos.QtyDailyTodaySell = aPos.QtyDailyTodaySell + nQtyInShares
            If aTrd.IsPosition Then
                If aPos.PriceClose >= DBL_EPSILON Then
                    If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                    aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aPos.PriceClose * nQtyInShares
                End If
            Else
                If aPos.PosDailyTodaySell = BAD_DOUBLE_VALUE Then aPos.PosDailyTodaySell = 0
                aPos.PosDailyTodaySell = aPos.PosDailyTodaySell + aTrd.Price * nQtyInShares
            End If
        End If
    End If
    
    aPos.Qty = aPos.Qty + nQty
    aPos.QtyInShares = aPos.QtyInShares + nQtyInShares
    
    If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
        If aUnd.OptQty = BAD_LONG_VALUE Then aUnd.OptQty = 0
        aUnd.OptQty = aUnd.OptQty + nQty
        
    ElseIf aPos.ContractType = enCtFuture Then
        If aUnd.FutQty = BAD_LONG_VALUE Then aUnd.FutQty = 0
        aUnd.FutQty = aUnd.FutQty + nQty
        
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty + nQtyInShares
        
    Else
        If aUnd.Qty = BAD_LONG_VALUE Then aUnd.Qty = 0
        aUnd.Qty = aUnd.Qty + nQtyInShares
    End If
End Sub

