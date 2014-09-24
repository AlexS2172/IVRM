VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxRiskView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_bShutDown As Boolean
Private m_RiskView As EtsMmRisksLib.MmRiskView

Private m_Idx As EtsMmRisksLib.MmRvUndAtom
Private m_Grp As EtsMmRisksLib.MmRvGrpAtom
Private m_Und As EtsMmRisksLib.MmRvUndColl

Public VolaSource As VolatilitySourcesLib.VolatilitySource
Public gePos As clsGridEx

Public gdFlt As clsGridDef
Public gdTot As clsGridDef
Public gdPos As clsGridDef

Public fgFlt As VSFlex7Ctl.VSFlexGrid
Public fgPos As VSFlex7Ctl.VSFlexGrid
Public fgTot As VSFlex7Ctl.VSFlexGrid

Private m_aFilter As EtsGeneralLib.EtsFilterData

Private m_GridLock(GT_RISKS_FILTER To GT_RISKS_POSITIONS) As New clsGridLock

Public RealTime As Boolean
Public RealTimeConnected As Boolean

Public SortColOrder As VSFlex7Ctl.SortSettings
Public SortColKey As Long

Private Sub Class_Initialize()
On Error GoTo error_handler
'--------------------------'

    Dim code_line As Long
    
    code_line = 0
    
    Set m_RiskView = New EtsMmRisksLib.MmRiskView
    Set m_Idx = m_RiskView.Idx
    Set m_Grp = m_RiskView.Grp
    Set m_Und = m_RiskView.Und
    Set m_RiskView.EtsMain = g_Main
    
    code_line = code_line + 1
    m_RiskView.setRealtimeCalculateCv g_Params.RealtimeCalculateCV
    
    code_line = code_line + 1
    m_RiskView.InitializeCvContext "RiskView"
    
    code_line = code_line + 1
    Set m_aFilter = New EtsGeneralLib.EtsFilterData
    
    code_line = code_line + 1
    InitRisksGridsWithCV
    
    code_line = code_line + 1
    m_aFilter.Initialize RFC_SYMBOL, RFC_LAST_COLUMN
    
    m_bShutDown = False
        
    Exit Sub
'-------------'
error_handler:
    Debug.Assert False
End Sub

Public Sub Term()
    On Error Resume Next
    
    m_bShutDown = True
    
    Set gePos = Nothing
    Set VolaSource = Nothing
    
    Set gdFlt = Nothing
    Set gdTot = Nothing
    Set gdPos = Nothing
    
    Set m_Idx = Nothing
    Set m_Grp = Nothing
    Set m_Und = Nothing
    Set m_RiskView = Nothing
    Set m_aFilter = Nothing
    
    Erase m_GridLock
    
    Set fgFlt = Nothing
    Set fgTot = Nothing
    Set fgPos = Nothing
    
End Sub

Public Property Get GridLock(nIdx&) As clsGridLock
    Set GridLock = m_GridLock(nIdx)
End Property
Public Property Get FilterData() As EtsGeneralLib.EtsFilterData
    Set FilterData = m_aFilter
End Property
Public Property Get Filter&(nIdx&)
    Filter = m_aFilter.Data(nIdx)
End Property

Public Property Let Filter(nIdx&, nFilter&)
    m_aFilter.Data(nIdx) = nFilter
End Property

Public Property Get RiskView() As EtsMmRisksLib.MmRiskView
    Set RiskView = m_RiskView
End Property

Public Sub KillRiskView()
    Set m_RiskView = Nothing
End Sub

Public Property Get Idx() As EtsMmRisksLib.MmRvUndAtom
    Set Idx = m_Idx
End Property

Public Property Let Idx(aIdx As EtsMmRisksLib.MmRvUndAtom)
    Set m_Idx = aIdx
End Property

Public Property Get Grp() As EtsMmRisksLib.MmRvGrpAtom
    Set Grp = m_Grp
End Property

Public Property Let Grp(aGrp As EtsMmRisksLib.MmRvGrpAtom)
    Set m_Grp = aGrp
End Property

Public Property Get Und() As EtsMmRisksLib.MmRvUndColl
    Set Und = m_Und
End Property

Public Property Let Und(aUnd As EtsMmRisksLib.MmRvUndColl)
    Set m_Und = aUnd
End Property

Public Property Get Exp() As EtsGeneralLib.EtsMmEntityAtomColl
    Set Exp = m_RiskView.Exp
End Property


Public Sub PosGridCompare(ByVal Row1&, ByVal Row2&, ByRef Cmp%)
    On Error Resume Next
End Sub

Public Sub InitFltData()
    On Error Resume Next
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw

        .ColComboList(RFC_TRADES) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, RFC_TRADES) = "0"
    
        .ColComboList(RFC_EXPIRY) = "#0;<All>"
        .TextMatrix(1, RFC_EXPIRY) = "0"
        Filter(RFC_EXPIRY) = 0
    
        .ColComboList(RFC_INDEX) = "#0;<None>"
        .TextMatrix(1, RFC_INDEX) = "0"
        Filter(RFC_INDEX) = 0
            
        FilterUpdateValue True, RFC_SYMBOL
        FilterUpdateValue True, RFC_GROUPS
        FilterUpdateValue True, RFC_TRADER
        FilterUpdateValue True, RFC_TRADER_GROUP
        FilterUpdateValue True, RFC_STRATEGY

        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .rows = 2
        .Cols = RFC_COLUMN_COUNT
        
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
    
    With fgTot
        .rows = 2
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
        .ExplorerBar = flexExMove
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

    With fgPos
        .rows = 1
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
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .OutlineCol = 1
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
    End With
End Sub

Public Sub FormatFltGrid()
    On Error Resume Next
    Dim i&
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        .Cols = RFC_COLUMN_COUNT
        
        For i = RFC_SYMBOL To RFC_LAST_COLUMN
            .TextMatrix(0, i) = gdFlt.Col(i).Caption
            .ColFormat(i) = gdFlt.Col(i).Format
            .ColDataType(i) = gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        Set .Font = Nothing
        
        aFont.Name = gdFlt.Font.Name
        aFont.Size = gdFlt.Font.Size
        aFont.Bold = gdFlt.Font.Bold
        aFont.Italic = gdFlt.Font.Italic
        aFont.Strikethrough = gdFlt.Font.Strikethrough
        aFont.Underline = gdFlt.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .rows - 1
        For i = RFC_SYMBOL To RFC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdFlt.Col(i).BackColor <> 0, gdFlt.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdFlt.Col(i).ForeColor <> 0, gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
        Next
    
        If gdFlt.HorizLinesVisible Then
            If gdFlt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdFlt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdFlt.GridLinesColor
        
        .BackColorSel = gdFlt.BackColorSel
        .ForeColorSel = gdFlt.ForeColorSel
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FormatTotGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgTot
        m_GridLock(GT_RISKS_TOTALS).LockRedraw
    
        nCols = 0
        For i = 0 To g_RTC_LAST_COLUMN
            If gdTot.Idx(i) <> RTC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            gdTot.Idx(0) = RTC_PNL_MTM
            nCols = 1
        End If
        
        .Cols = nCols
        
        For i = 0 To g_RTC_LAST_COLUMN
            nIdx = gdTot.Idx(i)
            If nIdx <> RTC_NONE Then
                .ColDataType(i) = gdTot.Col(nIdx).DataType
                .ColFormat(i) = gdTot.Col(nIdx).Format
                .ColKey(i) = nIdx
                .TextMatrix(0, i) = gdTot.Col(nIdx).Caption
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub

Public Sub FormatTotColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgTot
        m_GridLock(GT_RISKS_TOTALS).LockRedraw

        Set .Font = Nothing
        
        aFont.Name = gdTot.Font.Name
        aFont.Size = gdTot.Font.Size
        aFont.Bold = gdTot.Font.Bold
        aFont.Italic = gdTot.Font.Italic
        aFont.Strikethrough = gdTot.Font.Strikethrough
        aFont.Underline = gdTot.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> RTC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdTot.Col(nIdx).BackColor <> 0, gdTot.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdTot.Col(nIdx).ForeColor <> 0, gdTot.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next

        If gdTot.HorizLinesVisible Then
            If gdTot.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdTot.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdTot.GridLinesColor

        .BackColorSel = gdTot.BackColorSel
        .ForeColorSel = gdTot.ForeColorSel

        .AutoSize 0, .Cols - 1

        m_GridLock(GT_RISKS_TOTALS).UnlockRedraw
    End With
End Sub

Public Sub FormatPosGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgPos
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw
    
        gdPos.Idx(0) = RPC_SYMBOL
        nCols = 1
        For i = 1 To g_RPC_LAST_COLUMN
            If gdPos.Idx(i) <> RPC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColHidden(RPC_KEY) = True
        .ColKey(RPC_KEY) = RPC_NONE
        .ColDataType(RPC_KEY) = flexDTString
        
        Set .Cell(flexcpPicture, 1, 0, .rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        For i = 0 To g_RPC_LAST_COLUMN
            nIdx = gdPos.Idx(i)
            If nIdx <> RPC_NONE Then
                .TextMatrix(0, i + 1) = gdPos.Col(nIdx).Caption
                .ColFormat(i + 1) = gdPos.Col(nIdx).Format
                .ColDataType(i + 1) = gdPos.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(gdPos.Col(nIdx).Width > 0, gdPos.Col(nIdx).Width, -1)
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        gePos.AdjustSortImages
        
        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub FormatPosColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont, aRowData As EtsMmRisksLib.MmRvRowData
    
    With fgPos
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw

        Set .Font = Nothing
        
        aFont.Name = gdPos.Font.Name
        aFont.Size = gdPos.Font.Size
        aFont.Bold = gdPos.Font.Bold
        aFont.Italic = gdPos.Font.Italic
        aFont.Strikethrough = gdPos.Font.Strikethrough
        aFont.Underline = gdPos.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> RPC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdPos.Col(nIdx).BackColor <> 0, gdPos.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdPos.Col(nIdx).ForeColor <> 0, gdPos.Col(nIdx).ForeColor, RGB(1, 1, 1))
                
                If nRows > 1 Then
                    If nIdx = RPC_OPT_TYPE Then
                        .ColAlignment(i) = flexAlignCenterCenter
                    Else
                        .ColAlignment(i) = flexAlignGeneral
                    End If
                End If
            End If
        Next
        
        If gdPos.HorizLinesVisible Then
            If gdPos.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf gdPos.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = gdPos.GridLinesColor
        
        .BackColorSel = gdPos.BackColorSel
        .ForeColorSel = gdPos.ForeColorSel

        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End With
End Sub

Public Sub PosBeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    SortColKey = fgPos.ColKey(Col)
    SortColOrder = IIf(SortColOrder = flexSortGenericDescending, flexSortGenericAscending, flexSortGenericDescending)
        
    Order = flexSortCustom
End Sub

Public Sub PosAfterSort(ByVal Col&, ByRef Order%)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim bAsc As Boolean, nKey&
    
    nKey = fgPos.ColKey(Col)
    If SortColKey = nKey Then
        Order = SortColOrder
        SortColKey = -2
    End If
    
    If nKey <> RPC_OPT_TYPE And nKey <> RPC_EXPIRY And nKey <> RPC_STRIKE And nKey <> RPC_UND _
    And nKey <> RPC_DELTAP And nKey <> RPC_GAMMAP And nKey <> RPC_VEGAP And nKey <> RPC_THETAP And nKey <> RPC_RHOP Then 'And nKey <> RPC_SYNTHETIC_PRICE Then
        Select Case SortColOrder
            Case flexSortStringNoCaseAscending, flexSortStringAscending, flexSortNumericAscending, flexSortGenericAscending
                bAsc = True
            Case Else
                bAsc = False
        End Select
        
        m_GridLock(GT_RISKS_POSITIONS).LockRedraw
        SortPosNodes Col, bAsc
        m_GridLock(GT_RISKS_POSITIONS).UnlockRedraw
    End If
End Sub

Public Sub SortPosNodes(ByVal nCol As Long, ByVal bAsc As Boolean)
    On Error Resume Next
    
End Sub

Public Function CheckPosFilter(ByRef aPos As EtsMmRisksLib.MmRvPosAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    nValue = Filter(RFC_EXPIRY)
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

Public Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&
    
    'bMatched = False
    bMatched = True
    
    If Not aTrd Is Nothing Then
        
        If bMatched And Filter(RFC_SYMBOL) > 0 Then _
            bMatched = (aTrd.UndID = Filter(RFC_SYMBOL))
        If bMatched And Filter(RFC_GROUPS) > 0 Then _
            bMatched = Not g_UnderlyingGroup(Filter(RFC_GROUPS)).Und(aTrd.UndID) Is Nothing
        If bMatched And Filter(RFC_TRADER) > 0 Then _
            bMatched = (aTrd.Trader.ID = Filter(RFC_TRADER))
        If bMatched And Filter(RFC_TRADER_GROUP) > 0 Then _
            bMatched = (aTrd.Trader.GroupID = Filter(RFC_TRADER_GROUP))
        If bMatched And Filter(RFC_STRATEGY) <> 0 Then _
            bMatched = (aTrd.strategyID = IIf(Filter(RFC_STRATEGY) > 0, Filter(RFC_STRATEGY), 0))
        
        If bMatched Then
            nValue = Filter(RFC_TRADES)
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

    CheckTradeFilter = bMatched

End Function

Public Sub FilterUpdateAll()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        
        FilterUpdateValue False, RFC_SYMBOL
        FilterUpdateValue False, RFC_GROUPS
        FilterUpdateValue False, RFC_TRADER
        FilterUpdateValue False, RFC_TRADER_GROUP
        
        FilterUpdateValue False, RFC_STRATEGY
        .TextMatrix(1, RFC_TRADES) = Filter(RFC_TRADES)
        
        FilterUpdateIndex False
        FilterUpdateExpiry False
        FilterUpdateModel False
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With

End Sub
Public Sub FilterUpdateValue(ByVal bAutosize As Boolean, enColumn As RisksFilterColumnEnum)
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
           Case RFC_SYMBOL
                sComboList = g_Params.UnderlyingComboList
                
                Set aUnd = g_Underlying(Filter(RFC_SYMBOL))
                If Not aUnd Is Nothing Then
                    nValue = aUnd.ID
                    sValue = Trim$(Str$(nValue))
                    Set aUnd = Nothing
                End If
                
                If Len(sComboList) > 0 And nValue = 0 Then sComboList = "|" & sComboList
           Case RFC_GROUPS
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = Filter(RFC_GROUPS) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case RFC_TRADER
               For Each aTrader In g_Trader
                    sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
                    If aTrader.ID = Filter(RFC_TRADER) Then
                        nValue = aTrader.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case RFC_TRADER_GROUP
               For Each aTraderGroup In g_TraderGroup
                    If aTraderGroup.ID <> 0 Then
                        sComboList = sComboList & "|#" & Trim$(Str$(aTraderGroup.ID)) & ";" & aTraderGroup.Name
                    End If
                    If aTraderGroup.ID = Filter(RFC_TRADER_GROUP) Then
                        nValue = aTraderGroup.ID
                        sValue = Trim$(Str$(nValue))
                    End If
                Next
           Case RFC_STRATEGY
                sComboList = "|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
                If Filter(RFC_STRATEGY) = NO_STRATEGY_ID Then
                    nValue = NO_STRATEGY_ID
                    sValue = NO_STRATEGY_NAME
                End If
                
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = Filter(RFC_STRATEGY) Then
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
        Filter(enColumn) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateIndex(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim sComboList$, sValue$, m_Idx As EtsGeneralLib.IndexAtom
    Dim nValue&, nFirstValue&
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        nFirstValue = 0
        sComboList = ""
        For Each m_Idx In g_HedgeSymbols
            If m_Idx.HaveComponentBetas Then
                sComboList = sComboList & "|#" & Trim$(Str$(m_Idx.ID)) & ";" & m_Idx.Symbol
                If m_Idx.ID = Filter(RFC_INDEX) Then
                    nValue = m_Idx.ID
                    sValue = Trim$(Str$(m_Idx.ID))
                End If
                If nFirstValue = 0 Then
                    If (Len(g_Params.HedgeSymbol) > 0) Then
                        If (g_Params.HedgeSymbol = m_Idx.Symbol) Then
                            nFirstValue = m_Idx.ID
                        End If
                    Else
                        nFirstValue = m_Idx.ID
                    End If
                End If
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
        
        .ColComboList(RFC_INDEX) = sComboList
        .TextMatrix(1, RFC_INDEX) = sValue
        Filter(RFC_INDEX) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateExpiry(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nValue&, aExp As EtsGeneralLib.EtsMmEntityAtom, sValue$, sComboList$
    
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aExp In m_RiskView.Exp
            sComboList = sComboList & "|#" & Trim$(Str$(aExp.ID)) & ";" & aExp.Name
            If aExp.ID = Filter(RFC_EXPIRY) Then
                nValue = aExp.ID
                sValue = Trim$(Str$(aExp.ID))
            End If
        Next
        .ColComboList(RFC_EXPIRY) = sComboList
        .TextMatrix(1, RFC_EXPIRY) = sValue
        Filter(RFC_EXPIRY) = nValue
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub FilterUpdateModel(ByVal bAutosize As Boolean)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgFlt
        m_GridLock(GT_RISKS_FILTER).LockRedraw
    
        .TextMatrix(1, RFC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        
        If bAutosize Then .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_RISKS_FILTER).UnlockRedraw
    End With
End Sub

Public Sub GridAfterMoveColumn(ByRef gd As clsGridDef, ByVal Col&, ByRef Position&, ByVal nFirstCol&)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - nFirstCol
    nNewPos = Position - nFirstCol
    nIdx = gd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then gd.Idx(i) = gd.Idx(i - 1)
        Next
        gd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then gd.Idx(i) = gd.Idx(i + 1)
        Next
        gd.Idx(nNewPos) = nIdx
    End If
End Sub

Public Sub GridBeforeMoveColumn(ByRef fg As VSFlex7Ctl.VSFlexGrid, ByRef gd As clsGridDef, _
                                ByVal Col&, ByRef Position&, ByVal nNoneCol&)
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fg.ColKey(Col)
    nNewIdx = fg.ColKey(Position)
    
    If nOldIdx = nNoneCol Or nNewIdx = nNoneCol Then
        Position = Col
    ElseIf Not gd.Col(nOldIdx).CanChangeOrder Or Not gd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Function OptColumnExist(ByVal cvID As Long) As Boolean
On Error GoTo error_handler
'--------------------------'
   
    Dim lResult As Long
    
    lResult = m_RiskView.getIsCvInitialized(CVLEVEL_POS, cvID)
    
    OptColumnExist = IIf(lResult <> 0, True, False)
    
    Exit Function
    
error_handler:
    Debug.Assert True
End Function

Public Sub InitRisksGridsWithCV(Optional ByVal nNewVarID As Long = BAD_LONG_VALUE)
On Error GoTo error_handler
'--------------------------'

    Dim Info As CVLib.cvInfo
    Dim nOptCount As Long
    Dim nUndCount As Long
    Dim nGrpCount As Long
    
    Dim nNewOptColID As Long: nNewOptColID = BAD_LONG_VALUE
    Dim nNewGrpColID As Long: nNewGrpColID = BAD_LONG_VALUE
   
    Dim OptIDs() As Long
    Dim UndIDs() As Long
    Dim GrpIDs() As Long
        
    Dim cvContext As CVLib.cvRTContext
    
    Set cvContext = m_RiskView.cvRTContext
    
    cvContext.SetRecalculateCvCount g_Params.CalculateCVCount
    
    OptIDs = cvContext.GetCVIDs(m_RiskView.getCvLevelID(CVLEVEL_POS))
    nOptCount = UBound(OptIDs) + 1
   
    UndIDs = cvContext.GetCVIDs(m_RiskView.getCvLevelID(CVLEVEL_UND))
    nUndCount = UBound(UndIDs) + 1
   
    GrpIDs = cvContext.GetCVIDs(m_RiskView.getCvLevelID(CVLEVEL_GRP))
    nGrpCount = UBound(GrpIDs) + 1
   
    InitRisksGrids nGrpCount, nOptCount + nUndCount
   
    Dim columnID As Long
    Dim sGridID As String
    Dim i As Long
    For i = 0 To nOptCount - 1
        Info = cvContext.GetCVInfo(m_RiskView.getCvLevelID(CVLEVEL_POS), OptIDs(i))

        columnID = AddRisksColumn(Info, GT_RISKS_POSITIONS)

        m_RiskView.setCvID2Column CVLEVEL_POS, columnID, OptIDs(i)

        sGridID = CStr(columnID)

        If nNewVarID = OptIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewOptColID = sGridID
    Next
'
    For i = 0 To nUndCount - 1
        If Not OptColumnExist(UndIDs(i)) Then
            Info = cvContext.GetCVInfo(m_RiskView.getCvLevelID(CVLEVEL_UND), UndIDs(i))

            columnID = AddRisksColumn(Info, GT_RISKS_POSITIONS)

            m_RiskView.setCvID2Column CVLEVEL_POS, columnID, UndIDs(i)

            sGridID = CStr(columnID)

            If nNewVarID = UndIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewOptColID = sGridID
        End If
    Next
'
    For i = 0 To nGrpCount - 1
        Info = cvContext.GetCVInfo(m_RiskView.getCvLevelID(CVLEVEL_GRP), GrpIDs(i))

        columnID = AddRisksColumn(Info, GT_RISKS_TOTALS)

        m_RiskView.setCvID2Column CVLEVEL_GRP, columnID, GrpIDs(i)

        sGridID = CStr(columnID)

        If nNewVarID = GrpIDs(i) And nNewVarID <> BAD_LONG_VALUE Then nNewGrpColID = sGridID
    Next
'
    LoadRisksDefaults
'
'    If nNewOptColID <> BAD_LONG_VALUE Then DisplayNewCol GT_RISKS_POSITIONS, nNewOptColID
'    If nNewGrpColID <> BAD_LONG_VALUE Then DisplayNewCol GT_RISKS_TOTALS, nNewGrpColID
    
    Exit Sub
'-------------'
error_handler:
    Debug.Assert True
End Sub
