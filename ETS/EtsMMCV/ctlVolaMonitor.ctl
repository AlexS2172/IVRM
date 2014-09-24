VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlVolaMonitor 
   ClientHeight    =   7635
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9900
   KeyPreview      =   -1  'True
   ScaleHeight     =   7635
   ScaleWidth      =   9900
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
      Top             =   720
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
   Begin VB.Image imgBadPrice 
      Height          =   240
      Left            =   2640
      Picture         =   "ctlVolaMonitor.ctx":0000
      Top             =   3720
      Width           =   75
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxOpenVM 
         Caption         =   "Open Volatility Manager..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxVolaShiftSel 
         Caption         =   "Shift Volatility..."
         Shortcut        =   {F6}
      End
      Begin VB.Menu mnuCtxVolaShiftAll 
         Caption         =   "Shift Volatility for All..."
         Shortcut        =   +{F6}
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxSeparator2 
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
      Begin VB.Menu mnuCtxHideCol 
         Caption         =   "Hide Column"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlVolaMonitor"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Public Event OnSetCaption()
Public Event OnStateChange()

Private m_gdFlt As New clsGridDef
Private m_gdUnd As New clsGridDef

Private WithEvents geUnd As clsGridEx
Attribute geUnd.VB_VarHelpID = -1

Private WithEvents PriceProvider As PRICEPROVIDERSLib.BatchPriceInfo
Attribute PriceProvider.VB_VarHelpID = -1
Private m_GroupPriceProvider As PRICEPROVIDERSLib.IGroupPrice
'Private WithEvents VolaSource As VolatilitySourcesLib.VolatilitySource
Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private VolaSource As VolatilitySourcesLib.VolatilitySource

Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1
Private WithEvents frmVola As frmVolaShift
Attribute frmVola.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label
Public WithEvents imgStop As VB.Image
Attribute imgStop.VB_VarHelpID = -1
Public imgStopDis As VB.Image

Private m_bInProc As Boolean
Private m_bDataLoad As Boolean
Private m_bLastQuoteReqNow As Boolean

Private m_nOpenedExpiry1 As Long
Private m_nOpenedExpiry2 As Long

Private m_QuoteRequests As New clsVmRequestColl
Private m_Grp As New EtsGeneralLib.EtsMmEntityAtom
Private m_Und As New clsVmUndColl
Private m_Exp As New EtsGeneralLib.EtsMmEntityAtomColl

Private m_nLastQuoteReqCount As Long
Private m_nLastQuoteReqDone As Long

Public m_frmOwner As Form
Private m_nFilter(VFC_GROUP To VFC_COLUMN_COUNT) As Long

Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bGroupRequest As Boolean
Private m_nLastQuoteGroupReqCount As Long
Private m_nLastQuoteGroupReqDone As Long

Private m_bKeyDown(GT_VOLA_FILTER To GT_VOLA_UNDERLYING) As Boolean
Private m_GridLock(GT_VOLA_FILTER To GT_VOLA_UNDERLYING) As New clsGridLock

Private m_bShutDown As Boolean
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Public Function Init() As Boolean
    On Error GoTo EH
    
    m_bShutDown = False
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_VOLA_FILTER) = False
    m_bKeyDown(GT_VOLA_UNDERLYING) = False
    
    m_GridLock(GT_VOLA_FILTER).Init fgFlt
    m_GridLock(GT_VOLA_UNDERLYING).Init fgUnd

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
    Set TradeChannel = g_TradeChannel
    
    Dim aPT As PRICEPROVIDERSLib.IProvider
    Set PriceProvider = New PRICEPROVIDERSLib.BatchPriceInfo
    
    Set aPT = PriceProvider
    aPT.Type = g_Params.PriceProviderType
    Set aPT = Nothing
    
    PriceProvider.Connect
    
    m_bGroupRequest = False ' g_Params.PriceProviderIsGroupRequestSupported
    If m_bGroupRequest Then
        Set m_GroupPriceProvider = PriceProvider
        If m_GroupPriceProvider Is Nothing Then m_bGroupRequest = False
    Else
        Set m_GroupPriceProvider = Nothing
    End If
    
    fgFlt.Col = VFC_GROUP
    fgFlt.Row = 1
    
    Set frmLayout = New frmGridLayout
    Set frmVola = New frmVolaShift
    Set aParams = g_Params
    
    Init = True
    UpdateTotals
    
    AdjustCaption
    AdjustState
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to initialize volatility monitor."
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    m_nFilter(VFC_GROUP) = nType
    m_nFilter(VFC_VALUE) = nValue
    FilterUpdateAll
    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdateAll()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
        
        If m_nFilter(VFC_GROUP) <> TYPE_UNDERLYING _
            And m_nFilter(VFC_GROUP) <> TYPE_GROUP _
            And m_nFilter(VFC_GROUP) <> TYPE_TRADER Then
            
            m_nFilter(VFC_GROUP) = TYPE_UNDERLYING
            m_nFilter(VFC_VALUE) = 0
        End If
        .TextMatrix(1, VFC_GROUP) = m_nFilter(VFC_GROUP)
        
        FilterUpdateValue False
        FilterUpdateExpiry False
        FilterUpdateModel False

        .TextMatrix(1, VFC_VOLA_1) = m_nFilter(VFC_VOLA_1)
        .TextMatrix(1, VFC_VOLA_2) = m_nFilter(VFC_VOLA_2)
        .TextMatrix(1, VFC_VEGA) = m_nFilter(VFC_VEGA)
        .TextMatrix(1, VFC_GAMMA) = m_nFilter(VFC_GAMMA)
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateValue(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom
    Dim nValue&
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = ""
        
        Select Case m_nFilter(VFC_GROUP)
            Case TYPE_UNDERLYING
                sComboList = g_Params.UnderlyingComboList
                
                Set aUnd = g_Underlying(m_nFilter(VFC_VALUE))
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
                    If aTrader.ID = m_nFilter(VFC_VALUE) Then
                        nValue = aTrader.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
        
            Case TYPE_GROUP
                Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom
                
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(VFC_VALUE) Then
                        nValue = aUndGroup.ID
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
        
        .ColComboList(VFC_VALUE) = sComboList
        .TextMatrix(1, VFC_VALUE) = sValue
        m_nFilter(VFC_VALUE) = nValue
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FilterUpdateExpiry(ByVal bAutosize As Boolean)
    On Error Resume Next
    Dim nValue1&, sValue1$, nValue2&, sValue2$
    Dim aExp As EtsGeneralLib.EtsMmEntityAtom, sComboList$, nIdx&
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
        
        sValue1 = "0"
        nValue1 = 0
        sValue2 = "0"
        nValue2 = 0
        
        If m_Exp.Count > 0 Then
            For Each aExp In m_Exp
                sComboList = sComboList & "|#" & Trim$(Str$(aExp.ID)) & ";" & aExp.Name
                If aExp.ID = m_nFilter(VFC_EXPIRY_1) Then
                    sValue1 = Trim$(Str$(aExp.ID))
                    nValue1 = aExp.ID
                End If
                If aExp.ID = m_nFilter(VFC_EXPIRY_2) Then
                    sValue2 = Trim$(Str$(aExp.ID))
                    nValue2 = aExp.ID
                End If
            Next
        
            sComboList = Mid$(sComboList, InStr(1, sComboList, "|") + 1)
            If nValue1 = 0 Then
                nValue1 = m_Exp(1).ID
                sValue1 = Trim$(Str$(nValue1))
            End If
        
            If nValue2 = 0 Then
                nIdx = 1
                If m_Exp.Count > 1 Then nIdx = 2
                nValue2 = m_Exp(nIdx).ID
                sValue2 = Trim$(Str$(nValue2))
            End If
        Else
            sComboList = "#0;<None>"
        End If
        
        .ColComboList(VFC_EXPIRY_1) = sComboList
        .TextMatrix(1, VFC_EXPIRY_1) = sValue1
        m_nFilter(VFC_EXPIRY_1) = nValue1
        
        .ColComboList(VFC_EXPIRY_2) = sComboList
        .TextMatrix(1, VFC_EXPIRY_2) = sValue2
        m_nFilter(VFC_EXPIRY_2) = nValue2
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
    
        .TextMatrix(1, VFC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If bAutosize Then
            .AutoSize 0, .Cols - 1, , 100
        End If
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
        
        .ColComboList(VFC_GROUP) = "#" & CStr(TYPE_UNDERLYING) & ";Underlying" & _
                                    "|#" & CStr(TYPE_GROUP) & ";Group" & _
                                    "|#" & CStr(TYPE_TRADER) & ";Trader"

        .TextMatrix(1, VFC_GROUP) = CStr(TYPE_UNDERLYING)
        m_nFilter(VFC_GROUP) = TYPE_UNDERLYING
        
        .ColComboList(VFC_VALUE) = "#0;<None>"
        .TextMatrix(1, VFC_VALUE) = "0"
        m_nFilter(VFC_VALUE) = 0
    
        .ColComboList(VFC_EXPIRY_1) = "#0;<None>"
        .TextMatrix(1, VFC_EXPIRY_1) = "0"
        m_nFilter(VFC_EXPIRY_1) = 0
    
        .ColComboList(VFC_EXPIRY_2) = "#0;<None>"
        .TextMatrix(1, VFC_EXPIRY_2) = "0"
        m_nFilter(VFC_EXPIRY_2) = 0
    
        .ColComboList(VFC_VOLA_1) = "#0;<All>|#1;Vol > IVol|#2;Vol < IVol"
        .TextMatrix(1, VFC_VOLA_1) = "0"
        m_nFilter(VFC_VOLA_1) = 0
    
        .ColComboList(VFC_VOLA_2) = "#0;<All>|#1;Vol > IVol|#2;Vol < IVol"
        .TextMatrix(1, VFC_VOLA_2) = "0"
        m_nFilter(VFC_VOLA_2) = 0
    
        .ColComboList(VFC_VEGA) = "#0;<All>|#1;Long Vega|#2;Short Vega"
        .TextMatrix(1, VFC_VEGA) = "0"
        m_nFilter(VFC_VEGA) = 0
    
        .ColComboList(VFC_GAMMA) = "#0;<All>|#1;Long Gamma|#2;Short Gamma"
        .TextMatrix(1, VFC_GAMMA) = "0"
        m_nFilter(VFC_GAMMA) = 0
    
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = VFC_COLUMN_COUNT
        
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
        .AllowSelection = False
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
        .FocusRect = flexFocusHeavy
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
    End With
End Sub

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_VOLA_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_VOLA_UNDERLYING).CopyTo m_gdUnd
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
    
        nCols = VFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = VFC_GROUP To VFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_VOLA_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = VFC_GROUP To VFC_LAST_COLUMN
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
        
        m_GridLock(GT_VOLA_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgUnd
        m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
    
        m_gdUnd.Idx(0) = VUC_SYMBOL
        nCols = 1
        For i = 1 To VUC_LAST_COLUMN
            If m_gdUnd.Idx(i) <> VUC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColHidden(VUC_KEY) = True
        .ColKey(VUC_KEY) = VUC_NONE
        .ColDataType(VUC_KEY) = flexDTString
        
        Set .Cell(flexcpPicture, 1, 0, .Rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        For i = 0 To VUC_LAST_COLUMN
            nIdx = m_gdUnd.Idx(i)
            If nIdx <> VUC_NONE Then
                .TextMatrix(0, i + 1) = m_gdUnd.Col(nIdx).Caption
                .ColFormat(i + 1) = m_gdUnd.Col(nIdx).Format
                .ColDataType(i + 1) = m_gdUnd.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(m_gdUnd.Col(nIdx).Width > 0, m_gdUnd.Col(nIdx).Width, -1)
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        geUnd.AdjustSortImages
        
        m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub FormatUndColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgUnd
        m_GridLock(GT_VOLA_UNDERLYING).LockRedraw

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
            If nIdx <> VUC_NONE Then
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

'        .AutoSize 1, .Cols - 1

        m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
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
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    'mnuCtxOpenVM           "Open Volatility Manager..."
    'mnuCtxVolaShiftSel     "Shift Volatility..."
    'mnuCtxVolaShiftAll     "Shift Volatility for All..."
    'mnuCtxTradeNew         "New Trade..."
    'mnuCtxOrderNewStock    "New Stock Order..."
    'mnuCtxOrderNewOption   "New Option Order..."
    'mnuCtxRefresh          "Refresh"
    'mnuCtxCopy             "Copy"
    'mnuCtxHideCol          "Hide Column"
    'mnuCtxAutosizeCol      "Autosize Column"
    'mnuCtxAutosizeGrid     "Autosize Grid"
    'mnuCtxGridLayout       "Grid Layout..."
    
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    Select Case m_enMenuGrid
        Case GT_VOLA_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_nFilter(VFC_GROUP) = TYPE_UNDERLYING And m_nFilter(VFC_VALUE) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            
            mnuCtxOpenVM.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxVolaShiftSel.Enabled = False
            mnuCtxVolaShiftAll.Enabled = False
            
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
        Case GT_VOLA_UNDERLYING
            mnuCtxTradeNew.Enabled = (Not m_bInProc _
                                        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                        And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows)
            mnuCtxOpenVM.Enabled = mnuCtxTradeNew.Enabled
            
            mnuCtxVolaShiftSel.Enabled = mnuCtxOpenVM.Enabled And Not m_bLastQuoteReqNow
            mnuCtxVolaShiftAll.Enabled = mnuCtxVolaShiftSel.Enabled
            
            mnuCtxHideCol.Enabled = (m_nMenuGridCol > 1 And m_nMenuGridCol < m_nMenuGridCols And m_gdUnd.Idx(1) <> VUC_NONE)
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True
            
            PopupMenu mnuCtx, , , , mnuCtxTradeNew
        
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
    sCaption = sCaption & "Volatility Monitor"
    
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
    m_QuoteRequests.Clear
    
    FilterUpdateExpiry True
End Sub

Private Sub SetRefreshHint(ByVal bSet As Boolean)
    On Error Resume Next
    lblStatus.Visible = True
    lblStatus.Caption = IIf(bSet, "Use F5 to refresh results.", "")
End Sub

Private Sub aParams_PriceProfilesChange()
    If m_Grp.Data <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_PriceRoundingRuleChange()
    If m_Grp.Data <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub aParams_UnderlyingsParamsChange(collUpdUnd As EtsGeneralLib.EtsMmFilterAtomColl)
    Dim bUpdate As Boolean, aFilterAtom As EtsGeneralLib.EtsMmFilterAtom
    
    If m_Grp.Data = 0 Or m_Grp.ID = 0 Then Exit Sub
    
    bUpdate = False
    On Error Resume Next
    
    For Each aFilterAtom In collUpdUnd
        If Not m_Und(CStr(aFilterAtom.ID)) Is Nothing Then
            bUpdate = True
            Set aFilterAtom = Nothing
            Exit For
        End If
        Set aFilterAtom = Nothing
    Next
    If bUpdate Then SetRefreshHint True
End Sub

Private Sub aParams_UndPriceToleranceChange()
    If m_Grp.Data <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    With fgFlt
        m_enMenuGrid = GT_VOLA_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgUnd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nKey&, sValue$, dValue#, aUnd As clsVmUndAtom, bCalcGreeks As Boolean
    
    With fgUnd
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nKey = .ColKey(Col)
            If Err.Number = 0 Then
                bCalcGreeks = False
                Set aUnd = .RowData(Row)
                
                If Not aUnd Is Nothing Then
                    dValue = Abs(ReadDbl(sValue))

                    Select Case nKey
                        Case VUC_BID
                            If aUnd.PriceBid <> dValue Then
                                aUnd.PriceBid = dValue
                                bCalcGreeks = True
                            End If
                            
                        Case VUC_ASK
                            If aUnd.PriceAsk <> dValue Then
                                aUnd.PriceAsk = dValue
                                bCalcGreeks = True
                            End If
                    
                    End Select
                    
                    If bCalcGreeks Then
                        m_bInProc = True
                        UnderlyingCalc aUnd, True
                        m_bInProc = False
                        
                        UnderlyingUpdate Row, False, aUnd
                        
                        AdjustState
                        UpdateTotals
                        
'                        .AutoSize 0, .Cols - 1
                    End If
                End If
            End If
        End If
    End With
End Sub

Private Sub fgUnd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        With fgUnd
            nIdx = .ColKey(Col)
            If nIdx >= VUC_SYMBOL And nIdx <= VUC_LAST_COLUMN Then
                m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nKey&, aUnd As clsVmUndAtom
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    If m_bInProc Or m_bDataLoad Or m_bLastQuoteReqNow Then Exit Sub
    
    If m_Grp.ID <> 0 Then
        With fgUnd
            nKey = .ColKey(Col)
            If Err.Number = 0 Then
                Cancel = Not m_gdUnd.Col(nKey).CanEdit
                If Not Cancel Then
                    Set aUnd = .RowData(Row)
                    Cancel = aUnd Is Nothing
                    Set aUnd = Nothing
                End If
            End If
            
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End With
    End If
End Sub

Private Sub frmVola_OnOK(ByVal nKey As Long, ByVal bShiftAllExpiries As Boolean, ByVal dShiftValue As Double)
    On Error Resume Next
    Dim aUnd As clsVmUndAtom, i&, nCount&, nRow&
    If dShiftValue = 0# Or dShiftValue < -100# Or dShiftValue > 100# Then Exit Sub
    
    Screen.MousePointer = vbHourglass
    DoEvents
    If nKey = 0 Then
        nCount = m_Und.Count
        lblStatus.Visible = False
        imgStop.Visible = False
        imgStopDis.Visible = True
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = nCount
        pbProgress.Visible = True
        lblProcess.Caption = "Vola shifting..."
        lblProcess.Visible = True
        lblProcess.Refresh
        
        For Each aUnd In m_Und
            ShiftVola aUnd, bShiftAllExpiries, dShiftValue
            IncProgress pbProgress
        Next
    
        pbProgress.Value = 0
        lblProcess.Caption = "Calculation..."
        lblProcess.Refresh
        
        m_bInProc = True
        UnderlyingsCalc True, True
        m_bInProc = False
        
        UnderlyingsUpdate False
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
    Else
        Set aUnd = m_Und(CStr(nKey))
        If Not aUnd Is Nothing Then
            ShiftVola aUnd, bShiftAllExpiries, dShiftValue
            
            nRow = fgUnd.FindRow(aUnd.ID, , VUC_KEY)
            If nRow > 0 Then
                m_bInProc = True
                UnderlyingCalc aUnd, True
                m_bInProc = False
                
                UnderlyingUpdate nRow, False, aUnd
            End If
            
            Set aUnd = Nothing
        End If
    End If
    Screen.MousePointer = vbDefault
End Sub

Private Sub ShiftVola(ByRef aUnd As clsVmUndAtom, ByVal bShiftAllExpiries As Boolean, ByVal dShiftValue As Double)
    On Error Resume Next
    Dim e&, nExpCount&, bAlreadyShifted As Boolean
    Dim lSurfaceID As Long
    
    Dim aExpAtom As clsVmExpiryAtom
    Dim aStrikeAtom As clsVmStrikeAtom
    
    If aUnd Is Nothing Then Exit Sub
    
    If bShiftAllExpiries Then
        nExpCount = aUnd.Expiry.Count
        For e = 1 To nExpCount
            Err.Clear
            lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(0)
            aUnd.VolaSrv.ShiftExpiryVola aUnd.Expiry(e).ExpiryOV, dShiftValue, lSurfaceID
            If Err.Number <> 0 Then LogEvent EVENT_ERROR, aUnd.Symbol & ": " & Err.Description
        Next
    Else
        If Not aUnd.CurExpiry1 Is Nothing Then
            Err.Clear
            lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(0)
            aUnd.VolaSrv.ShiftExpiryVola aUnd.CurExpiry1.ExpiryOV, dShiftValue, lSurfaceID
            If Err.Number <> 0 Then LogEvent EVENT_ERROR, aUnd.Symbol & ": " & Err.Description
        End If
    
        If Not aUnd.CurExpiry2 Is Nothing Then
            bAlreadyShifted = False
            
            If Not aUnd.CurExpiry1 Is Nothing Then
                If aUnd.CurExpiry2.ExpiryOV = aUnd.CurExpiry1.ExpiryOV Then bAlreadyShifted = True
            End If
            
            If Not bAlreadyShifted Then
                Err.Clear
                lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(0)
                aUnd.VolaSrv.ShiftExpiryVola aUnd.CurExpiry2.ExpiryOV, dShiftValue, lSurfaceID
                If Err.Number <> 0 Then LogEvent EVENT_ERROR, aUnd.Symbol & ": " & Err.Description
            End If
        End If
    End If

    g_Params.FireInternalVolatilityChanged aUnd.Symbol
End Sub

Private Sub imgStop_Click()
    On Error Resume Next
    If m_bLastQuoteReqNow Then
        m_bLastQuoteReqNow = False
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
        
        UnderlyingsUpdate False
        UnderlyingsShow False
        
        UpdateTotals
        AdjustState
        
    ElseIf m_bDataLoad Then
        m_bDataLoad = False
        AdjustState
    End If
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = ReadLng(sValue)
            Select Case Col
                Case VFC_GROUP
                    m_nFilter(VFC_VALUE) = 0
                    m_nFilter(VFC_GROUP) = nValue
                    ClearViewAndData
                    FilterUpdateValue True
                    AdjustCaption
                    
                Case VFC_VALUE
                    If m_nFilter(VFC_VALUE) = 0 Then
                        sValue = .ColComboList(Col)
                        sValue = Mid$(sValue, InStr(1, sValue, "|") + 1)
                        .ColComboList(Col) = sValue
                        .TextMatrix(Row, Col) = CStr(nValue)
                    End If
                    m_nFilter(VFC_VALUE) = nValue
                    
                    tmrShow.Enabled = True
                    
                Case VFC_EXPIRY_1, VFC_EXPIRY_2
                    m_nFilter(Col) = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    m_bInProc = True
                    UnderlyingsUpdateCurExp
                    UnderlyingsCalc False, False
                    m_bInProc = False
                    
                    UnderlyingsUpdate False
            
                Case VFC_VOLA_1, VFC_VOLA_2, VFC_VEGA, VFC_GAMMA
                    m_nFilter(Col) = nValue
                    .AutoSize 0, .Cols - 1, , 100
                    UnderlyingsShow False
            
            End Select
        End If
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_VOLA_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_VOLA_FILTER) Then
        m_bKeyDown(GT_VOLA_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_VOLA_FILTER
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
            m_enMenuGrid = GT_VOLA_FILTER
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
        If Not m_bInProc And Not m_bDataLoad And Not m_bLastQuoteReqNow And Col >= VFC_GROUP And Col <= VFC_LAST_COLUMN Then
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
    
    nOldPos = Col - VUC_SYMBOL
    nNewPos = Position - VUC_SYMBOL
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

Private Sub fgUnd_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    If OldRow <> NewRow Then UpdateTotals
End Sub

Private Sub fgUnd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fgUnd.ColKey(Col)
    nNewIdx = fgUnd.ColKey(Position)
    
    If nOldIdx = VUC_NONE Or nNewIdx = VUC_NONE Then
        Position = Col
    ElseIf Not m_gdUnd.Col(nOldIdx).CanChangeOrder Or Not m_gdUnd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgUnd_DblClick()
    On Error Resume Next
    With fgUnd
        m_enMenuGrid = GT_VOLA_UNDERLYING
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
        m_bKeyDown(GT_VOLA_UNDERLYING) = True
    End If
End Sub

Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_VOLA_UNDERLYING) Then
        m_bKeyDown(GT_VOLA_UNDERLYING) = False
        With fgUnd
            m_enMenuGrid = GT_VOLA_UNDERLYING
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
            m_enMenuGrid = GT_VOLA_UNDERLYING
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

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    Dim nIdx&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLA_UNDERLYING
            With fgUnd
                m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= VUC_SYMBOL And nIdx <= VUC_LAST_COLUMN Then
                    m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    Dim nIdx&, i&, nCount&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLA_UNDERLYING
            With fgUnd
                m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
                .AutoSize 0, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= VUC_SYMBOL And nIdx <= VUC_LAST_COLUMN Then
                        m_gdUnd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_VOLA_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_VOLA_UNDERLYING
            g_ClipMgr.CopyGridToClipboard fgUnd

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_VOLA_FILTER
            CustomizeFltGridLayout

        Case GT_VOLA_UNDERLYING
            CustomizeUndGridLayout
        
    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    Dim i&, nColIdx&, bMove As Boolean
    
    Select Case m_enMenuGrid
        Case GT_VOLA_UNDERLYING
            bMove = False
            
            nColIdx = fgUnd.ColKey(m_nMenuGridCol)
            For i = 1 To VUC_LAST_COLUMN
                If m_gdUnd.Idx(i) = nColIdx Then
                    m_gdUnd.Idx(i) = VUC_NONE
                    m_gdUnd.Col(nColIdx).Visible = False
                    bMove = True
                End If
                
                If bMove Then
                    If i + 1 <= VUC_LAST_COLUMN Then
                        m_gdUnd.Idx(i) = m_gdUnd.Idx(i + 1)
                    Else
                        m_gdUnd.Idx(i) = VUC_NONE
                    End If
                End If
            Next
            
            If bMove Then
                FormatUndGrid
                UnderlyingsUpdate False
                FormatUndColumns
            End If
            
    End Select
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByVal bForceStockNetDelta As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
    On Error Resume Next
    Dim aCurUnd As clsVmUndAtom
    
    bBuy = True
    dPrice = 0#
    nQty = IIf(bIsStock, 100, 10)
    
    If m_enMenuGrid = GT_VOLA_UNDERLYING Then _
        Set aCurUnd = fgUnd.RowData(m_nMenuGridRow)

    If aCurUnd Is Nothing And m_nFilter(VFC_GROUP) = TYPE_UNDERLYING Then _
        aCurUnd = m_Und(CStr(m_nFilter(VFC_VALUE)))

    Set aUnd = g_Underlying(aCurUnd.ID)
    If bIsStock And Not aUnd Is Nothing Then
        nQty = Round(aCurUnd.NetDelta / 100) * 100
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
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean, ByVal bForceStockNetDelta As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim aUnd As EtsGeneralLib.UndAtom, dPrice#, nQty&, bBuy As Boolean
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, bForceStockNetDelta, aUnd, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, bBuy, nQty, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, Nothing, bBuy, nQty, dPrice
'    End If
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Volatility Monitor View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
    OrderNew False, False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    OrderNew True, False
End Sub

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom, dPrice#, nQty&
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, False, aUnd, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, Nothing, False, 10&, 0#
'
'    Set aUnd = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Volatility Monitor View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
End Sub

Public Sub NewStockOrderAtNetDelta()
    On Error Resume Next
    OrderNew True, True
End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error GoTo EH
    If m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aUnd As clsVmUndAtom
    
    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True
    
    If m_enMenuGrid = GT_VOLA_UNDERLYING Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            On Error Resume Next
            Set aUnd = fgUnd.RowData(m_nMenuGridRow)
            On Error GoTo EH
        End If
        If Not aUnd Is Nothing Then
            bBuy = (g_Params.QuoteOtherColBuy <> 0)
            nUndID = aUnd.ID
            nID = aUnd.ID
            Set aUnd = Nothing
        Else
            If m_nFilter(VFC_GROUP) = TYPE_UNDERLYING Then
                nUndID = m_nFilter(VFC_VALUE)
                nID = nUndID
                bBuy = (g_Params.QuoteOtherColBuy <> 0)
            End If
        End If
        
    ElseIf m_enMenuGrid = GT_VOLA_FILTER Then
        If m_nFilter(VFC_GROUP) = TYPE_UNDERLYING And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_nFilter(VFC_VALUE)
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

Private Sub mnuCtxOpenVM_Click()
    On Error Resume Next
    Dim aUnd As clsVmUndAtom, sSymbol$
    
    sSymbol = ""
    
    Select Case m_enMenuGrid
        Case GT_VOLA_FILTER
            If m_Grp.Data = TYPE_UNDERLYING And m_Grp.ID <> 0 Then
                sSymbol = m_Grp.Name
            End If
            
        Case GT_VOLA_UNDERLYING
            With fgUnd
                If .Row > 0 Then
                    Set aUnd = .RowData(.Row)
                    If Not aUnd Is Nothing Then
                        sSymbol = aUnd.Symbol
                        Set aUnd = Nothing
                    End If
                End If
            End With
    End Select
    
    frmMain.RunVolaManager sSymbol
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_VOLA_FILTER
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Monitor Filter", "Volatility Monitor Filter", fgFlt
'
'        Case GT_VOLA_UNDERLYING
'            frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Monitor Underlyings", "Volatility Monitor Underlyings", fgUnd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub mnuCtxVolaShiftAll_Click()
    On Error Resume Next
    ShowShiftVolatilityAll
End Sub

Private Sub mnuCtxVolaShiftSel_Click()
    On Error Resume Next
    ShowShiftVolatility
End Sub

Public Sub ShowShiftVolatilityAll()
    On Error Resume Next
    If Not frmVola Is Nothing Then
        frmVola.Execute 0, "Volatility Shift: All Symbols", m_frmOwner
    End If
End Sub

Public Sub ShowShiftVolatility()
    On Error Resume Next
    Dim aUnd As clsVmUndAtom
    If Not frmVola Is Nothing Then
        If fgUnd.Row > 0 Then
            Set aUnd = fgUnd.RowData(fgUnd.Row)
            If Not aUnd Is Nothing Then
                frmVola.Execute aUnd.ID, "Volatility Shift: " & aUnd.Symbol, m_frmOwner
                Set aUnd = Nothing
            End If
        End If
    End If
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False

    If m_bDataLoad Then Exit Sub
    
    'Screen.MousePointer = vbArrow
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
    AdjustCaption
    
    If m_nFilter(VFC_GROUP) <> 0 And m_nFilter(VFC_VALUE) <> 0 Then
        lblStatus.Visible = False
        imgStop.Visible = True
        imgStopDis.Visible = False
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = 100
        pbProgress.Visible = True
        lblProcess.Caption = "Data loading..."
        lblProcess.Visible = True
        lblProcess.Refresh
        
        If UnderlyingsLoad Then
            AdjustCaption
            
            lblProcess.Caption = "Filter applying..."
            lblProcess.Refresh
            
            m_nFilter(VFC_EXPIRY_1) = m_nOpenedExpiry1
            m_nFilter(VFC_EXPIRY_2) = m_nOpenedExpiry2
            FilterUpdateExpiry False
            UnderlyingsShow True
        
            UpdateTotals
        
            lblProcess.Caption = "Last quotes request..."
            lblProcess.Refresh
            
            RequestLastQuotes
        Else
            m_nFilter(VFC_VALUE) = 0
            ClearViewAndData
            FilterUpdateValue True
            AdjustCaption
            AdjustState
            
            imgStop.Visible = False
            imgStopDis.Visible = False
            pbProgress.Visible = False
            lblProcess.Visible = False
            lblStatus.Visible = True
        
            UpdateTotals
        End If
    End If
    
    m_nOpenedExpiry1 = 0
    m_nOpenedExpiry2 = 0
    
    Screen.MousePointer = vbDefault
End Sub

Private Function UnderlyingsLoad() As Boolean
    On Error GoTo EH
    Dim rsUnd As ADODB.Recordset
    Dim rsOpt As ADODB.Recordset
    Dim dtExpiry As Date, sExpiryKey$, dStrike#, sStrikeKey$
    Dim dtExpiryOV, dtTradingClose As Date
    Dim nOptType&, sKey$, vPos As Variant, nUndCount&, nCurUndNum&
    Dim aGUnd As EtsGeneralLib.UndAtom, nRootID&
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aCustDivColl As EtsGeneralLib.EtsDivColl
    Dim dDate As Date
    Dim dAmount As Double
    Dim rsCust As ADODB.Recordset
    Dim lOptionRootID As Long
    Dim lSurfaceID As Long


    Dim aExpiry As clsVmExpiryAtom, aStrike As clsVmStrikeAtom, aOpt As clsVmOptAtom, aUnd As clsVmUndAtom
    Dim i&, j&, nCount&, nUndID&, nExpCount&, aEnt As EtsGeneralLib.EtsMmEntityAtom, nExpiryMonth&
    Dim tmpExpColl As New EtsGeneralLib.EtsMmEntityAtomColl
    
    pbProgress.Min = 0
    pbProgress.Value = pbProgress.Min
    DoEvents
    
    nUndCount = -1
    UnderlyingsLoad = False
    m_bDataLoad = True
    If Not m_bDataLoad Or m_nFilter(VFC_VALUE) = 0 Then GoTo Ex
    
    Select Case m_nFilter(VFC_GROUP)
        Case TYPE_UNDERLYING
            Set rsUnd = gDBW.usp_Underlying_Get(m_nFilter(VFC_VALUE), g_CurTraderID, , 1)
            Set rsOpt = gDBW.usp_UnderlyingOptions_Get(m_nFilter(VFC_VALUE), g_CurTraderID, , 1, Date)
            m_Grp.Name = g_Underlying(m_nFilter(VFC_VALUE)).Symbol
            
        Case TYPE_GROUP
            Set rsUnd = gDBW.usp_Underlying_Get(, g_CurTraderID, m_nFilter(VFC_VALUE), 1)
            Set rsOpt = gDBW.usp_UnderlyingOptions_Get(, g_CurTraderID, m_nFilter(VFC_VALUE), 1, Date)
            m_Grp.Name = g_UnderlyingGroup(m_nFilter(VFC_VALUE)).Name
            
        Case TYPE_TRADER
            Set rsUnd = gDBW.usp_Underlying_Get(, m_nFilter(VFC_VALUE), , 1)
            Set rsOpt = gDBW.usp_UnderlyingOptions_Get(, m_nFilter(VFC_VALUE), , 1, Date)
            m_Grp.Name = g_Trader(m_nFilter(VFC_VALUE)).Name
            
        Case Else
            Debug.Assert False
            GoTo Ex
    End Select
    
    m_Grp.ID = m_nFilter(VFC_VALUE)
    m_Grp.Data = m_nFilter(VFC_GROUP)
    
    If Not rsOpt Is Nothing Then
        If rsUnd.RecordCount + rsOpt.RecordCount > 0 Then
            pbProgress.Max = rsUnd.RecordCount + rsOpt.RecordCount
        End If
    End If

    DoEvents
    If Not m_bDataLoad Then GoTo Ex
    
    nCurUndNum = 0
    Do While Not rsUnd.EOF
        If nUndCount >= 0 Then
            nCurUndNum = nCurUndNum + 1
            If nCurUndNum > nUndCount Then Exit Do
        End If
        
        nUndID = ReadLng(rsUnd!iContractID)
        sKey = CStr(nUndID)
        Set aUnd = m_Und(sKey)
        If aUnd Is Nothing Then
            Set aUnd = m_Und.Add(sKey)
            aUnd.ID = nUndID
            aUnd.UndType = ReadLng(rsUnd!iContractTypeID)
            aUnd.Symbol = ReadStr(rsUnd!vcSymbol)
            
            aUnd.IsAmerican = (ReadByte(rsUnd!tiCalcOptionType) <> 0)
            aUnd.IsHTB = (ReadByte(rsUnd!tiIsHTB) <> 0)

            'aUnd.PriceClose = ReadDbl(rsUnd!fPriceClose)
            aUnd.LotSize = 1 'ReadLng(rsUnd!iLotSize)
            aUnd.Skew = ReadDbl(rsUnd!fSkew)
            aUnd.Kurt = ReadDbl(rsUnd!fKurt)
            
            
            Set aDiv = New EtsGeneralLib.EtsIndexDivAtom
                
            aDiv.DivType = (ReadLng(rsUnd!tiIsDivCustom))
            aDiv.DivFreq = ReadLng(rsUnd!iDivFreq)
            aDiv.DivDate = ReadDate(rsUnd!dtDivDate)
            aDiv.DivAmt = ReadDbl(rsUnd!fDivAmt)
            aDiv.DivFreqCust = ReadLng(rsUnd!iDivFreqCustom)
            aDiv.DivDateCust = ReadDate(rsUnd!dtDivDateCustom)
            aDiv.DivAmtCust = ReadDbl(rsUnd!fDivAmtCustom)
                
            ' -------------------------------------------------- '
            '   Try to get custom dividend for underlying        '
            Set rsCust = gDBW.usp_MmCustomDividend_Get(aUnd.ID)
            If Not rsCust.EOF Then
                                        
                Set aCustDivColl = New EtsGeneralLib.EtsDivColl
                    
                While Not rsCust.EOF
                    dDate = ReadDate(rsCust!DivYtes)
                    dAmount = ReadDbl(rsCust!DivAmnt)
                    aCustDivColl.AddNonUnique dDate, dAmount
                    rsCust.MoveNext
                Wend
                rsCust.Close
                Set aDiv.CustomDivs = aCustDivColl
                    
            End If
            Set rsCust = Nothing
            Set aCustDivColl = Nothing
            '   Try to get custom dividend for underlying-------- '
            ' --------------------------------------------------- '
            Set aUnd.Dividend = aDiv
            
            If (Not g_Main.Holidays Is Nothing) Then
                Set aDiv.Holidays = g_Main.Holidays
                If (Not aDiv.CustomDivs Is Nothing) Then
                    Set aDiv.CustomDivs.Holidays = g_Main.Holidays
                End If
            End If
            
            If aUnd.UndType = enCtIndex Then
                aUnd.Yield = ReadDbl(rsUnd!fYield)
                Set aUnd.BasketIndex = g_Index(aUnd.ID)
            End If
        
            vPos = rsUnd!iQty
            aUnd.QtyInShares = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
            vPos = rsUnd!fPosition
            aUnd.Pos = IIf(IsNull(vPos), BAD_DOUBLE_VALUE, ReadLng(vPos))
        
            InitVola aUnd
        
            sKey = CStr(aUnd.UndType) & "_" & aUnd.Symbol
            If m_QuoteRequests(sKey) Is Nothing Then
                m_QuoteRequests.Add aUnd, , sKey
            End If
        
            Set aGUnd = g_Underlying(aUnd.ID)
            If Not aGUnd Is Nothing Then
                Set aUnd.UndPriceProfile = aGUnd.UndPriceProfile
                Set aUnd.OptPriceProfile = aGUnd.OptPriceProfile
                Set aGUnd = Nothing
            End If
            
            aUnd.UndPosForRates = 0
        End If
        
        Set aUnd = Nothing
        
        DoEvents
        If Not m_bDataLoad Then GoTo Ex
        IncProgress pbProgress
        DoEvents
        
        rsUnd.MoveNext
    Loop
    
    nUndCount = m_Und.Count
    If nUndCount > 0 Then
        While Not rsOpt.EOF
            nUndID = ReadLng(rsOpt!iUnderlyingContractID)
            
            Set aGUnd = g_Underlying(nUndID)
            
            If Not aGUnd Is Nothing Then
                If aGUnd.HaveSyntheticRoots And Not aGUnd.SyntheticRoots Is Nothing Then
                    nRootID = ReadLng(rsOpt!iOptionRootID)
                    If Not aGUnd.SyntheticRoots(nRootID) Is Nothing Then
                        If Not IsNull(rsOpt!iPosition) Or Not IsNull(rsOpt!iQty) Then
                            LogEvent EVENT_INFO, CStr(rsOpt!vcSymbol) & ": Has been removed from calculation due to synthetic option type"
                        End If
                        GoTo NEXT_OPTION
                    End If
                End If
            End If
            
            Set aUnd = m_Und(CStr(nUndID))
            If Not aUnd Is Nothing Then
                dtExpiry = ReadDate(rsOpt!dtExpiry)
                dtExpiryOV = ReadDate(rsOpt!dtExpiryOV)
                dtTradingClose = ClipDays(ReadDate(rsOpt!dtTradingClose))
                nExpiryMonth = DateSerial(Year(dtExpiry), Month(dtExpiry), Day(dtExpiry))
                sExpiryKey = CStr(CLng(nExpiryMonth))
                dStrike = Round(ReadDbl(rsOpt!fStrike), STRIKE_DECIMALS_COUNT)
                sStrikeKey = CStr(dStrike)
                nOptType = ReadLng(rsOpt!tiIsCall)
                lOptionRootID = ReadLng(rsOpt!iOptionRootID)
    
                Set aExpiry = aUnd.Expiry(sExpiryKey)
                If aExpiry Is Nothing Then
                    Set aExpiry = aUnd.Expiry.Add(sExpiryKey)
                    aExpiry.ExpiryMonth = nExpiryMonth
                    aExpiry.Expiry = dtExpiry
                    aExpiry.ExpiryOV = dtExpiryOV
                    aExpiry.TradingClose = dtTradingClose
                End If
    
                Set aStrike = aExpiry.Strike(sStrikeKey)
                If aStrike Is Nothing Then
                    Set aStrike = aExpiry.Strike.Add(sStrikeKey)
                    aStrike.Strike = dStrike
                End If
    
                Set aOpt = aStrike.Opt(nOptType)
                If aOpt.ID = BAD_LONG_VALUE Then
                    aOpt.ID = ReadLng(rsOpt!iContractID)
                    aOpt.Symbol = ReadStr(rsOpt!vcSymbol)
                    aOpt.OptType = nOptType
                    aOpt.Expiry = dtExpiry
                    aOpt.ExpiryOV = dtExpiryOV
                    aOpt.TradingClose = dtTradingClose
                    aOpt.Strike = dStrike
                    aOpt.LotSize = ReadLng(rsOpt!iLotSize)
                    aOpt.OptionRootID = lOptionRootID
                    
    
                    Set aOpt.Exp = aExpiry
    
                    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(lOptionRootID)
                    aOpt.Vola = aUnd.VolaSrv.OptionVola(dtExpiryOV, dStrike, lSurfaceID)
                    If aOpt.Vola < 0 Then
                        aOpt.Vola = BAD_DOUBLE_VALUE
                    End If
    
                    vPos = rsOpt!iQty
                    aOpt.Qty = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
                
                    vPos = rsOpt!iPosition
                    aOpt.QtyInShares = IIf(IsNull(vPos), BAD_LONG_VALUE, ReadLng(vPos))
                
                    sKey = CStr(enCtOption) & "_" & aOpt.Symbol
                    If m_QuoteRequests(sKey) Is Nothing Then
                        m_QuoteRequests.Add aUnd, aOpt, sKey
                    End If
                End If
                
                Set aOpt = Nothing
                Set aExpiry = Nothing
                Set aStrike = Nothing
            End If
            
            Set aUnd = Nothing
            
NEXT_OPTION:
            Set aGUnd = Nothing
            
            DoEvents
            If Not m_bDataLoad Then GoTo Ex
            IncProgress pbProgress
            DoEvents
            
            rsOpt.MoveNext
        Wend
        
        nExpCount = 0
        For i = 1 To nUndCount
            For j = 1 To m_Und(i).Expiry.Count
                sKey = Format$(m_Und(i).Expiry(j).Expiry)
                Set aEnt = tmpExpColl(sKey)
                If aEnt Is Nothing Then
                    nExpCount = nExpCount + 1
                    tmpExpColl.Add (sKey)
                    Set aEnt = m_Exp.Add(nExpCount)
                    aEnt.ID = nExpCount
                    aEnt.Name = Format(m_Und(i).Expiry(j).Expiry, "MMM, DD YY")
                    aEnt.Data = CLng(m_Und(i).Expiry(j).Expiry)
                    Set aEnt = Nothing
                End If
            Next
        Next
        tmpExpColl.Clear
        UnderlyingsAdjustRates True
        
    End If
    
    UnderlyingsLoad = True
Ex:
    m_bDataLoad = False
    Set aExpiry = Nothing
    Set aStrike = Nothing
    Set aUnd = Nothing
    Set aOpt = Nothing
    Set rsUnd = Nothing
    Set rsOpt = Nothing
    Exit Function
EH:
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to load underlyings."
    GoTo Ex
End Function

Private Sub RequestLastQuotes()
    On Error GoTo EH
    Dim i&, nCount&, aUnd As clsVmUndAtom, aParam As PRICEPROVIDERSLib.QuoteUpdateParams
    Dim intReq As QuoteUpdateParams
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
            pbProgress.Max = m_nLastQuoteReqCount
            pbProgress.Value = 0
            pbProgress.Visible = True
            
            lblStatus.Visible = False
            lblProcess.Visible = True
            lblProcess.Caption = "Last quotes request..."
            imgStop.Visible = True
            imgStopDis.Visible = False
            DoEvents
            
            If Not m_bGroupRequest Then
                m_nLastQuoteGroupReqCount = 0&
                m_nLastQuoteGroupReqDone = 0&
                intReq.Exchange = ""
                intReq.Symbol = ""
                intReq.Type = enMStart
                PriceProvider.RequestLastQuote intReq
                
                For i = 1 To nCount
                    If Not m_bLastQuoteReqNow Then Exit For
                    PriceProvider.RequestLastQuote m_QuoteRequests(i).GetQuoteUpdateParam
                    If (i Mod 100) = 0 Then DoEvents
                Next
                
                intReq.Type = enMStop
                PriceProvider.RequestLastQuote intReq
                
            Else
                nCount = m_Und.Count
                m_nLastQuoteGroupReqCount = nCount * 2
                m_nLastQuoteGroupReqDone = 0&
                
                For Each aUnd In m_Und
                    If Not m_bLastQuoteReqNow Then Exit For
                    aParam.Symbol = aUnd.Symbol
                    aParam.Exchange = ""
                    
                    aParam.Type = IIf(aUnd.UndType = enCtStock, enSTK, enIDX)
                    PriceProvider.RequestLastQuote aParam
                        
                    aParam.Type = IIf(aUnd.UndType = enCtStock, enGrSTK, enGrIDX)
                    m_GroupPriceProvider.RequestGroup aParam, enGroupRequestLastQuote
                    
                    DoEvents
                Next
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
        If Not m_bShutDown Then gCmn.MyMsgBox m_frmOwner, "You are in offline mode now.", vbExclamation
    End If
    Exit Sub
EH:
    m_bLastQuoteReqNow = False
    If Not m_bShutDown Then gCmn.ErrorMsgBox m_frmOwner, "Fail to request last quotes."
    On Error Resume Next
    PriceProvider.CancelLastQuote
    If m_bGroupRequest Then
        m_GroupPriceProvider.CancelGroup
    End If
    
    m_QuoteRequests.Clear
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
    Dim sKey$, bFinished As Boolean
    bFinished = False
    
    If ReqType = enRequestLastQuote Then
        If m_bLastQuoteReqNow Then
            If Request.Type <> enGrSTK And Request.Type <> enGrIDX Then
                sKey = CStr(IIf(Request.Type = enOPT, enCtOption, IIf(Request.Type = enSTK, enCtStock, enCtIndex))) & "_" & Request.Symbol
                
                If Not m_QuoteRequests(sKey) Is Nothing Then
                    LogEvent EVENT_WARNING, Request.Symbol & ": " & Description
                
                    IncProgress pbProgress
                    m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
                    If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                    bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                            Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
                End If
            Else
                LogEvent EVENT_WARNING, Request.Symbol & " options: " & Description
                m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
                bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
            End If
            
            If bFinished Then
                m_bLastQuoteReqNow = False
                
                PriceProvider.CancelLastQuote
                If m_bGroupRequest Then
                    m_GroupPriceProvider.CancelGroup
                End If
                
                m_QuoteRequests.Clear
                m_bInProc = True
                UnderlyingsUpdateCurExp
                UnderlyingsCalc True, False
                m_bInProc = False
                
                UnderlyingsUpdate False
                UnderlyingsShow False
                
                AdjustState
                UpdateTotals
                
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
    Dim sKey$, aReq As clsVmRequestAtom, bFinished As Boolean
    bFinished = False
    
    sKey = CStr(IIf(Params.Type = enOPT, enCtOption, IIf(Params.Type = enSTK, enCtStock, enCtIndex))) & "_" & Params.Symbol

    If Params.Type <> enGrSTK And Params.Type <> enGrIDX Then
        Set aReq = m_QuoteRequests(sKey)
        If Not aReq Is Nothing Then
            If Params.Type = enOPT Then
                Debug.Assert Not aReq.Opt Is Nothing
            
                If Results.BidPrice > BAD_DOUBLE_VALUE Then aReq.Opt.PriceBid = Results.BidPrice
                If Results.AskPrice > BAD_DOUBLE_VALUE Then aReq.Opt.PriceAsk = Results.AskPrice
                If Results.LastPrice > BAD_DOUBLE_VALUE Then aReq.Opt.PriceLast = Results.LastPrice
            
                If aReq.Opt.LotSize <= 0 Then
                    If Results.LotSize > 0 Then
                        aReq.Opt.LotSize = Results.LotSize
                    Else
                        aReq.Opt.LotSize = 100
                    End If
                End If
            Else
                Debug.Assert Not aReq.Und Is Nothing
                If m_bGroupRequest Then m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
            
                If Results.BidPrice > BAD_DOUBLE_VALUE Then aReq.Und.PriceBid = Results.BidPrice
                If Results.AskPrice > BAD_DOUBLE_VALUE Then aReq.Und.PriceAsk = Results.AskPrice
                If Results.LastPrice > BAD_DOUBLE_VALUE Then aReq.Und.PriceLast = Results.LastPrice
            
                If aReq.Und.LotSize <= 0 Then
                    If Results.LotSize > 0 Then
                        aReq.Und.LotSize = Results.LotSize
                    Else
                        aReq.Und.LotSize = 1
                    End If
                End If
                
                Debug.Assert (Not aReq.Und.UndPriceProfile Is Nothing)
                aReq.Und.VolaSrv.UnderlyingPrice = aReq.Und.UndPriceProfile.GetUndPriceMid(aReq.Und.PriceBid, aReq.Und.PriceAsk, _
                    aReq.Und.PriceLast, g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
            End If
            
            Set aReq = Nothing
            
            m_nLastQuoteReqDone = m_nLastQuoteReqDone + 1
            IncProgress pbProgress
            
            bFinished = (m_nLastQuoteReqDone = m_nLastQuoteReqCount _
                        Or m_bGroupRequest And m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
        End If
    Else
        m_nLastQuoteGroupReqDone = m_nLastQuoteGroupReqDone + 1
        bFinished = (m_nLastQuoteGroupReqCount = m_nLastQuoteGroupReqDone)
    End If

    If bFinished Then
        m_bLastQuoteReqNow = False
        
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        
        m_QuoteRequests.Clear
        m_bLastQuoteReqNow = False
        
        m_bInProc = True
        UnderlyingsUpdateCurExp
        UnderlyingsCalc True, False
        m_bInProc = False
        
        UnderlyingsUpdate False
        UnderlyingsShow False
        
        AdjustState
        UpdateTotals
        
        pbProgress.Visible = False
        lblStatus.Visible = True
        lblProcess.Visible = False
        lblProcess.Caption = ""
        imgStop.Visible = False
        imgStopDis.Visible = False
    End If
End Sub

Private Function CheckGreeksFilter(aUnd As clsVmUndAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    nValue = m_nFilter(VFC_VOLA_1)
    bMatched = (nValue = 0)
    If Not bMatched Then
        If aUnd.IV1 > BAD_DOUBLE_VALUE And aUnd.Vola1 > BAD_DOUBLE_VALUE Then
            bMatched = (nValue = 1 And aUnd.Vola1 > aUnd.IV1 Or nValue = 2 And aUnd.Vola1 < aUnd.IV1)
        End If
    End If
    
    If bMatched Then
        nValue = m_nFilter(VFC_VOLA_2)
        bMatched = (nValue = 0)
        If Not bMatched Then
            If aUnd.IV2 > BAD_DOUBLE_VALUE And aUnd.Vola2 > BAD_DOUBLE_VALUE Then
                bMatched = (nValue = 1 And aUnd.Vola2 > aUnd.IV2 Or nValue = 2 And aUnd.Vola2 < aUnd.IV2)
            End If
        End If
    End If
    
    If bMatched Then
        nValue = m_nFilter(VFC_VEGA)
        bMatched = (nValue = 0)
        If Not bMatched Then
            If aUnd.TotalVega > BAD_DOUBLE_VALUE Then
                bMatched = (nValue = 1 And aUnd.TotalVega > 0# Or nValue = 2 And aUnd.TotalVega < 0#)
            End If
        End If
    End If
    
    If bMatched Then
        nValue = m_nFilter(VFC_GAMMA)
        bMatched = (nValue = 0)
        If Not bMatched Then
            If aUnd.TotalGamma > BAD_DOUBLE_VALUE Then
                bMatched = (nValue = 1 And aUnd.TotalGamma > 0# Or nValue = 2 And aUnd.TotalGamma < 0#)
            End If
        End If
    End If
    
    CheckGreeksFilter = bMatched
End Function

Private Sub UnderlyingsShow(ByVal bReload As Boolean)
    On Error Resume Next
    Dim i&, nCount&, aUnd As clsVmUndAtom, nRow&, nRows&
    
    With fgUnd
        m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
        
        If bReload Then
            nCount = m_Und.Count
            For i = 1 To nCount
                Set aUnd = m_Und(i)
                
                .AddItem ""
                nRow = .Rows - 1
                .RowData(nRow) = aUnd
                .TextMatrix(nRow, VUC_KEY) = aUnd.ID
                
                UnderlyingUpdate nRow, True, aUnd
                
                Set aUnd = Nothing
            Next
            
            If .Rows > 1 Then
                .Row = 1
            End If
            
            i = -1
            i = .ColIndex(VUC_SYMBOL)
            If i >= 0 Then
                .Col = i
                .Row = 1
                .Sort = flexSortGenericAscending
            End If
            geUnd.ShowSortImage i, 1
            
            FormatUndColumns
        Else
            nRows = .Rows - 1
            For nRow = 1 To nRows
                Set aUnd = .RowData(nRow)
                .RowHidden(nRow) = Not CheckGreeksFilter(aUnd)
                Set aUnd = Nothing
            Next
        End If
        
        m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
    End With
End Sub
Private Function FindCurExpiry(aUnd As clsVmUndAtom, nMonth As Long) As clsVmExpiryAtom
    Dim i&, bMatched As Boolean

    If Not aUnd Is Nothing Then
        For i = 1 To aUnd.Expiry.Count
            If aUnd.Expiry(i).Expiry = CDate(m_Exp(nMonth).Data) Then
                bMatched = True
                Set FindCurExpiry = aUnd.Expiry(i)
            End If
        Next
    End If

    If Not bMatched Then Set FindCurExpiry = Nothing

End Function

Private Sub UnderlyingsUpdateCurExp()
    On Error Resume Next
    Dim i&, nCount&, aUnd As clsVmUndAtom, n&, nExpCount&, nStrCount&, aStr As clsVmStrikeAtom
    Dim nMonth1&, nMonth2&, dSpot#, dDiff#
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
        
    dToleranceValue = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    nMonth1 = m_nFilter(VFC_EXPIRY_1)
    nMonth2 = m_nFilter(VFC_EXPIRY_2)
    
    nCount = m_Und.Count
    For i = 1 To nCount
        Set aUnd = m_Und(i)

        Set aUnd.CurExpiry1 = Nothing
        Set aUnd.CurExpiry2 = Nothing
        
        Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
        dSpot = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
        nExpCount = aUnd.Expiry.Count
        
        If nExpCount > 0 Then
            If nMonth1 <> 0 Then
                Set aUnd.CurExpiry1 = FindCurExpiry(aUnd, nMonth1)
                Set aUnd.CurExpiry1.CurStrike = Nothing
            End If
            
            If nMonth2 <> 0 Then
                Set aUnd.CurExpiry2 = FindCurExpiry(aUnd, nMonth2)
                Set aUnd.CurExpiry2.CurStrike = Nothing
            End If
        End If
            
        If dSpot > 0# Then
            dDiff = 1E+308
            
            If Not aUnd.CurExpiry1 Is Nothing Then
                nStrCount = aUnd.CurExpiry1.Strike.Count
                For n = 1 To nStrCount
                    Set aStr = aUnd.CurExpiry1.Strike(n)
                    If Abs(dSpot - aStr.Strike) < dDiff Then
                        dDiff = Abs(dSpot - aStr.Strike)
                        Set aUnd.CurExpiry1.CurStrike = aStr
                    End If
                    Set aStr = Nothing
                Next
            End If
        
            dDiff = 1E+308
            If Not aUnd.CurExpiry1 Is Nothing Then
                nStrCount = aUnd.CurExpiry2.Strike.Count
                For n = 1 To nStrCount
                    Set aStr = aUnd.CurExpiry2.Strike(n)
                    If Abs(dSpot - aStr.Strike) < dDiff Then
                        dDiff = Abs(dSpot - aStr.Strike)
                        Set aUnd.CurExpiry2.CurStrike = aStr
                    End If
                    Set aStr = Nothing
                Next
            End If
        End If
        
        Set aUnd = Nothing
    Next
End Sub

Private Sub UnderlyingCalcTotals(aUnd As clsVmUndAtom)
    On Error Resume Next
    Dim e&, nExpCount&, S&, nStrCount&, n&
    Dim aExp As clsVmExpiryAtom, aStr As clsVmStrikeAtom, aOpt As clsVmOptAtom, dSpotPrice#
    
    If aUnd Is Nothing Then Exit Sub
    
    Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
    dSpotPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, _
        g_Params.UndPriceToleranceValue, g_Params.PriceRoundingRule)
    
    aUnd.ClearTheos
    nExpCount = aUnd.Expiry.Count
    For e = 1 To nExpCount
        Set aExp = aUnd.Expiry(e)
        
        nStrCount = aExp.Strike.Count
        For S = 1 To nStrCount
            Set aStr = aExp.Strike(S)
            
            For n = enOtPut To enOtCall
                Set aOpt = aStr.Opt(n)
                If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                    If Not IsBadLong(aOpt.QtyInShares) Then
                        If Not IsBadDouble(aOpt.Delta) Then
                            If IsBadDouble(aUnd.TotalDelta) Then aUnd.TotalDelta = 0#
                            aUnd.TotalDelta = aUnd.TotalDelta + aOpt.QtyInShares * aOpt.Delta / 100#
                        End If
                        If Not IsBadDouble(aOpt.Gamma) Then
                            If IsBadDouble(aUnd.TotalGamma) Then aUnd.TotalGamma = 0#
                            aUnd.TotalGamma = aUnd.TotalGamma + aOpt.QtyInShares * aOpt.Gamma / 100#
                        End If
                        If Not IsBadDouble(aOpt.Theta) Then
                            If IsBadDouble(aUnd.TotalTheta) Then aUnd.TotalTheta = 0#
                            aUnd.TotalTheta = aUnd.TotalTheta + aOpt.QtyInShares * aOpt.Theta
                        End If
                        If Not IsBadDouble(aOpt.Vega) Then
                            If IsBadDouble(aUnd.TotalVega) Then aUnd.TotalVega = 0#
                            aUnd.TotalVega = aUnd.TotalVega + aOpt.QtyInShares * aOpt.Vega
                        End If
                        If Not IsBadDouble(aOpt.Rho) Then
                            If IsBadDouble(aUnd.TotalRho) Then aUnd.TotalRho = 0#
                            aUnd.TotalRho = aUnd.TotalRho + aOpt.QtyInShares * aOpt.Rho
                        End If
                    End If
                    Set aOpt = Nothing
                End If
            Next
            
            Set aStr = Nothing
        Next
        
        Set aExp = Nothing
    Next
    
    If Not IsBadLong(aUnd.QtyInShares) Then aUnd.NetDelta = aUnd.QtyInShares
    If Not IsBadDouble(aUnd.TotalDelta) Then
        If IsBadDouble(aUnd.NetDelta) Then aUnd.NetDelta = 0#
        aUnd.NetDelta = aUnd.NetDelta + aUnd.TotalDelta
    End If
End Sub

Private Sub UnderlyingCalc(aUnd As clsVmUndAtom, ByVal bRecalc As Boolean)
    On Error Resume Next
    Dim e&, nExpCount&, S&, nStrCount&, n&
    Dim aExp As clsVmExpiryAtom, aStr As clsVmStrikeAtom, aOpt As clsVmOptAtom
    Dim dIV1#, dVola1#, dTargVola1#, dIV2#, dVola2#, dTargVola2#, dSpotPrice#
    Dim enPriceStatusMid As EtsGeneralLib.EtsReplacePriceStatusEnum
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    If Not aUnd Is Nothing Then
        aUnd.ClearVolas
        
        If bRecalc Then
            aUnd.ClearTheos
            Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
            
            enPriceStatusMid = enRpsNone
            dSpotPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule, enPriceStatusMid)
            aUnd.ReplacePriceStatus = enPriceStatusMid
            
            aUnd.VolaSrv.UnderlyingPrice = dSpotPrice
            
            nExpCount = aUnd.Expiry.Count
            For e = 1 To nExpCount
                Set aExp = aUnd.Expiry(e)
                
                nStrCount = aExp.Strike.Count
                For S = 1 To nStrCount
                    Set aStr = aExp.Strike(S)
                    
                    For n = enOtPut To enOtCall
                        Set aOpt = aStr.Opt(n)
                        If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                            CalcOptionGreeks aUnd, aOpt
                            
                            If Not IsBadLong(aOpt.QtyInShares) Then
                                If Not IsBadDouble(aOpt.Delta) Then
                                    If IsBadDouble(aUnd.TotalDelta) Then aUnd.TotalDelta = 0#
                                    aUnd.TotalDelta = aUnd.TotalDelta + aOpt.QtyInShares * aOpt.Delta / 100#
                                End If
                                If Not IsBadDouble(aOpt.Gamma) Then
                                    If IsBadDouble(aUnd.TotalGamma) Then aUnd.TotalGamma = 0#
                                    aUnd.TotalGamma = aUnd.TotalGamma + aOpt.QtyInShares * aOpt.Gamma / 100#
                                End If
                                If Not IsBadDouble(aOpt.Theta) Then
                                    If IsBadDouble(aUnd.TotalTheta) Then aUnd.TotalTheta = 0#
                                    aUnd.TotalTheta = aUnd.TotalTheta + aOpt.QtyInShares * aOpt.Theta
                                End If
                                If Not IsBadDouble(aOpt.Vega) Then
                                    If IsBadDouble(aUnd.TotalVega) Then aUnd.TotalVega = 0#
                                    aUnd.TotalVega = aUnd.TotalVega + aOpt.QtyInShares * aOpt.Vega
                                End If
                                If Not IsBadDouble(aOpt.Rho) Then
                                    If IsBadDouble(aUnd.TotalRho) Then aUnd.TotalRho = 0#
                                    aUnd.TotalRho = aUnd.TotalRho + aOpt.QtyInShares * aOpt.Rho
                                End If
                            End If
                            Set aOpt = Nothing
                        End If
                    Next
                    
                    Set aStr = Nothing
                Next
                
                Set aExp = Nothing
            Next
        
            If Not IsBadLong(aUnd.QtyInShares) Then aUnd.NetDelta = aUnd.QtyInShares
            If Not IsBadDouble(aUnd.TotalDelta) Then
                If IsBadDouble(aUnd.NetDelta) Then aUnd.NetDelta = 0#
                aUnd.NetDelta = aUnd.NetDelta + aUnd.TotalDelta
            End If
        End If
    
        If Not aUnd.CurExpiry1 Is Nothing Then
            If Not aUnd.CurExpiry1.CurStrike Is Nothing Then
                dIV1 = BAD_DOUBLE_VALUE
                dVola1 = BAD_DOUBLE_VALUE
                dTargVola1 = BAD_DOUBLE_VALUE
                dIV2 = BAD_DOUBLE_VALUE
                dVola2 = BAD_DOUBLE_VALUE
                dTargVola2 = BAD_DOUBLE_VALUE
                
                Set aOpt = aUnd.CurExpiry1.CurStrike.Opt(enOtCall)
                If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                    dIV1 = aOpt.IV
                    dVola1 = aOpt.Vola
                    dTargVola1 = aOpt.TargVola
                    Set aOpt = Nothing
                End If
                
                Set aOpt = aUnd.CurExpiry1.CurStrike.Opt(enOtPut)
                If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                    dIV2 = aOpt.IV
                    dVola2 = aOpt.Vola
                    dTargVola2 = aOpt.TargVola
                    Set aOpt = Nothing
                End If
                
                aUnd.IV1 = VolaMidEx(dIV1, dIV2)
                aUnd.Vola1 = VolaMidEx(dVola1, dVola2)
                aUnd.TargetVola1 = VolaMidEx(dTargVola1, dTargVola2)
            End If
        End If
        
        If Not aUnd.CurExpiry2 Is Nothing Then
            If Not aUnd.CurExpiry2.CurStrike Is Nothing Then
                dIV1 = BAD_DOUBLE_VALUE
                dVola1 = BAD_DOUBLE_VALUE
                dTargVola1 = BAD_DOUBLE_VALUE
                dIV2 = BAD_DOUBLE_VALUE
                dVola2 = BAD_DOUBLE_VALUE
                dTargVola2 = BAD_DOUBLE_VALUE
                
                Set aOpt = aUnd.CurExpiry2.CurStrike.Opt(enOtCall)
                If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                    dIV1 = aOpt.IV
                    dVola1 = aOpt.Vola
                    dTargVola1 = aOpt.TargVola
                    Set aOpt = Nothing
                End If
                
                Set aOpt = aUnd.CurExpiry2.CurStrike.Opt(enOtPut)
                If Not aOpt Is Nothing And aOpt.ID <> BAD_LONG_VALUE Then
                    dIV2 = aOpt.IV
                    dVola2 = aOpt.Vola
                    dTargVola2 = aOpt.TargVola
                    Set aOpt = Nothing
                End If
                
                aUnd.IV2 = VolaMidEx(dIV1, dIV2)
                aUnd.Vola2 = VolaMidEx(dVola1, dVola2)
                aUnd.TargetVola2 = VolaMidEx(dTargVola1, dTargVola2)
            End If
        End If
    End If
End Sub

Private Sub UnderlyingsCalc(ByVal bRecalcAll As Boolean, ByVal bShowProgress As Boolean)
    On Error Resume Next
    Dim i&, nCount&, aUnd As clsVmUndAtom
    
    nCount = m_Und.Count
    For i = 1 To nCount
        Set aUnd = m_Und(i)
        
        UnderlyingCalc aUnd, bRecalcAll
        
        If bShowProgress Then
            IncProgress pbProgress
        End If
        
        Set aUnd = Nothing
    Next
End Sub

Private Sub UnderlyingsAdjustRates(ByVal bForceRateUpdate As Boolean)
    On Error Resume Next
    Dim i&, nCount&
    
    nCount = m_Und.Count
    For i = 1 To nCount
        UnderlyingAdjustRates m_Und(i), bForceRateUpdate
    Next
End Sub

Private Function UnderlyingAdjustRates(ByRef aUnd As clsVmUndAtom, ByVal bForceRateUpdate As Boolean) As Boolean
    On Error Resume Next
    Dim aExp As clsVmExpiryAtom, bUseMidRates As Boolean, cPosThreshold@, dPos#
    
    Dim dtNow As Date
    dtNow = GetNewYorkTime
        
    UnderlyingAdjustRates = False
    If aUnd Is Nothing Then Exit Function
    
    dPos = g_UnderlyingAll(aUnd.ID).UndPosForRates
    
    If GetIrRuleType = enRateBasedOnPosition Then
        cPosThreshold = Abs(GetPosThreshold)
        bUseMidRates = (cPosThreshold = 0 Or cPosThreshold <> 0 And Abs(dPos) <= cPosThreshold)

        If Not bForceRateUpdate And Not bUseMidRates Then _
            bForceRateUpdate = (Sgn(dPos) * Sgn(aUnd.UndPosForRates) < 0)
    Else
        bUseMidRates = True
    End If
    
    If bForceRateUpdate Or aUnd.UseMidRates <> bUseMidRates Then
        aUnd.UseMidRates = bUseMidRates
        aUnd.UndPosForRates = dPos
        
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
        Next
        UnderlyingAdjustRates = True
    End If
End Function

Private Sub CalcOptionGreeks(aUnd As clsVmUndAtom, aOpt As clsVmOptAtom)
    On Error Resume Next
    Dim nDivCount&, RetCount&
    Dim dDivDte() As Double
    Dim dDivAmts() As Double, aBaskDivs() As REGULAR_DIVIDENDS, nBaskDivCount&
    Dim dRate#, dHTBRate#, dSpotPrice#, dOptPrice#, div#, dYield#
    Dim aGreeks As GreeksData, nModel As EtsGeneralLib.EtsCalcModelTypeEnum
    Dim dtDivDate As Date, nDivFreq&, dDivAmt#, dVola#, nIsAmerican&
    Dim nFlag&
    Dim dToleranceValue#, enRoundingRule As EtsGeneralLib.EtsPriceRoundingRuleEnum
    Dim aDiv As EtsGeneralLib.EtsIndexDivAtom
    Dim aBasketDivs As EtsGeneralLib.EtsIndexDivColl
    Dim enDivType As EtsGeneralLib.EtsDivTypeEnum
    Dim lSurfaceID As Long

    Dim dYte As Double
    Dim dtNow As Date
    Dim dtExpiryOV As Date
    Dim dtTradingClose As Date
    
    'checked
    dtNow = GetNewYorkTime
    GetCalcParams dtNow, aOpt.ExpiryOV, aOpt.TradingClose, g_Main.CalculationParametrs.UseTimePrecision, dtNow, dtExpiryOV, dtTradingClose, dYte
    
    dToleranceValue# = g_Params.UndPriceToleranceValue
    enRoundingRule = g_Params.PriceRoundingRule
    
    aGreeks.nMask = GM_DELTA Or GM_GAMMA Or GM_VEGA Or GM_THETA Or GM_RHO
    nModel = g_Params.CalcModel
    dSpotPrice = 0#
    dYield = 0#

    Debug.Assert (Not aUnd.UndPriceProfile Is Nothing)
    dSpotPrice = aUnd.UndPriceProfile.GetUndPriceMid(aUnd.PriceBid, aUnd.PriceAsk, aUnd.PriceLast, dToleranceValue, enRoundingRule)
    aOpt.ClearValues
    lSurfaceID = aUnd.VolaSrv.GetSurfaceByRoot(aOpt.OptionRootID)
    aOpt.Vola = aUnd.VolaSrv.OptionVola(aOpt.ExpiryOV, aOpt.Strike, lSurfaceID)
    aOpt.TargVola = aUnd.VolaSrv.OptionTargetVola(aOpt.ExpiryOV, aOpt.Strike)
        
    If dSpotPrice >= 0 Then
        nIsAmerican = IIf(aUnd.IsAmerican, 1, 0)
        
        Debug.Assert (Not aUnd.OptPriceProfile Is Nothing)
        dOptPrice = aUnd.OptPriceProfile.GetOptPriceMid(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, enRoundingRule, g_Params.UseTheoVolatility, 0)
        
       
        If True Then
                
            nDivCount = 0
            ReDim dDivDte(0 To 0)
            ReDim dDivAmts(0 To 0)
        
            Set aDiv = aUnd.Dividend
    
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
                    
            dRate = aOpt.Exp.Rate
            dHTBRate = aOpt.Exp.HTBRate
            
            If nDivCount < 0 Then nDivCount = 0


            If Not g_Params.UseTheoNoBid Or g_Params.UseTheoNoBid And aOpt.PriceBid > 0# Then
                If dOptPrice > 0# Then
                    nFlag = VF_OK
                    div = CalcVolatilityMM3(dRate, dYield, dHTBRate, dSpotPrice, dOptPrice, aOpt.Strike, dYte, _
                                        aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
                                        
                    If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                        div = aOpt.Vola
                    End If
                    If div > 0 Then
                        aOpt.IV = div
                    Else
                        aOpt.IV = BAD_DOUBLE_VALUE
                    End If
                ElseIf g_Params.UseTheoBadMarketVola Then
                    aOpt.IV = aOpt.Vola
                Else
                    aOpt.IV = BAD_DOUBLE_VALUE
                End If
            Else
                aOpt.IV = aOpt.Vola
            End If
            
            If g_Params.UseTheoVolatility Then
                dVola = aOpt.Vola
            Else
                dVola = IIf(aOpt.IV > BAD_DOUBLE_VALUE, aOpt.IV, 0)
            End If
            
            RetCount = 0&
            
            If aGreeks.nMask <> GM_NONE Then
                RetCount = CalcGreeksMM2(dRate, dYield, dHTBRate, dSpotPrice, aOpt.Strike, dVola, dYte, _
                                    aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aUnd.Skew, aUnd.Kurt, nModel, aGreeks)
                If g_Params.UseTheoVolatility And (dOptPrice <= 0# Or aOpt.PriceAsk <= 0# Or aOpt.PriceBid <= 0# Or aOpt.PriceLast <= 0#) Then
                    dOptPrice = aUnd.OptPriceProfile.GetOptPriceMid(aOpt.PriceBid, aOpt.PriceAsk, aOpt.PriceLast, enRoundingRule, g_Params.UseTheoVolatility, aGreeks.dTheoPrice)
  
              If Not g_Params.UseTheoNoBid Or g_Params.UseTheoNoBid And aOpt.PriceBid > 0# Then
                If dOptPrice > 0# Then
                    nFlag = VF_OK
                    div = CalcVolatilityMM3(dRate, dYield, dHTBRate, dSpotPrice, dOptPrice, aOpt.Strike, dYte, _
                                        aOpt.OptType, nIsAmerican, nDivCount, dDivAmts(0), dDivDte(0), 100, aUnd.Skew, aUnd.Kurt, nModel, nFlag)
                                        
                        If g_Params.UseTheoBadMarketVola And nFlag <> VF_OK Then
                            div = aOpt.Vola
                        End If
                        If div > 0 Then
                            aOpt.IV = div
                        Else
                            aOpt.IV = BAD_DOUBLE_VALUE
                        End If
                        ElseIf g_Params.UseTheoBadMarketVola Then
                            aOpt.IV = aOpt.Vola
                        Else
                            aOpt.IV = BAD_DOUBLE_VALUE
                        End If
                    Else
                        aOpt.IV = aOpt.Vola
                    End If
            
                    If g_Params.UseTheoVolatility Then
                        dVola = aOpt.Vola
                    Else
                        dVola = IIf(aOpt.IV > BAD_DOUBLE_VALUE, aOpt.IV, 0)
                    End If
                
                    RetCount = 0&

                End If
            End If
            
            Erase dDivDte
            Erase dDivAmts
            
            If aGreeks.nMask <> GM_NONE Then
                FixGreeks aGreeks
                
                aOpt.Delta = IIf((aGreeks.nMask And GM_DELTA) = GM_DELTA, aGreeks.dDelta * 100#, BAD_DOUBLE_VALUE)
                aOpt.Gamma = IIf((aGreeks.nMask And GM_GAMMA) = GM_GAMMA, aGreeks.dGamma * 100#, BAD_DOUBLE_VALUE)
                aOpt.Theta = IIf((aGreeks.nMask And GM_THETA) = GM_THETA, aGreeks.dTheta, BAD_DOUBLE_VALUE)
                aOpt.Vega = IIf((aGreeks.nMask And GM_VEGA) = GM_VEGA, aGreeks.dVega, BAD_DOUBLE_VALUE)
                aOpt.Rho = IIf((aGreeks.nMask And GM_RHO) = GM_RHO, aGreeks.dRho, BAD_DOUBLE_VALUE)
            End If
        End If
    End If
End Sub


Private Sub UnderlyingsUpdate(ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim nRows&, nRow&
    
    With fgUnd
        m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
        
        nRows = .Rows - 1
        For nRow = 1 To nRows
            UnderlyingUpdate nRow, bSymbol, Nothing
        Next
        
        m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
    End With
End Sub

Private Sub UnderlyingUpdate(ByVal nRow As Long, ByVal bSymbol As Boolean, aRowUnd As clsVmUndAtom)
    On Error Resume Next
    Dim aUnd As clsVmUndAtom
    Dim nCol&, bBadVolas1 As Boolean, bBadVolas2 As Boolean
    
    With fgUnd
        m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
        
        If aRowUnd Is Nothing Then
            Set aUnd = .RowData(nRow)
        Else
            Set aUnd = aRowUnd
        End If
        
        If Not aUnd Is Nothing Then
            If bSymbol Then
                nCol = .ColIndex(VUC_SYMBOL)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aUnd.Symbol
            End If
            
            nCol = .ColIndex(VUC_BID)
            If nCol >= 0 Then
                .TextMatrix(nRow, nCol) = IIf(aUnd.PriceBid > BAD_DOUBLE_VALUE, aUnd.PriceBid, STR_NA)
                
                If aUnd.ReplacePriceStatus And enRpsBid Then
                    Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                    .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                Else
                    If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                        Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                    End If
                End If
            End If
            
            nCol = .ColIndex(VUC_ASK)
            If nCol >= 0 Then
                .TextMatrix(nRow, nCol) = IIf(aUnd.PriceAsk > BAD_DOUBLE_VALUE, aUnd.PriceAsk, STR_NA)
                
                If aUnd.ReplacePriceStatus And enRpsAsk Then
                    Set .Cell(flexcpPicture, nRow, nCol) = imgBadPrice.Picture
                    .Cell(flexcpPictureAlignment, nRow, nCol) = flexPicAlignLeftCenter
                Else
                    If Not .Cell(flexcpPicture, nRow, nCol) Is Nothing Then
                        Set .Cell(flexcpPicture, nRow, nCol) = Nothing
                    End If
                End If
            End If
            
            nCol = .ColIndex(VUC_LAST)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.PriceLast > BAD_DOUBLE_VALUE, aUnd.PriceLast, STR_NA)
            
            bBadVolas1 = True
            If Not aUnd.CurExpiry1 Is Nothing Then
                nCol = .ColIndex(VUC_EXPIRY_1)

                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aUnd.CurExpiry1.ExpiryMonth
            
                If Not aUnd.CurExpiry1.CurStrike Is Nothing Then
                    nCol = .ColIndex(VUC_STRIKE_1)
                    If nCol >= 0 Then .TextMatrix(nRow, nCol) = aUnd.CurExpiry1.CurStrike.Strike
                    bBadVolas1 = False
                Else
                    nCol = .ColIndex(VUC_STRIKE_1)
                    If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
                End If
            Else
                nCol = .ColIndex(VUC_EXPIRY_1)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
                nCol = .ColIndex(VUC_STRIKE_1)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
            End If
            
            nCol = .ColIndex(VUC_IV_1)
            If nCol >= 0 Then
                If Not bBadVolas1 And aUnd.IV1 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = aUnd.IV1 * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
        
            nCol = .ColIndex(VUC_VOLA_1)
            If nCol >= 0 Then
                If Not bBadVolas1 And aUnd.Vola1 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = aUnd.Vola1 * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
        
'            nCol = .ColIndex(VUC_TARG_1)
'            If nCol >= 0 Then
'                If Not bBadVolas1 And aUnd.TargetVola1 > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aUnd.TargetVola1 * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
            
            bBadVolas2 = True
            If Not aUnd.CurExpiry2 Is Nothing Then
                nCol = .ColIndex(VUC_EXPIRY_2)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aUnd.CurExpiry2.ExpiryMonth
            
                If Not aUnd.CurExpiry1.CurStrike Is Nothing Then
                    nCol = .ColIndex(VUC_STRIKE_2)
                    If nCol >= 0 Then .TextMatrix(nRow, nCol) = aUnd.CurExpiry2.CurStrike.Strike
                    bBadVolas2 = False
                Else
                    nCol = .ColIndex(VUC_STRIKE_2)
                    If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
                End If
            Else
                nCol = .ColIndex(VUC_EXPIRY_2)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
                nCol = .ColIndex(VUC_STRIKE_2)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = STR_NA
            End If
            
            nCol = .ColIndex(VUC_IV_2)
            If nCol >= 0 Then
                If Not bBadVolas2 And aUnd.IV2 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = aUnd.IV2 * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
        
            nCol = .ColIndex(VUC_VOLA_2)
            If nCol >= 0 Then
                If Not bBadVolas2 And aUnd.Vola2 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = aUnd.Vola2 * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
        
'            nCol = .ColIndex(VUC_TARG_2)
'            If nCol >= 0 Then
'                If Not bBadVolas2 And aUnd.TargetVola2 > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = aUnd.TargetVola2 * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
            
            nCol = .ColIndex(VUC_VOLA_IV_1)
            If nCol >= 0 Then
                If Not bBadVolas1 And aUnd.Vola1 > BAD_DOUBLE_VALUE And aUnd.IV1 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = (aUnd.Vola1 - aUnd.IV1) * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
    
'            nCol = .ColIndex(VUC_TARG_IV_1)
'            If nCol >= 0 Then
'                If Not bBadVolas1 And aUnd.TargetVola1 > BAD_DOUBLE_VALUE And aUnd.IV1 > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = (aUnd.TargetVola1 - aUnd.IV1) * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
            
            nCol = .ColIndex(VUC_VOLA_IV_2)
            If nCol >= 0 Then
                If Not bBadVolas2 And aUnd.Vola2 > BAD_DOUBLE_VALUE And aUnd.IV2 > BAD_DOUBLE_VALUE Then
                    .TextMatrix(nRow, nCol) = (aUnd.Vola2 - aUnd.IV2) * 100#
                Else
                    .TextMatrix(nRow, nCol) = STR_NA
                End If
            End If
    
'            nCol = .ColIndex(VUC_TARG_IV_2)
'            If nCol >= 0 Then
'                If Not bBadVolas2 And aUnd.TargetVola2 > BAD_DOUBLE_VALUE And aUnd.IV2 > BAD_DOUBLE_VALUE Then
'                    .TextMatrix(nRow, nCol) = (aUnd.TargetVola2 - aUnd.IV2) * 100#
'                Else
'                    .TextMatrix(nRow, nCol) = STR_NA
'                End If
'            End If
    
            nCol = .ColIndex(VUC_POS)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA)
    
            nCol = .ColIndex(VUC_NET_DELTA)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.NetDelta > BAD_DOUBLE_VALUE, aUnd.NetDelta, STR_NA)
            
            nCol = .ColIndex(VUC_OPT_DELTA)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.TotalDelta > BAD_DOUBLE_VALUE, aUnd.TotalDelta, STR_NA)
    
            nCol = .ColIndex(VUC_GAMMA)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.TotalGamma > BAD_DOUBLE_VALUE, aUnd.TotalGamma, STR_NA)
    
            nCol = .ColIndex(VUC_VEGA)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.TotalVega > BAD_DOUBLE_VALUE, aUnd.TotalVega, STR_NA)
    
            nCol = .ColIndex(VUC_THETA)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.TotalTheta > BAD_DOUBLE_VALUE, aUnd.TotalTheta, STR_NA)
    
            nCol = .ColIndex(VUC_RHO)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aUnd.TotalRho > BAD_DOUBLE_VALUE, aUnd.TotalRho, STR_NA)
    
            Set aUnd = Nothing
        End If
        
        m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
    End With
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
    For i = VFC_GROUP To VFC_COLUMN_COUNT
        aStorage.SetLongValue "VolaFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next
    
    m_gdFlt.WriteToStorage "VolaFltGrid" & sKey, aStorage, False
    m_gdUnd.WriteToStorage "VolaUndGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
    m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
    m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
    m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
    
    ' common info
    For i = VFC_GROUP To VFC_COLUMN_COUNT
        m_nFilter(i) = aStorage.GetLongValue("VolaFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next
    
    m_gdFlt.ReadFromStorage "VolaFltGrid" & sKey, aStorage, False
    m_gdUnd.ReadFromStorage "VolaUndGrid" & sKey, aStorage
    
    FilterUpdateAll
    tmrShow.Enabled = True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub Refresh()
    On Error Resume Next
    m_nOpenedExpiry1 = m_nFilter(VFC_EXPIRY_1)
    m_nOpenedExpiry2 = m_nFilter(VFC_EXPIRY_2)
    tmrShow.Enabled = True
End Sub

Public Function Group() As EtsGeneralLib.EtsMmEntityAtom
    On Error Resume Next
    Set Group = m_Grp
End Function

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aUnd As clsVmUndAtom
    
    CurUnderlyingID = 0
    
    If m_Grp.Data = TYPE_UNDERLYING Then
        CurUnderlyingID = m_Grp.ID
    Else
        With fgUnd
            If .Row > 0 Then
                Set aUnd = .RowData(.Row)
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
        frmLayout.Execute GT_VOLA_FILTER, VFC_GROUP, VFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeUndGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_VOLA_UNDERLYING, VUC_SYMBOL, VUC_LAST_COLUMN, m_gdUnd, m_frmOwner
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
        Case GT_VOLA_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_VOLA_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_VOLA_FILTER).UnlockRedraw
            
        Case GT_VOLA_UNDERLYING
            gdGrid.CopyTo m_gdUnd
            
            m_GridLock(GT_VOLA_UNDERLYING).LockRedraw
            FormatUndGrid
            UnderlyingsUpdate True
            FormatUndColumns
            m_GridLock(GT_VOLA_UNDERLYING).UnlockRedraw
    
    End Select
    
    UserControl_Resize
End Sub

'Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsVmOptAtom, nRow&, nBS&, bUpdate As Boolean, aUnd As clsVmUndAtom, sExpiryKey$
'    Dim bRatesUpdated As Boolean
'
'    If g_CurTraderID <> 0 And aTrd.Trader.ID <> g_CurTraderID Or aTrd.Status = enTsSimulated Then Exit Sub
'
'    Set aUnd = m_Und(CStr(aTrd.UndID))
'    If aUnd Is Nothing Then Exit Sub
'
'    bUpdate = False
'    bRatesUpdated = False
'
'    nBS = IIf(aTrd.IsBuy, 1, -1)
'
'    If aTrd.ContractType = enCtOption Then
'        sExpiryKey = CStr(CLng(DateSerial(Year(aTrd.Opt.Expiry), Month(aTrd.Opt.Expiry), 1)))
'
'        Set aOpt = aUnd.Expiry(sExpiryKey).Strike(CStr(aTrd.Opt.Strike)).Opt(aTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'            aOpt.Qty = aOpt.Qty + aTrd.Quantity * nBS
'            If IsBadDouble(aOpt.Pos) Then aOpt.Pos = 0
'            aOpt.Pos = aOpt.Pos + aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'        End If
'
'    Else
'        If IsBadLong(aUnd.QtyInShares) Then aUnd.QtyInShares = 0
'        aUnd.QtyInShares = aUnd.QtyInShares + aTrd.Quantity * aTrd.LotSize * nBS
'        If IsBadDouble(aUnd.Pos) Then aUnd.Pos = 0
'        aUnd.Pos = aUnd.Pos + aTrd.Price * aTrd.Quantity * aTrd.LotSize * nBS
'
'        bRatesUpdated = UnderlyingAdjustRates(aUnd, False)
'
'        bUpdate = True
'    End If
'
'    If bUpdate Then
'        nRow = fgUnd.FindRow(aUnd.ID, , VUC_KEY)
'
'        If bRatesUpdated Then UnderlyingCalc aUnd, True
'
'        If aTrd.ContractType = enCtOption Or bRatesUpdated Then
'            UnderlyingCalcTotals aUnd
'            UpdateTotals
'        End If
'
'        If nRow > 0 Then
'            UnderlyingUpdate nRow, False, aUnd
'        End If
'    End If
'    Set aOpt = Nothing
'    Set aUnd = Nothing
'End Sub
'
'Private Sub UpdateTrade(aNewTrd As EtsGeneralLib.MmTradeInfoAtom, aOldTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsVmOptAtom, nRow&, nNewBS&, nOldBS&, bUpdate As Boolean
'    Dim aUnd As clsVmUndAtom, sExpiryKey$, bRatesUpdated As Boolean
'
'    Debug.Assert aNewTrd.ContractID = aOldTrd.ContractID
'    Debug.Assert aNewTrd.UndID = aOldTrd.UndID
'
'    If aNewTrd.ContractID <> aOldTrd.ContractID Then Exit Sub
'    If aNewTrd.UndID <> aOldTrd.UndID Then Exit Sub
'
'    Set aUnd = m_Und(CStr(aNewTrd.UndID))
'    If aUnd Is Nothing Then Exit Sub
'
'    bUpdate = False
'    bRatesUpdated = False
'    nNewBS = IIf(aNewTrd.IsBuy, 1, -1)
'    nOldBS = IIf(aOldTrd.IsBuy, 1, -1)
'
'    If aNewTrd.ContractType = enCtOption Then
'        sExpiryKey = CStr(CLng(DateSerial(Year(aNewTrd.Opt.Expiry), Month(aNewTrd.Opt.Expiry), 1)))
'
'        Set aOpt = aUnd.Expiry(sExpiryKey).Strike(CStr(aNewTrd.Opt.Strike)).Opt(aNewTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If (g_CurTraderID = 0 Or aOldTrd.Trader.ID = g_CurTraderID) And aOldTrd.Status <> enTsSimulated Then
'                If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'                aOpt.Qty = aOpt.Qty - aOldTrd.Quantity * nOldBS
'                If IsBadDouble(aOpt.Pos) Then aOpt.Pos = 0
'                aOpt.Pos = aOpt.Pos - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'                bUpdate = True
'            End If
'
'            If (g_CurTraderID = 0 Or aNewTrd.Trader.ID = g_CurTraderID) And aNewTrd.Status <> enTsSimulated Then
'                If IsBadLong(aOpt.Qty) Then aOpt.Qty = 0
'                aOpt.Qty = aOpt.Qty + aNewTrd.Quantity * nNewBS
'                If IsBadDouble(aOpt.Pos) Then aOpt.Pos = 0
'                aOpt.Pos = aOpt.Pos + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'                bUpdate = True
'            End If
'        End If
'
'    Else
'        If (g_CurTraderID = 0 Or aOldTrd.Trader.ID = g_CurTraderID) And aOldTrd.Status <> enTsSimulated Then
'            If IsBadLong(aUnd.QtyInShares) Then aUnd.QtyInShares = 0
'            aUnd.QtyInShares = aUnd.QtyInShares - aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'            If IsBadDouble(aUnd.Pos) Then aUnd.Pos = 0
'            aUnd.Pos = aUnd.Pos - aOldTrd.Price * aOldTrd.Quantity * aOldTrd.LotSize * nOldBS
'            bUpdate = True
'        End If
'
'        If (g_CurTraderID = 0 Or aNewTrd.Trader.ID = g_CurTraderID) And aNewTrd.Status <> enTsSimulated Then
'            If IsBadLong(aUnd.QtyInShares) Then aUnd.QtyInShares = 0
'            aUnd.QtyInShares = aUnd.QtyInShares + aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'            If IsBadDouble(aUnd.Pos) Then aUnd.Pos = 0
'            aUnd.Pos = aUnd.Pos + aNewTrd.Price * aNewTrd.Quantity * aNewTrd.LotSize * nNewBS
'            bUpdate = True
'        End If
'
'        If bUpdate Then
'            bRatesUpdated = UnderlyingAdjustRates(aUnd, False)
'        End If
'    End If
'
'    If bUpdate Then
'        nRow = fgUnd.FindRow(aUnd.ID, , VUC_KEY)
'
'        If bRatesUpdated Then UnderlyingCalc aUnd, True
'
'        If aNewTrd.ContractType = enCtOption Or bRatesUpdated Then
'            UnderlyingCalcTotals aUnd
'            UpdateTotals
'        End If
'
'        If nRow > 0 Then
'            UnderlyingUpdate nRow, False, aUnd
'        End If
'    End If
'    Set aOpt = Nothing
'End Sub
'
'Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim aOpt As clsVmOptAtom, nRow&, nBS&, bUpdate As Boolean
'    Dim aUnd As clsVmUndAtom, sExpiryKey$, bRatesUpdated As Boolean
'
'    If g_CurTraderID <> 0 And aTrd.Trader.ID <> g_CurTraderID Or aTrd.Status = enTsSimulated Then Exit Sub
'
'    Set aUnd = m_Und(CStr(aTrd.UndID))
'    If aUnd Is Nothing Then Exit Sub
'
'    bUpdate = False
'    bRatesUpdated = False
'    nBS = IIf(aTrd.IsBuy, 1, -1)
'
'    If aTrd.ContractType = enCtOption Then
'        sExpiryKey = CStr(CLng(DateSerial(Year(aTrd.Opt.Expiry), Month(aTrd.Opt.Expiry), 1)))
'
'        Set aOpt = aUnd.Expiry(sExpiryKey).Strike(CStr(aTrd.Opt.Strike)).Opt(aTrd.Opt.OptType)
'        If Not aOpt Is Nothing Then
'            If Not IsBadLong(aOpt.Qty) Then
'                aOpt.Qty = aOpt.Qty - aTrd.Quantity * nBS
'                bUpdate = True
'            End If
'            If Not IsBadDouble(aOpt.Pos) Then
'                aOpt.Pos = aOpt.Pos - aTrd.Quantity * aTrd.LotSize * nBS
'                bUpdate = True
'            End If
'            Set aOpt = Nothing
'        End If
'
'    Else
'        If Not IsBadLong(aUnd.QtyInShares) Then
'            aUnd.QtyInShares = aUnd.QtyInShares - aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'        End If
'
'        If Not IsBadDouble(aUnd.Pos) Then
'            aUnd.Pos = aUnd.Pos - aTrd.Price * aTrd.Quantity * aTrd.LotSize * nBS
'            bUpdate = True
'
'            bRatesUpdated = UnderlyingAdjustRates(aUnd, False)
'        End If
'    End If
'
'    If bUpdate Then
'        nRow = fgUnd.FindRow(aUnd.ID, , VUC_KEY)
'
'        If bRatesUpdated Then UnderlyingCalc aUnd, True
'
'        If aTrd.ContractType = enCtOption Or bRatesUpdated Then
'            UnderlyingCalcTotals aUnd
'            UpdateTotals
'        End If
'
'        If nRow > 0 Then
'            UnderlyingUpdate nRow, False, aUnd
'        End If
'    End If
'    Set aOpt = Nothing
'End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bDataLoad Then Exit Sub
    If m_Grp.Data <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True

    ' futures & futures options trades are not supported yet
    If Not aNewTrdInfo Is Nothing Then _
        If aNewTrdInfo.ContractType = enCtFuture Or aNewTrdInfo.ContractType = enCtFutOption Then Exit Sub
    If Not aOldTrdInfo Is Nothing Then _
        If aOldTrdInfo.ContractType = enCtFuture Or aOldTrdInfo.ContractType = enCtFutOption Then Exit Sub

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
End Sub

Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If m_bDataLoad Then Exit Sub
    If m_Grp.Data <> 0 And m_Grp.ID <> 0 Then SetRefreshHint True

'    Debug.Assert Not aTrdFrom Is Nothing
'    Debug.Assert Not aTrdTo Is Nothing
'
'    ' futures & futures options trades are not supported yet
'    If aTrdFrom.ContractType = enCtFuture Or aTrdFrom.ContractType = enCtFutOption Then Exit Sub
'    If aTrdTo.ContractType = enCtFuture Or aTrdTo.ContractType = enCtFutOption Then Exit Sub
'
'    AddTrade aTrdFrom
'    AddTrade aTrdTo
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
    Set frmVola = Nothing
    Set TradeChannel = Nothing
    Set VolaSource = Nothing
    
    Set geUnd = Nothing
    
    If Not PriceProvider Is Nothing Then
        PriceProvider.CancelLastQuote
        If m_bGroupRequest Then
            m_GroupPriceProvider.CancelGroup
        End If
        Set PriceProvider = Nothing
    End If
    Set m_GroupPriceProvider = Nothing
    
    Set m_QuoteRequests = Nothing
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

Private Sub UpdateTotals()
    On Error Resume Next
    Dim sTotals$, aUnd As clsVmUndAtom
    Dim sValue As String ' * 20
    
    With fgUnd
        If .Row > 0 Then
            Set aUnd = .RowData(.Row)
        End If
    End With
    
    sTotals = ""
    If Not aUnd Is Nothing Then
        sTotals = aUnd.Symbol & " - "
        With aUnd
            If .ID <> 0 Then
                sTotals = sTotals & "NetDlt: "
                If Not IsBadDouble(.NetDelta) Then
                    sValue = Format$(.NetDelta, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue & "    "
                
                sTotals = sTotals & "OptDlt: "
                If Not IsBadDouble(.TotalDelta) Then
                    sValue = Format$(.TotalDelta, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue & "    "
                
                sTotals = sTotals & "Gma$1: "
                If Not IsBadDouble(.TotalGamma) Then
                    sValue = Format$(.TotalGamma, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue & "    "
                
                sTotals = sTotals & "Vga: "
                If Not IsBadDouble(.TotalVega) Then
                    sValue = Format$(.TotalVega, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue & "    "
                
                sTotals = sTotals & "Tht: "
                If Not IsBadDouble(.TotalTheta) Then
                    sValue = Format$(.TotalTheta, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue & "    "
                
                sTotals = sTotals & "Rho: "
                If Not IsBadDouble(.TotalRho) Then
                    sValue = Format$(.TotalRho, "#,##0.00")
                Else
                    sValue = "N/A"
                End If
                sTotals = sTotals & sValue
            Else
                sTotals = sTotals & "N/A"
            End If
        End With
        
        Set aUnd = Nothing
    End If
    
    lblStatus.Caption = sTotals
End Sub

Public Property Get VolaShiftEnabled() As Boolean
    On Error Resume Next
    Dim aUnd As clsVmUndAtom
    
    VolaShiftEnabled = False
    
    If Not m_bInProc And Not m_bLastQuoteReqNow And Not m_bDataLoad Then
        With fgUnd
            If .Row > 0 Then
                Set aUnd = .RowData(.Row)
                If Not aUnd Is Nothing Then
                    VolaShiftEnabled = True
                    Set aUnd = Nothing
                End If
            End If
        End With
    End If
End Property

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom
    If m_nFilter(VFC_GROUP) = TYPE_UNDERLYING Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then FilterUpdateValue False
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If m_nFilter(VFC_GROUP) = TYPE_GROUP Then FilterUpdateValue False
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    If m_nFilter(VFC_GROUP) = TYPE_STRATEGY Then FilterUpdateValue False
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, m_Grp.Name & " Volatility Monitor", "Volatility Monitor", fgFlt, fgUnd
'    Screen.MousePointer = vbDefault
End Sub

Private Sub InitVola(ByRef aUnd As clsVmUndAtom)
    On Error GoTo EH
    aUnd.VolaSrv.Init aUnd.Symbol, aUnd.UndType, VolaSource
    Exit Sub
EH:
    LogEvent EVENT_WARNING, "Fail to init volatility for '" & aUnd.Symbol & "': " & Err.Description
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
