VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{3B008041-905A-11D1-B4AE-444553540000}#1.0#0"; "Vsocx6.ocx"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Begin VB.UserControl ctlVolaAnalysis 
   ClientHeight    =   7380
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   10995
   KeyPreview      =   -1  'True
   ScaleHeight     =   7380
   ScaleWidth      =   10995
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   1920
      Top             =   5880
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
      Height          =   2055
      Left            =   0
      TabIndex        =   1
      Top             =   720
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
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   5400
      Top             =   4020
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin vsOcx6LibCtl.vsAwk vsAwk 
      Left            =   6060
      Top             =   4020
      _ExtentX        =   688
      _ExtentY        =   688
      Version         =   600
      FileName        =   ""
      FS              =   " ,	"
      Case            =   0
      MatchQuotes     =   0   'False
      FileType        =   0
      FilterQuotes    =   0   'False
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxPortfolioCreate 
         Caption         =   "New Portfolio..."
      End
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
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
      Begin VB.Menu mnuCtxReload 
         Caption         =   "Reload Data"
         Shortcut        =   +{F5}
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
   End
End
Attribute VB_Name = "ctlVolaAnalysis"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const CALC_ERR_TEXT$ = "#ERR!"
Private Const CALC_ERR_VALUE# = 1E+308

Public Event OnSetCaption()
Public Event OnStateChange()

Private m_gdFlt As New clsGridDef
Private m_gdUnd As New clsGridDef

Private WithEvents geUnd As clsGridEx
Attribute geUnd.VB_VarHelpID = -1

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private VolaSource As VolatilitySourcesLib.VolatilitySource

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label
Public WithEvents imgStop As vB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As vB.Image

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bLastQuoteReqNow As Boolean

Private m_dVgaAmt As Double
Private m_sSymbolsFileName As String
Private m_sSymbolsFilePath As String
Private m_sFilePath As String

Private m_UndRequests As New clsVaRequestColl
Private m_OptRequests As New clsVaRequestColl
Private m_Grp As New EtsGeneralLib.EtsMmEntityAtom
Private m_Und As New EtsMmVolaAnalysisLib.MmVaUndColl
Private m_Exp As New EtsGeneralLib.EtsMmEntityAtomColl
Private m_UndLoadedValues As New EtsGeneralLib.EtsMmEntityAtomColl

Private m_nLastQuoteReqCountUnd As Long
Private m_nLastQuoteReqDoneUnd As Long

Public m_frmOwner As Form
Private m_nFilter(AFC_GROUP To AFC_COLUMN_COUNT) As Long

Private m_nOpenedExpiry As Long
Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_VOLAANALYSIS_FILTER To GT_VOLAANALYSIS_UNDERLYING) As Boolean
Private m_GridLock(GT_VOLAANALYSIS_FILTER To GT_VOLAANALYSIS_UNDERLYING) As New clsGridLock

Private m_bShutDown As Boolean

Private Const MIN_EXPIRY_DAYS As Long = 20&
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1
Public Property Get Filter(ByVal nFilter As Long) As Long
    Filter = m_nFilter(nFilter)
End Property

Public Function Init() As Boolean
    On Error GoTo EH

    m_bShutDown = False
    m_sFilePath = MakeCommonFilePath("", "", False)

    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"

    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_VOLAANALYSIS_FILTER) = False
    m_bKeyDown(GT_VOLAANALYSIS_UNDERLYING) = False

    m_GridLock(GT_VOLAANALYSIS_FILTER).Init fgFlt
    m_GridLock(GT_VOLAANALYSIS_UNDERLYING).Init fgUnd

    InitColumns
    InitGrids

    Set geUnd = New clsGridEx
    geUnd.Init fgUnd

    FormatFltGrid
    FormatFltColumns

    FormatUndGrid
    FormatUndColumns

    InitFltData

    Set VolaSource = g_VolaSource

    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo

    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing

    PriceProvider.Connect

    fgFlt.Col = AFC_GROUP
    fgFlt.Row = 1

    Set frmLayout = New frmGridLayout
    Set aParams = g_Params
    Init = True

    AdjustCaption
    AdjustState
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize volatility analysis."
End Function
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    m_nFilter(AFC_GROUP) = TYPE_UNDERLYING
    'm_nFilter(AFC_VALUE) = aFilters.Data(MFC_SYMBOL)
    'm_nFilter(AFC_EXPIRY) = aFilters.Data(MFC_EXPIRY)
    FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    m_nFilter(AFC_GROUP) = nType
    m_nFilter(AFC_VALUE) = nValue
    FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdateAll()
    On Error Resume Next

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        If m_nFilter(AFC_GROUP) <> TYPE_UNDERLYING _
            And m_nFilter(AFC_GROUP) <> TYPE_TRADER _
            And m_nFilter(AFC_GROUP) <> TYPE_GROUP _
            And m_nFilter(AFC_GROUP) <> TYPE_FILE Then
            
            m_nFilter(AFC_GROUP) = TYPE_FILE
            m_nFilter(AFC_VALUE) = 0
        End If
        .TextMatrix(1, AFC_GROUP) = m_nFilter(AFC_GROUP)

        FilterUpdateValue False
        FilterUpdateExpiry False
        FilterUpdateModel False
        FilterUpdateVegaAmt False

        .AutoSize 0, .Cols - 1, , 100

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateValue(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom
    Dim nValue&

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        sValue = "0"
        nValue = 0
        sComboList = ""

        Select Case m_nFilter(AFC_GROUP)
            Case TYPE_TRADER
                Dim aTrader As EtsGeneralLib.EtsTraderAtom
                
                For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = m_nFilter(AFC_VALUE) Then
                        nValue = aTrader.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
            
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
                
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(AFC_VALUE) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
                
            Case TYPE_UNDERLYING
                'sComboList = g_Params.UnderlyingComboList
                sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
                
                Set aUnd = g_Underlying(m_nFilter(AFC_VALUE))
                If Not aUnd Is Nothing Then
                    nValue = aUnd.ID
                    sValue = Trim$(Str$(nValue))
                    Set aUnd = Nothing
                End If
        
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
        
            Case TYPE_FILE
                sComboList = "..."
                If Len(m_sSymbolsFileName) > 0 Then
                    nValue = 1
                    sValue = m_sSymbolsFileName
                Else
                    nValue = 0
                    sValue = "<Browse>"
                End If
                
        End Select

        If m_nFilter(AFC_GROUP) <> TYPE_FILE Then
            If Len(sComboList) > 0 Then
                If nValue = 0 Then
                    sComboList = "#0;<Select>" & sComboList
                End If
            Else
                sComboList = "#0;<None>"
            End If
        End If

        .ColComboList(AFC_VALUE) = sComboList
        .TextMatrix(1, AFC_VALUE) = sValue
        m_nFilter(AFC_VALUE) = nValue

        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateExpiry(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim nValue&, sValue$
    Dim aExp As EtsGeneralLib.EtsMmEntityAtom, sComboList$, nCount&

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        sValue = "0"
        nValue = 0
        nCount = m_Exp.Count
        If nCount > 0 Then
            For Each aExp In m_Exp
                sComboList = sComboList & "|#" & Trim$(Str$(aExp.ID)) & ";" & aExp.Name
                If aExp.ID = m_nFilter(AFC_EXPIRY) Or nValue = 0 Then
                    sValue = aExp.Name
                    nValue = aExp.ID
                End If
            Next

            sComboList = Mid$(sComboList, InStr(1, sComboList, "|") + 1)
            If nValue = 0 Then
                nValue = m_Exp(1).ID
                sValue = Trim$(Str$(nValue))
            End If
        Else
            sComboList = "#0;<None>"
        End If

        .ColComboList(AFC_EXPIRY) = sComboList
        .TextMatrix(1, AFC_EXPIRY) = sValue
        m_nFilter(AFC_EXPIRY) = nValue

        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        .TextMatrix(1, AFC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)

        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateVegaAmt(ByVal bAutosize As Boolean)
    On Error Resume Next

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        .TextMatrix(1, AFC_VEGA_AMT) = m_dVgaAmt

        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        .ColComboList(AFC_GROUP) = "#" & CStr(TYPE_FILE) & ";File" & _
                                    "|#" & CStr(TYPE_UNDERLYING) & ";Underlying" & _
                                    "|#" & CStr(TYPE_GROUP) & ";Group" & _
                                    "|#" & CStr(TYPE_TRADER) & ";Trader"

        .TextMatrix(1, AFC_GROUP) = CStr(TYPE_FILE)
        m_nFilter(AFC_GROUP) = TYPE_FILE

        .ColComboList(AFC_VALUE) = "..."
        .TextMatrix(1, AFC_VALUE) = "<Browse>"
        m_nFilter(AFC_VALUE) = 0

        m_nFilter(AFC_TODAY) = Date
        .TextMatrix(1, AFC_TODAY) = Date

        .ColComboList(AFC_EXPIRY) = "#0;<None>"
        .TextMatrix(1, AFC_EXPIRY) = "0"
        m_nFilter(AFC_EXPIRY) = 0

        m_dVgaAmt = 100#

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = AFC_COLUMN_COUNT

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
        .Cols = 10

        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortAndMove
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .OutlineCol = 1
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
    End With
End Sub

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_VOLAANALYSIS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_VOLAANALYSIS_UNDERLYING).CopyTo m_gdUnd
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        nCols = AFC_COLUMN_COUNT
        .Cols = nCols

        For i = AFC_GROUP To AFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next

        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont

    With fgFlt
        m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw

        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline

        Set .Font = aFont

        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = AFC_GROUP To AFC_LAST_COLUMN
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

        m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&

    With fgUnd
        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
        
        m_gdUnd.Idx(0) = AUC_ROW
        m_gdUnd.Idx(1) = AUC_SYMBOL
        m_gdUnd.Idx(2) = AUC_BUY
        m_gdUnd.Idx(3) = AUC_SELL

        nCols = 1
        For i = 1 To AUC_LAST_COLUMN
            If m_gdUnd.Idx(i) <> AUC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next

        nCols = nCols + 1 ' for key column
        .Cols = nCols

        .ColHidden(AUC_KEY) = True
        .ColKey(AUC_KEY) = AUC_NONE
        .ColDataType(AUC_KEY) = flexDTString

        For i = 0 To AUC_LAST_COLUMN
            nIdx = m_gdUnd.Idx(i)
            If nIdx <> AUC_NONE Then
                .TextMatrix(0, i + 1) = m_gdUnd.Col(nIdx).Caption
                .ColFormat(i + 1) = m_gdUnd.Col(nIdx).Format
                .ColDataType(i + 1) = m_gdUnd.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                If nIdx <> AUC_ROW Then
                    .ColWidth(i + 1) = IIf(m_gdUnd.Col(nIdx).Width > 0, m_gdUnd.Col(nIdx).Width, -1)
                    
                    If nIdx = AUC_CALC_1 Or nIdx = AUC_CALC_2 Then
                        .ColComboList(i + 1) = "..."
                    Else
                        .ColComboList(i + 1) = ""
                    End If
                End If
            Else
                Exit For
            End If
        Next

        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        .AutoSize 1, 1, False, 0

        geUnd.AdjustSortImages

        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub FormatUndColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgUnd
        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw

        aFont.Name = m_gdUnd.Font.Name
        aFont.Size = m_gdUnd.Font.Size
        aFont.Bold = m_gdUnd.Font.Bold
        aFont.Italic = m_gdUnd.Font.Italic
        aFont.Strikethrough = m_gdUnd.Font.Strikethrough
        aFont.Underline = m_gdUnd.Font.Underline

        Set .Font = aFont

        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> AUC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).BackColor <> 0, m_gdUnd.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdUnd.Col(nIdx).ForeColor <> 0, m_gdUnd.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next

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
        .AutoSize 1, 1, False, 0

'        .AutoSize 1, .Cols - 1

        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
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

Private Sub ShowPopup()
    On Error Resume Next
    Dim bEnabled As Boolean, nIdx&
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    'mnuCtxPortfolioCreate "Create Portfolio"
    'mnuCtxTradeNew        "New Trade..."
    'mnuCtxOrderNewStock   "New Stock Order..."
    'mnuCtxOrderNewOption  "New Option Order..."
    'mnuCtxRefresh         "Refresh"
    'mnuCtxReload          "Reload Data"
    'mnuCtxCopy            "Copy Grid"
    'mnuCtxPrint           "Print Grid..."
    'mnuCtxHideCol         "Hide Column"
    'mnuCtxAutosizeCol     "Autosize Column"
    'mnuCtxAutosizeGrid    "Autosize Grid"
    'mnuCtxGridLayout      "Grid Layout..."

    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxPortfolioCreate.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bLastQuoteReqNow And m_Und.Count > 0
    
    mnuCtxOrderNewStock.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewOption.Visible = g_Params.OrdersVisible
    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.OrdersEnabled
    
'    mnuCtxOrderNewStock.Enabled = Not m_bInProc And g_Params.EnableOrders
'    mnuCtxOrderNewOption.Enabled = Not m_bInProc And g_Params.EnableOrders
    
    
    mnuCtxTntCardNew.Enabled = Not m_bInProc

    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nFilter(AFC_GROUP) = TYPE_UNDERLYING And m_nFilter(AFC_VALUE) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)

            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False

            PopupMenu mnuCtx

        Case GT_VOLAANALYSIS_UNDERLYING
            mnuCtxTradeNew.Enabled = (Not m_bInProc _
                                        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                        And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows)
            
            bEnabled = False
            nIdx = -1
            nIdx = fgUnd.ColKey(m_nMenuGridCol)
            
            bEnabled = m_gdUnd.Col(nIdx).CanChangeVisible
            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols And m_gdUnd.Idx(1) <> AUC_NONE And bEnabled)
            
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols And nIdx <> AUC_ROW)
            mnuCtxAutosizeGrid.Enabled = True

            PopupMenu mnuCtx

    End Select
End Sub

Private Sub AdjustState()
    On Error Resume Next
    UpdateMenu
    RaiseEvent OnStateChange
End Sub

Private Sub UpdateMenu()
    On Error Resume Next
    mnuCtxRefresh.Enabled = m_Grp.ID <> 0 And Not m_bInProc And Not m_bLastQuoteReqNow
    mnuCtxReload.Enabled = mnuCtxRefresh.Enabled
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
    sCaption = sCaption & "Volatility Analysis"

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

Private Sub ClearViewAndData()
    On Error Resume Next
    fgUnd.Rows = 1

    m_Grp.ID = 0
    m_Grp.Data = 0
    m_Grp.Name = ""

    m_Und.Clear
    m_Exp.Clear
    m_UndRequests.Clear
    m_OptRequests.Clear

    FilterUpdateExpiry True
    SetRefreshHint False
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    
    If m_Grp.Data = 0 Or m_Grp.ID = 0 Then Exit Sub
    
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

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_VOLAANALYSIS_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error GoTo Err
    Dim nKey&, sValue$, dValue#, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, bCalcGreeks As Boolean
    Dim nCol&, nValue&, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom

    With fgUnd
        sValue = Trim$(.TextMatrix(Row, Col))
        nKey = .ColKey(Col)
        If Err.Number = 0 Then
            bCalcGreeks = False
            Set aUnd = .RowData(Row).Und

            If Not aUnd Is Nothing Then
                If m_sCurrentOriginalText <> sValue Then
                    Select Case nKey
                        Case AUC_BUY
                            If .Cell(flexcpChecked, Row, Col) = flexChecked Then
                                aUnd.NewTradeType = enMmVaNewTradeBuy
                            Else
                                aUnd.NewTradeType = enMmVaNewTradeNone
                            End If
                            
                            .TextMatrix(Row, Col) = (aUnd.NewTradeType = enMmVaNewTradeBuy)
                            nCol = .ColIndex(AUC_SELL)
                            If nCol >= 0 Then .TextMatrix(Row, nCol) = (aUnd.NewTradeType = enMmVaNewTradeSell)
                            
                        Case AUC_SELL
                            If .Cell(flexcpChecked, Row, Col) = flexChecked Then
                                aUnd.NewTradeType = enMmVaNewTradeSell
                            Else
                                aUnd.NewTradeType = enMmVaNewTradeNone
                            End If
                            
                            .TextMatrix(Row, Col) = (aUnd.NewTradeType = enMmVaNewTradeSell)
                            nCol = .ColIndex(AUC_BUY)
                            If nCol >= 0 Then .TextMatrix(Row, nCol) = (aUnd.NewTradeType = enMmVaNewTradeBuy)
                        
'                        Case AUC_UND_LAST
'                            dValue = Abs(ReadDbl(sValue))
'                            If aUnd.PriceLast <> dValue Then
'                                aUnd.PriceLast = dValue
'                                bCalcGreeks = True
'                            End If

                        Case AUC_LOAD_1
                            dValue = ReadDbl(sValue)
                            If aUnd.LoadValue1 <> dValue Then
                                aUnd.LoadValue1 = dValue
                                UnderlyingCalcCustomValues aUnd
                                UnderlyingUpdate Row, False, aUnd
                            End If
                            
                        Case AUC_LOAD_2
                            dValue = ReadDbl(sValue)
                            If aUnd.LoadValue2 <> dValue Then
                                aUnd.LoadValue2 = dValue
                                UnderlyingCalcCustomValues aUnd
                                UnderlyingUpdate Row, False, aUnd
                            End If
                            
                        Case AUC_OPT_ROOT
                            nValue = .ComboData
                            Set aStr = aUnd.CurExpiry.AtmStrike
                            If Not aStr Is Nothing Then
                                Debug.Assert Not aStr.OptPair(nValue) Is Nothing
                                
                                Set aStr.CurOptPair = aStr.OptPair(nValue)
                                Set aStr = Nothing
                                
                                UnderlyingCalcCustomValues aUnd
                                UnderlyingUpdate Row, False, aUnd
                            End If

                    End Select

                    If bCalcGreeks Then
                        m_bInProc = True
                        UnderlyingCalc aUnd
                        m_bInProc = False
                        
                        UnderlyingUpdate Row, False, aUnd
                        
                        AdjustState
                    End If
                    
                    Set aUnd = Nothing
                Else
                    Select Case nKey
                        Case AUC_LOAD_1, AUC_LOAD_2
                            UnderlyingUpdate Row, False, aUnd
                    End Select
                End If
            End If
        End If
    End With
Exit Sub
Err:
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogFaults, "Und Filter AfterEdit Error " & CStr(Err.Number) & " " & Err.Description, m_frmOwner.GetCaption
End Sub

Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&

    If Col >= 0 And Row < 0 Then
        With fgUnd
            nIdx = .ColKey(Col)
            If nIdx >= AUC_SYMBOL And nIdx <= AUC_LAST_COLUMN Then
                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgUnd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nKey&, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom, sComboList$
    
    With fgUnd
        nKey = .ColKey(Col)
        sComboList = ""
        
        If nKey = AUC_OPT_ROOT Then
            Set aStr = .RowData(Row).Und.CurExpiry.AtmStrike
            If Not aStr Is Nothing Then
                If aStr.OptPair.Count > 1 Then sComboList = aStr.RootNames
                Set aStr = Nothing
            End If
        End If
        
        .ComboList = sComboList
    End With
End Sub

Private Sub fgUnd_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = (fgUnd.ColKey(Col) = AUC_ROW)
End Sub

Private Sub fgUnd_CellButtonClick(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nKey&, sValue$, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
    Dim frmExpression As frmExpressionEdit, sPrompt$, bApplyToAll As Boolean
    
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_Grp.ID = 0 Then Exit Sub
    
    With fgUnd
        nKey = .ColKey(Col)
        If Err.Number = 0 Then
            Set aUnd = .RowData(Row).Und

            If Not aUnd Is Nothing Then
                If nKey = AUC_CALC_1 Or nKey = AUC_CALC_2 Then
                    Select Case nKey
                        Case AUC_CALC_1
                            sValue = aUnd.CalcExpression1
                            sPrompt = "Enter 1st Expression for '" & aUnd.Symbol & "':"
                    
                        Case AUC_CALC_2
                            sValue = aUnd.CalcExpression2
                            sPrompt = "Enter 2nd Expression for '" & aUnd.Symbol & "':"
                    End Select
                    bApplyToAll = False
                    
                    Set frmExpression = New frmExpressionEdit
                    
                    If frmExpression.Execute("Expression Editor", sPrompt, "&Apply to All Underlyings", sValue, bApplyToAll, m_frmOwner) = vbOK Then
                        Select Case nKey
                            Case AUC_CALC_1
                                If Not bApplyToAll Then
                                    If sValue <> aUnd.CalcExpression1 Then
                                        aUnd.CalcExpression1 = sValue
                                        UnderlyingCalcCustomValues aUnd
                                        UnderlyingUpdate Row, False, aUnd
                                    End If
                                Else
                                    For Each aUnd In m_Und
                                        If sValue <> aUnd.CalcExpression1 Then
                                            aUnd.CalcExpression1 = sValue
                                            UnderlyingCalcCustomValues aUnd
                                        End If
                                    Next
                                    UnderlyingsUpdate False
                                End If
        
                            Case AUC_CALC_2
                                If Not bApplyToAll Then
                                    If sValue <> aUnd.CalcExpression2 Then
                                        aUnd.CalcExpression2 = sValue
                                        UnderlyingCalcCustomValues aUnd
                                        UnderlyingUpdate Row, False, aUnd
                                    End If
                                Else
                                    For Each aUnd In m_Und
                                        If sValue <> aUnd.CalcExpression2 Then
                                            aUnd.CalcExpression2 = sValue
                                            UnderlyingCalcCustomValues aUnd
                                        End If
                                    Next
                                    UnderlyingsUpdate False
                                End If
                        End Select
                    End If
                        
                    Set frmExpression = Nothing
                End If
                
                Set aUnd = Nothing
            End If
        End If
    End With
End Sub

Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nKey&, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
    Cancel = True

    If IsDblClickHandled Then Exit Sub
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Or m_Grp.ID = 0 Then Exit Sub

    With fgUnd
        nKey = .ColKey(Col)
        If Err.Number = 0 Then
            Cancel = Not m_gdUnd.Col(nKey).CanEdit
            If Not Cancel Then
                Set aUnd = .RowData(Row).Und
                Cancel = aUnd Is Nothing
                
                If Not Cancel Then
                    Select Case nKey
                        Case AUC_OPT_ROOT
                            Set aStr = aUnd.CurExpiry.AtmStrike
                            If Not aStr Is Nothing Then
                                Cancel = aStr.OptPair.Count <= 1 Or aStr.CurOptPair Is Nothing
                                Set aStr = Nothing
                            Else
                                Cancel = True
                            End If
                    
                        Case AUC_LOAD_1
                            If aUnd.LoadValue1 <= BAD_DOUBLE_VALUE Then .TextMatrix(Row, Col) = ""
                            
                        Case AUC_LOAD_2
                            If aUnd.LoadValue2 <= BAD_DOUBLE_VALUE Then .TextMatrix(Row, Col) = ""
                            
                    End Select
                End If
                Set aUnd = Nothing
            End If
        End If

        If Not Cancel Then
            m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
        End If
    End With
End Sub

Private Sub geUnd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim nCol&, nRow&
    
    With fgUnd
        nCol = .ColIndex(AUC_ROW)
        If nCol > -1 Then
            m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
    
            For nRow = 1 To .Rows - 1
                .TextMatrix(nRow, nCol) = nRow
            Next
    
            m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
        End If
    End With
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False

        UnderlyingsUpdate False
        AdjustState

    ElseIf m_bDataLoad Then
        m_bDataLoad = False
        AdjustState
    End If
End Sub

Private Sub fgFlt_CellButtonClick(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    
    If Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow And m_nFilter(AFC_GROUP) = TYPE_FILE Then
        If Col = AFC_VALUE Then
            With dlgCommon
                .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist Or cdlOFNFileMustExist
                .CancelError = True
                .DefaultExt = ".csv"
                .DialogTitle = "Open Underlyings List"
                .FileName = ""
                .InitDir = m_sFilePath
                .Filter = "CSV (Comma delimited)(*.csv)|*.csv|Text files (*.txt)|*.txt|All Files|*.*"
                .FilterIndex = 1
                .ShowOpen
                If Err.Number = 0 Then
                    m_sSymbolsFilePath = .FileName
                    m_sFilePath = ExtractFilePath(m_sSymbolsFilePath)
                    m_sSymbolsFileName = ExtractFileName(m_sSymbolsFilePath)
                    
                    fgFlt.ColComboList(Col) = "..."
                    fgFlt.TextMatrix(Row, Col) = m_sSymbolsFileName
                    m_nFilter(AFC_VALUE) = 1
                    
                    tmrShow.Enabled = True
                End If
            End With
        End If
    End If
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&, dValue#

    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            If Col <> AFC_VEGA_AMT Then
                nValue = ReadLng(sValue)
                Select Case Col
                    Case AFC_GROUP
                        m_nFilter(AFC_VALUE) = 0
                        m_nFilter(AFC_GROUP) = nValue
                        ClearViewAndData
                        m_sSymbolsFileName = ""
                        m_sSymbolsFilePath = ""
                        FilterUpdateValue True
                        AdjustCaption
                        AdjustState

                    Case AFC_VALUE
                        If m_nFilter(AFC_VALUE) = 0 Then
                            sValue = .ColComboList(Col)
                            sValue = Mid$(sValue, InStr(1, sValue, "|") + 1)
                            .ColComboList(Col) = sValue
                            .TextMatrix(Row, Col) = CStr(nValue)
                        End If
                        m_nFilter(AFC_VALUE) = nValue

                        tmrShow.Enabled = True

                    Case AFC_EXPIRY
                        m_nFilter(Col) = nValue
                        .AutoSize 0, .Cols - 1, , 100
                        m_bInProc = True
                        ExpiryOptionsLoad
                        UnderlyingsUpdate False
                        m_bInProc = False
                        
                        RequestLastUndQuotes
                
                End Select
            Else
                dValue = ReadDbl(sValue)
                If dValue > 0# And dValue <> m_dVgaAmt Then
                    m_dVgaAmt = dValue
                    
                    m_bInProc = True
                    UnderlyingsCalc False
                    m_bInProc = False
                    
                    UnderlyingsUpdate False
                Else
                    .TextMatrix(1, AFC_VEGA_AMT) = m_dVgaAmt
                End If
                .AutoSize 0, .Cols - 1, , 100
            End If
        End If
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_VOLAANALYSIS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_VOLAANALYSIS_FILTER) Then
        m_bKeyDown(GT_VOLAANALYSIS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_VOLAANALYSIS_FILTER
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

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_VOLAANALYSIS_FILTER
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
        If Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= AFC_GROUP And Col <= AFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgUnd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub

    nOldPos = Col - AUC_ROW
    nNewPos = Position - AUC_ROW
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

Private Sub fgUnd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nOldIdx&, nNewIdx&

    nOldIdx = fgUnd.ColKey(Col)
    nNewIdx = fgUnd.ColKey(Position)

    If nOldIdx = AUC_NONE Or nNewIdx = AUC_NONE Then
        Position = Col
    ElseIf Not m_gdUnd.Col(nOldIdx).CanChangeOrder Or Not m_gdUnd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgUnd_DblClick()
    On Error Resume Next
    With fgUnd
        m_enMenuGrid = GT_VOLAANALYSIS_UNDERLYING
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgUnd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_VOLAANALYSIS_UNDERLYING) = True
    End If
End Sub

Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_VOLAANALYSIS_UNDERLYING) Then
        m_bKeyDown(GT_VOLAANALYSIS_UNDERLYING) = False
        With fgUnd
            m_enMenuGrid = GT_VOLAANALYSIS_UNDERLYING
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
        
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                mnuCtxAutosizeGrid_Click
    
            Case KeyCode = vbKeyA And Shift = vbCtrlMask And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols
                mnuCtxAutosizeCol_Click
        End Select
    End If
End Sub

Private Sub fgUnd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgUnd
            m_enMenuGrid = GT_VOLAANALYSIS_UNDERLYING
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows

'            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
'                .Row = m_nMenuGridRow
'            End If

            ShowPopup
        End With
    End If
End Sub

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    Dim nIdx&

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_UNDERLYING
            With fgUnd
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= AUC_SYMBOL And nIdx <= AUC_LAST_COLUMN Then
                    m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
                    
                    .AutoSize m_nMenuGridCol, m_nMenuGridCol
                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                    
                    m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
                End If
            End With

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    Dim nIdx&, i&, nCount&

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_UNDERLYING
            With fgUnd
                m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
                .AutoSize 0, .Cols - 1

                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= AUC_SYMBOL And nIdx <= AUC_LAST_COLUMN Then
                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next

                m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
            End With

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_VOLAANALYSIS_UNDERLYING
            g_ClipMgr.CopyGridToClipboard fgUnd

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_FILTER
            CustomizeFltGridLayout

        Case GT_VOLAANALYSIS_UNDERLYING
            CustomizeUndGridLayout

    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    Dim i&, nColIdx&, bMove As Boolean

    Select Case m_enMenuGrid
        Case GT_VOLAANALYSIS_UNDERLYING
            bMove = False

            nColIdx = fgUnd.ColKey(m_nMenuGridCol)
            bMove = m_gdUnd.Col(nColIdx).CanChangeVisible
            If bMove Then
                bMove = False
                For i = 1 To AUC_LAST_COLUMN
                    If m_gdUnd.Idx(i) = nColIdx Then
                        m_gdUnd.Idx(i) = AUC_NONE
                        m_gdUnd.Col(nColIdx).Visible = False
                        bMove = True
                    End If
    
                    If bMove Then
                        If i + 1 <= AUC_LAST_COLUMN Then
                            m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
                        Else
                            m_gdUnd.Idx(i) = AUC_NONE
                        End If
                    End If
                Next
    
                If bMove Then
                    FormatUndGrid
                    UnderlyingsUpdate False
                    FormatUndColumns
                End If
            End If

    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, ByRef dPrice#)
    On Error Resume Next
    If m_enMenuGrid = GT_VOLAANALYSIS_UNDERLYING Then _
        Set aUnd = g_Underlying(fgUnd.RowData(m_nMenuGridRow).ID)

    If aUnd Is Nothing And m_nFilter(AFC_GROUP) = TYPE_UNDERLYING Then _
        Set aUnd = g_Underlying(m_nFilter(AFC_VALUE))

    If bIsStock And Not aUnd Is Nothing Then
        dPrice = m_Und(aUnd.ID).PriceBid
        If dPrice <= 0# Then dPrice = m_Und(CStr(aUnd.ID)).PriceLast
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
    gCmn.ErrorMsgBox m_frmOwner, "Volatility Analysis View: Fail to create new order."
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
'    gCmn.ErrorMsgBox m_frmOwner, "Volatility Analisys View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error GoTo EH
    If m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom

    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True

    If m_enMenuGrid = GT_VOLAANALYSIS_UNDERLYING Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            On Error Resume Next
            Set aUnd = fgUnd.RowData(m_nMenuGridRow).Und
            On Error GoTo EH
        End If
        If Not aUnd Is Nothing Then
            bBuy = (g_Params.QuoteOtherColBuy <> 0)
            nUndID = aUnd.ID
            nID = aUnd.ID
            Set aUnd = Nothing
        Else
            If m_nFilter(AFC_GROUP) = TYPE_UNDERLYING Then
                nUndID = m_nFilter(AFC_VALUE)
                nID = nUndID
                bBuy = (g_Params.QuoteOtherColBuy <> 0)
            End If
        End If

    ElseIf m_enMenuGrid = GT_VOLAANALYSIS_FILTER Then
        If m_nFilter(AFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_nFilter(AFC_VALUE)
            nID = nUndID
            bBuy = (g_Params.QuoteOtherColBuy <> 0)
        End If
    End If

    m_bInProc = False
    If nUndID <> 0 Then frmTradeEditor.NewTrade nID, nUndID, bBuy, m_frmOwner

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPortfolioCreate_Click()
    On Error Resume Next
    PortfolioCreate
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_VOLAANALYSIS_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Analysis Filter", "Volatility Analysis Filter", fgFlt
'
'        Case GT_VOLAANALYSIS_UNDERLYING
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Analysis Underlyings", "Volatility Analysis Underlyings", fgUnd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub mnuCtxReload_Click()
    On Error Resume Next
    Reload
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
    End If

    ClearViewAndData
    AdjustCaption

    If m_nFilter(AFC_GROUP) <> 0 And m_nFilter(AFC_VALUE) <> 0 Then
        Dim bSucceed As Boolean
        bSucceed = False

        If UnderlyingsLoad Then
            AdjustCaption

            m_nFilter(AFC_EXPIRY) = m_nOpenedExpiry
            FilterUpdateExpiry False

            If ExpiryOptionsLoad Then
                UnderlyingsShow
                
                RequestLastUndQuotes
                SetRefreshHint False

                bSucceed = True
            End If
        End If

        If Not bSucceed Then
            m_sSymbolsFileName = ""
            m_sSymbolsFilePath = ""
            m_nFilter(AFC_VALUE) = 0
            ClearViewAndData
            FilterUpdateValue False
            FilterUpdateExpiry True
            AdjustCaption
            AdjustState

            imgStop.Visible = False
            imgStopDis.Visible = False
            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
        End If
    End If
    
    lblStatus.Caption = CStr(m_Und.Count) & " underlying(s)"

    m_nOpenedExpiry = 0

    Screen.MousePointer = vbDefault
End Sub

Private Function UnderlyingAdd(ByVal nUndID As Long) As EtsMmVolaAnalysisLib.MmVaUndAtom
    On Error Resume Next
    Dim aGUnd As EtsGeneralLib.UndAtom, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, sKey$

    Set UnderlyingAdd = Nothing
    
    Set aGUnd = g_UnderlyingAll(nUndID)
    
    If Not aGUnd Is Nothing Then
        
        If aGUnd.IsTraderContract Then
            Set aUnd = m_Und(aGUnd.ID)
            If aUnd Is Nothing Then
                Set aUnd = New EtsMmVolaAnalysisLib.MmVaUndAtom
                aUnd.ID = aGUnd.ID
        
                aUnd.UndType = aGUnd.UndType
                aUnd.Symbol = aGUnd.Symbol
        
                aUnd.IsAmerican = aGUnd.IsAmerican
                aUnd.IsHTB = aGUnd.IsHTB
        
                aUnd.PriceClose = aGUnd.PriceClose
                aUnd.LotSize = aGUnd.LotSize
                aUnd.Skew = aGUnd.Skew
                aUnd.Kurt = aGUnd.Kurt
        
                aUnd.Yield = aGUnd.Yield
                If (Not aGUnd.Dividend Is Nothing) Then
                    Set aUnd.Dividend = aGUnd.Dividend
                End If
                If (Not g_Index(aUnd.ID) Is Nothing) Then
                    Set aUnd.BasketIndex = g_Index(aUnd.ID)
                End If
                    
                aUnd.QtyInShares = BAD_LONG_VALUE
                aUnd.Pos = BAD_DOUBLE_VALUE
              
                sKey = CStr(aUnd.UndType) & "_" & aUnd.Symbol
                If m_UndRequests(sKey) Is Nothing Then
                    m_UndRequests.Add aUnd, , , sKey
                End If
        
                m_Und.Add aUnd.ID, aUnd.Symbol, aUnd
                
                Set UnderlyingAdd = aUnd
            End If
            
            Set aUnd = Nothing
            
        Else
            LogEvent EVENT_WARNING, "Symbol '" & aGUnd.Symbol & "' is not a trader underlying."
        End If
        Set aGUnd = Nothing
    End If
End Function

Private Function UnderlyingsReadFromFile(ByRef arrLines() As String) As Boolean
    On Error Resume Next
    Dim nFile&, sLines$, nFileLen&
    
    UnderlyingsReadFromFile = False
    
    nFile = FreeFile
    
    Err.Clear
    nFileLen = FileLen(m_sSymbolsFilePath)
    If Err.Number <> 0 Then
        If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings. Can not open file '" & m_sSymbolsFilePath & "'."
        GoTo Ex
    End If
    
    If nFileLen = 0 Then
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "Fail to load underlyings. Empty file '" & m_sSymbolsFilePath & "'.", vbCritical
        GoTo Ex
    End If
    
    Err.Clear
    Open m_sSymbolsFilePath For Binary Access Read Lock Write As #nFile
    If Err.Number <> 0 Then
        If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings. Can not open file '" & m_sSymbolsFilePath & "'."
        GoTo Ex
    End If
    
    Err.Clear
    sLines = Space$(nFileLen)
    If Err.Number <> 0 Then
        If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings. Can not allocate buffer for file '" & m_sSymbolsFilePath & "'."
        GoTo Ex
    End If
    
    Err.Clear
    Get #nFile, , sLines
    If Err.Number <> 0 Then
        If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings. Can not read data from file '" & m_sSymbolsFilePath & "'."
        GoTo Ex
    End If
    
    Close #nFile
    
    Err.Clear
    arrLines = Split(Replace$(sLines, vbCrLf, vbLf, , , vbBinaryCompare), vbLf)
    If Err.Number <> 0 Then
        If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings. Can not allocate buffer for file '" & m_sSymbolsFilePath & "'."
        GoTo Ex
    End If
    If UBound(arrLines) < 0 Then
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "Fail to load underlyings. Invalid file '" & m_sSymbolsFilePath & "'.", vbCritical
        GoTo Ex
    End If
    
    UnderlyingsReadFromFile = True
Ex:
    Close #nFile
End Function

Private Function UnderlyingsLoad() As Boolean
    On Error GoTo EH
    Dim rsUnd As ADODB.Recordset, rsExp As ADODB.Recordset
    Dim dtExpiry As Date
    Dim vPos As Variant, aFlt As EtsGeneralLib.EtsMmFilterAtom, dtMinExpiry As Date
    Dim aExp As EtsMmVolaAnalysisLib.MmVaExpAtom
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
    Dim i&, nUndID&, aUndGroup As EtsGeneralLib.EtsUndGroupAtom, aEnt2 As EtsGeneralLib.EtsMmEntityAtom
    Dim arrLines() As String, arrValues() As String, nUBound&, dValue#
    Dim collUnd As New EtsGeneralLib.EtsMmEntityAtomColl, aGUnd As EtsGeneralLib.UndAtom, aEnt As EtsGeneralLib.EtsMmEntityAtom
    Dim sUnsIDs1$, sUnsIDs2$, sID$, nLen&, nNewProgressVal&, nCurMax&, nCurVal&


    pbProgress.Min = 0
    pbProgress.Value = pbProgress.Min
    DoEvents

    UnderlyingsLoad = False
    m_bDataLoad = True
    If Not m_bDataLoad Or m_nFilter(AFC_VALUE) = 0 Then GoTo Ex

    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100
    pbProgress.Visible = True
    lblProcess.Caption = "Underlyings loading..."
    lblProcess.Visible = True
    lblProcess.Refresh
    
    nNewProgressVal = 0&
    nCurMax = 0&
    nCurVal = 0&

    dtMinExpiry = DateAdd("d", MIN_EXPIRY_DAYS, Date)

    Select Case m_nFilter(AFC_GROUP)
        Case TYPE_UNDERLYING
            ' 40% = usp_UnderlyingPos_Get + usp_UnderlyingExpiry_Get
            ' 50% = UnderlyingAdd
            Set rsUnd = gDBW.usp_UnderlyingPos_Get(m_nFilter(AFC_VALUE), g_CurTraderID, , 1)
            Set rsExp = gDBW.usp_UnderlyingExpiry_Get(m_nFilter(AFC_VALUE), g_CurTraderID, , dtMinExpiry)
            m_Grp.Name = g_Underlying(m_nFilter(AFC_VALUE)).Symbol

            pbProgress.Value = 40

            Set aUnd = UnderlyingAdd(m_nFilter(AFC_VALUE))
            
            If Not aUnd Is Nothing Then
                Set aEnt2 = m_UndLoadedValues(CStr(aUnd.ID))
                If Not aEnt2 Is Nothing Then
                    aUnd.LoadValue1 = aEnt2.dValue1
                    aUnd.LoadValue2 = aEnt2.dValue2
                    aUnd.CalcExpression1 = aEnt2.sValue1
                    aUnd.CalcExpression2 = aEnt2.sValue2
                    Set aEnt2 = Nothing
                End If
                Set aUnd = Nothing
            End If
            
            pbProgress.Value = 90
            
            DoEvents
            If Not m_bDataLoad Then GoTo Ex
            IncProgress pbProgress
            DoEvents

        Case TYPE_GROUP
            ' 10% = usp_UnderlyingPos_Get + usp_UnderlyingExpiry_Get
            ' 80% = UnderlyingAdd
            Set rsUnd = gDBW.usp_UnderlyingPos_Get(, g_CurTraderID, m_nFilter(AFC_VALUE), 1)
            Set rsExp = gDBW.usp_UnderlyingExpiry_Get(, g_CurTraderID, m_nFilter(AFC_VALUE), dtMinExpiry)
            pbProgress.Value = 10

            Dim aGroupUnd As EtsGeneralLib.UndAtom
            
            Set aUndGroup = g_UnderlyingGroup(m_nFilter(AFC_VALUE))
            If Not aUndGroup Is Nothing Then
                m_Grp.Name = aUndGroup.Name

                If aUndGroup.Und.Count > 0 Then
                    nCurVal = 0&
                    nCurMax = aUndGroup.Und.Count

                    For Each aGroupUnd In aUndGroup.Und
                        'Futures and Future Underlyings are not supported in this screen yet
                        If aGroupUnd.UndType = enCtStock Or aGroupUnd.UndType = enCtIndex Then
                            
                            Set aUnd = UnderlyingAdd(aGroupUnd.ID)
                            
                            If Not aUnd Is Nothing Then
                                Set aEnt2 = m_UndLoadedValues(CStr(aUnd.ID))
                                If Not aEnt2 Is Nothing Then
                                    aUnd.LoadValue1 = aEnt2.dValue1
                                    aUnd.LoadValue2 = aEnt2.dValue2
                                    aUnd.CalcExpression1 = aEnt2.sValue1
                                    aUnd.CalcExpression2 = aEnt2.sValue2
                                    Set aEnt2 = Nothing
                                End If
                                Set aUnd = Nothing
                            End If
                            DoEvents
                            If Not m_bDataLoad Then GoTo Ex
                            
                            nCurVal = nCurVal + 1
                            nNewProgressVal = 10 + 80# * nCurVal / nCurMax
                            If nNewProgressVal <> pbProgress.Value Then
                                If nNewProgressVal <= pbProgress.Max Then
                                    pbProgress.Value = nNewProgressVal
                                Else
                                    pbProgress.Value = pbProgress.Max
                                End If
                            End If
                            
                            DoEvents
                        End If
                        If Not m_bDataLoad Then GoTo Ex
                    Next
                End If
                
                Set aUndGroup = Nothing
            End If
            
            pbProgress.Value = 90

        Case TYPE_TRADER
            ' 10% = usp_UnderlyingPos_Get + usp_UnderlyingExpiry_Get
            ' 80% = UnderlyingAdd
            Set rsUnd = gDBW.usp_UnderlyingPos_Get(, m_nFilter(AFC_VALUE), , 1)
            Set rsExp = gDBW.usp_UnderlyingExpiry_Get(, m_nFilter(AFC_VALUE), , dtMinExpiry)

            pbProgress.Value = 10

            Dim aTrader As EtsGeneralLib.EtsTraderAtom
            Dim aTraderUnd As EtsGeneralLib.EtsTraderUndAtom
            
            Set aTrader = g_Trader(m_nFilter(AFC_VALUE))
            If Not aTrader Is Nothing Then
                m_Grp.Name = aTrader.Name

                nCurVal = 0&
                nCurMax = aTrader.Und.Count

                For Each aTraderUnd In aTrader.Und
                    
                    Set aUnd = UnderlyingAdd(aTraderUnd.Und.ID)
                    
                    If Not aUnd Is Nothing Then
                        Set aEnt2 = m_UndLoadedValues(CStr(aUnd.ID))
                        If Not aEnt2 Is Nothing Then
                            aUnd.LoadValue1 = aEnt2.dValue1
                            aUnd.LoadValue2 = aEnt2.dValue2
                            aUnd.CalcExpression1 = aEnt2.sValue1
                            aUnd.CalcExpression2 = aEnt2.sValue2
                            Set aEnt2 = Nothing
                        End If
                        Set aUnd = Nothing
                    End If
                    
                    DoEvents
                    If Not m_bDataLoad Then GoTo Ex
                    
                    nCurVal = nCurVal + 1
                    nNewProgressVal = 10 + 80# * nCurVal / nCurMax
                    If nNewProgressVal <> pbProgress.Value Then
                        If nNewProgressVal <= pbProgress.Max Then
                            pbProgress.Value = nNewProgressVal
                        Else
                            pbProgress.Value = pbProgress.Max
                        End If
                    End If
                    
                    DoEvents
                Next
                
                Set aTrader = Nothing
            End If
            
            pbProgress.Value = 90
            
        Case TYPE_FILE
            On Error Resume Next
            ' 5% = ReadFromFile
            ' 5% = usp_UnderlyingPos_Get + usp_UnderlyingExpiry_Get
            ' 80% = UnderlyingAdd
            
            If Not UnderlyingsReadFromFile(arrLines) Then GoTo Ex
            m_Grp.Name = m_sSymbolsFileName
            
            nCurVal = 0&
            nCurMax = UBound(arrLines) - LBound(arrLines) + 1
            
            For i = LBound(arrLines) To UBound(arrLines)
                On Error Resume Next
                arrValues = Split(arrLines(i), ",")
                
                nUBound = -1
                nUBound = UBound(arrValues)
                If nUBound >= 0 Then
                    Set aGUnd = g_UnderlyingAll.BySortKey(arrValues(0))
                    If Not aGUnd Is Nothing Then
                        If aGUnd.IsTraderContract Then
                            sID = Trim$(Str$(aGUnd.ID))
                            If collUnd(sID) Is Nothing Then
                                
                                nLen = Len(sUnsIDs1)
                                If nLen + Len(sID) + 1 < 8000 Then
                                    If nLen > 0 Then sUnsIDs1 = sUnsIDs1 & ","
                                    sUnsIDs1 = sUnsIDs1 & sID
                                Else
                                    nLen = Len(sUnsIDs2)
                                    If nLen + Len(sID) + 1 < 8000 Then
                                        If nLen > 0 Then sUnsIDs2 = sUnsIDs2 & ","
                                        sUnsIDs2 = sUnsIDs2 & sID
                                    Else
                                        LogEvent EVENT_WARNING, "Too many underlyings in file '" & m_sSymbolsFilePath & "'. " & CStr(collUnd.Count) & " underlyings will be added."
                                        Exit For
                                    End If
                                End If
                            
                                Set aEnt = collUnd.Add(sID)
                                aEnt.ID = aGUnd.ID
                                aEnt.dValue1 = BAD_DOUBLE_VALUE
                                aEnt.dValue2 = BAD_DOUBLE_VALUE
                                Err.Clear
                                If nUBound > 0 Then
                                    dValue = CDbl(arrValues(1))
                                    If Err.Number <> 0 Then dValue = val(arrValues(1))
                                    If Err.Number = 0 Then
                                        aEnt.dValue1 = dValue
                                    End If
                                    
                                    If nUBound > 1 Then
                                        Err.Clear
                                        dValue = CDbl(arrValues(2))
                                        If Err.Number <> 0 Then dValue = val(arrValues(2))
                                        If Err.Number = 0 Then
                                            aEnt.dValue2 = dValue
                                        End If
                                    
                                        If nUBound > 2 Then
                                            aEnt.sValue1 = arrValues(3)
                                        
                                            If nUBound > 3 Then
                                                aEnt.sValue2 = arrValues(4)
                                            End If
                                        End If
                                    End If
                                End If
                                Set aEnt = Nothing
                            End If
                        Else
                            LogEvent EVENT_WARNING, "Symbol '" & arrValues(0) & "' is not a trader underlying."
                        End If
                        Set aGUnd = Nothing
                    Else
                        LogEvent EVENT_WARNING, "Symbol '" & arrValues(0) & "' is invalid underlying symbol."
                    End If
                    
                    DoEvents
                    If Not m_bDataLoad Then GoTo Ex
                
                    nCurVal = nCurVal + 1
                    nNewProgressVal = 5# * nCurVal / nCurMax
                    If nNewProgressVal <> pbProgress.Value Then
                        If nNewProgressVal <= pbProgress.Max Then
                            pbProgress.Value = nNewProgressVal
                        Else
                            pbProgress.Value = pbProgress.Max
                        End If
                    End If
                    
                End If
                Erase arrValues
            Next
            Erase arrLines
            
            pbProgress.Value = 5
            
            Set aGUnd = Nothing
            
            On Error GoTo EH
            If collUnd.Count > 0 Then
                Set rsUnd = gDBW.usp_UnderlyingPosByList_Get(g_CurTraderID, 1, sUnsIDs1, sUnsIDs2)
                Set rsExp = gDBW.usp_UnderlyingExpiryByList_Get(dtMinExpiry, sUnsIDs1, sUnsIDs2)
                
                pbProgress.Value = 10
    
                nCurVal = 0&
                nCurMax = collUnd.Count
            
                For Each aEnt In collUnd
                    Set aUnd = UnderlyingAdd(aEnt.ID)
                    
                    If Not aUnd Is Nothing Then
                        Set aEnt2 = m_UndLoadedValues(CStr(aUnd.ID))
                        If aEnt2 Is Nothing Then
                            aUnd.LoadValue1 = aEnt.dValue1
                            aUnd.LoadValue2 = aEnt.dValue2
                            aUnd.CalcExpression1 = aEnt.sValue1
                            aUnd.CalcExpression2 = aEnt.sValue2
                        Else
                            aUnd.LoadValue1 = aEnt2.dValue1
                            aUnd.LoadValue2 = aEnt2.dValue2
                            aUnd.CalcExpression1 = aEnt2.sValue1
                            aUnd.CalcExpression2 = aEnt2.sValue2
                            Set aEnt2 = Nothing
                        End If
                        Set aUnd = Nothing
                    End If
                    
                    DoEvents
                    If Not m_bDataLoad Then GoTo Ex
                    
                    nCurVal = nCurVal + 1
                    nNewProgressVal = 10 + 80# * nCurVal / nCurMax
                    If nNewProgressVal <> pbProgress.Value Then
                        If nNewProgressVal <= pbProgress.Max Then
                            pbProgress.Value = nNewProgressVal
                        Else
                            pbProgress.Value = pbProgress.Max
                        End If
                    End If
                    
                    DoEvents
                Next
                
                pbProgress.Value = 90
            Else
                pbProgress.Max = 100
                pbProgress.Value = 100
            End If
        
        Case Else
            Debug.Assert False
            GoTo Ex
    End Select

    m_Grp.ID = m_nFilter(AFC_VALUE)
    m_Grp.Data = m_nFilter(AFC_GROUP)

    DoEvents
    If Not m_bDataLoad Then GoTo Ex

    ' 10% = rsUnd.RecordCount + rsExp.RecordCount
    If m_Und.Count > 0 Then
        nCurVal = 0&
        nCurMax = rsUnd.RecordCount + rsExp.RecordCount
        
        While Not rsUnd.EOF
            nUndID = ReadLng(rsUnd!iContractID)
            Set aUnd = m_Und(nUndID)
            If Not aUnd Is Nothing Then
                vPos = rsUnd!iQty
                aUnd.QtyInShares = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
                vPos = rsUnd!fPosition
                aUnd.Pos = IIf(IsNull(vPos), BAD_DOUBLE_VALUE, ReadLng(vPos))
            End If

            Set aUnd = Nothing

            DoEvents
            If Not m_bDataLoad Then GoTo Ex
            
            nCurVal = nCurVal + 1
            nNewProgressVal = 90 + 10# * nCurVal / nCurMax
            If nNewProgressVal <> pbProgress.Value Then
                If nNewProgressVal <= pbProgress.Max Then
                    pbProgress.Value = nNewProgressVal
                Else
                    pbProgress.Value = pbProgress.Max
                End If
            End If
            
            DoEvents

            rsUnd.MoveNext
        Wend

        While Not rsExp.EOF
            nUndID = ReadLng(rsExp!iUnderlyingID)
            Set aUnd = m_Und(nUndID)
            If Not aUnd Is Nothing Then
                dtExpiry = ReadDate(rsExp!dtExpiryDate)
                If dtExpiry >= dtMinExpiry Then
                    
                    Set aExp = aUnd.Expiry(dtExpiry)
                    If aExp Is Nothing Then
                        Set aExp = New EtsMmVolaAnalysisLib.MmVaExpAtom
                        aExp.Expiry = dtExpiry
                        aExp.ExpiryOV = ReadDate(rsExp!dtExpiryOV)
                        aExp.TradingClose = ClipDays(ReadDate(rsExp!dtTradingClose))
                        aUnd.Expiry.Add dtExpiry, aExp

                        Set aEnt = m_Exp(CStr(CLng(dtExpiry)))
                        If aEnt Is Nothing Then
                            Set aEnt = m_Exp.Add(CStr(CLng(dtExpiry)))
                            aEnt.ID = CLng(dtExpiry)
                            aEnt.Name = Format$(aExp.Expiry, "MMM,DD YY")
                            Set aEnt = Nothing
                        End If

                    End If

                    Set aExp = Nothing
                    Set aUnd = Nothing
                End If
            End If

            DoEvents
            If Not m_bDataLoad Then GoTo Ex
            
            nCurVal = nCurVal + 1
            nNewProgressVal = 90 + 10# * nCurVal / nCurMax
            If nNewProgressVal <> pbProgress.Value Then
                If nNewProgressVal <= pbProgress.Max Then
                    pbProgress.Value = nNewProgressVal
                Else
                    pbProgress.Value = pbProgress.Max
                End If
            End If
            
            DoEvents

            rsExp.MoveNext
        Wend

        UnderlyingsAdjustRates True

    End If

    UnderlyingsLoad = True
Ex:
    m_bDataLoad = False
    Erase arrValues
    Erase arrLines
    m_UndLoadedValues.Clear
    Set aExp = Nothing
    Set aUnd = Nothing
    Set aEnt = Nothing
    Set aEnt2 = Nothing
    Set aFlt = Nothing
    Set rsUnd = Nothing
    Set rsExp = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings."
    GoTo Ex
End Function

Private Sub RemoveCurOptRequests(ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
    On Error Resume Next
    Dim aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom

    Set aStr = aUnd.CurExpiry.AtmStrike
    If Not aStr Is Nothing Then
        For Each aPair In aStr.OptPair
            If aPair.Opt(enOtCall).ID <> 0 Then
                m_OptRequests.Remove CStr(enCtOption) & "_" & aPair.Opt(enOtCall).Symbol
            End If

            If aPair.Opt(enOtPut).ID <> 0 Then
                m_OptRequests.Remove CStr(enCtOption) & "_" & aPair.Opt(enOtPut).Symbol
            End If
        Next
        Set aStr = Nothing
    End If
End Sub

'Private Sub AddCurOptRequests(ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
'    On Error Resume Next
'    Dim aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom, sKey$
'    Dim aOpt As EtsMmVolaAnalysisLib.MmVaOptAtom
'
'    Set aStr = aUnd.CurExpiry.AtmStrike
'    If Not aStr Is Nothing Then
'        For Each aPair In aStr.OptPair
'            Set aOpt = aPair.Opt(enOtCall)
'            If aOpt.ID <> 0 Then
'                sKey = CStr(enCtOption) & "_" & aOpt.Symbol
'                If m_OptRequests(sKey) Is Nothing Then
'                    m_OptRequests.Add aUnd, aOpt, aPair, sKey
'                End If
'            End If
'
'            Set aOpt = aPair.Opt(enOtPut)
'            If aOpt.ID <> 0 Then
'                sKey = CStr(enCtOption) & "_" & aOpt.Symbol
'                If m_OptRequests(sKey) Is Nothing Then
'                    m_OptRequests.Add aUnd, aOpt, aPair, sKey
'                End If
'            End If
'            Set aOpt = Nothing
'        Next
'        Set aStr = Nothing
'    End If
'End Sub

Private Function ExpiryOptionsLoad() As Boolean
    On Error GoTo EH
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, rsOpt As ADODB.Recordset, sSymbol$, sRoot$
    Dim aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, dStrike#, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
    Dim aExp As EtsMmVolaAnalysisLib.MmVaExpAtom, enOptType As EtsGeneralLib.EtsOptionTypeEnum, bLoad As Boolean
    Dim aOpt As EtsMmVolaAnalysisLib.MmVaOptAtom, aRoot As EtsMmVolaAnalysisLib.MmVaOptRootAtom, nOptRootID&
    Dim lSurfaceID As Long

    lblStatus.Visible = False
    imgStop.Visible = True
    imgStopDis.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100
    pbProgress.Visible = True
    lblProcess.Caption = "Current expiry options loading..."
    lblProcess.Visible = True
    lblProcess.Refresh

    DoEvents

    ExpiryOptionsLoad = False
    m_bDataLoad = True
    If Not m_bDataLoad Then GoTo Ex

    If m_Und.Count > 0 Then
        pbProgress.Max = m_Und.Count

        For Each aUnd In m_Und

            bLoad = True
            Set aExp = aUnd.CurExpiry
            If Not aExp Is Nothing Then
                If CLng(aExp.Expiry) <> m_nFilter(AFC_EXPIRY) Then
                    RemoveCurOptRequests aUnd
                Else
                    bLoad = False
                End If
                Set aExp = Nothing
            End If

            If bLoad Then
                Set aUnd.CurExpiry = aUnd.Expiry(m_nFilter(AFC_EXPIRY))
                Set aExp = aUnd.CurExpiry
                If Not aExp Is Nothing Then
                    If aExp.Strike.Count = 0 Then
                        
                        Set rsOpt = gDBW.usp_UnderlyingOptionByExpiry_Get(aUnd.ID, aUnd.CurExpiry.Expiry)
                        While Not rsOpt.EOF
                            dStrike = Round(ReadDbl(rsOpt!fStrike), STRIKE_DECIMALS_COUNT)
                            enOptType = IIf(ReadLng(rsOpt!tiIsCall) <> 0, enOtCall, enOtPut)
                            sSymbol = ReadStr(rsOpt!vcSymbol)

                            Set aStr = aExp.Strike(dStrike)
                            If aStr Is Nothing Then
                                Set aStr = aExp.Strike.Add(dStrike)
                                aStr.Strike = dStrike
                            End If

                            nOptRootID = ReadLng(rsOpt!iOptionRootID)
                            sRoot = ReadStr(rsOpt!vcOptionRootSymbol)

                            Set aRoot = aUnd.OptRoot(nOptRootID)
                            If aRoot Is Nothing Then
                                Set aRoot = New EtsMmVolaAnalysisLib.MmVaOptRootAtom
                                aRoot.ID = nOptRootID
                                aRoot.Name = sRoot

                                aUnd.OptRoot.Add aRoot.ID, aRoot.Name, aRoot
                            End If

                            Set aPair = aStr.OptPair(aRoot.ID)
                            If aPair Is Nothing Then
                                Set aPair = New EtsMmVolaAnalysisLib.MmVaOptPairAtom
                                aPair.RootID = aRoot.ID
                                aPair.LotSize = ReadLng(rsOpt!iLotSize)

                                aStr.OptPair.Add aRoot.ID, aRoot.Name, aPair
                            End If

                            Set aOpt = aPair.Opt(enOptType)
                            If aOpt.ID = 0 Then
                                aOpt.ID = ReadLng(rsOpt!iContractID)
                                aOpt.Symbol = sSymbol
                                aOpt.OptType = enOptType
                                aOpt.Expiry = aExp.Expiry
                                aOpt.ExpiryOV = aExp.ExpiryOV
                                aOpt.Strike = dStrike
                                aOpt.PriceClose = ReadDbl(rsOpt!fPriceClose)
                                aOpt.PriceTheoclose = ReadDbl(rsOpt!fPriceTheoClose)
                                aOpt.OptionRootID = nOptRootID

                                'lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(nOptRootID)
                                'aOpt.Vola = aUnd.VolaSrv.OptionVola(aOpt.Expiry, dStrike, lSurfaceID)

                                If aOpt.Vola < 0 Then
                                    aOpt.Vola = BAD_DOUBLE_VALUE
                                End If
                            End If

                            rsOpt.MoveNext
                        Wend
                        Set rsOpt = Nothing

                        For Each aStr In aExp.Strike
                            sRoot = ""

                            For Each aPair In aStr.OptPair
                                If Len(sRoot) > 0 Then sRoot = sRoot & "|#" Else sRoot = sRoot & "#"
                                sRoot = sRoot & Trim$(Str$(aPair.RootID)) & ";" & aUnd.OptRoot(aPair.RootID).Name
                                If aStr.CurOptPair Is Nothing Then Set aStr.CurOptPair = aPair
                            Next

                            aStr.RootNames = sRoot
                        Next
                    End If
                End If
            End If

            DoEvents
            If Not m_bDataLoad Then GoTo Ex
            IncProgress pbProgress
            DoEvents

        Next
    End If

    ExpiryOptionsLoad = True
Ex:
    imgStop.Visible = False
    imgStopDis.Visible = False
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    
    m_bDataLoad = False
    Set rsOpt = Nothing
    Set aOpt = Nothing
    Set aExp = Nothing
    Set aPair = Nothing
    Set aRoot = Nothing
    Set aStr = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load current expiry options."
    GoTo Ex
End Function

Private Sub RequestLastUndQuotes()
    On Error GoTo EH
    Dim aReq As clsVaRequestAtom

    If Not PriceProvider Is Nothing Then
        Debug.Assert Not m_bLastQuoteReqNow

        If m_UndRequests.Count > 0 Then
            m_bInProc = True
            m_bLastQuoteReqNow = True
            AdjustState

            m_nLastQuoteReqCountUnd = m_UndRequests.Count
            m_nLastQuoteReqDoneUnd = 0&

            pbProgress.Min = 0
            pbProgress.Max = m_nLastQuoteReqCountUnd * 2
            pbProgress.Value = 0
            pbProgress.Visible = True

            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Last quotes request..."
            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents

            For Each aReq In m_UndRequests
                If Not m_bLastQuoteReqNow Then Exit For
                aReq.OptionsRequestCount = 0
                aReq.OptionsRequestDone = 0
                PriceProvider.RequestLastQuote aReq.GetQuoteUpdateParam
                DoEvents
            Next

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
    Dim sKey$, bFinished As Boolean, aUndReq As clsVaRequestAtom, aOptReq As clsVaRequestAtom
    bFinished = False

    If ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If Request.Type = enOPT Then
                sKey = CStr(enCtOption) & "_" & Request.Symbol

                Set aOptReq = m_OptRequests(sKey)
                If Not aOptReq Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description

                    sKey = CStr(aOptReq.Und.UndType) & "_" & aOptReq.Und.Symbol

                    Set aUndReq = m_UndRequests(sKey)
                    If Not aUndReq Is Nothing Then
                        aUndReq.OptionsRequestDone = aUndReq.OptionsRequestDone + 1

                        If aUndReq.OptionsRequestDone = aUndReq.OptionsRequestCount Then
                            m_nLastQuoteReqDoneUnd = m_nLastQuoteReqDoneUnd + 1
                            IncProgress pbProgress
                            bFinished = (m_nLastQuoteReqDoneUnd = m_nLastQuoteReqCountUnd)
                        End If
                        Set aUndReq = Nothing
                    Else
                        Debug.Assert False
                    End If

                    Set aOptReq = Nothing
                End If

            Else
                sKey = CStr(IIf(Request.Type = enSTK, enCtStock, enCtIndex)) & "_" & Request.Symbol

                If Not m_UndRequests(sKey) Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description

                    m_nLastQuoteReqDoneUnd = m_nLastQuoteReqDoneUnd + 1
                    IncProgress pbProgress
                    IncProgress pbProgress
                    bFinished = (m_nLastQuoteReqDoneUnd = m_nLastQuoteReqCountUnd)
                End If
            End If

            If bFinished Then
                m_bLastQuoteReqNow = False

                PriceProvider.CancelLastQuote

                m_bInProc = True
                UnderlyingsCalc False
                m_bInProc = False
                
                UnderlyingsUpdate False
                AdjustState

                pbProgress.Visible = False
                lblStatus.Visible = True
                lblProcess.Visible = False
                lblProcess.Caption = ""
                imgStop.Visible = False
                imgStopDis.Visible = False
            End If
        Else
            Exit Sub
        End If

    Else
        LogEvent EVENT_ERROR, Description
    End If
End Sub

Private Sub PriceProvider_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error Resume Next
    If Not m_bLastQuoteReqNow Then Exit Sub
    Dim sKey$, aUndReq As clsVaRequestAtom, aOptReq As clsVaRequestAtom, bFinished As Boolean, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
    Dim aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
    Dim aOpt As EtsMmVolaAnalysisLib.MmVaOptAtom
    bFinished = False

    If Params.Type = enOPT Then
        sKey = CStr(enCtOption) & "_" & Params.Symbol

        Set aOptReq = m_OptRequests(sKey)
        If Not aOptReq Is Nothing Then
            Set aOpt = aOptReq.Opt
            Set aPair = aOptReq.Pair
            Debug.Assert Not aOpt Is Nothing And Not aPair Is Nothing

            sKey = CStr(aOptReq.Und.UndType) & "_" & aOptReq.Und.Symbol
            Set aUndReq = m_UndRequests(sKey)
            If Not aUndReq Is Nothing Then

                aOpt.PriceBid = Results.BidPrice
                aOpt.PriceAsk = Results.AskPrice
                aOpt.PriceLast = Results.LastPrice

                If aPair.LotSize <= 0 Then
                    If Results.LotSize > 0 Then
                        aPair.LotSize = Results.LotSize
                    Else
                        aPair.LotSize = 100
                    End If
                End If

                aUndReq.OptionsRequestDone = aUndReq.OptionsRequestDone + 1

                If aUndReq.OptionsRequestDone = aUndReq.OptionsRequestCount Then
                    m_nLastQuoteReqDoneUnd = m_nLastQuoteReqDoneUnd + 1
                    IncProgress pbProgress
                End If
                bFinished = (m_nLastQuoteReqDoneUnd = m_nLastQuoteReqCountUnd)
            Else
                Debug.Assert False
            End If

            Set aOpt = Nothing
            Set aPair = Nothing
            Set aOptReq = Nothing
            Set aUndReq = Nothing
        End If
    Else
        sKey = CStr(IIf(Params.Type = enSTK, enCtStock, enCtIndex)) & "_" & Params.Symbol

        Set aUndReq = m_UndRequests(sKey)
        If Not aUndReq Is Nothing Then
            Set aUnd = aUndReq.Und
            Debug.Assert Not aUnd Is Nothing

            aUnd.PriceBid = Results.BidPrice
            aUnd.PriceAsk = Results.AskPrice
            aUnd.PriceLast = Results.LastPrice

            If aUnd.LotSize <= 0 Then
                If Results.LotSize > 0 Then
                    aUnd.LotSize = Results.LotSize
                Else
                    aUnd.LotSize = 1
                End If
            End If

            RemoveCurOptRequests aUnd
            aUnd.CurExpiry.FindAtmStrike aUnd.PriceLast

            Set aStr = aUnd.CurExpiry.AtmStrike
            If Not aStr Is Nothing Then
                For Each aPair In aStr.OptPair
                    Set aOpt = aPair.Opt(enOtCall)
                    If aOpt.ID <> 0 Then
                        sKey = CStr(enCtOption) & "_" & aOpt.Symbol
                        Set aOptReq = m_OptRequests(sKey)
                        If aOptReq Is Nothing Then
                            Set aOptReq = m_OptRequests.Add(aUnd, aOpt, aPair, sKey)
                        Else
                            Debug.Assert False
                        End If

                        Err.Clear
                        PriceProvider.RequestLastQuote aOptReq.GetQuoteUpdateParam
                        If Err.Number = 0& Then
                            aUndReq.OptionsRequestCount = aUndReq.OptionsRequestCount + 1
                        Else
                            LogEvent EVENT_ERROR, "Fail to request last quote for option '" & aOpt.Symbol & "' of '" & aUnd.Symbol & "'. " & Err.Description
                        End If
                    End If

                    Set aOpt = aPair.Opt(enOtPut)
                    If aOpt.ID <> 0 Then
                        sKey = CStr(enCtOption) & "_" & aOpt.Symbol
                        Set aOptReq = m_OptRequests(sKey)
                        If aOptReq Is Nothing Then
                            Set aOptReq = m_OptRequests.Add(aUnd, aOpt, aPair, sKey)
                        Else
                            Debug.Assert False
                        End If

                        Err.Clear
                        PriceProvider.RequestLastQuote aOptReq.GetQuoteUpdateParam
                        If Err.Number = 0& Then
                            aUndReq.OptionsRequestCount = aUndReq.OptionsRequestCount + 1
                        Else
                            LogEvent EVENT_ERROR, "Fail to request last quote for option '" & aOpt.Symbol & "' of '" & aUnd.Symbol & "'. " & Err.Description
                        End If
                    End If

                    Set aOpt = Nothing
                Next
                Set aStr = Nothing
            Else
                m_nLastQuoteReqDoneUnd = m_nLastQuoteReqDoneUnd + 1
                IncProgress pbProgress
            End If
            IncProgress pbProgress

            bFinished = (m_nLastQuoteReqDoneUnd = m_nLastQuoteReqCountUnd)

            Set aUnd = Nothing
            Set aUndReq = Nothing
        End If
    End If

    If bFinished Then
        m_bLastQuoteReqNow = False

        PriceProvider.CancelLastQuote

        m_bInProc = True
        UnderlyingsCalc False
        m_bInProc = False

        UnderlyingsUpdate False
        AdjustState

        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If
End Sub

Private Sub UnderlyingsShow()
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, nRow&, aRowData As clsVaRowData, i&

    With fgUnd
        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw

        .Rows = 1

        For Each aUnd In m_Und

            .AddItem ""
            nRow = .Rows - 1

            Set aRowData = New clsVaRowData
            Set aRowData.Und = aUnd

            .RowData(nRow) = aRowData
            .TextMatrix(nRow, AUC_KEY) = aUnd.ID

            UnderlyingUpdate nRow, True, aUnd

            Set aRowData = Nothing
        Next

        If .Rows > 1 Then
            .Row = 1
        End If

        i = -1
        i = .ColIndex(AUC_SYMBOL)
        If i >= 0 Then
            .Col = i
            .Row = 1
            .Sort = flexSortGenericAscending
        End If
        geUnd.ShowSortImage i, 1

        If .Rows > 1 Then .Cell(flexcpAlignment, 1, 1, .Rows - 1, 1) = flexAlignCenterCenter
        FormatUndColumns

        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Function ExpressionSubstVariables(ByVal sExpression As String, ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom) As String
    On Error Resume Next
    Dim aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
    Dim aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom
    Dim dValue#, nValue&
    
    If Len(sExpression) > 0 Then
        sExpression = UCase$(sExpression)
        
        If InStr(1, sExpression, "#BID#", vbTextCompare) > 0 Then
            If aUnd.PriceBid > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#BID#", Trim$(Str$(aUnd.PriceBid)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#ASK#", vbTextCompare) > 0 Then
            If aUnd.PriceBid > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#ASK#", Trim$(Str$(aUnd.PriceAsk)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#LAST#", vbTextCompare) > 0 Then
            If aUnd.PriceLast > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#LAST#", Trim$(Str$(aUnd.PriceLast)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        Set aStr = aUnd.CurExpiry.AtmStrike
        Set aPair = aStr.CurOptPair
        
        If InStr(1, sExpression, "#ATMSTR#", vbTextCompare) > 0 Then
            If Not aStr Is Nothing Then
                sExpression = Replace$(sExpression, "#ATMSTR#", Trim$(Str$(aStr.Strike)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SBID#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.StraddleBid
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#SBID#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SASK#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.StraddleAsk
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#SASK#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SIV-B#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.StraddleIVBid
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#SIV-B#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SIV-A#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.StraddleIVAsk
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#SIV-A#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#CONT-B#", vbTextCompare) > 0 Then
            nValue = BAD_LONG_VALUE
            nValue = aPair.ContractsBid
            If nValue > BAD_LONG_VALUE Then
                sExpression = Replace$(sExpression, "#CONT-B#", Trim$(Str$(nValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#CONT-A#", vbTextCompare) > 0 Then
            nValue = BAD_LONG_VALUE
            nValue = aPair.ContractsAsk
            If nValue > BAD_LONG_VALUE Then
                sExpression = Replace$(sExpression, "#CONT-A#", Trim$(Str$(nValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#LOAD1#", vbTextCompare) > 0 Then
            If aUnd.LoadValue1 > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#LOAD1#", Trim$(Str$(aUnd.LoadValue1)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#LOAD2#", vbTextCompare) > 0 Then
            If aUnd.LoadValue2 > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#LOAD2#", Trim$(Str$(aUnd.LoadValue2)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#VGA-B#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.TotalVegaBid
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#VGA-B#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#VGA-A#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.TotalVegaAsk
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#VGA-A#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#DLT-B#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.NetDeltaBid
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#DLT-B#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SHRS-B#", vbTextCompare) > 0 Then
            nValue = BAD_LONG_VALUE
            nValue = aPair.DeltaInSharesBid
            If nValue > BAD_LONG_VALUE Then
                sExpression = Replace$(sExpression, "#SHRS-B#", Trim$(Str$(nValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#DLT-A#", vbTextCompare) > 0 Then
            dValue = BAD_DOUBLE_VALUE
            dValue = aPair.NetDeltaAsk
            If dValue > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#DLT-A#", Trim$(Str$(dValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#SHRS-A#", vbTextCompare) > 0 Then
            nValue = BAD_LONG_VALUE
            nValue = aPair.DeltaInSharesAsk
            If nValue > BAD_LONG_VALUE Then
                sExpression = Replace$(sExpression, "#SHRS-A#", Trim$(Str$(nValue)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
        If InStr(1, sExpression, "#VGAAMT#", vbTextCompare) > 0 Then
            If m_dVgaAmt > BAD_DOUBLE_VALUE Then
                sExpression = Replace$(sExpression, "#VGAAMT#", Trim$(Str$(m_dVgaAmt)), , , vbTextCompare)
            Else
                sExpression = ""
                GoTo Ex
            End If
        End If
        
Ex:
        Set aStr = Nothing
        Set aPair = Nothing
    End If
    ExpressionSubstVariables = sExpression
End Function

Private Sub UnderlyingCalcCustomValues(ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
    On Error Resume Next
    Dim sExpression$
    
    aUnd.CalcValue1 = BAD_DOUBLE_VALUE
    sExpression = ExpressionSubstVariables(aUnd.CalcExpression1, aUnd)
    If Len(sExpression) > 0 Then
        vsAwk = sExpression
        aUnd.CalcValue1 = vsAwk.val
        If vsAwk.Error <> aerNone Then
            aUnd.CalcValue1 = CALC_ERR_VALUE
        End If
    End If

    aUnd.CalcValue2 = BAD_DOUBLE_VALUE
    sExpression = ExpressionSubstVariables(aUnd.CalcExpression2, aUnd)
    If Len(sExpression) > 0 Then
        vsAwk = sExpression
        aUnd.CalcValue2 = vsAwk.val
        If vsAwk.Error <> aerNone Then
            aUnd.CalcValue2 = CALC_ERR_VALUE
        End If
    End If
End Sub

'Private Sub UnderlyingsCalcCustomValues()
'    On Error Resume Next
'    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
'
'    For Each aUnd In m_Und
'        UnderlyingCalcCustomValues aUnd
'    Next
'End Sub

Private Sub UnderlyingCalc(ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
    On Error Resume Next
    aUnd.CalcCurrentValues m_dVgaAmt, g_Params.CalcModel, g_Params.UseTheoVolatility, g_Main.CalculationParametrs
    Debug.Assert Err.Number = 0
    UnderlyingCalcCustomValues aUnd
End Sub

Private Sub UnderlyingsCalc(ByVal bShowProgress As Boolean)
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom

    For Each aUnd In m_Und

        UnderlyingCalc aUnd

        If bShowProgress Then
            IncProgress pbProgress
        End If
    Next
End Sub

Private Sub UnderlyingsAdjustRates(ByVal bForceUpdateCustom As Boolean)
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom

    For Each aUnd In m_Und
        UnderlyingAdjustRates aUnd, bForceUpdateCustom
    Next
End Sub

Private Sub UnderlyingAdjustRates(ByRef aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, ByVal bForceUpdateCustom As Boolean)
    On Error Resume Next
    Dim aExp As EtsMmVolaAnalysisLib.MmVaExpAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    If aUnd Is Nothing Then Exit Sub
    
    Dim dtNow As Date
    dtNow = GetNewYorkTime
    
    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then ' And aUnd.Pos > BAD_DOUBLE_VALUE Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)
    Else
        bUseMidRates = True
    End If

    If bForceUpdateCustom Or aUnd.UseMidRates <> bUseMidRates Or Not bUseMidRates Then
        aUnd.UseMidRates = bUseMidRates
        
        Dim bIsHTBRatesExist As Boolean: bIsHTBRatesExist = IsHTBRatesExist(aUnd.ID)
        
        For Each aExp In aUnd.Expiry
            If bUseMidRates Then
                aExp.Rate = GetNeutralRate(dtNow, aExp.ExpiryOV)
            Else
                aExp.Rate = IIf(dPos < 0#, GetShortRate(dtNow, aExp.ExpiryOV), GetLongRate(dtNow, aExp.ExpiryOV))
            End If
            
            If bIsHTBRatesExist Then
                aExp.HTBRate = GetHTBRate(aUnd.ID, dtNow, aExp.ExpiryOV)
            Else
                aExp.HTBRate = BAD_DOUBLE_VALUE
            End If

            Set aExp = Nothing
        Next
    End If
End Sub

Private Sub UnderlyingsUpdate(ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim nRows&, nRow&

    With fgUnd
        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw

        nRows = .Rows - 1
        For nRow = 1 To nRows
            UnderlyingUpdate nRow, bSymbol, Nothing
        Next

        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingUpdate(ByVal nRow As Long, ByVal bSymbol As Boolean, aRowUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
    Dim nCol&, aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aExp As EtsMmVolaAnalysisLib.MmVaExpAtom

    With fgUnd
        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw

        If aRowUnd Is Nothing Then
            Set aUnd = .RowData(nRow).Und
        Else
            Set aUnd = aRowUnd
        End If

        If Not aUnd Is Nothing Then
            Set aExp = aUnd.CurExpiry
            If Not aExp Is Nothing Then
                Set aStr = aUnd.CurExpiry.AtmStrike
                If Not aStr Is Nothing Then
                    Set aPair = aStr.CurOptPair
                End If
            End If

            Dim i&, nIdx&, nColIdx&
            i = 0
            nIdx = m_gdUnd.Idx(0)
            While nIdx >= 0 And i <= AUC_LAST_COLUMN
                nCol = i + 1
                nColIdx = nIdx

                If bSymbol Then
                    Select Case nIdx
                        Case AUC_ROW
                            .TextMatrix(nRow, nCol) = nRow

                        Case AUC_SYMBOL
                            .TextMatrix(nRow, nCol) = aUnd.Symbol

                        Case AUC_BUY
                            .TextMatrix(nRow, nCol) = (aUnd.NewTradeType = enMmVaNewTradeBuy)

                        Case AUC_SELL
                            .TextMatrix(nRow, nCol) = (aUnd.NewTradeType = enMmVaNewTradeSell)

                    End Select
                End If
                
                Select Case nIdx
                    Case AUC_UND_BID
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceBid > BAD_DOUBLE_VALUE, aUnd.PriceBid, STR_NA)

                    Case AUC_UND_ASK
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceAsk > BAD_DOUBLE_VALUE, aUnd.PriceAsk, STR_NA)

                    Case AUC_UND_LAST
                        .TextMatrix(nRow, nCol) = IIf(aUnd.PriceLast > BAD_DOUBLE_VALUE, aUnd.PriceLast, STR_NA)

                    Case AUC_EXPIRY
                        If Not aExp Is Nothing Then .TextMatrix(nRow, nCol) = aExp.Expiry Else .TextMatrix(nRow, nCol) = STR_NA

                    Case AUC_EXPIRY_DAYS
                        If Not aExp Is Nothing Then
                            Dim dtTimeToExp As Date
                            Dim nDays As Long
                            
                            dtTimeToExp = IIf(aExp.LocalExpiryOV - Now > 0, aExp.LocalExpiryOV - Now, 0)
                            nDays = DateDiff("d", 0, dtTimeToExp)
                            
                            .TextMatrix(nRow, nCol) = CStr(nDays) + IIf(nDays < 2, " day ", " days ") + Format(dtTimeToExp, "hh:mm")
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_ATM_STRIKE
                        If Not aStr Is Nothing Then .TextMatrix(nRow, nCol) = aStr.Strike Else .TextMatrix(nRow, nCol) = STR_NA

                    Case AUC_CALC_1
                        If aUnd.CalcValue1 > BAD_DOUBLE_VALUE Then
                            .TextMatrix(nRow, nCol) = IIf(aUnd.CalcValue1 < CALC_ERR_VALUE, aUnd.CalcValue1, CALC_ERR_TEXT)
                        
                            If aUnd.CalcValue1 < CALC_ERR_VALUE Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                            Else
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                        End If

                    Case AUC_CALC_2
                        If aUnd.CalcValue2 > BAD_DOUBLE_VALUE Then
                            .TextMatrix(nRow, nCol) = IIf(aUnd.CalcValue2 < CALC_ERR_VALUE, aUnd.CalcValue2, CALC_ERR_TEXT)
                        
                            If aUnd.CalcValue2 < CALC_ERR_VALUE Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                            Else
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                        End If

                    Case AUC_LOAD_1
                        .TextMatrix(nRow, nCol) = IIf(aUnd.LoadValue1 > BAD_DOUBLE_VALUE, aUnd.LoadValue1, STR_NA)

                    Case AUC_LOAD_2
                        .TextMatrix(nRow, nCol) = IIf(aUnd.LoadValue2 > BAD_DOUBLE_VALUE, aUnd.LoadValue2, STR_NA)

                    Case AUC_OPT_ROOT
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = aUnd.OptRoot(aPair.RootID).Name
                        
                            If aStr.OptPair.Count > 1 Then
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
                            Else
                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                            End If
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
                        End If

                    Case AUC_SUM_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.StraddleBid > BAD_DOUBLE_VALUE, aPair.StraddleBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_SUM_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.StraddleAsk > BAD_DOUBLE_VALUE, aPair.StraddleAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_IV_SUM_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.StraddleIVBid > BAD_DOUBLE_VALUE, aPair.StraddleIVBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_IV_SUM_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.StraddleIVAsk > BAD_DOUBLE_VALUE, aPair.StraddleIVAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_CONTRACTS_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.ContractsBid > BAD_LONG_VALUE, aPair.ContractsBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_CONTRACTS_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.ContractsAsk > BAD_LONG_VALUE, aPair.ContractsAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_VEGA_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.TotalVegaBid > BAD_DOUBLE_VALUE, aPair.TotalVegaBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_VEGA_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.TotalVegaAsk > BAD_DOUBLE_VALUE, aPair.TotalVegaAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_DELTA_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.NetDeltaBid > BAD_DOUBLE_VALUE, aPair.NetDeltaBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_SHARES_DELTA_BID
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.DeltaInSharesBid > BAD_LONG_VALUE, aPair.DeltaInSharesBid, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_DELTA_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.NetDeltaAsk > BAD_DOUBLE_VALUE, aPair.NetDeltaAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                    Case AUC_SHARES_DELTA_ASK
                        If Not aPair Is Nothing Then
                            .TextMatrix(nRow, nCol) = IIf(aPair.DeltaInSharesAsk > BAD_LONG_VALUE, aPair.DeltaInSharesAsk, STR_NA)
                        Else
                            .TextMatrix(nRow, nCol) = STR_NA
                        End If

                End Select

                i = i + 1
                nIdx = m_gdUnd.Idx(i)
            Wend

            Set aPair = Nothing
            Set aStr = Nothing
            Set aExp = Nothing

            Set aUnd = Nothing
        End If

        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
    End With
End Sub

'Private Sub UnderlyingsUpdateColors()
'    On Error Resume Next
'    Dim nRows&, nRow&
'
'    With fgUnd
'        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
'
'        nRows = .Rows - 1
'        For nRow = 1 To nRows
'            UnderlyingUpdateColors nRow, Nothing
'        Next
'
'        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
'    End With
'End Sub

'Private Sub UnderlyingUpdateColors(ByVal nRow As Long, aRowUnd As EtsMmVolaAnalysisLib.MmVaUndAtom)
'    On Error Resume Next
'    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, aStr As EtsMmVolaAnalysisLib.MmVaStrikeAtom
'    Dim nCol&, aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aExp As EtsMmVolaAnalysisLib.MmVaExpAtom
'
'    With fgUnd
'        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
'
'        If aRowUnd Is Nothing Then
'            Set aUnd = .RowData(nRow).Und
'        Else
'            Set aUnd = aRowUnd
'        End If
'
'        If Not aUnd Is Nothing Then
'            Set aExp = aUnd.CurExpiry
'            If Not aExp Is Nothing Then
'                Set aStr = aUnd.CurExpiry.AtmStrike
'                If Not aStr Is Nothing Then
'                    Set aPair = aStr.CurOptPair
'                End If
'            End If
'
'            Dim i&, nIdx&, nColIdx&
'            i = 0
'            nIdx = m_gdUnd.Idx(0)
'            While nIdx >= 0 And i <= AUC_LAST_COLUMN
'                nCol = i + 1
'                nColIdx = nIdx
'
'                Select Case nIdx
'                    Case AUC_CALC_1
'                        If aUnd.CalcValue1 > BAD_DOUBLE_VALUE Then
'                            If aUnd.CalcValue1 < CALC_ERR_VALUE Then
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                            Else
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
'                            End If
'                        Else
'                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                        End If
'
'                    Case AUC_CALC_2
'                        If aUnd.CalcValue2 > BAD_DOUBLE_VALUE Then
'                            If aUnd.CalcValue2 < CALC_ERR_VALUE Then
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                            Else
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
'                            End If
'                        Else
'                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                        End If
'
'                    Case AUC_OPT_ROOT
'                        If Not aPair Is Nothing Then
'                            If aStr.OptPair.Count > 1 Then
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColorAlt1 Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColorAlt1
'                            Else
'                                If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                    .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                            End If
'                        Else
'                            If .Cell(flexcpForeColor, nRow, nCol) <> m_gdUnd.Col(nColIdx).ForeColor Then _
'                                .Cell(flexcpForeColor, nRow, nCol) = m_gdUnd.Col(nColIdx).ForeColor
'                        End If
'
'                End Select
'
'                i = i + 1
'                nIdx = m_gdUnd.Idx(i)
'            Wend
'
'            Set aPair = Nothing
'            Set aStr = Nothing
'            Set aExp = Nothing
'
'            Set aUnd = Nothing
'        End If
'
'        m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw
'    End With
'End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, sTmp$
    If Len(sKey) > 0 Then sKey = "." & sKey

    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height

    ' common info
    For i = AFC_GROUP To AFC_COLUMN_COUNT
        aStorage.SetLongValue "VolaFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next

    If m_nFilter(AFC_GROUP) = TYPE_FILE Then
        aStorage.SetStringValue "VolaFlt" & sKey, "SymbolsFilePath", m_sSymbolsFilePath
    End If
    
    m_gdFlt.WriteToStorage "VolaFltGrid" & sKey, aStorage, False
    m_gdUnd.WriteToStorage "VolaUndGrid" & sKey, aStorage
    
    ' formulas and custom numbers
    aStorage.SetLongValue "VolaUndData" & sKey, "SymbolsCount", m_Und.Count
    i = 1
    For Each aUnd In m_Und
        sTmp = aUnd.Symbol & "," & Trim$(Str$(aUnd.LoadValue1)) & "," & Trim$(Str$(aUnd.LoadValue2)) & "," & _
                aUnd.CalcExpression1 & "," & aUnd.CalcExpression2
                
        aStorage.SetStringValue "VolaUndData" & sKey, "Sym" & CStr(i), sTmp
        i = i + 1
    Next
    
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&, nCount&, aEnt As EtsGeneralLib.EtsMmEntityAtom, sLine$, arrValues() As String, nUBound&
    Dim aGUnd As EtsGeneralLib.UndAtom, dValue#, sID$
    If Len(sKey) > 0 Then sKey = "." & sKey

    m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
    m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
    m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
    m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)

    ' common info
    For i = AFC_GROUP To AFC_COLUMN_COUNT
        m_nFilter(i) = aStorage.GetLongValue("VolaFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next
    
    If m_nFilter(AFC_GROUP) = TYPE_FILE Then
        m_sSymbolsFilePath = aStorage.GetStringValue("VolaFlt" & sKey, "SymbolsFilePath", "")
        m_sFilePath = ExtractFilePath(m_sSymbolsFilePath)
        m_sSymbolsFileName = ExtractFileName(m_sSymbolsFilePath)
    End If

    m_gdFlt.ReadFromStorage "VolaFltGrid" & sKey, aStorage, False
    m_gdUnd.ReadFromStorage "VolaUndGrid" & sKey, aStorage

    ' formulas and custom numbers
    On Error Resume Next
    m_UndLoadedValues.Clear
    nCount = aStorage.GetLongValue("VolaUndData" & sKey, "SymbolsCount")
    
    For i = 1 To nCount
        sLine = aStorage.GetStringValue("VolaUndData" & sKey, "Sym" & CStr(i), "")
        arrValues = Split(sLine, ",")
        
        nUBound = -1
        nUBound = UBound(arrValues)
        If nUBound >= 0 Then
            Set aGUnd = g_UnderlyingAll.BySortKey(arrValues(0))
            If Not aGUnd Is Nothing Then
                If aGUnd.IsTraderContract Then
                    sID = Trim$(Str$(aGUnd.ID))
                    If m_UndLoadedValues(sID) Is Nothing Then
                        Set aEnt = m_UndLoadedValues.Add(sID)
                        aEnt.ID = aGUnd.ID
                        aEnt.Name = arrValues(0)
                        aEnt.dValue1 = BAD_DOUBLE_VALUE
                        aEnt.dValue2 = BAD_DOUBLE_VALUE
                        Err.Clear
                        If nUBound > 0 Then
                            dValue = CDbl(arrValues(1))
                            If Err.Number <> 0 Then dValue = val(arrValues(1))
                            If Err.Number = 0 Then
                                aEnt.dValue1 = dValue
                            End If
                            
                            If nUBound > 1 Then
                                Err.Clear
                                dValue = CDbl(arrValues(2))
                                If Err.Number <> 0 Then dValue = val(arrValues(2))
                                If Err.Number = 0 Then
                                    aEnt.dValue2 = dValue
                                End If
                            
                                If nUBound > 2 Then
                                    aEnt.sValue1 = arrValues(3)
                                
                                    If nUBound > 3 Then
                                        aEnt.sValue2 = arrValues(4)
                                    End If
                                End If
                            End If
                        End If
                        Set aEnt = Nothing
                    End If
                End If
                Set aGUnd = Nothing
            End If
        End If
        Erase arrValues
    Next
    
    FilterUpdateAll
    tmrShow.Enabled = True
    Exit Sub
EH:
    Erase arrValues
    gCmn.ErrorHandler ""
End Sub

Public Sub Reload()
    On Error Resume Next
    Dim aEnt As EtsGeneralLib.EtsMmEntityAtom, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
    
    m_nOpenedExpiry = m_nFilter(AFC_EXPIRY)
    
    m_UndLoadedValues.Clear
    For Each aUnd In m_Und
        Set aEnt = m_UndLoadedValues.Add(CStr(aUnd.ID))
        aEnt.ID = aUnd.ID
        aEnt.Name = aUnd.Symbol
        aEnt.dValue1 = aUnd.LoadValue1
        aEnt.dValue2 = aUnd.LoadValue2
        aEnt.sValue1 = aUnd.CalcExpression1
        aEnt.sValue2 = aUnd.CalcExpression2
        Set aEnt = Nothing
    Next
    
    tmrShow.Enabled = True
End Sub

Public Sub Refresh()
    On Error Resume Next
    If m_Grp.ID <> 0 And Not m_bInProc And Not m_bLastQuoteReqNow Then RequestLastUndQuotes
    SetRefreshHint False
    lblStatus.Caption = CStr(m_Und.Count) & " underlying(s)"
End Sub

Public Function Group() As EtsGeneralLib.EtsMmEntityAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom

    CurUnderlyingID = 0

    If m_Grp.Data = TYPE_UNDERLYING Then
        CurUnderlyingID = m_Grp.ID
    Else
        With fgUnd
            If .Row > 0 Then
                Set aUnd = .RowData(.Row).Und
                If Not aUnd Is Nothing Then
                    CurUnderlyingID = aUnd.ID
                    Set aUnd = Nothing
                End If
            End If
        End With
    End If
End Function

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_VOLAANALYSIS_FILTER, AFC_GROUP, AFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeUndGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_VOLAANALYSIS_UNDERLYING, AUC_ROW, AUC_LAST_COLUMN, m_gdUnd, m_frmOwner
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
        Case GT_VOLAANALYSIS_FILTER
            gdGrid.CopyTo m_gdFlt

            m_GridLock(GT_VOLAANALYSIS_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_VOLAANALYSIS_FILTER).UnlockRedraw

        Case GT_VOLAANALYSIS_UNDERLYING
            gdGrid.CopyTo m_gdUnd

            m_GridLock(GT_VOLAANALYSIS_UNDERLYING).LockRedraw
            FormatUndGrid
            FormatUndColumns
            UnderlyingsUpdate True
            m_GridLock(GT_VOLAANALYSIS_UNDERLYING).UnlockRedraw

    End Select

    UserControl_Resize
End Sub

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

    Set frmLayout = Nothing
    Set VolaSource = Nothing

    Set geUnd = Nothing

    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        Set PriceProvider = Nothing
    End If

    m_UndLoadedValues.Clear
    Set m_UndRequests = Nothing
    Set m_OptRequests = Nothing
    Set m_Grp = Nothing
    Set m_Und = Nothing
    Set m_Exp = Nothing

    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set imgStop = Nothing
    Set imgStopDis = Nothing
    Set aParams = Nothing
End Sub

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_nFilter(AFC_GROUP) = TYPE_UNDERLYING Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then FilterUpdateValue False
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If m_nFilter(AFC_GROUP) = TYPE_GROUP Then FilterUpdateValue False
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If m_nFilter(AFC_GROUP) = TYPE_STRATEGY Then FilterUpdateValue False
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Analysis", "Volatility Analysis", fgFlt, fgUnd
'    Screen.MousePointer = vbDefault
End Sub

Public Sub PortfolioCreate()
    On Error Resume Next
    Dim aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom, aPair As EtsMmVolaAnalysisLib.MmVaOptPairAtom, aOpt As EtsMmVolaAnalysisLib.MmVaOptAtom
    Dim collTrades As New EtsGeneralLib.MmTradeInfoColl, aTrd As EtsGeneralLib.MmTradeInfoAtom, enOptType As EtsOptionTypeEnum
    Dim bAllNotChecked As Boolean, frmNewPortfolio As frmNewPortfolioTrades, nTradeID&
    
    If m_Grp.ID = 0 Or m_bInProc Or m_bLastQuoteReqNow Or m_Und.Count <= 0 Then Exit Sub
    
    bAllNotChecked = True
    
    pbProgress.Min = 0
    pbProgress.Max = m_Und.Count
    pbProgress.Value = 0
    pbProgress.Visible = True

    lblStatus.Visible = False
    lblProcess.Visible = True
    lblProcess.Caption = "Build trades list..."
    imgStop.Visible = False
    imgStopDis.Visible = True
    DoEvents
    
    For Each aUnd In m_Und
        If aUnd.NewTradeType <> enMmVaNewTradeNone Then
            bAllNotChecked = False
            
            Set aPair = aUnd.CurExpiry.AtmStrike.CurOptPair
            
            If Not aPair Is Nothing Then
                
                If (aUnd.NewTradeType = enMmVaNewTradeBuy _
                            And aPair.ContractsAsk > BAD_LONG_VALUE _
                            And aPair.DeltaInSharesAsk > BAD_LONG_VALUE _
                    Or aUnd.NewTradeType = enMmVaNewTradeSell _
                            And aPair.ContractsBid > BAD_LONG_VALUE _
                            And aPair.DeltaInSharesBid > BAD_LONG_VALUE) _
                    And aPair.Opt(enOtCall).ID <> 0 And aPair.Opt(enOtPut).ID <> 0 Then
                
                    For enOptType = enOtPut To enOtCall
                        Set aOpt = aPair.Opt(enOptType)
                        nTradeID = nTradeID + 1
                        
                        Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
                        aTrd.TradeID = nTradeID
                        
                        Set aTrd.Und = g_UnderlyingAll(aUnd.ID)
                        
                        If aUnd.NewTradeType = enMmVaNewTradeBuy Then
                            aTrd.IsBuy = True
                            aTrd.price = aOpt.PriceAsk
                            aTrd.Quantity = Abs(aPair.ContractsAsk)
                            aTrd.SpotReference = aUnd.PriceLast 'aUnd.PriceAsk
                            aTrd.TradedIV = aOpt.IVAsk
                        Else
                            aTrd.IsBuy = False
                            aTrd.price = aOpt.PriceBid
                            aTrd.Quantity = Abs(aPair.ContractsBid)
                            aTrd.SpotReference = aUnd.PriceLast 'aUnd.PriceBid
                            aTrd.TradedIV = aOpt.IVBid
                        End If
                    
                        aTrd.TradeDate = Now
                        aTrd.IsPosition = 0
                        aTrd.Status = enTsManual
                        aTrd.Mark = 0
            
                        Set aTrd.OptRoot = aTrd.Und.Roots(aPair.RootID)
                        Set aTrd.Opt = New EtsGeneralLib.EtsOptAtom
                        
                        aTrd.Opt.ID = aOpt.ID
                        aTrd.Opt.Symbol = aOpt.Symbol
                        aTrd.Opt.RootID = aTrd.OptRoot.ID
                        aTrd.Opt.OptType = aOpt.OptType
                        aTrd.Opt.Expiry = aOpt.Expiry
                        aTrd.Opt.ExpiryOV = aOpt.ExpiryOV
                        aTrd.Opt.Strike = aOpt.Strike
                        aTrd.Opt.PriceClose = aOpt.PriceClose
                        aTrd.Opt.PriceTheoclose = aOpt.PriceTheoclose
                        
                        collTrades.Add aTrd.TradeID, nTradeID, aTrd
                        Set aTrd = Nothing
                        Set aOpt = Nothing
                    Next
                    
                    If aUnd.NewTradeType = enMmVaNewTradeBuy _
                                And Abs(aPair.DeltaInSharesAsk) > 0 _
                        Or aUnd.NewTradeType = enMmVaNewTradeSell _
                                And Abs(aPair.DeltaInSharesBid) > 0 Then
                                
                        nTradeID = nTradeID + 1
                        
                        Set aTrd = New EtsGeneralLib.MmTradeInfoAtom
                        aTrd.TradeID = nTradeID
                        
                        Set aTrd.Und = g_UnderlyingAll(aUnd.ID)
                        aTrd.IsPosition = 0
                        aTrd.Status = enTsManual
                        
                        If aUnd.NewTradeType = enMmVaNewTradeBuy Then
                            If aPair.DeltaInSharesAsk > 0# Then
                                aTrd.IsBuy = False
                                aTrd.price = IIf(aUnd.PriceBid > 0#, aUnd.PriceBid, aUnd.PriceLast)
                            Else
                                aTrd.IsBuy = True
                                aTrd.price = IIf(aUnd.PriceAsk > 0#, aUnd.PriceAsk, aUnd.PriceLast)
                            End If
                            aTrd.Quantity = Abs(aPair.DeltaInSharesAsk)
                        Else
                            If aPair.DeltaInSharesBid > 0# Then
                                aTrd.IsBuy = False
                                aTrd.price = IIf(aUnd.PriceBid > 0#, aUnd.PriceBid, aUnd.PriceLast)
                            Else
                                aTrd.IsBuy = True
                                aTrd.price = IIf(aUnd.PriceAsk > 0#, aUnd.PriceAsk, aUnd.PriceLast)
                            End If
                            aTrd.Quantity = Abs(aPair.DeltaInSharesBid)
                        End If
                    
                        aTrd.SpotReference = aTrd.price
                        aTrd.TradedIV = 0#
                
                        collTrades.Add aTrd.TradeID, nTradeID, aTrd
                        Set aTrd = Nothing
                    End If
                End If
                
                Set aPair = Nothing
            End If
        End If
        
        IncProgress pbProgress
        DoEvents
    Next
    
    If bAllNotChecked Then gCmn.MyMsgBox m_frmOwner, "Please select type of new trade for underlyings.", vbCritical: GoTo Ex
    If collTrades.Count <= 0 Then gCmn.MyMsgBox m_frmOwner, "No trades can be created.", vbCritical: GoTo Ex
    
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    On Error GoTo EH
    
    Set frmNewPortfolio = New frmNewPortfolioTrades
    
    If frmNewPortfolio.Execute(collTrades, m_frmOwner) = vbOK Then
        pbProgress.Min = 0
        pbProgress.Max = collTrades.Count
        pbProgress.Value = 0
        pbProgress.Visible = True
    
        lblStatus.Visible = False
        lblProcess.Visible = True
        lblProcess.Caption = "Generate trades..."
        imgStop.Visible = False
        imgStopDis.Visible = True
        DoEvents
        
        nTradeID = 1
        For Each aTrd In collTrades
            
            If Not g_TradeChannel.NewTrade(aTrd) Then
                gCmn.MyMsgBox m_frmOwner, CStr(nTradeID) & " of " & CStr(collTrades.Count) & " trade(s) were created.", vbExclamation
                GoTo Ex
            End If
            nTradeID = nTradeID + 1
            
            IncProgress pbProgress
            DoEvents
        Next
    End If

Ex:
    pbProgress.Visible = False
    lblStatus.Visible = True
    lblProcess.Visible = False
    lblProcess.Caption = ""
    imgStop.Visible = False
    imgStopDis.Visible = False
    
    Set frmNewPortfolio = Nothing
    Set aTrd = Nothing
    Exit Sub
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to create new portfolio."
    GoTo Ex
End Sub

Public Function UnderlyingCount() As Long
    On Error Resume Next
    UnderlyingCount = m_Und.Count
End Function

Public Sub SaveStocksAsGroup()
    On Error GoTo EH
    Dim frmEdit As frmEntityEditor, aUnd As EtsMmVolaAnalysisLib.MmVaUndAtom
    Dim sName$, sDescription$, nGroupID&, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    
    If m_Grp.ID <> 0 And m_Grp.Data = TYPE_FILE And m_Und.Count > 0 Then
        Set frmEdit = New frmEntityEditor
        Do While frmEdit.Execute("New Group", sName, sDescription, 30, 120, m_frmOwner) = vbOK
            nGroupID = gDBW.usp_ContractGroup_Save(Null, sName, sDescription, 0)
            If nGroupID <> -1 Then
                
                Set aUndGroup = g_UnderlyingGroup(nGroupID)
                If aUndGroup Is Nothing Then
                    Set aUndGroup = New EtsGeneralLib.EtsUndGroupAtom
                    aUndGroup.ID = nGroupID
                    aUndGroup.Name = sName
                    
                    g_UnderlyingGroup.Add aUndGroup.ID, aUndGroup.Name, aUndGroup
                End If
                
                aUndGroup.Und.Clear
                
                For Each aUnd In m_Und
                    gDBW.usp_ContractInGroupByContract_Save aUnd.ID, nGroupID
                    
                    If aUndGroup.Und(aUnd.ID) Is Nothing Then
                        aUndGroup.Und.Add aUnd.ID, aUnd.Symbol, aUnd
                    End If
                Next
            
                g_Params.FireNewUnderlyingGroupAdded nGroupID
                
                Exit Do
            Else
                gCmn.MyMsgBox m_frmOwner, "Underlyings group with this name already exists! Please enter other group name.", vbCritical + vbOKOnly
            End If
        Loop
        
        Set frmEdit = Nothing
        Set aUndGroup = Nothing
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to save new underlyings group."
    On Error Resume Next
    Set frmEdit = Nothing
    Set aUndGroup = Nothing
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



