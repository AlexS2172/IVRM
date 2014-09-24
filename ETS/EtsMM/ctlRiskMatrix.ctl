VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlRiskMatrix 
   Alignable       =   -1  'True
   ClientHeight    =   6585
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   11430
   FillColor       =   &H8000000E&
   KeyPreview      =   -1  'True
   ScaleHeight     =   6585
   ScaleWidth      =   11430
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1800
      Top             =   4440
   End
   Begin VSFlex7Ctl.VSFlexGrid fgVal 
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
   End
   Begin VSFlex7Ctl.VSFlexGrid fgScn 
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
      End
      Begin VB.Menu mnuCtxSeparator1 
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
      Begin VB.Menu mnuCtxWtdVega 
         Caption         =   "Weighted Vega Settings..."
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxScnNew 
         Caption         =   "New Scenario"
      End
      Begin VB.Menu mnuCtxScnSave 
         Caption         =   "Save Scenario"
      End
      Begin VB.Menu mnuCtxScnDelete 
         Caption         =   "Delete Scenario"
      End
      Begin VB.Menu mnuCtxScnLoadParams 
         Caption         =   "Load Specific Params"
      End
      Begin VB.Menu mnuCtxSeparator4 
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
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDisplayInGraph 
         Caption         =   "Display RiskMatrix in Graphics"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlRiskMatrix"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()

Public Event OnScreenRefresh(ByRef Res() As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                             ByRef Axis() As RmAxisTypeEnum, _
                             ByRef Filter As EtsGeneralLib.EtsFilterData)

Public Event OnScreenClose()


Public Event OnRefreshComplete()
Public Event OnRefreshError()
Public Event OnRefreshEmpty()
Public Event OnRefreshCancel()


Private m_gdFlt As New clsGridDef
Private m_gdScn As New clsGridDef
Private m_gdVal As New clsGridDef

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
Private VolaSource As VolatilitySourcesLib.VolatilitySource
Private TradeChannel As clsTradeChannel

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1
Private WithEvents frmWtdVega As frmWtdVegaSettings
Attribute frmWtdVega.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label
Public lblValue As vB.Label
Public WithEvents imgStop As vB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As vB.Image

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bRecalc As Boolean
Private m_bLastQuoteReqNow As Boolean

Private m_nOpenedExpiry As Long

Private m_QuoteRequests As EtsMmRisksLib.MmRvReqColl
Private m_Idx As EtsMmRisksLib.MmRvUndAtom
Private m_Grp As EtsMmRisksLib.MmRvGrpAtom
Private m_Und As EtsMmRisksLib.MmRvUndColl
Private m_Exp As EtsGeneralLib.EtsMmEntityAtomColl
Private m_Scn As New clsRmScenarioAtom
'Private m_Scenarios As New clsRmScenarioColl
Private WithEvents m_View As EtsMmRisksLib.MmRiskView
Attribute m_View.VB_VarHelpID = -1


Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Public m_frmOwner As Form

Private m_sCurrentOriginalText As String
Private m_nCurrentOriginalScnID As Long
Private m_aFilter As EtsGeneralLib.EtsFilterData
Private m_nGreeks As Long

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long
Private m_bFirstTime As Boolean

Private m_bKeyDown(GT_MATRIX_FILTER To GT_MATRIX_VALUES) As Boolean
Private m_GridLock(GT_MATRIX_FILTER To GT_MATRIX_VALUES) As New clsGridLock

Private m_Res() As EtsMmRisksLib.MmRvMatrixCalcResultType
Private m_BasePoint(RMA_HORZ To RMA_VERT) As Long

Private m_bGroupRequest As Boolean
Private m_bGroupRequestSupported As Boolean
Private m_bShutDown As Boolean
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Public Property Get Filter() As EtsGeneralLib.EtsFilterData
    Set Filter = m_aFilter
End Property

Public Function Init() As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    m_bFirstTime = True
    
    Set m_aFilter = New EtsGeneralLib.EtsFilterData
    m_aFilter.Initialize MFC_SYMBOL, MFC_COLUMN_COUNT
    
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set m_View = New EtsMmRisksLib.MmRiskView
    m_View.ViewMode = RV_RISKMATRIX
    
    Set m_Idx = m_View.Idx
    Set m_Grp = m_View.Grp
    Set m_Und = m_View.Und
    Set m_Exp = m_View.Exp
    Set m_QuoteRequests = m_View.QuoteReqsAll
    
    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_MATRIX_FILTER) = False
    m_bKeyDown(GT_MATRIX_SCENARIO) = False
    m_bKeyDown(GT_MATRIX_VALUES) = False
    
    m_GridLock(GT_MATRIX_FILTER).Init fgFlt
    m_GridLock(GT_MATRIX_SCENARIO).Init fgScn
    m_GridLock(GT_MATRIX_VALUES).Init fgVal
    
    InitColumns
    InitGrids
    
    FormatFltGrid
    FormatFltColumns
    
    FormatScnGrid
    FormatScnColumns
    
    InitFltData
    InitScnData
    
    If g_RmScenario.Count > 0 Then
        g_RmScenario(1).CopyTo m_Scn
    End If
    
    InitScnList
    
    Set VolaSource = g_VolaSource
    Set TradeChannel = g_TradeChannel
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing

    PriceProvider.Connect
    
    m_bGroupRequest = False
    
    m_bGroupRequestSupported = False ' g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequestSupported Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequestSupported = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    fgFlt.Col = MFC_SYMBOL
    fgFlt.Row = 1
    
    fgScn.Col = 0
    fgScn.Row = 1
    
    Set frmLayout = New frmGridLayout
    Set frmWtdVega = g_frmWtdVegaSettings
    Set aParams = g_Params
    
    Init = True
    
    AdjustCaption
    AdjustState
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize risk matrix."
    RaiseEvent OnRefreshError
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Select Case nType
        Case TYPE_UNDERLYING
            m_aFilter.Data(MFC_SYMBOL) = nValue
        Case TYPE_GROUP
            m_aFilter.Data(MFC_GROUPS) = nValue
        Case TYPE_TRADER
            m_aFilter.Data(MFC_TRADER) = nValue
        Case TYPE_TRADER_GROUP
            m_aFilter.Data(MFC_TRADER_GROUP) = nValue
         Case TYPE_STRATEGY
            m_aFilter.Data(MFC_STRATEGY) = nValue
    End Select
    
    FilterUpdateAll
    ScenarioUpdate True
    ClearValGrid True
    
    tmrShow.Enabled = True
End Sub
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
     m_aFilter.Data(MFC_SYMBOL) = aFilters.Data(MFC_SYMBOL)
     m_aFilter.Data(MFC_GROUPS) = aFilters.Data(MFC_GROUPS)
     m_aFilter.Data(MFC_TRADER) = aFilters.Data(MFC_TRADER)
     m_aFilter.Data(MFC_TRADER_GROUP) = aFilters.Data(MFC_TRADER_GROUP)
     m_aFilter.Data(MFC_STRATEGY) = aFilters.Data(MFC_STRATEGY)
     m_aFilter.Data(MFC_TRADES) = aFilters.Data(MFC_TRADES)
    
    FilterUpdateAll
    
    ScenarioUpdate True
    ClearValGrid True
    
    tmrShow.Enabled = True

End Sub

Public Sub StopNow()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
    
        fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        AdjustState
        
    ElseIf m_bDataLoad Then
        m_bDataLoad = False
    
        fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
        
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
    
    RaiseEvent OnRefreshCancel
End Sub

Private Sub ClearValGrid(ByVal bAutosize As Boolean)
    On Error Resume Next
    InitResults
    FormatValGrid
    fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
    FormatValColumns bAutosize
End Sub

Private Sub InitScnData()
    On Error Resume Next
    Dim i&, sComboList$
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
        
        .ColComboList(MSC_HORZ_AXIS) = "#" & CStr(RMAT_VOLA) & ";Vola" & _
                                    "|#" & CStr(RMAT_SPOT) & ";Spot" & _
                                    "|#" & CStr(RMAT_TIME) & ";Time"
        
        .ColComboList(MSC_VERT_AXIS) = .ColComboList(MSC_HORZ_AXIS)
        
        sComboList = "#1;1"
        For i = 3 To RM_MAX_POINTS
            If (i Mod 2) = 1 Then
                sComboList = sComboList & "|#" & CStr(i) & ";" & CStr(i)
            End If
        Next
        .ColComboList(MSC_HORZ_POINTS) = sComboList
        .ColComboList(MSC_VERT_POINTS) = sComboList
        
        .ColComboList(MSC_VOLA_SHIFT) = "#" & CStr(RMVS_COMMON) & ";Common" & _
                                    "|#" & CStr(RMVS_WEIGHTED) & ";Weighted"
                                    
        .ColComboList(MSC_SPOT_SHIFT) = "#0;Parallel|#1;Correlated"
        .TextMatrix(1, MSC_SPOT_SHIFT) = "0"
        
        .ColComboList(MSC_CORR_INDEX) = "#0;<None>"
        .TextMatrix(1, MSC_CORR_INDEX) = "0"
        
        .ColComboList(MSC_CALC_MODEL) = "#" & CStr(enCmBinomial) & ";" & g_Params.CalcModelName(enCmBinomial) & _
                                    "|#" & CStr(enCmBinomialOpt) & ";" & g_Params.CalcModelName(enCmBinomialOpt) & _
                                    "|#" & CStr(enCmVskLog) & ";" & g_Params.CalcModelName(enCmVskLog)
        .TextMatrix(1, MSC_CALC_MODEL) = g_Params.MatrixCalcModel
        
        .ColComboList(MSC_SPOT_SPECIFIC) = "#0;Common|#1;Specific"
        .TextMatrix(1, MSC_SPOT_SPECIFIC) = "0"
        
        .ColComboList(MSC_VOLA_SPECIFIC) = "#0;Common|#1;Specific"
        .TextMatrix(1, MSC_VOLA_SPECIFIC) = "0"
        
        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
    
    ScenarioUpdateIndex True
    
End Sub

Private Sub ScenarioUpdate(ByVal bName As Boolean)
    On Error Resume Next
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
        
        If bName Then
            .TextMatrix(1, MSC_NAME) = m_Scn.ScenarioName
        End If
        
        .TextMatrix(1, MSC_HORZ_AXIS) = m_Scn.Axis(RMA_HORZ)
        .TextMatrix(1, MSC_HORZ_POINTS) = m_Scn.Points(RMA_HORZ)
        .TextMatrix(1, MSC_HORZ_STEP) = m_Scn.Step(RMA_HORZ)
        .TextMatrix(1, MSC_HORZ_STEP_HOUR) = m_Scn.Hour(RMA_HORZ)
        .TextMatrix(1, MSC_HORZ_STEP_MIN) = m_Scn.Minute(RMA_HORZ)
        
        .TextMatrix(1, MSC_VERT_AXIS) = m_Scn.Axis(RMA_VERT)
        .TextMatrix(1, MSC_VERT_POINTS) = m_Scn.Points(RMA_VERT)
        .TextMatrix(1, MSC_VERT_STEP) = m_Scn.Step(RMA_VERT)
        .TextMatrix(1, MSC_VERT_STEP_HOUR) = m_Scn.Hour(RMA_VERT)
        .TextMatrix(1, MSC_VERT_STEP_MIN) = m_Scn.Minute(RMA_VERT)
        
        .TextMatrix(1, MSC_VALUE_CHANGE) = m_Scn.ShowChange
        
        
        If m_Und.Count > 1 Then
            If m_Scn.Axis(RMA_HORZ) <> RMAT_TIME Then
                .ColComboList(MSC_HORZ_UNITS) = "#" & CStr(RMUT_PERC) & ";%%"
                .ColDataType(MSC_HORZ_STEP) = flexDTDouble
                .ColFormat(MSC_HORZ_STEP) = "#,##0.00"
                .TextMatrix(0, MSC_HORZ_STEP) = "hStep"
                
                .ColHidden(MSC_HORZ_STEP_HOUR) = True
                .ColHidden(MSC_HORZ_STEP_MIN) = True
            Else
                .ColComboList(MSC_HORZ_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs"
                .ColDataType(MSC_HORZ_STEP) = flexDTLong
                .ColFormat(MSC_HORZ_STEP) = "#,##0"
                .TextMatrix(0, MSC_HORZ_STEP) = "hStep days"
                
                .ColHidden(MSC_HORZ_STEP_HOUR) = False
                .ColHidden(MSC_HORZ_STEP_MIN) = False
            End If
            
            If m_Scn.Axis(RMA_VERT) <> RMAT_TIME Then
                .ColComboList(MSC_VERT_UNITS) = "#" & CStr(RMUT_PERC) & ";%%"
                .ColDataType(MSC_VERT_STEP) = flexDTDouble
                .ColFormat(MSC_VERT_STEP) = "#,##0.00"
                .TextMatrix(0, MSC_VERT_STEP) = "vStep"
                
                .ColHidden(MSC_VERT_STEP_HOUR) = True
                .ColHidden(MSC_VERT_STEP_MIN) = True
            Else
                .ColComboList(MSC_VERT_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs"
                .ColDataType(MSC_VERT_STEP) = flexDTLong
                .ColFormat(MSC_VERT_STEP) = "#,##0"
                .TextMatrix(0, MSC_VERT_STEP) = "vStep days"
                
                .ColHidden(MSC_VERT_STEP_HOUR) = False
                .ColHidden(MSC_VERT_STEP_MIN) = False
            End If
        Else
            If m_Scn.Axis(RMA_HORZ) <> RMAT_TIME Then
                .ColComboList(MSC_HORZ_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs" & _
                                            "|#" & CStr(RMUT_PERC) & ";%%"
                .ColDataType(MSC_HORZ_STEP) = flexDTDouble
                .ColFormat(MSC_HORZ_STEP) = "#,##0.00"
                .TextMatrix(0, MSC_HORZ_STEP) = "hStep"
                
                .ColHidden(MSC_HORZ_STEP_HOUR) = True
                .ColHidden(MSC_HORZ_STEP_MIN) = True
            Else
                .ColComboList(MSC_HORZ_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs"
                .ColDataType(MSC_HORZ_STEP) = flexDTLong
                .ColFormat(MSC_HORZ_STEP) = "#,##0"
                .TextMatrix(0, MSC_HORZ_STEP) = "hStep days"
                
                .ColHidden(MSC_HORZ_STEP_HOUR) = False
                .ColHidden(MSC_HORZ_STEP_MIN) = False
            End If
                                        
            If m_Scn.Axis(RMA_VERT) <> RMAT_TIME Then
                .ColComboList(MSC_VERT_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs" & _
                                            "|#" & CStr(RMUT_PERC) & ";%%"
            
                .ColDataType(MSC_VERT_STEP) = flexDTDouble
                .ColFormat(MSC_VERT_STEP) = "#,##0.00"
                .TextMatrix(0, MSC_VERT_STEP) = "vStep"
                
                .ColHidden(MSC_VERT_STEP_HOUR) = True
                .ColHidden(MSC_VERT_STEP_MIN) = True
            Else
                .ColComboList(MSC_VERT_UNITS) = "#" & CStr(RMUT_ABS) & ";Abs"
                .ColDataType(MSC_VERT_STEP) = flexDTLong
                .ColFormat(MSC_VERT_STEP) = "#,##0"
                .TextMatrix(0, MSC_VERT_STEP) = "vStep days"
                
                .ColHidden(MSC_VERT_STEP_HOUR) = False
                .ColHidden(MSC_VERT_STEP_MIN) = False
            End If
        End If
        
        .TextMatrix(1, MSC_HORZ_UNITS) = m_Scn.Units(RMA_HORZ)
        .TextMatrix(1, MSC_VERT_UNITS) = m_Scn.Units(RMA_VERT)
        
        .TextMatrix(1, MSC_VOLA_SHIFT) = m_Scn.VolaShiftType
        .TextMatrix(1, MSC_SPOT_SHIFT) = m_Scn.SpotShiftType
        
        .TextMatrix(1, MSC_SPOT_SPECIFIC) = m_Scn.SpotSpecificShiftType
        .TextMatrix(1, MSC_VOLA_SPECIFIC) = m_Scn.VolaSpecificShiftType
        
        .TextMatrix(1, MSC_CALC_MODEL) = m_Scn.CalcModel
        .TextMatrix(1, MSC_CORR_INDEX) = m_Scn.CorrIndex
        .ColHidden(MSC_VOLA_SHIFT) = Not (m_Scn.Axis(RMA_HORZ) = RMAT_VOLA Or m_Scn.Axis(RMA_VERT) = RMAT_VOLA)
    
        .AutoSize 0, .Cols - 1, , 100
        
        ReloadGroupRatio
        
        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
End Sub

Private Sub InitScnList()
    On Error Resume Next
    Dim sComboList$, sValue$, aScn As clsRmScenarioAtom
    Dim nValue&
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw

        sValue = "0"
        nValue = 0
        sComboList = ""
        
        If m_Scn.ID = 0 Then
            sComboList = sComboList & "|#" & Trim$(Str$(m_Scn.ID)) & ";" & m_Scn.ScenarioName
        End If
        
        For Each aScn In g_RmScenario
            If aScn.ID <> m_Scn.ID Then
                sComboList = sComboList & "|#" & Trim$(Str$(aScn.ID)) & ";" & aScn.ScenarioName
            Else
                nValue = m_Scn.ID
                sValue = m_Scn.ScenarioName
                sComboList = sComboList & "|#" & Trim$(Str$(m_Scn.ID)) & ";" & m_Scn.ScenarioName
            End If
        Next

        .Cell(flexcpData, 1, MSC_NAME) = sComboList
        If nValue <> 0 Then .TextMatrix(1, MSC_NAME) = sValue

        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateAll()
    On Error Resume Next
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
        
        FilterUpdateValue True, MFC_SYMBOL
        FilterUpdateValue True, MFC_GROUPS
        FilterUpdateValue True, MFC_TRADER
        FilterUpdateValue True, MFC_TRADER_GROUP
        FilterUpdateValue True, MFC_STRATEGY

        .TextMatrix(1, MFC_TRADES) = m_aFilter.Data(MFC_TRADES)
        
        FilterUpdateGreeks
        
        'FilterUpdateIndex True
        
        FilterUpdateExpiry True

        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateValue(ByVal bAutosize As Boolean, enColumn As MatrixFilterColumnEnum)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim nValue&, aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    Dim aTraderGroup As EtsGeneralLib.EtsTraderGroupAtom
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = ""
        Select Case enColumn
           Case MFC_SYMBOL
                sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
                
                Set aUnd = g_Underlying(m_aFilter.Data(MFC_SYMBOL))
                If Not aUnd Is Nothing Then
                    If aUnd.UndType <> enCtFutUnd Then
                        nValue = aUnd.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                    
                    Set aUnd = Nothing
                End If
                
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
           Case MFC_GROUPS
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_aFilter.Data(MFC_GROUPS) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case MFC_TRADER
               For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = m_aFilter.Data(MFC_TRADER) Then
                        nValue = aTrader.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case MFC_TRADER_GROUP
               For Each aTraderGroup In g_TraderGroup
                    If aTraderGroup.ID <> 0 Then
                        sComboList = sComboList & "|#" & Trim$(Str$(aTraderGroup.ID)) & ";" & aTraderGroup.Name
                    End If
                    If aTraderGroup.ID = m_aFilter.Data(MFC_TRADER_GROUP) Then
                        nValue = aTraderGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case MFC_STRATEGY
                sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
                If m_aFilter.Data(MFC_STRATEGY) = NO_STRATEGY_ID Then
                    nValue = NO_STRATEGY_ID
                    sValue = NO_STRATEGY_NAME
                End If
                
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_aFilter.Data(MFC_STRATEGY) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case Else
             Debug.Assert False
             Exit Sub
        End Select
        
        
        If Len(sComboList) > 0 Then
                If InStr(1, sComboList, "|") = 1 Then
                    sComboList = "#0;<All>|" & Mid$(sComboList, InStr(1, sComboList, "|") + 1)
                Else
                    sComboList = "#0;<All>|" & sComboList
                End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(enColumn) = sComboList
        .TextMatrix(1, enColumn) = sValue
        m_aFilter.Data(enColumn) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub ScenarioUpdateIndex(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim sComboList$, sValue$, aIdx As EtsGeneralLib.IndexAtom
    Dim nValue&, nFirstValue&
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
        
        sValue = "0"
        nValue = 0
        nFirstValue = 0
        sComboList = "|#0;<None>"
        For Each aIdx In g_HedgeSymbols
            
            If aIdx.HaveComponentBetas Then
                sComboList = sComboList & "|#" & Trim$(Str$(aIdx.ID)) & ";" & aIdx.Symbol
                If aIdx.ID = m_Scn.CorrIndex Then
                    nValue = aIdx.ID
                    sValue = Trim$(Str$(aIdx.ID))
                End If
                If nFirstValue = 0 Then nFirstValue = aIdx.ID
            End If
            
        Next
        
        If Len(sComboList) > 0 Then
            sComboList = Mid$(sComboList, 2)
            If nValue = 0 Then
                nValue = nFirstValue
                sValue = Trim$(Str$(nValue))
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(MSC_CORR_INDEX) = sComboList
        .TextMatrix(1, MSC_CORR_INDEX) = nValue
        m_Scn.CorrIndex = nValue
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If
        
        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateExpiry(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim nValue&
    Dim aExp As EtsGeneralLib.EtsMmEntityAtom, sValue$, sComboList$
    
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aExp In m_Exp
            sComboList = sComboList & "|#" & Trim$(Str$(aExp.ID)) & ";" & aExp.Name
            If aExp.ID = m_aFilter.Data(MFC_EXPIRY) Then
                nValue = aExp.ID
                sValue = Trim$(Str$(aExp.ID))
            End If
        Next
        .ColComboList(MFC_EXPIRY) = sComboList
        .TextMatrix(1, MFC_EXPIRY) = sValue
        m_aFilter.Data(MFC_EXPIRY) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Function CheckPosFilter(ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    nValue = m_aFilter.Data(MFC_EXPIRY)
    bMatched = (nValue = 0)
    If Not bMatched Then
        If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
            bMatched = (nValue = CLng(aPos.Expiry))
        Else
            bMatched = False
        End If
    End If

    CheckPosFilter = bMatched
End Function

Public Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_MATRIX_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_MATRIX_SCENARIO).CopyTo m_gdScn
    g_DefGrids(GT_MATRIX_VALUES).CopyTo m_gdVal
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = MFC_COLUMN_COUNT
        
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
    
    With fgScn
        .Rows = 2
        .Cols = MSC_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
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
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        
        .HighLight = flexHighlightWithFocus
    End With

    With fgVal
        .Rows = 1
        .Cols = 10
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 2
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
    
        .HighLight = flexHighlightWithFocus
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim i&
    
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
        
        .ColComboList(MFC_TRADES) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, MFC_TRADES) = "0"
    
        .ColComboList(MFC_EXPIRY) = "#0;<All>"
        .TextMatrix(1, MFC_EXPIRY) = "0"
        m_aFilter.Data(MFC_EXPIRY) = 0
    
        
        .ColComboList(MFC_TRADER_GROUP) = "#0;<None>"
        .TextMatrix(1, MFC_TRADER_GROUP) = "0"
        m_aFilter.Data(MFC_TRADER_GROUP) = 0
    
        FilterUpdateAll
        For i = MFC_PNL To MFC_LAST_COLUMN
            .TextMatrix(1, i) = 1
            m_aFilter.Data(i) = 1
        Next
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
    
        nCols = MFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = MFC_SYMBOL To MFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
        
        Set .Font = Nothing
        
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = MFC_SYMBOL To MFC_LAST_COLUMN
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
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatScnGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
    
        nCols = MSC_COLUMN_COUNT
        .Cols = nCols
        
        For i = MSC_NAME To MSC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdScn.Col(i).Caption
            .ColFormat(i) = m_gdScn.Col(i).Format
            .ColDataType(i) = m_gdScn.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
End Sub

Private Sub FormatScnColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgScn
        m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
    
        Set .Font = Nothing
        
        aFont.Name = m_gdScn.Font.Name
        aFont.Size = m_gdScn.Font.Size
        aFont.Bold = m_gdScn.Font.Bold
        aFont.Italic = m_gdScn.Font.Italic
        aFont.Strikethrough = m_gdScn.Font.Strikethrough
        aFont.Underline = m_gdScn.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = MSC_NAME To MSC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdScn.Col(i).BackColor <> 0, m_gdScn.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdScn.Col(i).ForeColor <> 0, m_gdScn.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If m_gdScn.HorizLinesVisible Then
            If m_gdScn.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdScn.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdScn.GridLinesColor
        
        .BackColorSel = m_gdScn.BackColorSel
        .ForeColorSel = m_gdScn.ForeColorSel
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateGreeks()
    On Error Resume Next
    
    On Error Resume Next
    Dim i&
    
    With fgFlt
        m_GridLock(GT_MATRIX_FILTER).LockRedraw
        
        m_nGreeks = 0
        For i = MFC_PNL To MFC_LAST_COLUMN
            If m_aFilter.Data(i) <> 0 Then m_nGreeks = m_nGreeks + 1
            .TextMatrix(1, i) = m_aFilter.Data(i)
        Next
        
        If m_nGreeks = 0 Then
            m_aFilter.Data(MFC_PNL) = 1
            m_nGreeks = 1
            .TextMatrix(1, MFC_PNL) = 1
        End If
        
        m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatValGrid()
    On Error Resume Next
    Dim i&, p&, g&, nLastCol&, nLastRow&, nCount&, sCaption$
    Dim dtDate As Date
    
    With fgVal
        m_GridLock(GT_MATRIX_VALUES).LockRedraw
        
        If m_nGreeks = 0 Then
            m_aFilter.Data(MFC_PNL) = 1
            m_nGreeks = 1
            .TextMatrix(1, MFC_PNL) = 1
        End If
        
        .Cols = m_Scn.Points(RMA_HORZ) + 2
        .Rows = m_nGreeks * m_Scn.Points(RMA_VERT) + 1
        nLastCol = .Cols - 1
        nLastRow = .Rows - 1
        
        For i = 2 To nLastCol
            .ColFormat(i) = m_gdVal.Col(MVC_VALUE).Format
            .ColDataType(i) = m_gdVal.Col(MVC_VALUE).DataType
        Next
         
        .Cell(flexcpAlignment, 0, 0, 0, nLastCol) = flexAlignCenterCenter
        .Cell(flexcpAlignment, 0, 0, nLastRow, 1) = flexAlignCenterCenter
        
        nCount = m_Scn.Points(RMA_VERT)
        For p = 1 To nCount
        
            If m_Scn.Axis(RMA_VERT) <> RMAT_TIME Then
                .Cell(flexcpText, (p - 1) * m_nGreeks + 1, 0, p * m_nGreeks, 0) = Format$((p - m_BasePoint(RMA_VERT)) * m_Scn.Step(RMA_VERT), "#,##0.00")
            Else
                dtDate = DateAdd("d", (p - m_BasePoint(RMA_VERT)) * m_Scn.Step(RMA_VERT), GetNewYorkTime)
                dtDate = DateAdd("h", (p - m_BasePoint(RMA_VERT)) * m_Scn.Hour(RMA_VERT), dtDate)
                dtDate = DateAdd("n", (p - m_BasePoint(RMA_VERT)) * m_Scn.Minute(RMA_VERT), dtDate)
                
                .Cell(flexcpText, (p - 1) * m_nGreeks + 1, 0, p * m_nGreeks, 0) = Format$(dtDate, "MM/DD/YYYY hh:mm AMPM")
            End If
            
            g = 0
            For i = 0 To MFC_LAST_COLUMN - MFC_PNL
                If m_aFilter.Data(MFC_PNL + i) <> 0 Then
                    sCaption = m_gdFlt.Col(MFC_PNL + i).Caption
                    .TextMatrix((p - 1) * m_nGreeks + g + 1, 1) = sCaption
                    g = g + 1
                End If
            Next
        Next
        
        .Cell(flexcpText, 0, 0, 0, 0) = m_Scn.AxisName(RMA_VERT)
        .Cell(flexcpText, 0, 1, 0, 1) = m_Scn.AxisName(RMA_HORZ)
        
        nCount = m_Scn.Points(RMA_HORZ)
        For p = 1 To nCount
            If m_Scn.Axis(RMA_HORZ) <> RMAT_TIME Then
                .Cell(flexcpText, 0, p + 1) = (p - m_BasePoint(RMA_HORZ)) * m_Scn.Step(RMA_HORZ)
            Else
                dtDate = DateAdd("d", (p - m_BasePoint(RMA_HORZ)) * m_Scn.Step(RMA_HORZ), GetNewYorkTime)
                dtDate = DateAdd("h", (p - m_BasePoint(RMA_HORZ)) * m_Scn.Hour(RMA_HORZ), dtDate)
                dtDate = DateAdd("n", (p - m_BasePoint(RMA_HORZ)) * m_Scn.Minute(RMA_HORZ), dtDate)
            
                .Cell(flexcpText, 0, p + 1) = Format$(dtDate, "MM/DD/YYYY hh:mm AMPM")
            End If
        Next
        
        .MergeCol(0) = True
        
        .AutoSize 0, 1, , 100

        m_GridLock(GT_MATRIX_VALUES).UnlockRedraw
    End With
End Sub

Private Sub FormatValColumns(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim nCols&, nRows&, aFont As New StdFont
    
    With fgVal
        m_GridLock(GT_MATRIX_VALUES).LockRedraw
    
        Set .Font = Nothing
        
        aFont.Name = m_gdVal.Font.Name
        aFont.Size = m_gdVal.Font.Size
        aFont.Bold = m_gdVal.Font.Bold
        aFont.Italic = m_gdVal.Font.Italic
        aFont.Strikethrough = m_gdVal.Font.Strikethrough
        aFont.Underline = m_gdVal.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        
        .Cell(flexcpBackColor, 1, 2, nRows, nCols) = IIf(m_gdVal.Col(MVC_VALUE).BackColor <> 0, m_gdVal.Col(MVC_VALUE).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, 2, nRows, nCols) = IIf(m_gdVal.Col(MVC_VALUE).ForeColor <> 0, m_gdVal.Col(MVC_VALUE).ForeColor, RGB(1, 1, 1))
            
        .Cell(flexcpBackColor, 1, m_BasePoint(RMA_HORZ) + 1, nRows, m_BasePoint(RMA_HORZ) + 1) = IIf(m_gdVal.Col(MVC_BASE).BackColor <> 0, m_gdVal.Col(MVC_BASE).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, 1, m_BasePoint(RMA_HORZ) + 1, nRows, m_BasePoint(RMA_HORZ) + 1) = IIf(m_gdVal.Col(MVC_BASE).ForeColor <> 0, m_gdVal.Col(MVC_BASE).ForeColor, RGB(1, 1, 1))
            
        .Cell(flexcpBackColor, (m_BasePoint(RMA_VERT) - 1) * m_nGreeks + 1, 2, m_BasePoint(RMA_VERT) * m_nGreeks, nCols) = IIf(m_gdVal.Col(MVC_BASE).BackColor <> 0, m_gdVal.Col(MVC_BASE).BackColor, RGB(1, 1, 1))
        .Cell(flexcpForeColor, (m_BasePoint(RMA_VERT) - 1) * m_nGreeks + 1, 2, m_BasePoint(RMA_VERT) * m_nGreeks, nCols) = IIf(m_gdVal.Col(MVC_BASE).ForeColor <> 0, m_gdVal.Col(MVC_BASE).ForeColor, RGB(1, 1, 1))
            
        If m_gdVal.HorizLinesVisible Then
            If m_gdVal.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdVal.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdVal.GridLinesColor
        
        .BackColorSel = m_gdVal.BackColorSel
        .ForeColorSel = m_gdVal.ForeColorSel
        
        If bAutosize Then
            .AutoSize 2, .Cols - 1
        End If
    
        m_GridLock(GT_MATRIX_VALUES).UnlockRedraw
    End With
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = ""
    If m_Grp.ID <> 0 Then sCaption = sCaption & m_Grp.Name & " - "
    sCaption = sCaption & "Risk Matrix"
    
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

Public Property Get DataLoad() As Boolean
    DataLoad = m_bDataLoad
End Property

Public Property Get LastQuoteReqNow() As Boolean
    LastQuoteReqNow = m_bLastQuoteReqNow
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_MATRIX_FILTER, MFC_SYMBOL, MFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeScnGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_MATRIX_SCENARIO, MSC_NAME, MSC_LAST_COLUMN, m_gdScn, m_frmOwner
    End If
End Sub

Public Sub CustomizeValGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_MATRIX_VALUES, MVC_VALUE, MVC_LAST_COLUMN, m_gdVal, m_frmOwner
    End If
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey

    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height

    ' common info
    For i = MFC_SYMBOL To MFC_COLUMN_COUNT
        aStorage.SetLongValue "MatrixFlt" & sKey, "Filter" & CStr(i), m_aFilter.Data(i)
    Next
    
    m_gdFlt.WriteToStorage "MatrixFltGrid" & sKey, aStorage, False
    m_gdScn.WriteToStorage "MatrixScnGrid" & sKey, aStorage
    m_gdVal.WriteToStorage "MatrixValGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String, _
                        Optional ByVal bRefreshData As Boolean = True, _
                        Optional ByVal bAllowResize As Boolean = True)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    If (bAllowResize = True) Then
        m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
        m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
        m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
        m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
    End If
    ' common info
    For i = MFC_SYMBOL To MFC_COLUMN_COUNT
        m_aFilter.Data(i) = aStorage.GetLongValue("MatrixFlt" & sKey, "Filter" & CStr(i), m_aFilter.Data(i))
    Next
    
    If m_aFilter.Data(MFC_TRADES) < 0 Or m_aFilter.Data(MFC_TRADES) > 6 Then m_aFilter.Data(MFC_TRADES) = 0
    
    m_gdFlt.ReadFromStorage "MatrixFltGrid" & sKey, aStorage, False
    m_gdScn.ReadFromStorage "MatrixScnGrid" & sKey, aStorage
    m_gdVal.ReadFromStorage "MatrixValGrid" & sKey, aStorage
    
    FilterUpdateAll
    ScenarioUpdate True
    ClearValGrid True
    tmrShow.Enabled = bRefreshData 'True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
    RaiseEvent OnRefreshError
End Sub

Public Function Group() As EtsMmRisksLib.MmRvGrpAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Function CurUnderlyingID() As Long
    On Error Resume Next
   
    If m_Grp.GroupType = TYPE_UNDERLYING Then
        CurUnderlyingID = m_Grp.ID
    Else
        CurUnderlyingID = 0
    End If
End Function

Public Sub Refresh()
    On Error Resume Next
    m_nOpenedExpiry = m_aFilter.Data(MFC_EXPIRY)
    tmrShow.Enabled = True
End Sub

Private Sub ClearViewAndData()
    On Error Resume Next
    
    fgVal.Cell(flexcpText, 1, 2, fgVal.Rows - 1, fgVal.Cols - 1) = ""
    lblValue.Caption = ""

    m_Grp.Clear
    m_Und.Clear
    m_Exp.Clear
    m_QuoteRequests.Clear
    
    FilterUpdateExpiry True
    SetRefreshHint False
End Sub

Private Sub UpdateCurrentValue()
    On Error Resume Next
'    Dim sValue$, dValue#
'
'    lblValue.Caption = ""
'    With fgVal
'        If .Row > 0 And .Col > 1 Then
'            sValue = .TextMatrix(.Row, .Col)
'            If sValue <> "" Then
'                If sValue <> STR_NA Then
'                    lblValue.Caption = .TextMatrix(.Row, 1) & ": " & Format$(.ValueMatrix(.Row, .Col), "#,##0.##############################")
'                Else
'                    lblValue.Caption = .TextMatrix(.Row, 1) & ": N/A"
'                End If
'            End If
'        End If
'    End With
End Sub

Private Sub UnderlyingsCalculate()
    On Error Resume Next
    m_bInProc = True
    AdjustState
                
    CalcMatrix
                
    m_bInProc = False
    AdjustState
End Sub

Private Sub aParams_PriceProfilesChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    Dim aGUnd As EtsGeneralLib.UndAtom
    
    If m_Grp.GroupType = 0 Or m_Grp.ID = 0 Then Exit Sub
    bUpdate = False
    
    On Error Resume Next
    For Each aFilterAtom In collUpdUnd
        If Not m_Und(aFilterAtom.ID) Is Nothing Then
            bUpdate = True
            Set aFilterAtom = Nothing
            Exit For
        End If
    Next
    If bUpdate Then SetRefreshHint True
End Sub

Private Sub aParams_UndPriceToleranceChange()
    If m_Grp.GroupType <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_MATRIX_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_MATRIX_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_MATRIX_FILTER) Then
        m_bKeyDown(GT_MATRIX_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_MATRIX_FILTER
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

Private Sub fgScn_DblClick()
    On Error Resume Next
    With fgScn
        m_enMenuGrid = GT_MATRIX_SCENARIO
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgScn_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_MATRIX_SCENARIO) = True
    End If
End Sub

Private Sub fgScn_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_MATRIX_SCENARIO) Then
        m_bKeyDown(GT_MATRIX_SCENARIO) = False
        With fgScn
            m_enMenuGrid = GT_MATRIX_SCENARIO
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

Private Sub fgVal_DblClick()
    On Error Resume Next
    With fgVal
        m_enMenuGrid = GT_MATRIX_VALUES
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick
    End With
End Sub

Private Sub fgVal_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_MATRIX_VALUES) = True
    End If
End Sub

Private Sub fgVal_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_MATRIX_VALUES) Then
        m_bKeyDown(GT_MATRIX_VALUES) = False
        With fgVal
            m_enMenuGrid = GT_MATRIX_VALUES
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

Private Sub fgVal_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    UpdateCurrentValue
End Sub

Private Sub frmWtdVega_OnOK()
    On Error Resume Next
    If m_Grp.ID <> 0 Then
        m_bInProc = True
        UpdateWtdVega
        If m_aFilter.Data(MFC_WTD_VEGA) <> 0 Then CalcMatrix
        m_bInProc = False
    End If
End Sub

Private Sub UpdateWtdVega()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom
    
    For Each aUnd In m_Und
        For Each aPos In aUnd.Pos
            If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
                aPos.VegaWeight = g_ExpCalendar.GetVegaWeight(aPos.ExpiryOV)
            End If
        Next
    Next
End Sub

Private Sub imgStop_Click()
    StopNow
End Sub

Private Sub fgFlt_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    With fgFlt
        Select Case Col
            Case MFC_SYMBOL To MFC_STRATEGY, MFC_EXPIRY
                .ComboList = .Cell(flexcpData, Row, Col)
        End Select
    End With
End Sub

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_MATRIX_FILTER
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
        If Not m_bInProc And Not m_bRecalc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= MFC_SYMBOL And Col <= MFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            
'            If Not Cancel Then
'                Cancel = (Col = MFC_VALUE And m_aFilter.Data(MFC_SYMBOL) = TYPE_ALL)
'            End If
            
            If Not Cancel And Col >= MFC_PNL And Col <= MFC_LAST_COLUMN Then
                Cancel = (m_nGreeks = 1 And m_aFilter.Data(Col) <> 0)
            End If
            
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = .ComboData
            Select Case Col
                Case MFC_SYMBOL, MFC_GROUPS To MFC_STRATEGY
                    If (Col = MFC_SYMBOL) Then
                            If (Not g_UnderlyingAll(nValue) Is Nothing) Then
                                If (g_UnderlyingAll(nValue).IsHead) Then
                                    m_aFilter.Data(Col) = g_UnderlyingAll(nValue).ID
                                    .TextMatrix(Row, Col) = g_UnderlyingAll(nValue).Symbol
                                Else
                                    If (Not g_UnderlyingAll(nValue).HeadComponent Is Nothing) Then
                                        m_aFilter.Data(Col) = g_UnderlyingAll(nValue).HeadComponent.ID
                                        .TextMatrix(Row, Col) = g_UnderlyingAll(nValue).HeadComponent.Symbol
                                    Else
                                        m_aFilter.Data(Col) = nValue
                                    End If
                                End If
                            Else
                                m_aFilter.Data(Col) = 0
                            End If
                    Else
                        m_aFilter.Data(Col) = nValue
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    tmrShow.Enabled = True
                
                Case MFC_TRADES
                    m_aFilter.Data(MFC_TRADES) = nValue
                    m_nOpenedExpiry = m_aFilter.Data(MFC_EXPIRY)
                    .AutoSize 0, .Cols - 1, , 100
                    tmrShow.Enabled = True
                    
                Case MFC_EXPIRY
                    m_aFilter.Data(MFC_EXPIRY) = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    CalcMatrix
                                        
                Case MFC_PNL To MFC_LAST_COLUMN
                    m_aFilter.Data(Col) = IIf(.Cell(flexcpValue, Row, Col) <> 0, 1, 0)
                    FilterUpdateGreeks
                    FormatValGrid
                    FormatValColumns False
                    CalcMatrix
            End Select
        End If
    End With
End Sub

Private Sub fgScn_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&, aScn As clsRmScenarioAtom, dValue#, dOldValue#
    
    With fgScn
        sValue = Trim$(.TextMatrix(Row, Col))
        nValue = .ComboData
        
        If m_sCurrentOriginalText <> sValue Or nValue <> m_nCurrentOriginalScnID Then
            Select Case Col
                Case MSC_NAME
                    nValue = .ComboData
                    If nValue <> 0 Then
                        If ScenarioSave(True, True) Then
                            Set aScn = g_RmScenario(CStr(nValue))
                            If Not aScn Is Nothing Then
                                aScn.CopyTo m_Scn
                                m_Scn.FixUnits m_Und.Count < 2
                                ClearValGrid True
                                SetRefreshHint True
                                ScenarioUpdate True
                            Else
                                m_Scn.ID = 0
                                If Len(sValue) > 0 Then
                                    m_Scn.ScenarioName = sValue
                                End If
                                m_Scn.Dirty = True
                                ScenarioUpdate True
                            End If
                        Else
                            ScenarioUpdate True
                        End If
                    Else
                        If Len(sValue) > 0 Then
                            m_Scn.ScenarioName = sValue
                            m_Scn.Dirty = True
                            InitScnList
                            ScenarioUpdate False
                        Else
                            ScenarioUpdate True
                        End If
                    End If
                    
                    
                    
                Case MSC_HORZ_AXIS
                    nValue = .ComboData
                    m_Scn.Axis(RMA_HORZ) = nValue
                    m_Scn.FixAxis RMA_VERT
                    m_Scn.FixUnits m_Und.Count < 2
                    ScenarioUpdate False
                    SetRefreshHint True
                    ClearValGrid True
                    
                Case MSC_HORZ_POINTS
                    nValue = .ComboData
                    m_Scn.Points(RMA_HORZ) = nValue
                    ClearValGrid True
                    ScenarioUpdate False
                    SetRefreshHint True
                    .AutoSize 0, .Cols - 1, , 100
                
                Case MSC_HORZ_STEP
                    dOldValue = m_Scn.Step(RMA_HORZ)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# Then
                        m_Scn.Step(RMA_HORZ) = dValue
                        m_Scn.FixSteps
                    End If
                    
                    If m_Scn.Step(RMA_HORZ) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_HORZ_STEP_HOUR
                    dOldValue = m_Scn.Step(RMA_HORZ)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# And dValue <= 24# Then
                        m_Scn.Hour(RMA_HORZ) = dValue
                        m_Scn.FixSteps
                    End If
                    
                    If m_Scn.Hour(RMA_HORZ) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_HORZ_STEP_MIN
                    dOldValue = m_Scn.Step(RMA_HORZ)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# And dValue <= 60# Then
                        m_Scn.Minute(RMA_HORZ) = dValue
                        m_Scn.FixSteps
                    End If
                    
                    If m_Scn.Minute(RMA_HORZ) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_HORZ_UNITS
                    nValue = .ComboData
                    m_Scn.Units(RMA_HORZ) = nValue
                    m_Scn.FixSteps
                    SetRefreshHint True
                    ClearValGrid True
                    .AutoSize 0, .Cols - 1, , 100
                
                Case MSC_VERT_AXIS
                    nValue = .ComboData
                    m_Scn.Axis(RMA_VERT) = nValue
                    m_Scn.FixAxis RMA_HORZ
                    m_Scn.FixUnits m_Und.Count < 2
                    ScenarioUpdate False
                    SetRefreshHint True
                    ClearValGrid True
                
                Case MSC_VERT_POINTS
                    nValue = .ComboData
                    m_Scn.Points(RMA_VERT) = nValue
                    ScenarioUpdate False
                    SetRefreshHint True
                    ClearValGrid True
                    .AutoSize 0, .Cols - 1, , 100
                
                Case MSC_VERT_STEP
                    dOldValue = m_Scn.Step(RMA_VERT)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# Then
                        m_Scn.Step(RMA_VERT) = dValue
                        m_Scn.FixSteps
                    End If
                    .TextMatrix(1, MSC_VERT_STEP) = m_Scn.Step(RMA_VERT)
                
                    If m_Scn.Step(RMA_VERT) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_VERT_STEP_HOUR
                    dOldValue = m_Scn.Hour(RMA_VERT)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# And dValue <= 24# Then
                        m_Scn.Hour(RMA_VERT) = dValue
                        m_Scn.FixSteps
                    End If
                    .TextMatrix(1, MSC_VERT_STEP_HOUR) = m_Scn.Hour(RMA_VERT)
                
                    If m_Scn.Hour(RMA_VERT) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_VERT_STEP_MIN
                    dOldValue = m_Scn.Minute(RMA_VERT)
                    dValue = ReadDbl(sValue)
                    If dValue >= 0# And dValue <= 60# Then
                        m_Scn.Minute(RMA_VERT) = dValue
                        m_Scn.FixSteps
                    End If
                    .TextMatrix(1, MSC_VERT_STEP_MIN) = m_Scn.Minute(RMA_VERT)
                
                    If m_Scn.Minute(RMA_VERT) <> dOldValue Then
                        SetRefreshHint True
                        ClearValGrid True
                    End If
                    .AutoSize 0, .Cols - 1, , 100
                
                Case MSC_VERT_UNITS
                    nValue = .ComboData
                    m_Scn.Units(RMA_VERT) = nValue
                    m_Scn.FixSteps
                    SetRefreshHint True
                    ClearValGrid True
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_VOLA_SHIFT
                    nValue = .ComboData
                    m_Scn.VolaShiftType = nValue
                    SetRefreshHint True
                    ClearValGrid True
                    .AutoSize 0, .Cols - 1, , 100
                    
                Case MSC_VALUE_CHANGE
                    m_Scn.ShowChange = IIf(.Cell(flexcpValue, Row, Col) <> 0, True, False)
                    SetRefreshHint True
                    ClearValGrid True
                    .AutoSize 0, .Cols - 1, , 100
                
                Case MSC_SPOT_SHIFT
                    m_Scn.SpotShiftType = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    CalcMatrix
                    
                Case MSC_CORR_INDEX
                    m_Scn.CorrIndex = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    IndexLoad

                    If m_Und.Count > 0 Then
                        lblStatus.Visible = False
                        imgStop.Visible = True
                        imgStopDis.Visible = False
                        pbProgress.Visible = True
                        pbProgress.Min = 0
                        pbProgress.Value = 0
                        lblProcess.Caption = "Last quotes request..."
                        lblProcess.Visible = True
                        lblProcess.Refresh
    
                        RequestLastQuotes True
                    End If
                    
                Case MSC_CALC_MODEL
                    g_Params.MatrixCalcModel = nValue
                    m_Scn.CalcModel = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    CalcMatrix
                
                Case MSC_SPOT_SPECIFIC
                    m_Scn.SpotSpecificShiftType = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    CalcMatrix
                
                Case MSC_VOLA_SPECIFIC
                    m_Scn.VolaSpecificShiftType = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    CalcMatrix
                    
            End Select
        End If
    End With
End Sub

Private Sub fgScn_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    With fgScn
        Select Case Col
            Case MSC_CALC_MODEL To MSC_SPOT_SPECIFIC, MSC_NAME
                .ComboList = .Cell(flexcpData, Row, Col)
                .EditMaxLength = 30
                
            Case Else
                .ComboList = ""
                
        End Select
    End With
End Sub

Private Sub fgScn_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgScn
            m_enMenuGrid = GT_MATRIX_SCENARIO
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgScn_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgScn
        If Not m_bInProc And Not m_bRecalc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= MSC_NAME And Col <= MSC_LAST_COLUMN Then
            Cancel = Not m_gdScn.Col(Col).CanEdit
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
                m_nCurrentOriginalScnID = .ComboData
            End If
        End If
    End With
End Sub

Private Sub fgVal_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = (Col < 2)
End Sub

Private Sub fgVal_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgVal
            m_enMenuGrid = GT_MATRIX_VALUES
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
        Case GT_MATRIX_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_MATRIX_FILTER).LockRedraw
            FormatFltColumns
            m_GridLock(GT_MATRIX_FILTER).UnlockRedraw
            
        Case GT_MATRIX_SCENARIO
            gdGrid.CopyTo m_gdScn
            
            m_GridLock(GT_MATRIX_SCENARIO).LockRedraw
            FormatScnColumns
            m_GridLock(GT_MATRIX_SCENARIO).UnlockRedraw
    
        Case GT_MATRIX_VALUES
            gdGrid.CopyTo m_gdVal
            
            FormatValGrid
            m_GridLock(GT_MATRIX_VALUES).LockRedraw
            FormatValColumns True
            m_GridLock(GT_MATRIX_VALUES).UnlockRedraw
    
    End Select
    
    UserControl_Resize
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_MATRIX_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_MATRIX_SCENARIO
            g_ClipMgr.CopyGridToClipboard fgScn

        Case GT_MATRIX_VALUES
            g_ClipMgr.CopyGridToClipboard fgVal

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxDisplayInGraph_Click()
    On Error Resume Next
    Dim i As Long
    Dim nParamRowID As Long, nParamID
    Dim nParamsCount As Long
    
    For i = MFC_PNL To (MFC_COLUMN_COUNT)
        If m_aFilter.Data(i) <> 0 Then nParamsCount = nParamsCount + 1
    Next
    
    nParamRowID = (m_nMenuGridRow Mod nParamsCount)
    
    nParamID = MFC_PNL - 1
    While nParamRowID > 0
        nParamID = nParamID + 1
        If m_aFilter.Data(i) <> 0 Then nParamRowID = nParamRowID - 1
    Wend
    
    Dim frmGraph As frmRiskMatrixGraph
    Set frmGraph = New frmRiskMatrixGraph
    
    Dim Axis(RMA_HORZ To RMA_VERT) As RmAxisTypeEnum
    
    Axis(RMA_HORZ) = m_Scn.Axis(RMA_HORZ)
    Axis(RMA_VERT) = m_Scn.Axis(RMA_VERT)
    
    frmGraph.ShowRiskMatrixGraph m_Res, Axis, m_aFilter, nParamID, Me
    
    Erase Axis
    Set frmGraph = Nothing
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_MATRIX_FILTER
            CustomizeFltGridLayout

        Case GT_MATRIX_SCENARIO
            CustomizeScnGridLayout
        
        Case GT_MATRIX_VALUES
            CustomizeValGridLayout
        
    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, ByRef dPrice#)
    On Error Resume Next
    If m_aFilter.Data(MFC_SYMBOL) > 0 Then
        Set aUnd = g_Underlying(m_aFilter.Data(MFC_SYMBOL))
    End If
            
    If bIsStock And Not aUnd Is Nothing Then
        dPrice = m_Und(aUnd.ID).price.Ask
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
    gCmn.ErrorMsgBox m_frmOwner, "Risk Matrix View: Fail to create new order."
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

Private Sub mnuCtxScnLoadParams_Click()
On Error GoTo exception_handler
    LoadGroupSpecificRatio
Exit Sub
exception_handler:
MsgBox "Exception occured on Loading Scenrio Params (" & Err.Description & " : " & CStr(Err.Number) & ")"
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
'    gCmn.ErrorMsgBox m_frmOwner, "Risk Matrix View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_MATRIX_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Risk Matrix Filter", "Risk Matrix Filter", fgFlt
'
'        Case GT_MATRIX_SCENARIO
'            frmPrintPreview.Execute m_frmOwner, m_Scn.ScenarioName & " Risk Matrix Screnario", "Risk Matrix Screnario", fgScn
'
'        Case GT_MATRIX_VALUES
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Risk Matrix Values", "Risk Matrix Values", fgVal
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub mnuCtxScnDelete_Click()
    On Error Resume Next
    ScenarioDelete
End Sub

Private Sub mnuCtxScnNew_Click()
    On Error Resume Next
    ScenarioNew
End Sub

Private Sub mnuCtxScnSave_Click()
    On Error Resume Next
    ScenarioSave True, True
End Sub

Private Sub mnuCtxWtdVega_Click()
    On Error Resume Next
    ShowWeightedVegaSettings
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False

    If m_bInProc Or m_bDataLoad Then Exit Sub

    DoEvents
    
    If m_Grp.ID <> 0 And Not PriceProvider Is Nothing Then
        If m_bLastQuoteReqNow Then PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        m_bLastQuoteReqNow = False
        m_QuoteRequests.Clear
    End If
    
    ClearViewAndData
    lblStatus.Caption = ""

    
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
        m_Scn.FixUnits m_Und.Count < 2
        ScenarioUpdate False
        
        IndexLoad
        lblProcess.Caption = "Filter applying..."
        lblProcess.Refresh
        
        m_aFilter.Data(MFC_EXPIRY) = m_nOpenedExpiry
        FilterUpdateExpiry False
    
        lblProcess.Caption = "Last quotes request..."
        lblProcess.Refresh
        
        RequestLastQuotes False
    Else
        If m_Grp.ID <> -1 Then
            ClearViewAndData
            lblStatus.Caption = ""
            FilterUpdateAll
            AdjustCaption
            AdjustState
        Else
            lblStatus.Caption = "Press F5 to Refresh"
        End If
        
        pbProgress.Visible = False
        lblProcess.Visible = False
        lblStatus.Visible = True
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If
    
    m_nOpenedExpiry = False
    
    Screen.MousePointer = vbDefault
End Sub

Private Function PositionsLoad() As Boolean
    On Error Resume Next
    PositionsLoad = False
    
    If m_bInProc Then Exit Function
        
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, aPos As EtsMmRisksLib.MmRvPosAtom, aExp As EtsGeneralLib.EtsMmEntityAtom
    
    m_Grp.GroupType = TYPE_ALL

    m_bInProc = True
    m_bDataLoad = True
    AdjustState

    Err.Clear
    m_Grp.Name = ""
    If m_aFilter.Data(MFC_SYMBOL) > 0 Then
        m_Grp.Name = g_Underlying(m_aFilter.Data(MFC_SYMBOL)).Symbol
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_aFilter.Data(MFC_GROUPS) > 0 Then
        If Len(m_Grp.Name) > 0 Then m_Grp.Name = m_Grp.Name & "; "
        m_Grp.Name = m_Grp.Name & g_UnderlyingGroup(m_aFilter.Data(MFC_GROUPS)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_aFilter.Data(MFC_TRADER) > 0 Then
        If Len(m_Grp.Name) > 0 Then m_Grp.Name = m_Grp.Name & "; "
        m_Grp.Name = m_Grp.Name & g_Trader(m_aFilter.Data(MFC_TRADER)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_aFilter.Data(MFC_TRADER_GROUP) > 0 Then
        If Len(m_Grp.Name) > 0 Then m_Grp.Name = m_Grp.Name & "; "
        m_Grp.Name = m_Grp.Name & g_TraderGroup(m_aFilter.Data(MFC_TRADER_GROUP)).Name
        If Err.Number <> 0 Then m_bDataLoad = False
    End If
    If m_aFilter.Data(MFC_STRATEGY) <> 0 Then
        If Len(m_Grp.Name) > 0 Then m_Grp.Name = m_Grp.Name & "; "
        If m_aFilter.Data(MFC_STRATEGY) > 0 Then
            m_Grp.Name = m_Grp.Name & g_Strategy(m_aFilter.Data(MFC_STRATEGY)).Name
        Else
            m_Grp.Name = m_Grp.Name & NO_STRATEGY_NAME
        End If
        
        If Err.Number <> 0 Then m_bDataLoad = False
        
    End If
    If Len(m_Grp.Name) = 0 Then
        If m_bFirstTime Then
            m_Grp.ID = -1
            m_bInProc = False
            
            AdjustState
            m_bDataLoad = False
        End If
        m_Grp.Name = "<All>"
    End If
    m_bFirstTime = False
    
    
    If Not m_bDataLoad Then GoTo Ex
    On Error GoTo EH
    Dim mmTradesColl As MmTradeInfoColl
    
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100
    pbProgress.Visible = True
  
    Set mmTradesColl = g_TradeChannel.Trades.FilterTrades(m_aFilter, g_UnderlyingGroup, False)
    m_Grp.ID = mmTradesColl.Count
    
    Set m_View.EtsMain = g_Main
    Set m_View.VolaSource = VolaSource
    m_View.PositionsLoad mmTradesColl
       
    UnderlyingsAdjustRates
    PositionsLoad = m_bDataLoad
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

Private Sub InitResults()
    On Error Resume Next
    Dim nLastX&, nLastY&, nX&, nY&, dValue#, dShift#
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim dtDate As Date
    Dim dtNow As Date: dtNow = GetNewYorkTime
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    nLastX = m_Scn.Points(RMA_HORZ) - 1
    nLastY = m_Scn.Points(RMA_VERT) - 1
    Debug.Assert nLastX >= 0 And nLastY >= 0
    
    Erase m_Res
    ReDim m_Res(0 To nLastX, 0 To nLastY)
    
    If m_Scn.Axis(RMA_HORZ) <> RMAT_TIME Then
        m_BasePoint(RMA_HORZ) = nLastX \ 2 + 1
    Else
        m_BasePoint(RMA_HORZ) = 1
    End If
    
    If m_Scn.Axis(RMA_VERT) <> RMAT_TIME Then
        m_BasePoint(RMA_VERT) = nLastY \ 2 + 1
    Else
        m_BasePoint(RMA_VERT) = 1
    End If
    
    
    For nX = 0 To nLastX
        Select Case m_Scn.Axis(RMA_HORZ)
            Case RMAT_SPOT
                dShift = (nX - nLastX \ 2) * m_Scn.Step(RMA_HORZ)
                If m_Scn.Units(RMA_HORZ) = RMUT_PERC Then dShift = dShift / 100#
                            
            Case RMAT_VOLA
                dShift = (nX - nLastX \ 2) * m_Scn.Step(RMA_HORZ) / 100#
                
            Case RMAT_TIME
                dtDate = DateAdd("d", nX * m_Scn.Step(RMA_HORZ), dtNow)
                dtDate = DateAdd("h", nX * m_Scn.Hour(RMA_HORZ), dtDate)
                dtDate = DateAdd("n", nX * m_Scn.Minute(RMA_HORZ), dtDate)
            
                dShift = dtDate - dtNow
        End Select
        
        For nY = 0 To nLastY
            m_Res(nX, nY).ShiftX = dShift
            
            m_Res(nX, nY).PnL = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Delta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).NetDelta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Gamma = BAD_DOUBLE_VALUE
            m_Res(nX, nY).GammaPerc = BAD_DOUBLE_VALUE
            m_Res(nX, nY).NetGamma = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Theta = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Vega = BAD_DOUBLE_VALUE
            m_Res(nX, nY).Rho = BAD_DOUBLE_VALUE
            m_Res(nX, nY).WtdVega = BAD_DOUBLE_VALUE
        
            m_Res(nX, nY).BadPnL = False
            m_Res(nX, nY).BadDelta = False
            m_Res(nX, nY).BadNetDelta = False
            m_Res(nX, nY).BadGamma = False
            m_Res(nX, nY).BadGammaPerc = False
            m_Res(nX, nY).BadNetGamma = False
            m_Res(nX, nY).BadTheta = False
            m_Res(nX, nY).BadVega = False
            m_Res(nX, nY).BadRho = False
            m_Res(nX, nY).BadWtdVega = False
        Next
    Next
    
    For nY = 0 To nLastY
        Select Case m_Scn.Axis(RMA_VERT)
            Case RMAT_SPOT
                dShift = (nY - nLastY \ 2) * m_Scn.Step(RMA_VERT)
                If m_Scn.Units(RMA_VERT) = RMUT_PERC Then dShift = dShift / 100#
                
            Case RMAT_VOLA
                dShift = (nY - nLastY \ 2) * m_Scn.Step(RMA_VERT) / 100#
                
            Case RMAT_TIME
                dtDate = DateAdd("d", nY * m_Scn.Step(RMA_VERT), dtNow)
                dtDate = DateAdd("h", nY * m_Scn.Hour(RMA_VERT), dtDate)
                dtDate = DateAdd("n", nY * m_Scn.Minute(RMA_VERT), dtDate)
            
                dShift = dtDate - dtNow
        End Select
        
        For nX = 0 To nLastX
            m_Res(nX, nY).ShiftY = dShift
        Next
    Next
    
    If m_Und.Count = 1 Then
        If m_Scn.Units(RMA_HORZ) = RMUT_ABS And m_Scn.Axis(RMA_HORZ) = RMAT_SPOT Then
            Debug.Assert (Not m_Und(1).UndPriceProfile Is Nothing)
            If (m_Und(1).price.IsUseManualActive) Then
                dValue = m_Und(1).price.Active
            Else
                dValue = m_Und(1).UndPriceProfile.GetUndPriceMid(m_Und(1).price.Bid, m_Und(1).price.Ask, m_Und(1).price.Last, dToleranceValue, enRoundingRule)
            End If
            
            If dValue > 0# Then
                dShift = 0
                
                For nX = 0 To nLastX
                    If dValue + m_Res(nX, 0).ShiftX < 0 Then
                        dShift = dShift + m_Scn.Step(RMA_HORZ)
                        m_BasePoint(RMA_HORZ) = m_BasePoint(RMA_HORZ) - 1
                    Else
                        Exit For
                    End If
                Next
                
                If dShift > 0 Then
                    For nX = 0 To nLastX
                        For nY = 0 To nLastY
                            m_Res(nX, nY).ShiftX = m_Res(nX, nY).ShiftX + dShift
                        Next
                    Next
                End If
            End If
        End If
    
        If m_Scn.Units(RMA_VERT) = RMUT_ABS And m_Scn.Axis(RMA_VERT) = RMAT_SPOT Then
            Debug.Assert (Not m_Und(1).UndPriceProfile Is Nothing)
            
            If (m_Und(1).price.IsUseManualActive) Then
                dValue = m_Und(1).price.Active
            Else
                dValue = m_Und(1).UndPriceProfile.GetUndPriceMid(m_Und(1).price.Bid, m_Und(1).price.Ask, m_Und(1).price.Last, dToleranceValue, enRoundingRule)
            End If
            
            If dValue > 0# Then
                dShift = 0
                
                For nY = 0 To nLastY
                    If dValue + m_Res(0, nY).ShiftY < 0 Then
                        dShift = dShift + m_Scn.Step(RMA_VERT)
                        m_BasePoint(RMA_VERT) = m_BasePoint(RMA_VERT) - 1
                    Else
                        Exit For
                    End If
                Next
                
                If dShift > 0 Then
                    For nY = 0 To nLastY
                        For nX = 0 To nLastX
                            m_Res(nX, nY).ShiftY = m_Res(nX, nY).ShiftY + dShift
                        Next
                    Next
                End If
            End If
        End If
    End If
End Sub

Private Sub CalcMatrix()

On Error GoTo error_exception

    Dim aUnd As EtsMmRisksLib.MmRvUndAtom, bCorrelatedShift As Boolean, nModel As EtsGeneralLib.EtsCalcModelTypeEnum
    Dim nLastX&, nLastY&, nX&, nY&, nUndCount&, i&, nRow&, nMask&, nBadForeColor&, nForeColor&
    Dim aPos As EtsMmRisksLib.MmRvPosAtom
    
    Dim nX0 As Long
    Dim nY0 As Long
    Dim dValue As Double
    Dim bStatus As Boolean
    
    nUndCount = m_Und.Count
    If m_bInProc Or m_bRecalc Then Exit Sub
    
    ClearValGrid False
    SetRefreshHint False
    
    If nUndCount <= 0 Then Exit Sub
    
    nLastX = UBound(m_Res, 1)
    nLastY = UBound(m_Res, 2)
    Debug.Assert nLastX >= 0 And nLastY >= 0
    
    If nLastX < 0 Or nLastY < 0 Then Exit Sub
    
    m_bInProc = True
    m_bRecalc = True
    AdjustState
    
    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 1
    pbProgress.Visible = True
    lblProcess.Caption = "Calculating Matrix..."
    lblProcess.Visible = True
    lblProcess.Refresh
    
    pbProgress.Max = nUndCount * m_Scn.Points(RMA_HORZ) * m_Scn.Points(RMA_VERT)
    
    nMask = GM_NONE
    If m_aFilter.Data(MFC_PNL) <> 0 Then nMask = nMask Or GM_THEOPRICE
    If m_aFilter.Data(MFC_DELTA) <> 0 Or m_aFilter.Data(MFC_NET_DELTA) <> 0 Then nMask = nMask Or GM_DELTA
    If m_aFilter.Data(MFC_GAMMA) <> 0 Or m_aFilter.Data(MFC_NET_GAMMA) <> 0 Or m_aFilter.Data(MFC_GAMMA_PERC) <> 0 Then nMask = nMask Or GM_GAMMA
    If m_aFilter.Data(MFC_THETA) <> 0 Then nMask = nMask Or GM_THETA
    If m_aFilter.Data(MFC_VEGA) <> 0 Or m_aFilter.Data(MFC_WTD_VEGA) <> 0 Then nMask = nMask Or GM_VEGA
    If m_aFilter.Data(MFC_RHO) <> 0 Then nMask = nMask Or GM_RHO
    
    bCorrelatedShift = (m_Scn.SpotShiftType = RMSS_CORRELATED)
    If bCorrelatedShift Then
        For Each aUnd In m_Und
            If Not aUnd.HasSynthetic Then
                If (aUnd.IsHead Or aUnd.HeadComponent Is Nothing) And aUnd.ID <> 0 Then
                    If (IsBadDouble(aUnd.Beta) Or aUnd.Beta = 0#) Then
                        bCorrelatedShift = False
                        Set aUnd = Nothing
                        Exit For
                    End If
                Else
                    If (Not aUnd.HeadComponent Is Nothing And aUnd.ID <> 0) Then
                        If (IsBadDouble(aUnd.HeadComponent.Beta) Or aUnd.HeadComponent.Beta = 0#) Then
                            bCorrelatedShift = False
                            Set aUnd = Nothing
                            Exit For
                        End If
                    End If
                End If
            Else
                For Each aPos In aUnd.Pos
                    If aPos.IsSynthetic Then
                            Dim sRoot As SynthRootAtom
                            Set sRoot = aUnd.SynthRoots(aPos.OptionRootID)
                            If Not sRoot Is Nothing Then
                                Dim sRootComp As SynthRootCompAtom
                                 For Each sRootComp In sRoot.SynthRootComponents
                                    If Not IsBadDouble(sRootComp.Weight) And sRootComp.Weight > 0# Then
                                        If Not g_Index(m_Idx.ID).HaveComponentBetas Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                        Dim IdxComp As IndexCompAtom
                                        Set IdxComp = g_Index(m_Idx.ID).Components(sRootComp.UndID)
                                        If IdxComp Is Nothing Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                        If IsBadDouble(IdxComp.Beta) Or IdxComp.Beta = 0# Then
                                            bCorrelatedShift = False
                                            GoTo ExitFor
                                        End If
                                    End If
                                 Next
                            Else
                                bCorrelatedShift = False
                                GoTo ExitFor
                            End If
                    Else
                        If IsBadDouble(aUnd.Beta) Or aUnd.Beta = 0# Then
                            bCorrelatedShift = False
                            GoTo ExitFor
                        End If
                    End If
                Next
            End If
        Next
ExitFor:
    End If
    
    nModel = g_Params.MatrixCalcModel
    
    For Each aUnd In m_Und
        CalcPosition aUnd, nLastX, nLastY, nMask, bCorrelatedShift, m_Scn.CalcModel
        DoEvents
        If Not m_bRecalc Then Exit For
    Next
        
    nX0 = m_BasePoint(RMA_HORZ) - 1
    nY0 = m_BasePoint(RMA_VERT) - 1
    
    With fgVal
        m_GridLock(GT_MATRIX_VALUES).LockRedraw
        
        If m_bRecalc Then
            For nX = 0 To nLastX
                
                For nY = 0 To nLastY
                    If nX <> m_BasePoint(RMA_HORZ) - 1 Or nY <> m_BasePoint(RMA_HORZ) - 1 Then
                        nForeColor = m_gdVal.Col(MVC_VALUE).ForeColor
                        nBadForeColor = m_gdVal.Col(MVC_VALUE).ForeColorAlt1
                    Else
                        nForeColor = m_gdVal.Col(MVC_BASE).ForeColor
                        nBadForeColor = m_gdVal.Col(MVC_BASE).ForeColorAlt1
                    End If
                
                    nRow = nY * m_nGreeks + 1
                    For i = 0 To MFC_LAST_COLUMN - MFC_PNL
                        If m_aFilter.Data(MFC_PNL + i) <> 0 Then
                            Select Case MFC_PNL + i
                                Case MFC_PNL
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).PnL, m_Res(nX, nY).PnL, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadPnL, m_Res(nX, nY).BadPnL, m_Scn.ShowChange)
                                    
                                Case MFC_DELTA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).Delta, m_Res(nX, nY).Delta, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadDelta, m_Res(nX, nY).BadDelta, m_Scn.ShowChange)
                                    
                                Case MFC_NET_DELTA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).NetDelta, m_Res(nX, nY).NetDelta, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadNetDelta, m_Res(nX, nY).BadNetDelta, m_Scn.ShowChange)
                                    
                                Case MFC_GAMMA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).Gamma, m_Res(nX, nY).Gamma, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadGamma, m_Res(nX, nY).BadGamma, m_Scn.ShowChange)

                                Case MFC_GAMMA_PERC
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).GammaPerc, m_Res(nX, nY).GammaPerc, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadGammaPerc, m_Res(nX, nY).BadGammaPerc, m_Scn.ShowChange)

                                Case MFC_NET_GAMMA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).NetGamma, m_Res(nX, nY).NetGamma, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadNetGamma, m_Res(nX, nY).BadNetGamma, m_Scn.ShowChange)

                                Case MFC_THETA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).Theta, m_Res(nX, nY).Theta, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadTheta, m_Res(nX, nY).BadTheta, m_Scn.ShowChange)

                                Case MFC_VEGA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).Vega, m_Res(nX, nY).Vega, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadVega, m_Res(nX, nY).BadVega, m_Scn.ShowChange)

                                Case MFC_RHO
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).Rho, m_Res(nX, nY).Rho, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadRho, m_Res(nX, nY).BadRho, m_Scn.ShowChange)

                                Case MFC_WTD_VEGA
                                    dValue = GetMatrixValue(m_Res(nX0, nY0).WtdVega, m_Res(nX, nY).WtdVega, m_Scn.ShowChange)
                                    bStatus = GetMatrixValueStatus(m_Res(nX0, nY0).BadWtdVega, m_Res(nX, nY).BadWtdVega, m_Scn.ShowChange)

                            End Select
                            
                            'Update View
                            .TextMatrix(nRow, nX + 2) = IIf(dValue > BAD_DOUBLE_VALUE, dValue, STR_NA)
                            .Cell(flexcpForeColor, nRow, nX + 2) = IIf(bStatus, nBadForeColor, nForeColor)
                            
                            nRow = nRow + 1
                        End If
                    Next
                Next
            Next
        Else
            .Cell(flexcpText, 1, 2, .Rows - 1, .Cols - 1) = ""
        End If
        
        .AutoSize 2, .Cols - 1, True
        
        m_GridLock(GT_MATRIX_VALUES).UnlockRedraw
    End With
    
    If Not bCorrelatedShift And (m_Scn.SpotShiftType = RMSS_CORRELATED) Then
        lblStatus.Caption = "Some stocks do not correlate with current index. Parallel shift used."
    Else
        lblStatus.Caption = ""
    End If
    
    UpdateCurrentValue
    
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    m_bInProc = False
    m_bRecalc = False
    AdjustState
    
    ' for risk matrix graph
    Dim Axis(RMA_HORZ To RMA_VERT) As RmAxisTypeEnum
    
    Axis(RMA_HORZ) = m_Scn.Axis(RMA_HORZ)
    Axis(RMA_VERT) = m_Scn.Axis(RMA_VERT)
    
    RaiseEvent OnScreenRefresh(m_Res, Axis, m_aFilter)
    Erase Axis
    
    Exit Sub
error_exception:
    MsgBox "Exception on CalcMatrix: " & Err.Description
End Sub

Private Function GetMatrixValue(ByVal vC As Double, ByVal vB As Double, ByVal bDelta As Boolean) As Double
On Error Resume Next

    Dim dRetValue As Double
    dRetValue = BAD_DOUBLE_VALUE
    
    If (bDelta = True) Then
        If (vC > BAD_DOUBLE_VALUE And vB > BAD_DOUBLE_VALUE) Then
            dRetValue = vB - vC
        End If
    Else
        If (vB > BAD_DOUBLE_VALUE) Then
            dRetValue = vB
        End If
    End If
    
    GetMatrixValue = dRetValue
End Function

Private Function GetMatrixValueStatus(ByVal vC As Boolean, ByVal vB As Boolean, ByVal bDelta As Boolean) As Boolean
On Error Resume Next

    Dim dRetValue As Boolean
    dRetValue = False
    
    If (bDelta = True) Then
        If (vC = True Or vB = True) Then
            dRetValue = True
        End If
    Else
        If (vB = True) Then
            dRetValue = True
        End If
    End If
    
    GetMatrixValueStatus = dRetValue
End Function

Private Sub UnderlyingsAdjustRates()
    On Error Resume Next
    Dim aUnd As EtsMmRisksLib.MmRvUndAtom
    
    For Each aUnd In m_Und
        UnderlyingAdjustRates aUnd
    Next
End Sub

Private Sub UnderlyingAdjustRates(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error Resume Next
    Dim aPos As EtsMmRisksLib.MmRvPosAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    If aUnd Is Nothing Then Exit Sub

    Dim dtToday As Date: dtToday = GetNewYorkTime
    
    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    Else
        bUseMidRates = True
    End If

    aUnd.UseMidRates = bUseMidRates
    Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
    
    For Each aPos In aUnd.Pos
        If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
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
        End If
    Next
End Sub

Private Sub ShiftSpot(ByVal dBeta#, ByVal enUnits As RmUnitTypeEnum, _
                    ByVal dShift#, ByVal bCorrelatedShift As Boolean, _
                    ByRef dUndSpot#, ByRef dUndBid#, ByRef dUndAsk#, _
                    ByVal dUndDriverPrice#, ByVal dWeight#, ByVal dGroupRatio#)
    On Error Resume Next
    Dim dBetaFactor#
    Dim dShiftDelta#
    
    
    If (m_Scn.SpotSpecificShiftType = RMSSS_GROUP_SPECIFIC) Then
        dShift = dShift * dGroupRatio
    End If
    
    If enUnits = RMUT_PERC Then
        If Not bCorrelatedShift Or dBeta = 0# Or dBeta = 1# Or IsBadDouble(dBeta) Then
            dShiftDelta = dUndDriverPrice * dWeight * dShift
        Else
            dBetaFactor = (1 + dShift)
            
            If dBetaFactor > 0# Then
                dShiftDelta = dUndDriverPrice * dWeight * (Exp(Log(dBetaFactor) * dBeta) - 1#)
            Else
                dShiftDelta = 0#
            End If
        End If
    Else
        If (Not bCorrelatedShift) Then
            dShiftDelta = dShift
        Else
            Dim enReplaceStatus As EtsReplacePriceStatusEnum
            Dim bFutPriceReplaced As Boolean
            Dim dIdxPrice As Double
            
            dIdxPrice = 0#
            'Try to get IdxPrice
            If (Not m_Idx Is Nothing) Then
                dIdxPrice = m_Idx.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
            End If
            
            'Try to compute shifted spot prices
            If (dIdxPrice > 0#) Then
                dBetaFactor = 1# + (dShift / dIdxPrice)
                If dBetaFactor > 0# Then
                    dShiftDelta = (dUndDriverPrice * dWeight) * (Exp(Log(dBetaFactor) * dBeta) - 1#)
                Else
                    dShiftDelta = 0#
                End If
            Else
                'If not idxPrice use Abs shift
                dShiftDelta = dShift
            End If
        End If
    End If
       
    
    If (dUndSpot + dShiftDelta > 0#) Then
        dUndSpot = dUndSpot + dShiftDelta
    Else
        dUndSpot = 0#
    End If
    
    If (dUndBid + dShiftDelta > 0#) Then
        dUndBid = dUndBid + dShiftDelta
    Else
        dUndBid = 0#
    End If
    
    If (dUndAsk + dShiftDelta > 0#) Then
        dUndAsk = dUndAsk + dShiftDelta
    Else
        dUndAsk = 0#
    End If
        
End Sub

Private Sub ShiftSyntSpot(ByRef aSynthAtom As SynthRootAtom, _
                          ByVal enUnits As RmUnitTypeEnum, _
                          ByVal dShift#, _
                          ByVal bCorrelatedShift As Boolean, _
                          ByRef dUndSpot#, _
                          ByRef dUndBid#, _
                          ByRef dUndAsk#, _
                          ByVal dDriverPrice#, _
                          ByVal dMainBetta#, _
                          ByVal dGroupCompWeight#, _
                          ByVal dGroupRatio#)
                          
    On Error Resume Next
    
    Dim dBetaFactor#
    Dim bBadAskValue As Boolean: bBadAskValue = False
    Dim bBadBidValue As Boolean: bBadBidValue = False
    Dim bBadSpotValue As Boolean: bBadSpotValue = False
    
    dUndSpot = 0#
    dUndBid = 0#
    dUndAsk = 0#
    
   
    Dim sRootComp As SynthRootCompAtom
    For Each sRootComp In aSynthAtom.SynthRootComponents
        Dim dCompUndSpot#, dUndCompBid#, dUndCompAsk#
        
        Dim aUnd As MmRvUndAtom
        Set aUnd = m_Und(sRootComp.UndID)
        
        If (Not aUnd.price.IsUseManualActive And Not aUnd.PriceByHead) Then
            dCompUndSpot = aUnd.price.Last
            dUndCompBid = aUnd.price.Bid
            dUndCompAsk = aUnd.price.Ask
        Else
            dCompUndSpot = aUnd.price.Active: dUndCompAsk = aUnd.price.Active: dUndCompBid = aUnd.price.Active
        End If
        
        If (aUnd.HeadComponent Is Nothing) Then
            dDriverPrice = aUnd.UndPriceProfile.GetUndPriceMid(dUndCompBid, dUndCompAsk, dCompUndSpot, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
        End If
            
        ShiftSpot dMainBetta, enUnits, dShift, _
                    bCorrelatedShift, dCompUndSpot, dUndCompBid, _
                    dUndCompAsk, dDriverPrice, dGroupCompWeight, dGroupRatio
        
        If Not bBadSpotValue And dCompUndSpot > 0# Then
            dUndSpot = dUndSpot + dCompUndSpot * sRootComp.Weight
        Else
            dUndSpot = BAD_DOUBLE_VALUE
            bBadSpotValue = True
        End If
        
        If Not bBadBidValue And dUndCompBid > 0# Then
            dUndBid = dUndBid + dUndCompBid * sRootComp.Weight
        Else
            dUndBid = BAD_DOUBLE_VALUE
            bBadBidValue = True
        End If
        
        If Not bBadAskValue And dUndCompAsk > 0# Then
            dUndAsk = dUndAsk + dUndCompAsk * sRootComp.Weight
        Else
            dUndAsk = BAD_DOUBLE_VALUE
            bBadAskValue = True
        End If
    Next
    
    If Not bBadSpotValue Then dUndSpot = dUndSpot + aSynthAtom.CashValue
    If Not bBadBidValue Then dUndBid = dUndBid + aSynthAtom.CashValue
    If Not bBadAskValue Then dUndAsk = dUndAsk + aSynthAtom.CashValue
    
    dUndSpot = aUnd.UndPriceProfile.GetUndPriceMid(dUndBid, dUndAsk, dUndSpot, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    
End Sub


Private Sub ShiftVola(ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                        ByVal dVegaWeight#, ByRef dVola#, ByVal dGroupRatio#)
On Error Resume Next
    Dim dShift As Double
    Dim enShiftUnit As RmUnitTypeEnum
    
    If (m_Scn.Axis(RMA_HORZ) = RMAT_VOLA) Then
        dShift = aRes.ShiftX
        enShiftUnit = m_Scn.Units(RMA_HORZ)
    ElseIf (m_Scn.Axis(RMA_VERT) = RMAT_VOLA) Then
        dShift = aRes.ShiftY
        enShiftUnit = m_Scn.Units(RMA_VERT)
    End If
    
    If (m_Scn.VolaShiftType = RMVS_WEIGHTED And dVegaWeight > 0#) Then
        dShift = dShift * dVegaWeight
    End If
    
    If (m_Scn.VolaSpecificShiftType = RMVSS_GROUP_SPECIFIC) Then
        dShift = dShift * dGroupRatio
    End If
    
    If (enShiftUnit = RMUT_ABS) Then
        dVola = dVola + dShift
    ElseIf (enShiftUnit = RMUT_PERC) Then
        dVola = dVola * (1# + dShift)
    End If
    
    If dVola <= 0# Then dVola = 0.001
    
End Sub

Private Sub SetResultsBadStatus(ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType)
    On Error Resume Next
    aRes.BadPnL = True
    aRes.BadDelta = True
    aRes.BadNetDelta = True
    aRes.BadGamma = True
    aRes.BadGammaPerc = True
    aRes.BadNetGamma = True
    aRes.BadTheta = True
    aRes.BadVega = True
    aRes.BadRho = True
    aRes.BadWtdVega = True
End Sub

Private Function CalcTheoPnLCommonExercItmLTD(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dSpotPriceBid#, ByVal dSpotPriceAsk#) As Double
    On Error Resume Next
    Dim bBadPnl As Boolean, dPnlTheo#
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE
    
    If dSpotPriceBid > 0# And aPos.QtyLTDBuy > BAD_LONG_VALUE _
        Or dSpotPriceAsk > 0# And aPos.QtyLTDSell > BAD_LONG_VALUE Then
        
        If aPos.OptType = enOtCall Then
            If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyLTDBuy * (dSpotPriceBid - aPos.Strike) - aPos.PosLTDBuy
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyLTDSell * (dSpotPriceAsk - aPos.Strike) - aPos.PosLTDSell
                Else
                    bBadPnl = True
                End If
            End If
        
        Else
            If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyLTDBuy * (aPos.Strike - dSpotPriceBid) - aPos.PosLTDBuy
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyLTDSell * (aPos.Strike - dSpotPriceAsk) - aPos.PosLTDSell
                Else
                    bBadPnl = True
                End If
            End If
            
        End If
    End If
    
    CalcTheoPnLCommonExercItmLTD = IIf(Not bBadPnl, dPnlTheo, BAD_DOUBLE_VALUE)
End Function

Private Function CalcTheoPnLCommonExercItmDailyPrevDate(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dSpotPriceBid#, ByVal dSpotPriceAsk#, _
                                                        ByRef bBadPnl As Boolean) As Double
    On Error Resume Next
    Dim dPnlTheo#
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE
        
    'SUD-0909
    Dim dPriceClose As Double
    dPriceClose = aPos.Quote.price.Close
    If (Not g_Main Is Nothing) Then
        If (g_Main.UseTheoCloseForPNL = True And aPos.Quote.price.TheoClose > 0#) Then
            dPriceClose = aPos.Quote.price.TheoClose
        End If
    End If
    
    If dPriceClose > 0# And (dSpotPriceBid > 0# And aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE _
        Or dSpotPriceAsk > 0# And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE) Then
    
        If aPos.OptType = enOtCall Then
            If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyDailyPrevDateBuy * (dSpotPriceBid - aPos.Strike - dPriceClose)
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyDailyPrevDateSell * (dSpotPriceAsk - aPos.Strike - dPriceClose)
                Else
                    bBadPnl = True
                End If
            End If
        
        Else
            If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyDailyPrevDateBuy * (aPos.Strike - dSpotPriceBid - dPriceClose)
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyDailyPrevDateSell * (aPos.Strike - dSpotPriceAsk - dPriceClose)
                Else
                    bBadPnl = True
                End If
            End If
            
        End If
    End If
    
    CalcTheoPnLCommonExercItmDailyPrevDate = IIf(Not bBadPnl, dPnlTheo, BAD_DOUBLE_VALUE)
End Function

Private Function CalcTheoPnLCommonExercItmDailyToday(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dSpotPriceBid#, ByVal dSpotPriceAsk#, _
                                                        ByRef bBadPnl As Boolean) As Double
    On Error Resume Next
    Dim dPnlTheo#
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE

    If dSpotPriceBid > 0# And aPos.QtyDailyTodayBuy > BAD_LONG_VALUE _
        Or dSpotPriceAsk > 0# And aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
    
        If aPos.OptType = enOtCall Then
            If aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyDailyTodayBuy * (dSpotPriceBid - aPos.Strike) - aPos.PosDailyTodayBuy
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyDailyTodaySell * (dSpotPriceAsk - aPos.Strike) - aPos.PosDailyTodaySell
                Else
                    bBadPnl = True
                End If
            End If
        
        Else
            If aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > 0# Then
                    dPnlTheo = aPos.QtyDailyTodayBuy * (aPos.Strike - dSpotPriceBid) - aPos.PosDailyTodayBuy
                Else
                    bBadPnl = True
                End If
            End If
            
            If Not bBadPnl And dSpotPriceAsk > 0# And aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > 0# Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + aPos.QtyDailyTodaySell * (aPos.Strike - dSpotPriceAsk) - aPos.PosDailyTodaySell
                Else
                    bBadPnl = True
                End If
            End If
            
        End If
    End If
    
    CalcTheoPnLCommonExercItmDailyToday = IIf(Not bBadPnl, dPnlTheo, BAD_DOUBLE_VALUE)
End Function

Private Function CalcTheoPnLCommonExercItmDaily(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dSpotPriceBid#, ByVal dSpotPriceAsk#) As Double
    On Error Resume Next
    Dim dPnlTheo#, dPnlTheoPrevDate#, bBadPnl As Boolean
    
    dPnlTheo = CalcTheoPnLCommonExercItmDailyToday(aPos, dSpotPriceBid, dSpotPriceAsk, bBadPnl)
    
    If Not bBadPnl Then
        dPnlTheoPrevDate = CalcTheoPnLCommonExercItmDailyPrevDate(aPos, dSpotPriceBid, dSpotPriceAsk, bBadPnl)
        If Not bBadPnl And dPnlTheoPrevDate > BAD_DOUBLE_VALUE Then
            If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
            dPnlTheo = dPnlTheo + dPnlTheoPrevDate
        End If
    End If
    
    CalcTheoPnLCommonExercItmDaily = IIf(Not bBadPnl, dPnlTheo, BAD_DOUBLE_VALUE)
End Function

Private Function CalcTheoPnLCommonExercOtm(ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Double
    On Error Resume Next
    Dim dPnlTheo#
    
    dPnlTheo = BAD_DOUBLE_VALUE
    
    'SUD-0909
    Dim dPriceClose As Double
    dPriceClose = aPos.Quote.price.Close
    If (Not g_Main Is Nothing) Then
        If (g_Main.UseTheoCloseForPNL = True And aPos.Quote.price.TheoClose > 0#) Then
            dPriceClose = aPos.Quote.price.TheoClose
        End If
    End If
    
    If g_Params.PnLCalcType = PNLCT_DAILY _
        And (aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Or aPos.QtyDailyTodaySell > BAD_LONG_VALUE) Then
    
        If dPriceClose > 0# Then
            If aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                dPnlTheo = -aPos.QtyDailyTodayBuy * dPriceClose
            End If
        
            If aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
                If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                dPnlTheo = dPnlTheo - aPos.QtyDailyTodaySell * dPriceClose
            End If
        End If
    Else
        If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
            dPnlTheo = -aPos.PosLTDBuy
        End If
    
        If aPos.QtyLTDSell > BAD_LONG_VALUE Then
            If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
            dPnlTheo = dPnlTheo - aPos.PosLTDSell
        End If
    End If
    
    CalcTheoPnLCommonExercOtm = dPnlTheo
End Function

Private Sub CalcTheoPnLCommonExerc(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, _
                                    ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                                    ByVal dUndSpot#, ByVal dUndBid#, ByVal dUndAsk#)
    On Error Resume Next
    Dim bBadPnl As Boolean, dTmp#, dPnlTheo#
    
    Dim dCoeff#
    
    dCoeff = 1#
    If (Not m_Und(aPos.UndID).HeadComponent Is Nothing) Then
        dCoeff = m_Und(aPos.UndID).Coeff
    End If
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE
    
    ' ITM options
    If aPos.OptType = enOtCall And aPos.Strike < dUndSpot _
        Or aPos.OptType = enOtPut And aPos.Strike > dUndSpot Then
        
        If g_Params.PnLCalcType = PNLCT_LTD Then
            dPnlTheo = CalcTheoPnLCommonExercItmLTD(aPos, dUndBid, dUndAsk)
        Else
            dPnlTheo = CalcTheoPnLCommonExercItmDaily(aPos, dUndBid, dUndAsk)
        End If
        
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        dTmp = IIf(aPos.OptType = enOtCall, 1, -1)
        If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
            aRes.Delta = aRes.Delta + aPos.QtyLTDBuy * dTmp * dUndSpot
            aRes.NetDelta = aRes.NetDelta + aPos.QtyLTDBuy * dTmp * dCoeff
        End If
        If aPos.QtyLTDSell > BAD_LONG_VALUE Then
            aRes.Delta = aRes.Delta + aPos.QtyLTDSell * dTmp * dUndSpot
            aRes.NetDelta = aRes.NetDelta + aPos.QtyLTDSell * dTmp * dCoeff
        End If
    Else ' OTM options
        dPnlTheo = CalcTheoPnLCommonExercOtm(aPos)
    End If

    If dPnlTheo > BAD_DOUBLE_VALUE Then
        If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
        aRes.PnL = aRes.PnL + dPnlTheo
    Else
        aRes.BadPnL = True
    End If
    
End Sub

Private Sub CalcTheoPnLCommon(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, _
                                    ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, ByVal dtToday As Date)
    On Error Resume Next
    Dim dPnlTheo#: dPnlTheo = BAD_DOUBLE_VALUE
    Dim bBadPnl As Boolean: bBadPnl = False
    
    If (aGreeks.nMask And GM_THEOPRICE) = GM_THEOPRICE Then
        If aGreeks.dTheoPrice >= 0 Then 'DBL_EPSILON Then
            If g_Params.PnLCalcType = PNLCT_LTD Then
                If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                    If aPos.PosLTDBuy > BAD_DOUBLE_VALUE Then
                        dPnlTheo = aGreeks.dTheoPrice * aPos.QtyLTDBuy - aPos.PosLTDBuy
                    Else
                        bBadPnl = False
                    End If
                End If
                    
                If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                    If aPos.PosLTDSell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyLTDSell - aPos.PosLTDSell
                    Else
                        bBadPnl = False
                    End If
                End If
            Else
                If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                    If aPos.PosDailyPrevDateBuy > BAD_DOUBLE_VALUE Then
                        dPnlTheo = aGreeks.dTheoPrice * aPos.QtyDailyPrevDateBuy - aPos.PosDailyPrevDateBuy
                    Else
                        bBadPnl = False
                    End If
                End If

                If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                    If aPos.PosDailyPrevDateSell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyPrevDateSell - aPos.PosDailyPrevDateSell
                    Else
                        bBadPnl = False
                    End If
                End If

                If Not bBadPnl And aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                    If aPos.PosDailyTodayBuy > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyTodayBuy - aPos.PosDailyTodayBuy
                    Else
                        bBadPnl = False
                    End If
                End If
                    
                If Not bBadPnl And aPos.QtyDailyTodaySell > BAD_LONG_VALUE And aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                    If aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                        If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                        dPnlTheo = dPnlTheo + aGreeks.dTheoPrice * aPos.QtyDailyTodaySell - aPos.PosDailyTodaySell
                    Else
                        bBadPnl = False
                    End If
                End If
            End If
            
            If Not bBadPnl And dPnlTheo > BAD_DOUBLE_VALUE Then
                If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
                aRes.PnL = aRes.PnL + dPnlTheo
            Else
                aRes.BadPnL = True
            End If
        Else
            aRes.BadPnL = True
        End If
    Else
        aRes.BadPnL = True
    End If
End Sub

Private Sub CalcUndPnL(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                        ByVal dtToday As Date, ByVal dUndSpot#, ByVal dUndBid#, ByVal dUndAsk#)
    On Error Resume Next
    Dim bBadPnl As Boolean, dPnlTheo#
    Dim dSpotPriceBid#, dSpotPriceAsk#
    
    bBadPnl = False
    dPnlTheo = BAD_DOUBLE_VALUE

    dSpotPriceBid = dUndBid
    dSpotPriceAsk = dUndAsk

    If dSpotPriceBid > BAD_DOUBLE_VALUE And aPos.QtyLTDBuy > BAD_LONG_VALUE _
        Or dSpotPriceAsk > BAD_DOUBLE_VALUE And aPos.QtyLTDSell > BAD_LONG_VALUE Then
    
        If g_Params.PnLCalcType = PNLCT_LTD Then
            If aPos.QtyLTDBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > BAD_DOUBLE_VALUE And aPos.PosLTDBuy > BAD_DOUBLE_VALUE Then
                    dPnlTheo = dSpotPriceBid * aPos.QtyLTDBuy - aPos.PosLTDBuy
                Else
                    bBadPnl = True
                End If
            End If
                
            If Not bBadPnl And aPos.QtyLTDSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > BAD_DOUBLE_VALUE And aPos.PosLTDSell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyLTDSell - aPos.PosLTDSell
                Else
                    bBadPnl = True
                End If
            End If
        Else
            If aPos.QtyDailyPrevDateBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > BAD_DOUBLE_VALUE And aPos.PosDailyPrevDateBuy > BAD_DOUBLE_VALUE Then
                    dPnlTheo = dSpotPriceBid * aPos.QtyDailyPrevDateBuy - aPos.PosDailyPrevDateBuy
                Else
                    bBadPnl = True
                End If
            End If
                            
            If Not bBadPnl And aPos.QtyDailyPrevDateSell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > BAD_DOUBLE_VALUE And aPos.PosDailyPrevDateSell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyDailyPrevDateSell - aPos.PosDailyPrevDateSell
                Else
                    bBadPnl = True
                End If
            End If
                        
            If Not bBadPnl And aPos.QtyDailyTodayBuy > BAD_LONG_VALUE Then
                If dSpotPriceBid > BAD_DOUBLE_VALUE And aPos.PosDailyTodayBuy > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceBid * aPos.QtyDailyTodayBuy - aPos.PosDailyTodayBuy
                Else
                    bBadPnl = True
                End If
            End If
                            
            If Not bBadPnl And aPos.QtyDailyTodaySell > BAD_LONG_VALUE Then
                If dSpotPriceAsk > BAD_DOUBLE_VALUE And aPos.PosDailyTodaySell > BAD_DOUBLE_VALUE Then
                    If dPnlTheo <= BAD_DOUBLE_VALUE Then dPnlTheo = 0#
                    dPnlTheo = dPnlTheo + dSpotPriceAsk * aPos.QtyDailyTodaySell - aPos.PosDailyTodaySell
                Else
                    bBadPnl = True
                End If
            End If
        End If
    End If
    
    If Not bBadPnl And dPnlTheo > BAD_DOUBLE_VALUE Then
        If aRes.PnL <= BAD_DOUBLE_VALUE Then aRes.PnL = 0#
        aRes.PnL = aRes.PnL + dPnlTheo
    Else
        aRes.BadPnL = True
    End If
End Sub

Private Sub CalcPosTotalsCommon(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, ByVal dUndSpot#)
    On Error Resume Next
    Dim dCoeff#
    
    dCoeff = 1#
    If (Not m_Und(aPos.UndID).HeadComponent Is Nothing) Then
        dCoeff = m_Und(aPos.UndID).Coeff
    End If
    
    If (aGreeks.nMask And GM_DELTA) = GM_DELTA And Not IsBadDouble(aGreeks.dDelta) Then
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.Qty * dUndSpot * aPos.Fut.KEq '* dCoeff
                aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.Qty * IIf(aPos.Fut.MultOptDltEq, 1, aPos.Fut.FutLotSize) * dCoeff
            Else
                aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * dUndSpot '* dCoeff
                aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * dCoeff
            End If
        Else
            aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * dUndSpot '* dCoeff
            aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * dCoeff
        End If
    Else
        aRes.BadDelta = True
        aRes.BadNetDelta = True
    End If
    
    If (aGreeks.nMask And GM_GAMMA) = GM_GAMMA And Not IsBadDouble(aGreeks.dGamma) Then
        If aRes.Gamma <= BAD_DOUBLE_VALUE Then aRes.Gamma = 0#
        If aRes.NetGamma <= BAD_DOUBLE_VALUE Then aRes.NetGamma = 0#
        If aRes.GammaPerc <= BAD_DOUBLE_VALUE Then aRes.GammaPerc = 0#
        
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.Gamma = aRes.Gamma + (aGreeks.dGamma * aPos.Qty * dUndSpot * dUndSpot / 100# * aPos.Fut.KEq) '* dCoeff * dCoeff
                aRes.NetGamma = aRes.NetGamma + (aGreeks.dGamma * aPos.Qty * IIf(aPos.Fut.MultOptDltEq, 1, aPos.Fut.FutLotSize)) * dCoeff * dCoeff
                aRes.GammaPerc = aRes.GammaPerc + (aGreeks.dGamma * aPos.Qty * dUndSpot / 100# * IIf(aPos.Fut.MultOptDltEq, 1, aPos.Fut.FutLotSize)) * dCoeff '* dCoeff
            Else
                aRes.Gamma = aRes.Gamma + (aGreeks.dGamma * aPos.QtyInShares * dUndSpot * dUndSpot / 100#) '* dCoeff * dCoeff
                aRes.NetGamma = aRes.NetGamma + (aGreeks.dGamma * aPos.QtyInShares) * dCoeff * dCoeff
                aRes.GammaPerc = aRes.GammaPerc + (aGreeks.dGamma * aPos.QtyInShares * dUndSpot / 100#) * dCoeff '* dCoeff
            End If
        Else
            aRes.Gamma = aRes.Gamma + (aGreeks.dGamma * aPos.QtyInShares * dUndSpot * dUndSpot / 100#) '* dCoeff * dCoeff
            aRes.NetGamma = aRes.NetGamma + (aGreeks.dGamma * aPos.QtyInShares) * dCoeff * dCoeff
            aRes.GammaPerc = aRes.GammaPerc + (aGreeks.dGamma * aPos.QtyInShares * dUndSpot / 100#) * dCoeff '* dCoeff
        End If
    Else
        aRes.BadGamma = True
        aRes.BadNetGamma = True
        aRes.BadGammaPerc = True
    End If
    
    If (aGreeks.nMask And GM_VEGA) = GM_VEGA And Not IsBadDouble(aGreeks.dVega) Then
        If aRes.Vega <= BAD_DOUBLE_VALUE Then aRes.Vega = 0#
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.Qty * aPos.Fut.KEq
            Else
                aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.QtyInShares
            End If
        Else
            aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.QtyInShares
        End If
    
        If aRes.WtdVega <= BAD_DOUBLE_VALUE Then aRes.WtdVega = 0#
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.Qty * aPos.VegaWeight * aPos.Fut.KEq
            Else
                aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.QtyInShares * aPos.VegaWeight
            End If
        Else
            aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.QtyInShares * aPos.VegaWeight
        End If
    Else
        aRes.BadVega = True
        aRes.BadWtdVega = True
    End If
    
    If (aGreeks.nMask And GM_THETA) = GM_THETA And Not IsBadDouble(aGreeks.dTheta) Then
        If aRes.Theta <= BAD_DOUBLE_VALUE Then aRes.Theta = 0#
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.Qty * aPos.Fut.KEq
            Else
                aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.QtyInShares
            End If
        Else
            aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.QtyInShares
        End If
    Else
        aRes.BadTheta = True
    End If
    
    If (aGreeks.nMask And GM_RHO) = GM_RHO And Not IsBadDouble(aGreeks.dRho) Then
        If aRes.Rho <= BAD_DOUBLE_VALUE Then aRes.Rho = 0#
        If (Not aPos.Fut Is Nothing) Then
            If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.Qty * aPos.Fut.KEq
            Else
                aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.QtyInShares
            End If
        Else
            aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.QtyInShares
        End If
    Else
        aRes.BadRho = True
    End If
End Sub

Private Sub CalcPosTotalsSynth(ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByRef aGreeks As GreeksData, ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, _
                                aSynthRoot As EtsGeneralLib.SynthRootAtom, ByVal dSynthUndSpot#, ByVal dSynthUndSpotBase#)
    On Error Resume Next
    Dim dTmp#, aSynthRootComp As EtsGeneralLib.SynthRootCompAtom, aSynthUnd As EtsMmRisksLib.MmRvUndAtom
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim dUndLast#, dUndAsk#, dUndBid#, dCoeff#
    
    dCoeff = 1#
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    If (Not m_Und(aPos.UndID).HeadComponent Is Nothing) Then
            dCoeff = m_Und(aPos.UndID).Coeff
    End If
    
    If (aGreeks.nMask And GM_DELTA) = GM_DELTA And Not IsBadDouble(aGreeks.dDelta) Then
        If aRes.Delta <= BAD_DOUBLE_VALUE Then aRes.Delta = 0#
        If aRes.NetDelta <= BAD_DOUBLE_VALUE Then aRes.NetDelta = 0#
        
        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
            Set aSynthUnd = m_Und(aSynthRootComp.UndID)
            If Not aSynthUnd Is Nothing Then
                Debug.Assert (Not aSynthUnd.UndPriceProfile Is Nothing)
                
                If (Not aSynthUnd.price.IsUseManualActive And Not aSynthUnd.PriceByHead) Then
                    dUndLast = aSynthUnd.price.Last
                    dUndBid = aSynthUnd.price.Bid
                    dUndAsk = aSynthUnd.price.Ask
                Else
                    dUndLast = aSynthUnd.price.Active: dUndAsk = aSynthUnd.price.Active: dUndBid = aSynthUnd.price.Active
                End If
                
                dTmp = aSynthUnd.UndPriceProfile.GetUndPriceMid(dUndBid, dUndAsk, dUndLast, dToleranceValue, enRoundingRule)
                If Not IsBadDouble(dTmp) And dTmp > 0# Then
                    dTmp = dTmp / dSynthUndSpotBase * dSynthUndSpot
                    aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * aSynthRootComp.Weight * dTmp '* dCoeff
                End If
                aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * aSynthRootComp.Weight * dCoeff
            End If
        Next
    
        If aSynthRoot.CashValue > 0# Then
            aRes.Delta = aRes.Delta + aGreeks.dDelta * aPos.QtyInShares * aSynthRoot.CashValue '* dCoeff
            aRes.NetDelta = aRes.NetDelta + aGreeks.dDelta * aPos.QtyInShares * aSynthRoot.CashValue * dCoeff
        End If
    Else
        aRes.BadDelta = True
        aRes.BadNetDelta = True
    End If
    
    If (aGreeks.nMask And GM_GAMMA) = GM_GAMMA And Not IsBadDouble(aGreeks.dGamma) Then
        If aRes.Gamma <= BAD_DOUBLE_VALUE Then aRes.Gamma = 0#
        If aRes.NetGamma <= BAD_DOUBLE_VALUE Then aRes.NetGamma = 0#
        If aRes.GammaPerc <= BAD_DOUBLE_VALUE Then aRes.GammaPerc = 0#
        
        For Each aSynthRootComp In aSynthRoot.SynthRootComponents
            Set aSynthUnd = m_Und(aSynthRootComp.UndID)
            If Not aSynthUnd Is Nothing Then
                Debug.Assert (Not aSynthUnd.UndPriceProfile Is Nothing)
                
                If (Not aSynthUnd.price.IsUseManualActive And Not aSynthUnd.PriceByHead) Then
                    dUndLast = aSynthUnd.price.Last
                    dUndBid = aSynthUnd.price.Bid
                    dUndAsk = aSynthUnd.price.Ask
                Else
                    dUndLast = aSynthUnd.price.Active: dUndAsk = aSynthUnd.price.Active: dUndBid = aSynthUnd.price.Active
                End If
                
                dTmp = aSynthUnd.UndPriceProfile.GetUndPriceMid(dUndBid, dUndAsk, dUndLast, dToleranceValue, enRoundingRule)
                If Not IsBadDouble(dTmp) And dTmp > 0# Then
                    dTmp = dTmp / dSynthUndSpotBase * dSynthUndSpot
                    aRes.Gamma = aRes.Gamma + (aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight * dTmp * dTmp / 100#) '* dCoeff * dCoeff
                    aRes.GammaPerc = aRes.GammaPerc + (aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight * dTmp / 100#) * dCoeff '* dCoeff
                End If
                aRes.NetGamma = aRes.NetGamma + (aGreeks.dGamma * aPos.QtyInShares * aSynthRootComp.Weight) * dCoeff * dCoeff
            End If
        Next
    
        If aSynthRoot.CashValue > 0# Then
            aRes.Gamma = aRes.Gamma + (aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue / 100#) '* dCoeff * dCoeff
            aRes.NetGamma = aRes.NetGamma + (aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue) * dCoeff * dCoeff
            aRes.GammaPerc = aRes.GammaPerc + (aGreeks.dGamma * aPos.QtyInShares * aSynthRoot.CashValue / 100#) * dCoeff '* dCoeff
        End If
    Else
        aRes.BadGamma = True
        aRes.BadNetGamma = True
        aRes.BadGammaPerc = True
    End If
    
    If (aGreeks.nMask And GM_VEGA) = GM_VEGA And Not IsBadDouble(aGreeks.dVega) Then
        If aRes.Vega <= BAD_DOUBLE_VALUE Then aRes.Vega = 0#
        aRes.Vega = aRes.Vega + aGreeks.dVega * aPos.QtyInShares
    
        If aRes.WtdVega <= BAD_DOUBLE_VALUE Then aRes.WtdVega = 0#
        aRes.WtdVega = aRes.WtdVega + aGreeks.dVega * aPos.QtyInShares * aPos.VegaWeight
    Else
        aRes.BadVega = True
        aRes.BadWtdVega = True
    End If
    
    If (aGreeks.nMask And GM_THETA) = GM_THETA And Not IsBadDouble(aGreeks.dTheta) Then
        If aRes.Theta <= BAD_DOUBLE_VALUE Then aRes.Theta = 0#
        aRes.Theta = aRes.Theta + aGreeks.dTheta * aPos.QtyInShares
    Else
        aRes.BadTheta = True
    End If
    
    If (aGreeks.nMask And GM_RHO) = GM_RHO And Not IsBadDouble(aGreeks.dRho) Then
        If aRes.Rho <= BAD_DOUBLE_VALUE Then aRes.Rho = 0#
        aRes.Rho = aRes.Rho + aGreeks.dRho * aPos.QtyInShares
    Else
        aRes.BadRho = True
    End If
End Sub

Private Function CalcGreeksCommon(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dtToday As Date, _
                                ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, ByRef aGreeks As GreeksData, _
                                ByRef dUndSpot#, ByRef dUndSpotBase#, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum, _
                                Optional ByVal dFutSpot As Double = 0) As Boolean
    On Error Resume Next
    Dim nDivCount&, nBaskDivCount&, dYield#, dVola#, dOptSpot#, nIsAmerican&, nIsAmericanFut&
    Dim dDivDte() As Double, dDivAmts() As Double
    Dim nFlag&
    Dim bIsBasket As Boolean
    Dim lSurfaceID As Long
    
    Dim dYte As Double
    
    Dim dtNow As Date
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    Dim dGrVola As Double
    dGrVola = GetGroupRatio(aUnd, enGrVola, Filter.Data(MFC_GROUPS))
    
    dtNow = dtToday
    GetCalcParams dtNow, aPos.ExpiryOV, aPos.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
    
    nDivCount = 0
    ReDim dDivDte(0 To 0)
    ReDim dDivAmts(0 To 0)
    
    nIsAmerican = IIf(aUnd.IsAmerican, 1, 0)
    CalcGreeksCommon = False
    
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Set aDiv = aUnd.Dividend
    Dim enDivType As EtsGeneralLib.EtsDivTypeEnum
    
    enDivType = enDivCustomStream
    If (Not aDiv Is Nothing) Then enDivType = aDiv.DivType
    
    Select Case enDivType
            Case enDivMarket, enDivCustomPeriodical, enDivCustomStream
                If Not aDiv Is Nothing Then
                    aDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                    If nDivCount > 0 Then
                        aDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                    End If
                    Set aDiv = Nothing
                End If
            Case enDivStockBasket
                If Not aUnd.BasketIndex Is Nothing Then
                    Dim aBasketDiv As EtsGeneralLib.EtsIndexDivColl
                    Set aBasketDiv = aUnd.BasketIndex.BasketDivs
                    bIsBasket = aUnd.BasketIndex.IsBasket
                    If Not aBasketDiv Is Nothing Then
                        aBasketDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nDivCount
                        If nDivCount > 0 Then
                                aBasketDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nDivCount, dDivAmts, dDivDte, nDivCount
                        End If
                    End If
                    Set aBasketDiv = Nothing
                End If
            Case enDivIndexYield
                dYield = aUnd.Yield
    End Select
        
        
'   incorrect computation's in case of implied volatility usage
    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aPos.OptionRootID)
    dVola = 0#

'    If g_Params.UseTheoVolatility Then
        dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'    Else
'        If Not g_Params.UseTheoNoBid Or g_Params.UseTheoNoBid And aPos.Quote.price.Bid > DBL_EPSILON Then
'
'            dOptSpot = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0#)
'            If (aPos.Quote.price.IsUseManualActive) Then
'                dOptSpot = aPos.Quote.price.Active
'            End If
'
'            If dOptSpot > 0# Then
'                nFlag = VF_OK
'                If aPos.ContractType = enCtOption Then
'                dVola = CalcVolatilityMM3(aPos.Rate, dYield, aPos.HTBRate, dUndSpot, dOptSpot, aPos.Strike, dYte, _
'                                    aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), _
'                                    100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
'                ElseIf aPos.ContractType = enCtFutOption Then
'                    dVola = CalcFutureOptionVolatility(aPos.Rate, dFutSpot, dOptSpot, aPos.Strike, dYte, _
'                                        aPos.OptType, nIsAmerican, 100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
'                End If
'
'                If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
'                    dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'                End If
'            ElseIf g_Params.UseTheoBadMarketVola Then
'                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'            End If
'        Else
'            dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'        End If
'    End If
    
    If dVola > 0# Then
        ShiftVola aRes, aPos.VegaWeight, dVola, dGrVola
        
        Dim RetCount As Long
        RetCount = 0
        If aPos.ContractType = enCtOption Then
            RetCount = CalcGreeksMM2(aPos.Rate, dYield, aPos.HTBRate, dUndSpot, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aUnd.Skew, aUnd.Kurt, nModel, aGreeks)
        ElseIf aPos.ContractType = enCtFutOption Then
            nIsAmericanFut = IIf(aPos.Fut.IsAmerican, 1, 0)
            Debug.Assert (dFutSpot >= 0)
            If aUnd.ContractType = enCtFutUnd Then
                RetCount = CalcFutureOptionGreeks2(aPos.Rate, dFutSpot, False, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmericanFut, 100, aUnd.Skew, aUnd.Kurt, nModel, aGreeks)
            Else
                RetCount = CalcFutureOptionGreeks3(aPos.Rate, aUnd.Yield, dFutSpot, True, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmericanFut, 100, aUnd.Skew, aUnd.Kurt, nModel, nDivCount, _
                            dDivAmts(0), dDivDte(0), aGreeks)
            End If
        End If
            
        If RetCount <> 0 Then
            FixGreeks aGreeks
            CalcGreeksCommon = True
        End If
    End If

    Erase dDivDte
    Erase dDivAmts
End Function

Private Function CalcGreeksSynth(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef aPos As EtsMmRisksLib.MmRvPosAtom, ByVal dtToday As Date, _
                                ByRef aRes As EtsMmRisksLib.MmRvMatrixCalcResultType, ByRef aGreeks As GreeksData, ByRef aSynthRoot As EtsGeneralLib.SynthRootAtom, _
                                ByRef dSynthUndSpot#, ByRef dSynthUndSpotBase#, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum) As Boolean
    On Error Resume Next
    Dim nDivCount&, RetCount&, nBaskDivCount&, dYield#, dVola#, dOptSpot#, nIsAmerican&
    Dim dDivDte() As Double, dDivAmts() As Double, aBaskDivs() As REGULAR_DIVIDENDS
    Dim nFlag&
    Dim dYte As Double
    Dim lSurfaceID As Long
    
    nDivCount = 0
    ReDim dDivDte(0 To 0)
    ReDim dDivAmts(0 To 0)
    
    nIsAmerican = IIf(aUnd.IsAmerican, 1, 0)
    CalcGreeksSynth = False
    
    Dim dtNow As Date
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    Dim dGrVola As Double
    dGrVola = GetGroupRatio(aUnd, enGrVola, Filter.Data(MFC_GROUPS))
    
    dtNow = dtToday
    GetCalcParams dtNow, aPos.ExpiryOV, aPos.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
    
    If aSynthRoot.Basket Then
            Dim aBasketDiv As EtsGeneralLib.EtsIndexDivColl
            Set aBasketDiv = aSynthRoot.BasketDivs
            If Not aBasketDiv Is Nothing Then
                aBasketDiv.GetDividendCount2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount
                If nBaskDivCount > 0 Then _
                        aBasketDiv.GetDividends2 dtNow, dtExpiryOV, dtTradingClose, nBaskDivCount, dDivAmts, dDivDte, nDivCount
            End If
            Set aBasketDiv = Nothing
        Erase aBaskDivs
        If nDivCount <= 0 Then nDivCount = 0
    Else
        dYield = aSynthRoot.Yield
        nDivCount = 0
    End If
    
    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aPos.OptionRootID)
    dVola = 0#
'   If Not g_Params.UseTheoVolatility And (Not g_Params.UseTheoNoBid Or g_Params.UseTheoNoBid And aPos.Quote.price.Bid > 0#) Then
'
'        Debug.Assert (Not aUnd.OptPriceProfile Is Nothing)
'        dOptSpot = aUnd.OptPriceProfile.GetOptPriceMid(aPos.Quote.price.Bid, aPos.Quote.price.Ask, aPos.Quote.price.Last, g_Params.PriceRoundingRule, g_Params.UseTheoVolatility, 0#)
'
'        If dOptSpot > 0# Then
'            nFlag = VF_OK
'            dVola = CalcVolatilityMM3(aPos.Rate, dYield, aPos.HTBRate, dSynthUndSpotBase, dOptSpot, aPos.Strike, dYte, _
'                aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, nFlag)
'            If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
'                dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'            End If
'        ElseIf g_Params.UseTheoBadMarketVola Then
'            dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'        End If
'    Else
        dVola = aUnd.VolaSrv.OptionVola(aPos.ExpiryOV, aPos.Strike, lSurfaceID)
'    End If
    
    If dVola > 0# Then
        ShiftVola aRes, aPos.VegaWeight, dVola, dGrVola
        
        RetCount = CalcGreeksMM2(aPos.Rate, dYield, aPos.HTBRate, dSynthUndSpot, aPos.Strike, dVola, dYte, _
                            aPos.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aSynthRoot.Skew, aSynthRoot.Kurt, nModel, aGreeks)
        
        If RetCount <> 0 Then
            FixGreeks aGreeks
            CalcGreeksSynth = True
        End If
    End If

    Erase dDivDte
    Erase dDivAmts
End Function

Private Sub GetShifts(ByVal nX As Integer, ByVal nY As Integer, ByVal dBeta#, ByVal dDriverPrice#, ByVal dWeight#, ByVal bCorrelatedShift As Boolean, _
                        ByRef dUndSpot#, ByRef dUndBid#, ByRef dUndAsk#, _
                        ByRef dFutSpot#, ByRef dFutBid#, ByRef dFutAsk#, _
                        ByRef dtToday As Date, ByVal dGroupRatio#)
    
            Dim dtNow As Date
            dtNow = GetNewYorkTime
            dtToday = dtNow
    
            Select Case m_Scn.Axis(RMA_HORZ)
                Case RMAT_SPOT
                    If dUndSpot > 0# Then
                        ShiftSpot dBeta, m_Scn.Units(RMA_HORZ), m_Res(nX, nY).ShiftX, bCorrelatedShift, dUndSpot, dUndBid, dUndAsk, dDriverPrice, dWeight, dGroupRatio
                    End If
                    If dFutSpot > 0# Then
                        ShiftSpot dBeta, m_Scn.Units(RMA_HORZ), m_Res(nX, nY).ShiftX, bCorrelatedShift, dFutSpot, dFutBid, dFutAsk, dDriverPrice, dWeight, dGroupRatio
                    End If
                Case RMAT_TIME
                    dtToday = dtNow + m_Res(nX, nY).ShiftX
            End Select
            
            Select Case m_Scn.Axis(RMA_VERT)
                Case RMAT_SPOT
                    If dUndSpot > 0# Then
                        ShiftSpot dBeta, m_Scn.Units(RMA_VERT), m_Res(nX, nY).ShiftY, bCorrelatedShift, dUndSpot, dUndBid, dUndAsk, dDriverPrice, dWeight, dGroupRatio
                    End If
                    If dFutSpot > 0# Then
                        ShiftSpot dBeta, m_Scn.Units(RMA_VERT), m_Res(nX, nY).ShiftY, bCorrelatedShift, dFutSpot, dFutBid, dFutAsk, dDriverPrice, dWeight, dGroupRatio
                    End If
                Case RMAT_TIME
                    dtToday = dtNow + m_Res(nX, nY).ShiftY
            End Select
End Sub

Private Sub GetBasePrices(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByRef dUndSpotBase#, ByRef dUndBidBase#, ByRef dUndAskBase#, ByRef dDriverPrice#, _
                            ByRef dGroupCompWeight#, ByRef dMainBetta#, ByVal bCorrelatedShift As Boolean)

On Error GoTo Exception

    Dim enReplaceStatus As EtsReplacePriceStatusEnum
    Dim bFutPriceReplaced As Boolean
    dGroupCompWeight = 1#
    
    'Calc base price's of current underlying
    dUndSpotBase = aUnd.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
    If (aUnd.IsHead) Then
        If (bFutPriceReplaced Or aUnd.price.IsUseManualActive) Then
            dUndBidBase = dUndSpotBase: dUndAskBase = dUndSpotBase
        Else
            dUndBidBase = aUnd.UndPriceProfile.GetUndPriceBidForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
            dUndAskBase = aUnd.UndPriceProfile.GetUndPriceAskForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
        End If
    ElseIf (Not aUnd.HeadComponent Is Nothing And aUnd.PriceByHead) Then
        dUndBidBase = dUndSpotBase: dUndAskBase = dUndSpotBase
    Else
        If (aUnd.price.IsUseManualActive Or bFutPriceReplaced) Then
            dUndBidBase = dUndSpotBase: dUndAskBase = dUndSpotBase
        Else
            dUndBidBase = aUnd.UndPriceProfile.GetUndPriceBidForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
                    g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
            dUndAskBase = aUnd.UndPriceProfile.GetUndPriceAskForPnL(aUnd.price.Bid, aUnd.price.Ask, aUnd.price.Last, _
                    g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
        End If
    End If
    'Get main driver price
    If (Not bCorrelatedShift) Then
        If (aUnd.IsHead) Then
            If (Not aUnd.ActiveFuture Is Nothing) Then
                dDriverPrice = aUnd.ActiveFuture.GetFuturePrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
            Else
                dDriverPrice = dUndSpotBase
            End If
        ElseIf (Not aUnd.HeadComponent Is Nothing And aUnd.PriceByHead) Then
            If (Not aUnd.HeadComponent.ActiveFuture Is Nothing) Then
                dDriverPrice = aUnd.HeadComponent.ActiveFuture.GetFuturePrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
            Else
                dDriverPrice = aUnd.HeadComponent.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
            End If
        Else
            If (Not aUnd.ActiveFuture Is Nothing) Then
                dDriverPrice = aUnd.ActiveFuture.GetFuturePrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
            Else
                dDriverPrice = dUndSpotBase
            End If
        End If
        
        'Get asset group component weight
        If (Not aUnd.HeadComponent Is Nothing) Then
            If (aUnd.PriceByHead) Then
                dGroupCompWeight = aUnd.Coeff
            End If
        End If
    Else
        If (Not aUnd.HeadComponent Is Nothing) Then
            dDriverPrice = aUnd.HeadComponent.GetUnderlyingPrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
        Else
            dDriverPrice = dUndSpotBase
        End If
        
        'Get asset group component weight
        If (Not aUnd.HeadComponent Is Nothing) Then
            dGroupCompWeight = aUnd.Coeff
        End If
    End If
    'Get Main Betta for calc
    dMainBetta = aUnd.Beta
    If (Not aUnd.HeadComponent Is Nothing) Then
        dMainBetta = aUnd.HeadComponent.Beta
    End If
    Exit Sub
    
Exception:
    Debug.Print "Erorr while trying to get Base prices"
End Sub

Private Sub GetFutureBasePrices(ByRef aFut As EtsMmRisksLib.MmRvFutAtom, ByRef dFutSpot#, ByRef dFutBid#, ByRef dFutAsk#)
On Error GoTo Exception

    Dim enReplaceStatus As EtsReplacePriceStatusEnum
    Dim bFutPriceReplaced As Boolean
    
    dFutSpot = aFut.GetFuturePrice(g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule, enReplaceStatus, bFutPriceReplaced)
                            
    If (bFutPriceReplaced Or aFut.price.IsUseManualActive) Then
        dFutBid = dFutSpot: dFutAsk = dFutSpot
    Else
        dFutBid = aFut.UndPriceProfile.GetUndPriceBidForPnL(aFut.price.Bid, aFut.price.Ask, aFut.price.Last, g_Params.UndPriceToleranceValue, enReplaceStatus)
        dFutAsk = aFut.UndPriceProfile.GetUndPriceAskForPnL(aFut.price.Bid, aFut.price.Ask, aFut.price.Last, g_Params.UndPriceToleranceValue, enReplaceStatus)
    End If
    
    Exit Sub
Exception:
    Debug.Print "Error while trying to GetFutureBasePrice"
End Sub

Private Function GetGroupRatio(ByRef aAsset As EtsMmRisksLib.MmRvUndAtom, _
                                ByVal Idx As GroupRatioEnum, _
                                ByVal SectorID As Long) As Double
On Error GoTo error_exception
    ' in this case we will find maximum ratio for currect asset
    GetGroupRatio = BAD_DOUBLE_VALUE
    
    Dim itrGroup As EtsUndGroupAtom
    Dim dblRatio As Double
    Dim bFound As Boolean
    
    bFound = False
    dblRatio = 0#
    If (Not aAsset Is Nothing) Then
        For Each itrGroup In g_Main.UnderlyingGroup
            If (Not itrGroup.Und Is Nothing) Then
            
                If (SectorID <> 0) Then
                    If (itrGroup.ID <> SectorID) Then GoTo SkipSector
                End If
                
                If (Not itrGroup.Und.Item(aAsset.ID) Is Nothing) Then
                    If (Abs(itrGroup.Ratio(Idx)) > Abs(dblRatio)) Then
                        dblRatio = itrGroup.Ratio(Idx)
                        bFound = True
                    End If
                End If
                
SkipSector:
            End If
        Next
    End If
    
    If ((Not bFound) And SectorID <> 0) Then
        If (Not aAsset Is Nothing) Then
            For Each itrGroup In g_Main.UnderlyingGroup
                If (Not itrGroup.Und.Item(aAsset.ID) Is Nothing) Then
                    If (Abs(itrGroup.Ratio(Idx)) > Abs(dblRatio)) Then
                        dblRatio = itrGroup.Ratio(Idx)
                        bFound = True
                    End If
                End If
            Next
        End If
    End If
    
    GetGroupRatio = IIf(bFound, dblRatio, 1)
    
    Exit Function
error_exception:
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogInformation, "GetGroupRatio: " & aAsset.Symbol, m_frmOwner.GetCaption
End Function

Private Sub CalcPosition(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom, ByVal nLastX As Long, ByVal nLastY As Long, ByVal nGreeksMask As Long, _
                        ByVal bCorrelatedShift As Boolean, ByVal nModel As EtsGeneralLib.EtsCalcModelTypeEnum)
    On Error Resume Next
    Dim dtToday As Date, nX&, nY&, aPos As EtsMmRisksLib.MmRvPosAtom, aFut As EtsMmRisksLib.MmRvFutAtom
    Dim aGreeks As GreeksData, dUndSpotBase#, dUndSpot#, dUndBidBase#, dUndBid#, dUndAskBase#, dUndAsk#
    Dim dSynthUndSpotBase#, dSynthUndSpot#, dSynthUndBidBase#, dSynthUndBid#
    Dim dSynthUndAskBase#, dSynthUndAsk#, dSynthUndLastBase#, nSynthOptRootID&
    Dim aSynthRoot As EtsGeneralLib.SynthRootAtom
    Dim dActiveFutPrice As Double
    Dim enReplaceStatus As EtsReplacePriceStatusEnum
    Dim bFutPriceReplaced As Boolean
    Dim dUndBidSave#, dUndAskSave#, dUndSave#, dDriverPrice#, dGroupCompWeight#, dMainBetta#
    Dim dFutSpot#, dFutBid#, dFutAsk#, dCoeff#
    Dim dGrVola As Double, dGrSpot As Double
    

    nSynthOptRootID = BAD_LONG_VALUE
    
    GetBasePrices aUnd, dUndSpotBase, dUndBidBase, dUndAskBase, dDriverPrice, dGroupCompWeight, dMainBetta, bCorrelatedShift
    dCoeff = 1#
    If (Not aUnd.HeadComponent Is Nothing) Then
        dCoeff = aUnd.Coeff
    End If
    'Next Calc all greeks
    '-------------------------------------------------------------------------------------------------------------------'
        dGrSpot = GetGroupRatio(aUnd, enGrSpot, Filter.Data(MFC_GROUPS))
        dGrVola = GetGroupRatio(aUnd, enGrVola, Filter.Data(MFC_GROUPS))
        
        dUndSpot = dUndSpotBase: dUndBid = dUndBidBase: dUndAsk = dUndAskBase

        dtToday = Date
    
        For nX = 0 To nLastX
            nY = 0
                       
            For nY = 0 To nLastY
                
                dUndBidSave = dUndBid
                dUndAskSave = dUndAsk
                dUndSave = dUndSpot
                
                For Each aPos In aUnd.Pos

                    If CheckPosFilter(aPos) Then
                        
                        If ((aPos.ContractType = enCtFuture Or aPos.ContractType = enCtFutOption) And Not aPos.Fut Is Nothing) Then
                            GetFutureBasePrices aPos.Fut, dFutSpot, dFutBid, dFutAsk
                        Else
                            dFutSpot = 0#: dFutBid = 0#: dFutAsk = 0#
                        End If
                        
                        dUndSpot = dUndSpotBase: dUndBid = dUndBidBase: dUndAsk = dUndAskBase
                        GetShifts nX, nY, dMainBetta, dDriverPrice, dGroupCompWeight, bCorrelatedShift, _
                                dUndSpot, dUndBid, dUndAsk, dFutSpot, dFutBid, dFutAsk, dtToday, dGrSpot
                        
                        If aPos.ContractType = enCtOption Or aPos.ContractType = enCtFutOption Then
                            
                            If dUndSpot > 0# Or aUnd.HasSynthetic Then
                                    If Not aPos.IsSynthetic Then
                                        
                                        If dUndSpot <= 0# Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If
                                    
                                        If aPos.ExpiryOV >= dtToday Then

                                            ClearGreeks aGreeks

                                            aGreeks.nMask = nGreeksMask
                                            If CalcGreeksCommon(aUnd, aPos, dtToday, m_Res(nX, nY), aGreeks, dUndSpot, dUndSpotBase, nModel, dFutSpot) Then
                                            
                                                CalcTheoPnLCommon aPos, aGreeks, m_Res(nX, nY), dtToday
                                                CalcPosTotalsCommon aPos, aGreeks, m_Res(nX, nY), dUndSpot
                                                
                                            Else
                                                SetResultsBadStatus m_Res(nX, nY)
                                            End If
                                            
                                        ElseIf g_Params.MatrixExerciseOptions Then
                                        
                                                CalcTheoPnLCommonExerc aPos, m_Res(nX, nY), dUndSpot, dUndBid, dUndAsk
                                            
                                        End If
                                    
                                    Else ' synthetic position
                                    
                                        If nSynthOptRootID <> aPos.OptionRootID Then

                                            If Not aUnd.SynthRoots Is Nothing Then Set aSynthRoot = aUnd.SynthRoots(aPos.OptionRootID)

                                            aUnd.GetSyntheticUnderlyingPrice m_Und, aUnd.SynthRoots(aPos.OptionRootID), dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase
                                            
                                            dSynthUndSpotBase = aUnd.UndPriceProfile.GetUndPriceMid(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                                                
                                            dSynthUndBidBase = aUnd.UndPriceProfile.GetUndPriceBidForPnL(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                                                
                                            dSynthUndAskBase = aUnd.UndPriceProfile.GetUndPriceAskForPnL(dSynthUndBidBase, dSynthUndAskBase, dSynthUndLastBase, _
                                                g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)

                                            dSynthUndSpot = dSynthUndSpotBase: dSynthUndBid = dSynthUndBidBase: dSynthUndAsk = dSynthUndAskBase

                                            nSynthOptRootID = aPos.OptionRootID
                                        End If

                                        If aSynthRoot Is Nothing Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If

                                        If m_Scn.Axis(RMA_HORZ) = RMAT_SPOT Then
                                            dSynthUndSpot = dSynthUndSpotBase: dSynthUndBid = dSynthUndBidBase: dSynthUndAsk = dSynthUndAskBase
                                            ShiftSyntSpot aUnd.SynthRoots(aPos.OptionRootID), m_Scn.Units(RMA_HORZ), m_Res(nX, nY).ShiftX, _
                                                        bCorrelatedShift, dSynthUndSpot, dSynthUndBid, dSynthUndAsk, dDriverPrice, dMainBetta, dGroupCompWeight, _
                                                        dGrSpot

                                        ElseIf m_Scn.Axis(RMA_VERT) = RMAT_SPOT Then
                                            dSynthUndSpot = dSynthUndSpotBase: dSynthUndBid = dSynthUndBidBase: dSynthUndAsk = dSynthUndAskBase
                                            ShiftSyntSpot aUnd.SynthRoots(aPos.OptionRootID), m_Scn.Units(RMA_VERT), m_Res(nX, nY).ShiftY, _
                                                        bCorrelatedShift, dSynthUndSpot, dSynthUndBid, dSynthUndAsk, dDriverPrice, dMainBetta, dGroupCompWeight, _
                                                        dGrSpot
                                        End If

                                        If dSynthUndSpot <= 0# Then
                                            SetResultsBadStatus m_Res(nX, nY)
                                            GoTo NextPos
                                        End If

                                        If aPos.ExpiryOV >= dtToday Then

                                            aGreeks.nMask = nGreeksMask
                                            If CalcGreeksSynth(aUnd, aPos, dtToday, m_Res(nX, nY), aGreeks, aSynthRoot, dSynthUndSpot, dSynthUndSpotBase, nModel) Then

                                                CalcTheoPnLCommon aPos, aGreeks, m_Res(nX, nY), dtToday
                                                CalcPosTotalsSynth aPos, aGreeks, m_Res(nX, nY), aSynthRoot, dSynthUndSpot, dSynthUndSpotBase

                                            Else
                                                SetResultsBadStatus m_Res(nX, nY)
                                            End If
                                        Else
                                            If g_Params.MatrixExerciseOptions Then

                                                CalcTheoPnLCommonExerc aPos, m_Res(nX, nY), dUndSpot, dUndBid, dUndAsk

                                            End If
                                        End If
                                End If
                            End If
                            
                        ElseIf (aPos.ContractType = enCtFuture) Then
                        
                            If dUndSpot <= 0# Then
                                SetResultsBadStatus m_Res(nX, nY)
                                GoTo NextPos
                            End If
                            
                            CalcUndPnL aPos, m_Res(nX, nY), dtToday, dFutSpot, dFutBid, dFutAsk

                            If m_Res(nX, nY).Delta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).Delta = 0#
                            If m_Res(nX, nY).NetDelta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).NetDelta = 0#
                            
                            If (Not aPos.Fut Is Nothing) Then
                                If (aPos.Fut.Underlying.ContractType = enCtFutUnd) Then
                                    m_Res(nX, nY).Delta = m_Res(nX, nY).Delta + aPos.Qty * dUndSpot * aPos.Fut.KEq '* dCoeff
                                    m_Res(nX, nY).NetDelta = m_Res(nX, nY).NetDelta + aPos.Qty * IIf(aPos.Fut.MultOptDltEq, 1, aPos.Fut.FutLotSize) * dCoeff
                                Else
                                    m_Res(nX, nY).Delta = m_Res(nX, nY).Delta + aPos.QtyInShares * dUndSpot '* dCoeff
                                    m_Res(nX, nY).NetDelta = m_Res(nX, nY).NetDelta + aPos.QtyInShares * dCoeff
                                End If
                            End If
                            
                        Else
                            
                            If dUndSpot <= 0# Then
                                SetResultsBadStatus m_Res(nX, nY)
                                GoTo NextPos
                            End If
                            
                            CalcUndPnL aPos, m_Res(nX, nY), dtToday, dUndSpot, dUndBid, dUndAsk
                            
                            If m_Res(nX, nY).Delta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).Delta = 0#
                            If m_Res(nX, nY).NetDelta <= BAD_DOUBLE_VALUE Then m_Res(nX, nY).NetDelta = 0#
                            
                            m_Res(nX, nY).Delta = m_Res(nX, nY).Delta + aPos.QtyInShares * dUndSpot '* dCoeff
                            m_Res(nX, nY).NetDelta = m_Res(nX, nY).NetDelta + aPos.QtyInShares * dCoeff
                        End If
NextPos:
                            DoEvents
                            If Not m_bRecalc Then Exit Sub
                    End If
                Next
                IncProgress pbProgress
            Next
        Next
End Sub

Private Sub PriceProvider_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, bFinished As Boolean
    
    bFinished = False
    
    If Not Request Is Nothing And ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If PpIsNonGroupReqType(Request.Type) Then
                
            Select Case Request.Type
                Case enOPT
                    sKey = CStr(enCtOption)
                Case enSTK
                    sKey = CStr(enCtStock)
                Case enIDX
                    sKey = CStr(enCtIndex)
                Case enFUT
                    sKey = CStr(enCtFuture)
                Case enFOP
                    sKey = CStr(enCtFutOption)
            End Select
            sKey = sKey & "_" & Request.Symbol
            
            If (Len(Request.Exchange) > 0) Then
                sKey = sKey & "." & Request.Exchange
            End If
                
                Set aReq = m_QuoteRequests(sKey)
                If Not aReq Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                
                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                    IncProgress pbProgress
                    Debug.Print "Eror " & CStr(m_nLastQuoteReqDone) & " " & Request.Symbol
                    
                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                                Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
                Else
                   Debug.Print "Eror! " & CStr(m_nLastQuoteReqDone) & " " & Request.Symbol
                End If
            ElseIf PpIsGroupReqType(Request.Type) Then
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                AdjustState
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequest Then
                    m_GroupPriceProvider.CancelGroup
                End If
                
                CalcMatrix
                
                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                imgStop.Visible = False
                imgStopDis.Visible = False
                
                RaiseEvent OnRefreshComplete
            End If
        Else
            'Debug.Assert False
        End If
    
    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    
    Dim sKey$, aReq As EtsMmRisksLib.MmRvReqAtom, dPriceBid#, dPriceAsk#, dPriceLast#, nLotSize&
    Dim aReqFutData As EtsMmRisksLib.MmRvFutAtom
    Dim aReqUndData As EtsMmRisksLib.MmRvUndAtom
    Dim aReqIdxData As EtsMmRisksLib.MmRvUndAtom
    Dim bFinished As Boolean
    bFinished = False
    
    If PpIsNonGroupReqType(Params.Type) Then
            Select Case Params.Type
                Case enOPT
                    sKey = CStr(enCtOption)
                Case enSTK
                    sKey = CStr(enCtStock)
                Case enIDX
                    sKey = CStr(enCtIndex)
                Case enFUT
                    sKey = CStr(enCtFuture)
                Case enFOP
                    sKey = CStr(enCtFutOption)
            End Select
            sKey = sKey & "_" & Params.Symbol
            
            If (Len(Params.Exchange) > 0) Then
                sKey = sKey & "." & Params.Exchange
            End If
            
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
            
            If Not aReq.IndexOnly Then
            
                If Not aReq.Pos Is Nothing Then
                    If dPriceBid > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Bid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Ask = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then aReq.Pos.Quote.price.Last = dPriceLast
                End If
                
                If Params.Type <> enOPT Then
                    Set aReqUndData = aReq.Und
                    
                    If Params.Type = enSTK Or Params.Type = enIDX Then
                        If dPriceBid > BAD_DOUBLE_VALUE Then aReqUndData.price.Bid = dPriceBid
                        If dPriceAsk > BAD_DOUBLE_VALUE Then aReqUndData.price.Ask = dPriceAsk
                        If dPriceLast > BAD_DOUBLE_VALUE Then aReqUndData.price.Last = dPriceLast
                        
                        Debug.Assert (Not aReq.Und.UndPriceProfile Is Nothing)
                        If (aReqUndData.price.IsUseManualActive) Then
                            aReqUndData.VolaSrv.UnderlyingPrice = aReqUndData.price.Active
                        Else
                            aReqUndData.VolaSrv.UnderlyingPrice = aReqUndData.UndPriceProfile.GetUndPriceMid(aReqUndData.price.Bid, aReqUndData.price.Ask, aReqUndData.price.Last, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
                        End If
                    
                        If m_Idx.ID = aReq.Und.ID Then
                            Set aReqIdxData = m_Idx
                            If dPriceBid > BAD_DOUBLE_VALUE Then aReqIdxData.price.Bid = dPriceBid
                            If dPriceAsk > BAD_DOUBLE_VALUE Then aReqIdxData.price.Ask = dPriceAsk
                            If dPriceLast > BAD_DOUBLE_VALUE Then aReqIdxData.price.Last = dPriceLast
                        End If
                        
                    ElseIf Params.Type = enFUT Then
                    
                        If Not aReq.Pos Is Nothing Then
                            Set aReqFutData = aReq.Pos.Fut
                        ElseIf Not aReq.Fut Is Nothing Then
                            Set aReqFutData = aReq.Fut
                        End If
                    
                        If Not aReqFutData Is Nothing Then
                            If dPriceBid > BAD_DOUBLE_VALUE Then aReqFutData.price.Bid = dPriceBid
                            If dPriceAsk > BAD_DOUBLE_VALUE Then aReqFutData.price.Ask = dPriceAsk
                            If dPriceLast > BAD_DOUBLE_VALUE Then aReqFutData.price.Last = dPriceLast
    
                            Set aReqFutData = Nothing
                        End If
                    End If
                End If
            Else
                Debug.Assert m_Idx.ID = aReq.Und.ID
                If m_Idx.ID = aReq.Und.ID Then
                    
                    Set aReqIdxData = m_Idx
                    If dPriceBid > BAD_DOUBLE_VALUE Then aReqIdxData.price.Bid = dPriceBid
                    If dPriceAsk > BAD_DOUBLE_VALUE Then aReqIdxData.price.Ask = dPriceAsk
                    If dPriceLast > BAD_DOUBLE_VALUE Then aReqIdxData.price.Last = dPriceLast
                    
                End If
            End If
        
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            Set aReq = Nothing
            Debug.Print "Success " & CStr(m_nLastQuoteReqDone) & " " & Params.Symbol
        
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                        Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
        Else
                Debug.Print "Success! " & CStr(m_nLastQuoteReqDone) & " " & Params.Symbol
        End If
    ElseIf PpIsGroupReqType(Params.Type) Then
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If
    
    If bFinished Then
        m_bLastQuoteReqNow = False
        AdjustState
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If

        CalcMatrix
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        RaiseEvent OnRefreshComplete
    End If
End Sub

Private Sub RequestLastQuotes(ByVal bIndexOnly As Boolean)
    On Error GoTo EH
    Dim i&, nCount&, aIdxReq As EtsMmRisksLib.MmRvReqAtom
    Dim aParam As PRICEPROVIDERSLib.QuoteUpdateParams, aUnd As EtsMmRisksLib.MmRvUndAtom
    Dim aReq As EtsMmRisksLib.MmRvReqAtom
    
    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow
        nCount = 0
        
        m_bGroupRequest = m_bGroupRequestSupported And Not g_Params.MatrixReqTypeAlwaysNonGroup
        
        If m_Idx.ID > 0 Then
            Set aIdxReq = m_QuoteRequests(CStr(enCtIndex) & "_" & m_Idx.Symbol)
        End If
        
        If Not bIndexOnly Then
            nCount = m_QuoteRequests.Count
            
        ElseIf Not aIdxReq Is Nothing Then
            If aIdxReq.IndexOnly Then
                nCount = 1
            End If
        End If
        
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
            
            If Not m_bGroupRequest Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                
                aParam.Type = enMStart
                PriceProvider.RequestLastQuote aParam
                
                If Not bIndexOnly Then
                    For Each aReq In m_QuoteRequests
                        If Not m_bLastQuoteReqNow Then Exit For
                        PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                    Next
                Else
                    If Not aIdxReq Is Nothing Then
                        PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        Set aIdxReq = Nothing
                    End If
                End If
                
                aParam.Type = enMStop
                PriceProvider.RequestLastQuote aParam
            Else
                If Not bIndexOnly Then
                    m_nLastQuoteGroupReqCount = m_Und.Count * 2
                    m_nLastQuoteGroupReqDone = 0&
                
                    If Not aIdxReq Is Nothing Then
                        If aIdxReq.IndexOnly Then
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount + 1
                            PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        End If
                    End If
                
                    For Each aUnd In m_Und
                        If Not m_bLastQuoteReqNow Then Exit For
                        aParam.Symbol = aUnd.Symbol
                        If Not g_Exch(aUnd.PrimaryExchangeID) Is Nothing Then
                            aParam.Exchange = g_Exch(aUnd.PrimaryExchangeID).Code
                        Else
                            aParam.Exchange = ""
                        End If
                        
                        aParam.Type = IIf(aUnd.ContractType = enCtStock, enSTK, enIDX)
                        PriceProvider.RequestLastQuote aParam
                        
                        If aUnd.HasOptPos Then
                            aParam.Type = IIf(aUnd.ContractType = enCtStock, enGrSTK, enGrIDX)
                            m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                        Else
                            m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqCount - 1
                        End If
                        
                        DoEvents
                    Next
                Else
                    If Not aIdxReq Is Nothing Then
                        m_nLastQuoteGroupReqCount = 1
                        m_nLastQuoteGroupReqDone = 0&
                        PriceProvider.RequestLastQuote aIdxReq.GetQuoteUpdateParam
                        Set aIdxReq = Nothing
                    End If
                End If
            End If
            
            m_bInProc = False
            AdjustState
        Else
            pbProgress.Visible = False
            lblStatus.Visible = True
            lblProcess.Visible = False
            imgStop.Visible = False
            imgStopDis.Visible = False
            
            RaiseEvent OnRefreshEmpty
        End If
    Else
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If

    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
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
    If m_bGroupRequest Then
        m_GroupPriceProvider.CancelGroup
    End If
    
    RaiseEvent OnRefreshError
End Sub

Private Function IndexLoad() As Boolean
    On Error Resume Next
    IndexLoad = False
    If m_bInProc Then Exit Function
    Dim aGIdx As EtsGeneralLib.IndexAtom, aComp As EtsGeneralLib.IndexCompAtom, aUnd As EtsMmRisksLib.MmRvUndAtom, sKey$, sKeyActiveFuture$
    Dim aReq As EtsMmRisksLib.MmRvReqAtom, aReqActiveFuture As EtsMmRisksLib.MmRvReqAtom, aPos As EtsMmRisksLib.MmRvPosAtom, nKey&
    Dim aUndIdx As UndAtom
    
    m_bInProc = True
    AdjustState
    
    If m_Idx.ID <> 0 Then
        For Each aUnd In m_Und
            aUnd.Beta = BAD_DOUBLE_VALUE
        Next
        
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogInformation, "Set active index " & m_Idx.Symbol, m_frmOwner.GetCaption
        
        sKey = CStr(enCtIndex) & "_" & m_Idx.Symbol
        Set aReq = m_QuoteRequests(sKey)
        
        Dim aAcFut As EtsMmRisksLib.MmRvFutAtom
        ' check for existing pp request for active future for previous future
        If Not m_Idx.ActiveFuture Is Nothing Then
            sKeyActiveFuture = CStr(enCtFuture) & "_" & m_Idx.ActiveFuture.Symbol
            Set aReqActiveFuture = m_QuoteRequests(sKeyActiveFuture)
            
            m_Idx.ActiveFuture.price.IsUseManualActive = False
            If (g_ContractAll(m_Idx.ID).Und.ActiveFuture.manualActivePrice > 0) Then
                m_Idx.ActiveFuture.price.IsUseManualActive = True
                m_Idx.ActiveFuture.price.Active = g_ContractAll(m_Idx.ID).Und.ActiveFuture.manualActivePrice
            End If
        End If
        
        If Not aReq Is Nothing Then
            If aReq.IndexOnly Then
                If Not PriceProvider Is Nothing Then
                    PriceProvider.CancelLastQuote aReq.GetQuoteUpdateParam
                    If Not aReqActiveFuture Is Nothing Then PriceProvider.CancelLastQuote aReqActiveFuture.GetQuoteUpdateParam
                End If
                Set aReq = Nothing
                
                m_QuoteRequests.Remove sKey
                
                If Not aReqActiveFuture Is Nothing Then
                    m_QuoteRequests.Remove sKeyActiveFuture
                End If
                
                Set aReqActiveFuture = Nothing
                
            End If
        End If
    
        m_Idx.Clear
    End If
    
    If m_Scn.CorrIndex <> 0 Then
        nKey = m_Scn.CorrIndex
        Set aGIdx = g_HedgeSymbols(nKey)
        Debug.Assert Not aGIdx Is Nothing
        If Not aGIdx Is Nothing Then
            m_Idx.ID = m_Scn.CorrIndex
            m_Idx.Symbol = aGIdx.Symbol
            m_Idx.ContractType = enCtIndex
            
            For Each aUnd In m_Und
                Set aComp = aGIdx.Components(aUnd.ID)
                
                If Not aComp Is Nothing Then
                    aUnd.Beta = aComp.Beta
                    Set aComp = Nothing
                End If
            Next
            
            sKey = CStr(enCtIndex) & "_" & m_Idx.Symbol
            Set aReq = m_QuoteRequests(sKey)
            If aReq Is Nothing Then
                Set aReq = m_QuoteRequests.Add(sKey)
                Set aReq.Und = m_Idx
                aReq.IndexOnly = True
                Set aReq = Nothing
            Else
                m_Idx.price.Bid = aReq.Und.price.Bid
                m_Idx.price.Ask = aReq.Und.price.Ask
                m_Idx.price.Last = aReq.Und.price.Last
                m_Idx.price.NetChange = aReq.Und.price.NetChange
            End If
            
            Set aUndIdx = g_ContractAll(m_Idx.ID).Und
            If (Not aUndIdx Is Nothing) Then
                m_Idx.price.Close = aUndIdx.PriceClose
                m_Idx.price.TheoClose = aUndIdx.PriceTheoclose
                If (aUndIdx.manualActivePrice > 0) Then
                    m_Idx.price.IsUseManualActive = True
                    m_Idx.price.Active = aUndIdx.manualActivePrice
                End If
                Set aUndIdx = Nothing
            End If
            
            Set m_Idx.UndPriceProfile = aGIdx.UndPriceProfile
            Set m_Idx.OptPriceProfile = aGIdx.OptPriceProfile
            
            ' try to get active future for selected index
            '******************************************************
            'Dim aActiveFuture As EtsMmRisksLib.MmRvFutAtom
'            Set m_Idx.ActiveFuture = Nothing    ' delete active future for previous active
'            If Not aReqActiveFuture Is Nothing Then
'                If Not aReqActiveFuture.Fut Is Nothing Then
'                    Set m_Idx.ActiveFuture = aReqActiveFuture.Fut
'                End If
'            End If
            If m_Idx.ActiveFuture Is Nothing Then
                If Not g_ContractAll(m_Idx.ID) Is Nothing Then
                    If Not g_ContractAll(m_Idx.ID).Und Is Nothing Then
                        If Not g_ContractAll(m_Idx.ID).Und.ActiveFuture Is Nothing Then
                            If m_Idx.ActiveFuture Is Nothing Then
                                sKey = CStr(enCtFuture) & "_" & g_ContractAll(m_Idx.ID).Und.ActiveFuture.Symbol 'm_Idx.ActiveFuture.Symbol
                                Set aReq = m_QuoteRequests(sKey)
                                If Not aReq Is Nothing Then
                                    Set m_Idx.ActiveFuture = aReq.Fut
                                Else
                                    Set m_Idx.ActiveFuture = New EtsMmRisksLib.MmRvFutAtom
                                    ' fill up active future info for newly selected index
                                    m_Idx.ActiveFuture.ID = g_ContractAll(m_Idx.ID).Und.ActiveFuture.ID
                                    Set m_Idx.ActiveFuture.UndPriceProfile = g_ContractAll(m_Idx.ID).Und.ActiveFuture.UndPriceProfile
                                    Set m_Idx.ActiveFuture.OptPriceProfile = g_ContractAll(m_Idx.ID).Und.ActiveFuture.OptPriceProfile
                                    m_Idx.ActiveFuture.price.Close = g_ContractAll(m_Idx.ID).Und.ActiveFuture.PriceClose
                                    m_Idx.ActiveFuture.price.TheoClose = g_ContractAll(m_Idx.ID).Und.ActiveFuture.PriceTheoclose
                                    
                                    m_Idx.ActiveFuture.price.IsUseManualActive = False
                                    If (g_ContractAll(m_Idx.ID).Und.ActiveFuture.manualActivePrice > 0) Then
                                        m_Idx.ActiveFuture.price.IsUseManualActive = True
                                        m_Idx.ActiveFuture.price.Active = g_ContractAll(m_Idx.ID).Und.ActiveFuture.manualActivePrice
                                    End If
                                    
                                    m_Idx.ActiveFuture.CalcGreeks = False
                                    m_Idx.ActiveFuture.bAsIs = g_ContractAll(m_Idx.ID).Und.ActiveFuture.FutureBasis
                                    m_Idx.ActiveFuture.Symbol = g_ContractAll(m_Idx.ID).Und.ActiveFuture.Symbol
                                    m_Idx.ActiveFuture.ContractName = g_ContractAll(m_Idx.ID).Und.ActiveFuture.ContractName
                                    m_Idx.ActiveFuture.IsAmerican = g_ContractAll(m_Idx.ID).Und.ActiveFuture.IsAmerican
                                    m_Idx.ActiveFuture.FutRootID = g_ContractAll(m_Idx.ID).Und.ActiveFuture.FutRootID
                                    m_Idx.ActiveFuture.Maturity = g_ContractAll(m_Idx.ID).Und.ActiveFuture.MaturityDate
                                    If Not g_ContractAll(m_Idx.ID).Und.FutRoots(m_Idx.ActiveFuture.FutRootID) Is Nothing Then
                                        m_Idx.ActiveFuture.FutRootSymbol = g_ContractAll(m_Idx.ID).Und.FutRoots(m_Idx.ActiveFuture.FutRootID).Symbol
                                        m_Idx.ActiveFuture.FutLotSize = g_ContractAll(m_Idx.ID).Und.FutRoots(m_Idx.ActiveFuture.FutRootID).FutLotSize
                                        m_Idx.ActiveFuture.UndID = g_ContractAll(m_Idx.ID).Und.FutRoots(m_Idx.ActiveFuture.FutRootID).UndID
                                        m_Idx.ActiveFuture.OptLotSize = g_ContractAll(m_Idx.ID).Und.FutRoots(m_Idx.ActiveFuture.FutRootID).OptLotSize
                                    End If
                                    If aReq Is Nothing Then
                                        Set aReq = m_QuoteRequests.Add(sKey)
                                        Set aReq.Fut = m_Idx.ActiveFuture
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
            m_Scn.CorrIndex = 0
            ScenarioUpdateIndex True
        End If
    End If
    
    IndexLoad = True
    m_bInProc = False
    AdjustState
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
        
        Dim nGridColWidth&, i&
        
        .Top = GRID_VERT_SPACE
        .Left = 0
        
        nGridColWidth = 0&
        For i = 0 To .Cols - 1
            If Not .ColHidden(i) Then nGridColWidth = nGridColWidth + .ColWidth(i) + .GridLineWidth '+ ScaleY(.GridLineWidth, vbPixels, vbTwips)
        Next
            
        If ScaleWidth < nGridColWidth Then
            .ScrollBars = flexScrollBarHorizontal
        Else
            .ScrollBars = flexScrollBarNone
        End If
        
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        If .ScrollBars = flexScrollBarHorizontal Then
            .Height = .Height + ScaleY(GetSystemMetrics(SM_CYHSCROLL), vbPixels, vbTwips)
        End If
        .Width = ScaleWidth
    End With

    With fgScn
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Left = 0
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
    End With

    With fgVal
        .Left = 0
        .Top = fgFlt.Height + fgScn.Height + GRID_VERT_SPACE * 2
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
    Set frmWtdVega = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        Set PriceProvider = Nothing
    End If
    Set m_GroupPriceProvider = Nothing
    
    m_Grp.Clear
    m_Und.Clear
    m_Exp.Clear
    m_QuoteRequests.Clear
    
    Set m_QuoteRequests = Nothing
    Set m_Idx = Nothing
    Set m_Grp = Nothing
    Set m_Und = Nothing
    Set m_Exp = Nothing
    Set m_Scn = Nothing

    Set m_View = Nothing
    Set m_aFilter = Nothing
    
    Set fgFlt.Font = Nothing
    Set fgScn.Font = Nothing
    Set fgVal.Font = Nothing
    
    Set m_gdFlt = Nothing
    Set m_gdScn = Nothing
    Set m_gdVal = Nothing
    
    Erase m_GridLock
    Erase m_Res
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set lblValue = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
    
    RaiseEvent OnScreenClose
    
    Erase m_Res
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRefresh.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow
    mnuCtxScnNew.Enabled = Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow
    mnuCtxScnSave.Enabled = mnuCtxScnNew.Enabled
    mnuCtxScnDelete.Enabled = mnuCtxScnNew.Enabled
    mnuCtxWtdVega.Enabled = Not m_bInProc And Not frmWtdVega.IsOpened And Not m_bLastQuoteReqNow
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    'mnuCtxOrderNewStock     "New Stock Order..."
    'mnuCtxOrderNewOption    "New Option Order..."
    'mnuCtxScnNew            "New Scenario"
    'mnuCtxScnSave           "Save Scenario"
    'mnuCtxScnDelete         "Delete Scenario"
    'mnuCtxRefresh           "Refresh"
    'mnuCtxCopy              "Copy Grid"
    'mnuCtxPrint             "Print Grid"
    'mnuCtxGridLayout        "Grid Layout..."
    
    mnuCtxCopy.Enabled = Not m_bInProc
    
    mnuCtxOrderNewStock.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewOption.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    PopupMenu mnuCtx
End Sub

Public Sub ScenarioNew()
    On Error Resume Next
    ScenarioSave False, True
    
    m_Scn.ID = 0
    m_Scn.ScenarioName = "Untitled"
    m_Scn.Description = ""
    m_Scn.Dirty = True
    
    InitScnList
    ScenarioUpdate True
End Sub

Public Function ScenarioSave(ByVal bUpdateGrid As Boolean, ByVal bShowError As Boolean) As Boolean
    On Error GoTo EH
    Dim nID&, aScn As clsRmScenarioAtom
    ScenarioSave = False
    Screen.MousePointer = vbHourglass
    
    With m_Scn
        'If m_Scn.Dirty Then
            nID = gDBW.usp_RMScenario_Save(IIf(.ID <> 0, .ID, Null), .ScenarioName, .Description, _
                                    .Points(RMA_HORZ), .Step(RMA_HORZ), .Units(RMA_HORZ), .Axis(RMA_HORZ), _
                                    .Points(RMA_VERT), .Step(RMA_VERT), .Units(RMA_VERT), .Axis(RMA_VERT), .VolaShiftType, _
                                    IIf(.Axis(RMA_HORZ) = RMAT_TIME, .Hour(RMA_HORZ), .Hour(RMA_VERT)), _
                                    IIf(.Axis(RMA_HORZ) = RMAT_TIME, .Minute(RMA_HORZ), .Minute(RMA_VERT)), CLng(.ShowChange), _
                                    .VolaSpecificShiftType, .SpotShiftType, .SpotSpecificShiftType, .CalcModel, _
                                    .CorrIndex, .SpecificParams)
                                    
            If .ID = 0 Then .ID = nID
            .Dirty = False
            
            LoadScenariosFromDB
            
            Set aScn = g_RmScenario(CStr(m_Scn.ID))
            
            m_Scn.CopyTo aScn
            
            Set aScn = Nothing
        'End If
        
        If bUpdateGrid Then
            InitScnList
            ScenarioUpdate True
        End If
        
        ScenarioSave = True
    End With
    
    Screen.MousePointer = vbDefault
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    If bShowError Then
        gCmn.ErrorMsgBox m_frmOwner, "Fail to save scenario."
    End If
    Set aScn = Nothing
End Function

Public Function ScenarioDelete() As Boolean
    On Error GoTo EH
    ScenarioDelete = False
    
    If m_Scn.ID <> 0 Then
        If gCmn.MyMsgBox(m_frmOwner, "Do you wish to delete scenario?", vbQuestion + vbYesNo) = vbYes Then
        
            gDBW.usp_RMScenario_Del m_Scn.ID
            If Not g_RmScenario(CStr(m_Scn.ID)) Is Nothing Then
                g_RmScenario.Remove CStr(m_Scn.ID)
            End If
            
        End If
    End If
    
    m_Scn.Clear
    
    If g_RmScenario.Count > 0 Then
        g_RmScenario(1).CopyTo m_Scn
    End If
    
    InitScnList
    ScenarioUpdate True
    
    ScenarioDelete = True
    
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to delete scenario."
End Function

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_aFilter.Data(MFC_SYMBOL) <> 0 Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then FilterUpdateValue False, MFC_SYMBOL
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    FilterUpdateValue False, MFC_GROUPS
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    FilterUpdateValue False, MFC_STRATEGY
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Risk Matrix", "Risk Matrix", fgFlt, fgScn, fgVal
'    Screen.MousePointer = vbDefault
End Sub

Public Sub ShowWeightedVegaSettings()
    On Error Resume Next
    frmWtdVega.Execute m_frmOwner
End Sub

Public Property Get WtdVegaIsOpened() As Boolean
    On Error Resume Next
    WtdVegaIsOpened = frmWtdVega.IsOpened
End Property

Private Sub InitVola(ByRef aUnd As EtsMmRisksLib.MmRvUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, IIf(aUnd.ContractType = enCtStock, enCtStock, enCtIndex), VolaSource
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
End Sub

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

Public Sub ImmediateRefresh()
    On Error Resume Next
    If m_bFirstTime Then m_Grp.ID = -1
    m_bFirstTime = False
End Sub

Public Function ExportToHTML(ByVal sFileName As String, _
                             ByVal sFilePath As String, _
                             ByVal bShowFilter As Boolean) As Boolean
                            '-----------------------------------------'
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportToHTML = g_ScreenExport.SaveToHTML(sFileName, _
                                             sFilePath, _
                                             fgVal, _
                                             IIf(bShowFilter, fgScn, Nothing), _
                                             IIf(bShowFilter, fgFlt, Nothing))
    Screen.MousePointer = vbNormal
End Function

Public Function ExportToCSV(ByVal sFileName As String, _
                            ByVal sFilePath As String, _
                            ByVal bShowFilter As Boolean) As Boolean
                            '----------------------------------------'
On Error Resume Next

    Screen.MousePointer = vbHourglass
    ExportToCSV = g_ScreenExport.SaveToCSV(sFileName, _
                                            sFilePath, _
                                            fgVal, _
                                            IIf(bShowFilter, fgScn, Nothing), _
                                            IIf(bShowFilter, fgFlt, Nothing))
    Screen.MousePointer = vbNormal
End Function

Public Sub LoadGroupSpecificRatio()
On Error GoTo exception_handler

    Dim sFileName As String
    
    If (Not g_Main Is Nothing) Then
        If (Not g_Main.UnderlyingGroup Is Nothing) Then
            
            sFileName = m_frmOwner.GroupSpecificRatioFileDialog
            If (Len(Trim(sFileName)) > 0) Then
                If (IsFileExist(sFileName)) Then
                    
                    If (Not m_Scn Is Nothing) Then
                        m_Scn.SpecificParams = sFileName
                        ReloadGroupRatio
                    End If
                    
                Else
                    MsgBox "File: " & sFileName & " not exist."
                End If
            End If
            
        End If
    End If
    
Exit Sub
exception_handler:
    gCmn.ErrorHandler "Fail to Load Group Specific Ratio. (" & Err.Description & ":" & CStr(Err.Number) & ")"
End Sub

Private Sub ReloadGroupRatio()
On Error GoTo exception_handler

    Dim sFileName As String
    
    If (Not g_Main Is Nothing) Then
        If (Not g_Main.UnderlyingGroup Is Nothing) Then
            If (Not m_Scn Is Nothing) Then
                sFileName = m_Scn.SpecificParams
                If (Len(Trim(sFileName)) > 0) Then
                    If (IsFileExist(sFileName)) Then
                        g_Main.UnderlyingGroup.LoadProperty sFileName
                        g_Params.GroupSpecificRatioFile = sFileName
                    End If
                Else
                    sFileName = g_Params.GroupSpecificRatioFile
                    If (Len(Trim(sFileName)) > 0) Then
                        If (IsFileExist(sFileName)) Then
                            g_Main.UnderlyingGroup.LoadProperty sFileName
                            g_Params.GroupSpecificRatioFile = sFileName
                        End If
                    End If
                End If
            End If
        End If
    End If
    
Exit Sub
exception_handler:
    MsgBox "ReloadGroupRatio failed Error: (" & Err.Description & ":" & CStr(Err.Number) & ")"
End Sub

