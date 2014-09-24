VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsAuxQuotesView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private m_bShutDown As Boolean

Public QV As EtsMmQuotesLib.EtsMmQuotesView
Public Grp As EtsMmQuotesLib.MmQvGrpAtom
Public QuoteReqsAll As MmQvRequestColl

Public gdUnd As clsGridDef
Public gdDiv As clsGridDef
Public gdVol As clsGridDef
Public gdFut As clsGridDef
Public gdOpt As clsGridDef

Public geFut As clsGridEx
Public geOpt As clsGridEx

Public fgUnd As VSFlex7Ctl.VSFlexGrid
Public fgDiv As VSFlex7Ctl.VSFlexGrid
Public fgVol As VSFlex7Ctl.VSFlexGrid
Public fgFut As VSFlex7Ctl.VSFlexGrid
Public fgOpt As VSFlex7Ctl.VSFlexGrid

Private m_GridLock(GT_QUOTES_UNDERLYING To GT_QUOTES_OPTIONS) As New clsGridLock

Public RealTime As Boolean
Public RealTimeConnected As Boolean

Public SortColOrder As VSFlex7Ctl.SortSettings
Public SortColKey As Long

Public OptionsFilter As Long

Public CalcModelVisible As Boolean
Public DividendsVisible As Boolean
Public VolaVisible As Boolean
Public RatesVisible As Boolean
Public FuturesVisible As Boolean
Public IndexOptionsComboVisible As Boolean

Public UseCustRates As Boolean
Public IsAmerican As Long


Public IsVolaSimulated As Boolean
Public IsSimulatedFlat As Boolean
Public bFlatVolaToATM  As Boolean

Private Sub Class_Initialize()
    On Error Resume Next
    m_bShutDown = False
End Sub

Public Sub Term()
    On Error Resume Next
    m_bShutDown = True
    Set geOpt = Nothing
End Sub

Public Property Get GridLock(nIdx&) As clsGridLock
    Set GridLock = m_GridLock(nIdx)
End Property

Public Sub InitGrids()
    On Error Resume Next
    If m_bShutDown Then Exit Sub
    With fgUnd
        .Rows = 2
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
        .ExplorerBar = flexExMove
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
        .ColWidthMin = 150
    End With
    
    With fgDiv
        .Rows = 2
        .Cols = QDC_COLUMN_COUNT
        
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
        .ScrollBars = flexScrollBarHorizontal
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
        .ColWidthMin = 150
    End With
    
    With fgVol
        .Rows = 7
        .Cols = QVC_COLUMN_COUNT
        
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
        .FixedCols = 1
        .FixedRows = 2
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarHorizontal
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
        .ColWidthMin = 150
    End With
    
    With fgFut
        .Rows = 1
        .Cols = QOF_COLUMN_COUNT
        
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
        .HighLight = flexHighlightAlways
        .ColWidthMin = 150
    End With
    
    With fgOpt
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
        .HighLight = flexHighlightAlways
        .ColWidthMin = 150
    End With
End Sub

Public Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&, strFuturesComboList$
    
    
    With fgUnd
        m_GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        gdUnd.Idx(0) = QUC_SYMBOL
        nCols = 1
        For i = 1 To QUC_LAST_COLUMN
            If gdUnd.Idx(i) <> QUC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColHidden(QUC_KEY) = True
        .ColKey(QUC_KEY) = QUC_NONE
        strFuturesComboList = BuildFuturesCombolist
        
        For i = 0 To QUC_LAST_COLUMN
            nIdx = gdUnd.Idx(i)
            If nIdx <> QUC_NONE Then
                .TextMatrix(0, i + 1) = gdUnd.Col(nIdx).Caption
                .ColFormat(i + 1) = gdUnd.Col(nIdx).Format
                .ColDataType(i + 1) = gdUnd.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(gdUnd.Col(nIdx).Width > 0, gdUnd.Col(nIdx).Width, -1)
                
                If QUC_ACTIVEFUTURE = nIdx Then
                    If Len(strFuturesComboList) > 0 Then
                        .ColComboList(i + 1) = strFuturesComboList
                    Else
                        .ColComboList(i + 1) = "#0;<None>"
                    End If
                    
                End If
                
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        Set .Cell(flexcpPicture, 1, 0, .Rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        
        m_GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
End Sub

Public Sub FormatDivGrid()
    On Error Resume Next
    Dim i&, sComboList$
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    With fgDiv
        m_GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
    
        .Cols = QDC_LAST_COLUMN
        
        For i = QDC_TRADES To QDC_LAST_COLUMN
            .TextMatrix(0, i) = gdDiv.Col(i).Caption
            .ColFormat(i) = gdDiv.Col(i).Format
            .ColDataType(i) = gdDiv.Col(i).DataType
        Next
        
        .ColComboList(QDC_TRADES) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, QDC_TRADES) = CStr(QV.TradeFilter.Data(enFtTrades))
        
        .ColComboList(QDC_OPTIONS) = "#0;Index|#1;Futures"
        .TextMatrix(1, QDC_OPTIONS) = OptionsFilter
        
        .TextMatrix(1, QDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        .ColComboList(QDC_STYLE) = "#0;European|#1;American"
        .TextMatrix(1, QDC_STYLE) = "1"
        
        .ColEditMask(QDC_DATE) = "99/99/9999"
        
        .TextMatrix(1, QDC_DATECALC) = ClipSeconds(Now)
        
        sComboList = "#0;<All>"
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
        Next
        .ColComboList(QDC_FLT_TRADER) = sComboList
        .TextMatrix(1, QDC_FLT_TRADER) = IIf(g_CurTraderID <> 0, CStr(g_CurTraderID), CStr(QV.TradeFilter.Data(enFtTrader)))
        
        sComboList = "#0;<All>"
        For Each aStrategy In g_Strategy
            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
        Next
        .ColComboList(QDC_FLT_STRATEGY) = sComboList
        .TextMatrix(1, QDC_FLT_STRATEGY) = CStr(QV.TradeFilter.Data(enFtStrategy))
        
        sComboList = ""
        For Each aPrProf In g_PriceProfile
            If Not aPrProf.IsOptionProfile Then sComboList = sComboList & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
        Next
        If Len(sComboList) > 0 Then
            .ColComboList(QDC_UNDPROFILE) = Mid$(sComboList, 2)
            .TextMatrix(1, QDC_UNDPROFILE) = CStr(aPrProf.ID)
        Else
            .ColComboList(QDC_UNDPROFILE) = "#0;<None>"
            .TextMatrix(1, QDC_UNDPROFILE) = "0"
        End If
        
        sComboList = ""
        For Each aPrProf In g_PriceProfile
            If aPrProf.IsOptionProfile Then sComboList = sComboList & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
        Next
        If Len(sComboList) > 0 Then
            .ColComboList(QDC_OPTPROFILE) = Mid$(sComboList, 2)
            .TextMatrix(1, QDC_OPTPROFILE) = CStr(aPrProf.ID)
        Else
            .ColComboList(QDC_OPTPROFILE) = "#0;<None>"
            .TextMatrix(1, QDC_OPTPROFILE) = "0"
        End If
        
        .TextMatrix(1, QDC_DIV) = "0"
        .ColComboList(QDC_DIV) = "#0;Market|#1;Custom Periodical|#2;Custom Stream"
        .ColComboList(QDC_FREQ) = "#0;<No Divs>|#1;Annually|#2;Semiannually|#4;Quarterly|#12;Monthly"
        
        .TextMatrix(1, QDC_RATE) = "0"
        .ColComboList(QDC_RATE) = "#0;Market|#1;Custom"
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub FormatVolGrid()
    On Error Resume Next
    Dim val&
    With fgVol
        m_GridLock(GT_QUOTES_VOLA).LockRedraw
    
        .Cols = QVC_LAST_COLUMN
        .Rows = 7
        .FixedRows = 2
        .FixedCols = 1
        
        .TextMatrix(0, QVC_VOLA) = "Moneyness"
        .TextMatrix(1, QVC_VOLA) = "Moneyness"
        
        .TextMatrix(2, 0) = "+20%"
        .TextMatrix(3, 0) = "+10%"
        .TextMatrix(4, 0) = "ATM"
        .TextMatrix(5, 0) = "-10%"
        .TextMatrix(6, 0) = "-20%"
               
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeCol(0) = True
        
        .ColFormat(QVC_VOLA) = gdVol.Col(QVC_VOLA).Format
        .ColDataType(QVC_VOLA) = gdVol.Col(QVC_VOLA).DataType
       
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        .ColAlignment(0) = flexAlignCenterCenter
        .RowHeight(1) = 250

        m_GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Sub FormatFutGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    Dim sComboList_Und$, sComboList_Opt$, strFuturesComboList$
    Dim aPrProf As EtsGeneralLib.EtsPriceProfileAtom
    
    With fgFut
        m_GridLock(GT_QUOTES_FUTURES).LockRedraw
    
        nCols = 0
        For i = 0 To QOF_LAST_COLUMN
            If gdFut.Idx(i) <> QOF_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            gdFut.Idx(0) = QOF_SYMBOL
            nCols = 1
        End If
        

        nCols = nCols + 1 ' for key columns
        .Cols = nCols

        .ColHidden(QOF_KEY) = True
        .ColKey(QOF_KEY) = QOC_NONE
        
        Set .Cell(flexcpPicture, 1, 0, .Rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        sComboList_Und = ""
        For Each aPrProf In g_PriceProfile
            If Not aPrProf.IsOptionProfile Then sComboList_Und = sComboList_Und & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
        Next
        
        sComboList_Opt = ""
        For Each aPrProf In g_PriceProfile
            If aPrProf.IsOptionProfile Then sComboList_Opt = sComboList_Opt & "|#" & Trim$(Str$(aPrProf.ID)) & ";" & aPrProf.Name
        Next
        
        strFuturesComboList = BuildFuturesCombolist
        
        For i = 0 To QOF_LAST_COLUMN
            nIdx = gdFut.Idx(i)
            If nIdx <> QOF_NONE Then
                'changed from 2 => 1
                .TextMatrix(0, i + 1) = gdFut.Col(nIdx).Caption
                .ColFormat(i + 1) = gdFut.Col(nIdx).Format
                .ColDataType(i + 1) = gdFut.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(gdFut.Col(nIdx).Width > 0, gdFut.Col(nIdx).Width, -1)
                                
                If QOF_UND_PROFILE = nIdx Then
                    If Len(sComboList_Und) > 0 Then
                        .ColComboList(i + 1) = Mid$(sComboList_Und, 2)
                    Else
                        .ColComboList(i + 1) = "#0;<None>"
                    End If
                End If
                If QOF_OPT_PROFILE = nIdx Then
                    If Len(sComboList_Opt) > 0 Then
                        .ColComboList(i + 1) = Mid$(sComboList_Opt, 2)
                    Else
                        .ColComboList(i + 1) = "#0;<None>"
                    End If
                End If
                If QOF_ACTIVE = nIdx Then
                    If Len(strFuturesComboList) > 0 Then
                        .ColComboList(i + 1) = strFuturesComboList
                    Else
                        .ColComboList(i + 1) = "#0;<None>"
                    End If
                    
                End If
            Else
                Exit For
            End If
        Next
               
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geFut.AdjustSortImages
        
        m_GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With
End Sub
Public Function BuildFuturesCombolist() As String
    Dim aFut As MmQvFutAtom
    
    BuildFuturesCombolist = ""
    If Not QV.Grp.Und Is Nothing And Not QV.Grp.Und.Fut Is Nothing Then
        If QV.Grp.Und.Fut.Count > 0 Then
            BuildFuturesCombolist = "#0;<None>"
            For Each aFut In QV.Grp.Und.Fut
                BuildFuturesCombolist = BuildFuturesCombolist & "|#" & CStr(aFut.ID) & ";" & aFut.Symbol
            Next
        End If
    End If
End Function


Public Sub FormatOptGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgOpt
        m_GridLock(GT_QUOTES_OPTIONS).LockRedraw
    
        nCols = 0
        For i = 0 To QOC_LAST_COLUMN
            If gdOpt.Idx(i) <> QOC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            gdOpt.Idx(0) = QOC_C_SYMBOL
            nCols = 1
        End If
        
        nCols = nCols + 2 ' for key columns
        .Cols = nCols
        
        .ColHidden(QOC_C_KEY) = True
        .ColKey(QOC_C_KEY) = QOC_NONE
        .ColHidden(QOC_P_KEY) = True
        .ColKey(QOC_P_KEY) = QOC_NONE
        
        Set .Cell(flexcpPicture, 1, 0, .Rows - 1, nCols - 1) = Nothing
        .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignLeftCenter
        
        For i = 0 To QOC_LAST_COLUMN
            nIdx = gdOpt.Idx(i)
            If nIdx <> QOC_NONE Then
                .TextMatrix(0, i + 2) = gdOpt.Col(nIdx).Caption
                .ColFormat(i + 2) = gdOpt.Col(nIdx).Format
                .ColDataType(i + 2) = gdOpt.Col(nIdx).DataType
                .ColKey(i + 2) = nIdx
                .ColWidth(i + 2) = IIf(gdOpt.Col(nIdx).Width > 0, gdOpt.Col(nIdx).Width, -1)
                
                If nIdx = QOC_IS_SYNTH Then .Cell(flexcpPictureAlignment, 1, 0, .Rows - 1, nCols - 1) = flexPicAlignCenterCenter
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geOpt.AdjustSortImages
        
        m_GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    End With
End Sub

Public Sub FormatUndColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    Dim aRowData As MmQvRowData
    
    With fgUnd
        m_GridLock(GT_QUOTES_UNDERLYING).LockRedraw
    
        aFont.Name = gdUnd.Font.Name
        aFont.Size = gdUnd.Font.Size
        aFont.Bold = gdUnd.Font.Bold
        aFont.Italic = gdUnd.Font.Italic
        aFont.Strikethrough = gdUnd.Font.Strikethrough
        aFont.Underline = gdUnd.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> QUC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdUnd.Col(nIdx).BackColor <> 0, gdUnd.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdUnd.Col(nIdx).ForeColor <> 0, gdUnd.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next
    
        If gdUnd.HorizLinesVisible Then
            If gdUnd.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
            
        ElseIf gdUnd.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        
        .GridColor = gdUnd.GridLinesColor

        For i = 1 To nRows - 1
            Set aRowData = .RowData(i)
            If Not aRowData Is Nothing Then
                Debug.Assert Not aRowData.Exch Is Nothing
                If aRowData.Exch.ID <> aRowData.Und.PosExchID Then .Cell(flexcpBackColor, i, QUC_SYMBOL) = .GridColor
            End If
        Next
            
        .BackColorSel = gdUnd.BackColorSel
        .ForeColorSel = gdUnd.ForeColorSel
        
        m_GridLock(GT_QUOTES_UNDERLYING).UnlockRedraw
    End With
End Sub

Public Sub AdjustDivsAndRatesVisible()
    On Error Resume Next
    Dim nCols&, i&, bValidDivs As Boolean
    
    
    With fgDiv
        m_GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
        
        If DividendsVisible And (Grp.ID = 0 Or Grp.ID <> 0 And (Grp.ContractType = enCtStock Or Grp.ContractType = enCtIndex)) _
            Or RatesVisible Or CalcModelVisible Then
            
            If Grp.Und.ID <> 0 Then
                bValidDivs = Grp.Und.Dividend.IsValidDivs(Grp.Und.Dividend.DivType)   '  IsValidDivFreq(IIf(Grp.Und.UseCustDivs, Grp.Und.DivFreqCust, Grp.Und.DivFreq))
            Else
                bValidDivs = False
            End If
            
            nCols = .Cols - 1
            For i = 0 To nCols
                If i = QDC_OPTIONS Then
                    .ColHidden(i) = Not IndexOptionsComboVisible
                    
                ElseIf i < QDC_UNDPROFILE Then
                    .ColHidden(i) = Not CalcModelVisible
                    
                ElseIf i < QDC_DIV Then
                    .ColHidden(i) = False
                                    
                ElseIf i < QDC_RATE Then
                    If (i >= QDC_DATECALC And i <= QDC_VOLA_FLAT) Then
                        If (i = QDC_VOLA_FLAT) Then
                            .ColHidden(i) = Not IsVolaSimulated
                        Else
                            .ColHidden(i) = False
                        End If
                    Else
                        .ColHidden(i) = Not (DividendsVisible _
                                And (Grp.ID = 0 Or Grp.ID <> 0 And (Grp.ContractType = enCtStock Or Grp.ContractType = enCtIndex)) _
                                And (i <= QDC_FREQ Or i > QDC_FREQ And bValidDivs))
                    End If
                Else
                    .ColHidden(i) = Not RatesVisible
                End If
            Next
            .Visible = True
        Else
            .Visible = False
        End If
    
        m_GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub AdjustVolaVisible()
    On Error Resume Next
    
    With fgVol
        m_GridLock(GT_QUOTES_VOLA).LockRedraw
        
        .Visible = VolaVisible
    
        m_GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Sub AdjustFuturesVisible()
    On Error Resume Next
    
    With fgFut
        m_GridLock(GT_QUOTES_FUTURES).LockRedraw
        
        .Visible = FuturesVisible
    
        m_GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With
End Sub

Public Sub FormatDivColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    
    With fgDiv
        m_GridLock(GT_QUOTES_DIVIDENDS).LockRedraw
        
        aFont.Name = gdDiv.Font.Name
        aFont.Size = gdDiv.Font.Size
        aFont.Bold = gdDiv.Font.Bold
        aFont.Italic = gdDiv.Font.Italic
        aFont.Strikethrough = gdDiv.Font.Strikethrough
        aFont.Underline = gdDiv.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            If i < QDC_RATE_VAL Then
                nIdx = i
            Else
                nIdx = QDC_RATE_VAL
            End If
            
            .Cell(flexcpBackColor, 1, i) = IIf(gdDiv.Col(nIdx).BackColor <> 0, gdDiv.Col(nIdx).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i) = IIf(gdDiv.Col(nIdx).ForeColor <> 0, gdDiv.Col(nIdx).ForeColor, RGB(1, 1, 1))
        Next
    
        If gdDiv.HorizLinesVisible Then
            If gdDiv.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
            
        ElseIf gdDiv.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        
        .GridColor = gdDiv.GridLinesColor
        
        .BackColorSel = gdDiv.BackColorSel
        .ForeColorSel = gdDiv.ForeColorSel
        
        .AutoSize 0, .Cols - 1
    
        m_GridLock(GT_QUOTES_DIVIDENDS).UnlockRedraw
    End With
End Sub

Public Sub FormatVolColumns()
    On Error Resume Next
    Dim i&, j&, nCols&, nRows&, nIdx&, aFont As New StdFont
    
    With fgVol
        m_GridLock(GT_QUOTES_VOLA).LockRedraw
        
        aFont.Name = gdVol.Font.Name
        aFont.Size = gdVol.Font.Size
        aFont.Bold = gdVol.Font.Bold
        aFont.Italic = gdVol.Font.Italic
        aFont.Strikethrough = gdVol.Font.Strikethrough
        aFont.Underline = gdVol.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For j = 2 To 7
            For i = QVC_VOLA_VAL To nCols
                .Cell(flexcpBackColor, j, i) = IIf(gdVol.Col(QVC_VOLA_VAL).BackColor <> 0, gdVol.Col(QVC_VOLA_VAL).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, j, i) = IIf(gdVol.Col(QVC_VOLA_VAL).ForeColor <> 0, gdVol.Col(QVC_VOLA_VAL).ForeColor, RGB(1, 1, 1))
            Next
        Next
           
        If gdVol.HorizLinesVisible Then
            If gdVol.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
            
        ElseIf gdVol.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        
        .GridColor = gdVol.GridLinesColor
        
        .BackColorSel = gdVol.BackColorSel
        .ForeColorSel = gdVol.ForeColorSel
        
        .AutoSize 0, .Cols - 1
    
        m_GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

Public Sub FormatFutColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    
    With fgFut
        m_GridLock(GT_QUOTES_FUTURES).LockRedraw
    
        aFont.Name = gdFut.Font.Name
        aFont.Size = gdFut.Font.Size
        aFont.Bold = gdFut.Font.Bold
        aFont.Italic = gdFut.Font.Italic
        aFont.Strikethrough = gdFut.Font.Strikethrough
        aFont.Underline = gdFut.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> QOF_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdFut.Col(nIdx).BackColor <> 0, gdFut.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdFut.Col(nIdx).ForeColor <> 0, gdFut.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next
    
        If gdFut.HorizLinesVisible Then
            If gdFut.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
            
        ElseIf gdFut.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        
        .GridColor = gdFut.GridLinesColor
        
        .BackColorSel = gdFut.BackColorSel
        .ForeColorSel = gdFut.ForeColorSel
        
        m_GridLock(GT_QUOTES_FUTURES).UnlockRedraw
    End With
End Sub

Public Sub FormatOptColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    
    With fgOpt
        m_GridLock(GT_QUOTES_OPTIONS).LockRedraw
    
        aFont.Name = gdOpt.Font.Name
        aFont.Size = gdOpt.Font.Size
        aFont.Bold = gdOpt.Font.Bold
        aFont.Italic = gdOpt.Font.Italic
        aFont.Strikethrough = gdOpt.Font.Strikethrough
        aFont.Underline = gdOpt.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> QOC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(gdOpt.Col(nIdx).BackColor <> 0, gdOpt.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(gdOpt.Col(nIdx).ForeColor <> 0, gdOpt.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next
    
        If gdOpt.HorizLinesVisible Then
            If gdOpt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
            
        ElseIf gdOpt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        
        .GridColor = gdOpt.GridLinesColor
        
        .BackColorSel = gdOpt.BackColorSel
        .ForeColorSel = gdOpt.ForeColorSel
        
        m_GridLock(GT_QUOTES_OPTIONS).UnlockRedraw
    End With
End Sub

Public Function IsValidDivFreq(ByVal nDivFreq As Long) As Boolean
    On Error Resume Next
    IsValidDivFreq = (nDivFreq = 1 Or nDivFreq = 2 Or nDivFreq = 4 Or nDivFreq = 12)
End Function

'Public Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean
'
'    bMatched = False
'
'    If Not aTrd Is Nothing Then
'        bMatched = (TradesFilter = 0)
'
'        If Not bMatched Then
'            Select Case TradesFilter
'                Case 1 ' Today's
'                    bMatched = (Int(aTrd.TradeDate) = Int(Date) And aTrd.Status <> enTsSimulated)
'
'                Case 2 ' OpenPos
'                    bMatched = (Int(aTrd.TradeDate) < Int(Date) And aTrd.IsPosition <> 0)
'
'                Case 3 ' NetPos
'                    bMatched = (aTrd.Status <> enTsSimulated)
'
'                Case 4 ' Simulated
'                    bMatched = (aTrd.Status = enTsSimulated)
'
'                Case 5 ' Today's & Sim
'                    bMatched = (Int(aTrd.TradeDate) = Int(Date)) 'Or aTrd.Status = enTsSimulated)
'
'                Case 6 ' OpenPos & Sim
'                    bMatched = (Int(aTrd.TradeDate) < Int(Date)) 'And aTrd.IsPosition <> 0 Or aTrd.Status = enTsSimulated)
'
'                Case 7 ' Manual
'                    bMatched = (aTrd.Status = enTsManual)
'
'                Case 8 ' Manual & Sim
'                    bMatched = (aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated)
'
'            End Select
'        End If
'    End If
'
'    CheckTradeFilter = bMatched
'End Function

'Private Sub AppendUndDataSnapshot(ByRef aUnd As EtsMmQuotesLib.MmQvUndAtom, ByRef sDesc$)
'    On Error Resume Next
'    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom
'
'    For Each aQuote In aUnd.Quote
'        sDesc = sDesc & aUnd.Symbol & vbTab
'        sDesc = sDesc & aQuote.Exch.Code & vbTab
'        sDesc = sDesc & IIf(aQuote.ReplacePriceStatus And enRpsBid, "!", "") & _
'                        IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.ReplacePriceStatus And enRpsAsk, "!", "") & _
'                        IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.PriceHigh > BAD_DOUBLE_VALUE, aQuote.PriceHigh, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.PriceLow > BAD_DOUBLE_VALUE, aQuote.PriceLow, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.PriceOpen > BAD_DOUBLE_VALUE, aQuote.PriceOpen, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.PriceClose > BAD_DOUBLE_VALUE, aQuote.PriceClose, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA) & vbTab
'        If aQuote.Exch.ID = 0 And aUnd.ID = Grp.Und.ID Then
'            sDesc = sDesc & IIf(aUnd.QtyInShares > BAD_LONG_VALUE, aUnd.QtyInShares, STR_NA) & vbTab
'        Else
'            sDesc = sDesc & STR_NA & vbTab
'        End If
'        sDesc = sDesc & IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA) & vbTab
'        sDesc = sDesc & IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA) & vbTab
'
'        If aQuote.Exch.ID = 0 Then
'            sDesc = sDesc & IIf(aUnd.NetDelta > BAD_DOUBLE_VALUE, aUnd.NetDelta, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aUnd.TotalDelta > BAD_DOUBLE_VALUE, aUnd.TotalDelta, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aUnd.TotalGamma > BAD_DOUBLE_VALUE, aUnd.TotalGamma, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aUnd.TotalVega > BAD_DOUBLE_VALUE, aUnd.TotalVega, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aUnd.TotalTheta > BAD_DOUBLE_VALUE, aUnd.TotalTheta, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aUnd.TotalRho > BAD_DOUBLE_VALUE, aUnd.TotalRho, STR_NA) & vbTab
'        Else
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'        End If
'
'        sDesc = sDesc & IIf(aQuote.Exch.ID = 0, aUnd.SymbolName, "") & vbTab
'        sDesc = sDesc & IIf(aQuote.Exch.ID = 0, aQuote.LotSize, "") & vbTab
'        sDesc = sDesc & vbTab & vbTab
'        sDesc = sDesc & vbCrLf
'    Next
'
'    If aUnd.ID = Grp.Und.ID And Not Grp.IsStockOrIndex Then
'        For Each aQuote In Grp.Fut.Quote
'            sDesc = sDesc & Grp.Fut.Symbol & vbTab
'            sDesc = sDesc & aQuote.Exch.Code & vbTab
'            sDesc = sDesc & IIf(aQuote.ReplacePriceStatus And enRpsBid, "!", "") & _
'                            IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.ReplacePriceStatus And enRpsAsk, "!", "") & _
'                            IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.PriceHigh > BAD_DOUBLE_VALUE, aQuote.PriceHigh, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.PriceLow > BAD_DOUBLE_VALUE, aQuote.PriceLow, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.PriceOpen > BAD_DOUBLE_VALUE, aQuote.PriceOpen, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.PriceClose > BAD_DOUBLE_VALUE, aQuote.PriceClose, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA) & vbTab
'            If aQuote.Exch.ID = 0 And Grp.Fut.ID = Grp.Und.ID Then
'                sDesc = sDesc & IIf(Grp.Fut.QtyInShares > BAD_LONG_VALUE, Grp.Fut.QtyInShares, STR_NA) & vbTab
'            Else
'                sDesc = sDesc & STR_NA & vbTab
'            End If
'            sDesc = sDesc & IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA) & vbTab
'            sDesc = sDesc & IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA) & vbTab
'
'            If aQuote.Exch.ID = 0 Then
'                sDesc = sDesc & IIf(Grp.Fut.NetDelta > BAD_DOUBLE_VALUE, Grp.Fut.NetDelta, STR_NA) & vbTab
'                sDesc = sDesc & IIf(Grp.Fut.TotalDelta > BAD_DOUBLE_VALUE, Grp.Fut.TotalDelta, STR_NA) & vbTab
'                sDesc = sDesc & IIf(Grp.Fut.TotalGamma > BAD_DOUBLE_VALUE, Grp.Fut.TotalGamma, STR_NA) & vbTab
'                sDesc = sDesc & IIf(Grp.Fut.TotalVega > BAD_DOUBLE_VALUE, Grp.Fut.TotalVega, STR_NA) & vbTab
'                sDesc = sDesc & IIf(Grp.Fut.TotalTheta > BAD_DOUBLE_VALUE, Grp.Fut.TotalTheta, STR_NA) & vbTab
'                sDesc = sDesc & IIf(Grp.Fut.TotalRho > BAD_DOUBLE_VALUE, Grp.Fut.TotalRho, STR_NA) & vbTab
'            Else
'                sDesc = sDesc & STR_NA & vbTab
'                sDesc = sDesc & STR_NA & vbTab
'                sDesc = sDesc & STR_NA & vbTab
'                sDesc = sDesc & STR_NA & vbTab
'                sDesc = sDesc & STR_NA & vbTab
'                sDesc = sDesc & STR_NA & vbTab
'            End If
'
'            sDesc = sDesc & IIf(aQuote.Exch.ID = 0, Grp.Fut.ContractName, "") & vbTab
'            sDesc = sDesc & IIf(aQuote.Exch.ID = 0, aQuote.LotSize, "") & vbTab
'            sDesc = sDesc & vbTab & vbTab
'            sDesc = sDesc & vbCrLf
'        Next
'    End If
'End Sub
'
'Private Sub AppendOptionData(ByRef aOpt As EtsMmQuotesLib.MmQvOptAtom, ByRef aQuote As EtsMmQuotesLib.MmQvQuoteAtom, ByRef sDesc$)
'    On Error Resume Next
'    sDesc = sDesc & aOpt.Symbol & vbTab
'    sDesc = sDesc & aQuote.Exch.Code & vbTab
'    sDesc = sDesc & aQuote.Series & vbTab
'    sDesc = sDesc & aOpt.Expiry & vbTab
'    sDesc = sDesc & aOpt.Strike & vbTab
'    sDesc = sDesc & IIf(aQuote.PriceBid > BAD_DOUBLE_VALUE, aQuote.PriceBid, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.PriceAsk > BAD_DOUBLE_VALUE, aQuote.PriceAsk, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.PriceLast > BAD_DOUBLE_VALUE, aQuote.PriceLast, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Volume > BAD_LONG_VALUE, aQuote.Volume, STR_NA) & vbTab
'    If aQuote.Exch.ID = 0 Then
'        sDesc = sDesc & IIf(aOpt.Qty > BAD_LONG_VALUE, aOpt.Qty, STR_NA) & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    If aOpt.Vola > BAD_DOUBLE_VALUE Then
'        sDesc = sDesc & aOpt.Vola * 100# & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    sDesc = sDesc & IIf(aQuote.PriceTheo > BAD_DOUBLE_VALUE, aQuote.PriceTheo, STR_NA) & vbTab
'    If aQuote.IV > BAD_DOUBLE_VALUE Then
'        sDesc = sDesc & aQuote.IV * 100# & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    If aQuote.IVBid > BAD_DOUBLE_VALUE Then
'        sDesc = sDesc & aQuote.IVBid * 100# & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    If aQuote.IVAsk > BAD_DOUBLE_VALUE Then
'        sDesc = sDesc & aQuote.IVAsk * 100# & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    sDesc = sDesc & IIf(aQuote.OpenInterest > BAD_LONG_VALUE, aQuote.OpenInterest, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Delta > BAD_DOUBLE_VALUE, aQuote.Delta, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Gamma > BAD_DOUBLE_VALUE, aQuote.Gamma, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Vega > BAD_DOUBLE_VALUE, aQuote.Vega, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Theta > BAD_DOUBLE_VALUE, aQuote.Theta, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.Rho > BAD_DOUBLE_VALUE, aQuote.Rho, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.VegaDelta > BAD_DOUBLE_VALUE, aQuote.VegaDelta, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.VegaGamma > BAD_DOUBLE_VALUE, aQuote.VegaGamma, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.ThetaDelta > BAD_DOUBLE_VALUE, aQuote.ThetaDelta, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.ThetaGamma > BAD_DOUBLE_VALUE, aQuote.ThetaGamma, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.GammaPerTheta > BAD_DOUBLE_VALUE, aQuote.GammaPerTheta, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.NetChange > BAD_DOUBLE_VALUE, aQuote.NetChange, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.SizeBid > BAD_LONG_VALUE, aQuote.SizeBid, STR_NA) & vbTab
'    sDesc = sDesc & IIf(aQuote.SizeAsk > BAD_LONG_VALUE, aQuote.SizeAsk, STR_NA) & vbTab
'    If aQuote.IVSpread > BAD_DOUBLE_VALUE Then
'        sDesc = sDesc & aQuote.IVSpread * 100# & vbTab
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'    sDesc = sDesc & IIf(aQuote.UpdateTime > 0, aQuote.UpdateTime, STR_NA) & vbTab
'End Sub
'
'Private Sub AppendOptionsData(ByRef aLog As clsLogMgr)
'    On Error Resume Next
'    Dim aExp As EtsMmQuotesLib.MmQvExpAtom, aStr As EtsMmQuotesLib.MmQvStrikeAtom
'    Dim aQuote As EtsMmQuotesLib.MmQvQuoteAtom, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
'    Dim aPair As EtsMmQuotesLib.MmQvOptPairAtom, aExch As EtsGeneralLib.ExchAtom, sTemp$
'    Dim aSyntComp As EtsGeneralLib.SynthRootCompAtom, sDesc$
'    Dim aOpt As EtsMmQuotesLib.MmQvOptAtom
'
'    If Grp.IsStockOrIndex Then
'        For Each aExp In Grp.Und.Exp
'            For Each aStr In aExp.Strike
'                For Each aPair In aStr.OptPair
'                    Set aRoot = Grp.Und.OptRoot(aPair.RootID)
'
'                    For Each aExch In Grp.Und.OptExch
'                        sDesc = ""
'                        Set aOpt = aPair.Opt(enOtCall)
'                        Set aQuote = aOpt.Quote(aExch.ID)
'                        If aOpt.ID <> 0 And Not aQuote Is Nothing Then
'                            AppendOptionData aOpt, aQuote, sDesc
'                        Else
'                            sDesc = sDesc & String$(QOC_C_UPDATE_TIME - QOC_C_SYMBOL + 1, vbTab)
'                        End If
'                        Set aOpt = Nothing
'                        Set aQuote = Nothing
'
'                        Set aOpt = aPair.Opt(enOtPut)
'                        Set aQuote = aOpt.Quote(aExch.ID)
'                        If aOpt.ID <> 0 And Not aQuote Is Nothing Then
'                            AppendOptionData aOpt, aQuote, sDesc
'                        Else
'                            sDesc = sDesc & String$(QOC_P_UPDATE_TIME - QOC_P_SYMBOL + 1, vbTab)
'                        End If
'                        Set aOpt = Nothing
'                        Set aQuote = Nothing
'
'                        sDesc = sDesc & aRoot.Name & vbTab
'                        sDesc = sDesc & IIf(aRoot.Synthetic, "Yes", "No") & vbTab
'                        sDesc = sDesc & aRoot.DPC & vbCrLf
'
'                        aLog.LogInfo sDesc
'                    Next
'                    Set aRoot = Nothing
'                Next
'            Next
'        Next
'    Else
'        For Each aExp In Grp.Fut.Exp
'            For Each aStr In aExp.Strike
'                For Each aPair In aStr.OptPair
'
'                    For Each aExch In Grp.Fut.OptExch
'                        sDesc = ""
'                        Set aOpt = aPair.Opt(enOtCall)
'                        Set aQuote = aOpt.Quote(aExch.ID)
'                        If aOpt.ID <> 0 And Not aQuote Is Nothing Then
'                            AppendOptionData aOpt, aQuote, sDesc
'                        Else
'                            sDesc = sDesc & String$(QOC_C_UPDATE_TIME - QOC_C_SYMBOL + 1, vbTab)
'                        End If
'                        Set aOpt = Nothing
'                        Set aQuote = Nothing
'
'                        Set aOpt = aPair.Opt(enOtPut)
'                        Set aQuote = aOpt.Quote(aExch.ID)
'                        If aOpt.ID <> 0 And Not aQuote Is Nothing Then
'                            AppendOptionData aOpt, aQuote, sDesc
'                        Else
'                            sDesc = sDesc & String$(QOC_P_UPDATE_TIME - QOC_P_SYMBOL + 1, vbTab)
'                        End If
'                        Set aOpt = Nothing
'                        Set aQuote = Nothing
'
'                        sDesc = sDesc & Grp.Fut.Symbol & vbTab
'                        sDesc = sDesc & "No" & vbTab
'
'                        sDesc = sDesc & Trim$(Str$(Grp.FutRoot.FutLotSize * Grp.FutRoot.OptLotSize)) & " " & Grp.Und.Symbol & vbCrLf
'
'                        aLog.LogInfo sDesc
'                    Next
'                    Set aRoot = Nothing
'                Next
'            Next
'        Next
'    End If
'
'End Sub
'
'Private Sub AppendUndParams(ByRef aLog As clsLogMgr)
'    On Error Resume Next
'    Dim sDesc$, nIdx&, aExp As EtsMmQuotesLib.MmQvExpAtom
'    sDesc = "Model/Divs/Rates Data" & vbCrLf
'
'    For nIdx = QDC_TRADES To QDC_RATE
'        sDesc = sDesc & gdDiv.Col(nIdx).Caption & vbTab
'    Next
'
'    For Each aExp In Grp.Und.Exp
'        sDesc = sDesc & Format$(aExp.ExpiryMonth, "MMMYY") & vbTab
'    Next
'    sDesc = sDesc & vbCrLf
'
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_TRADES) & vbTab
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_MODEL) & vbTab
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_STYLE) & vbTab
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_CALENDAR) & vbTab
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_UNDPROFILE) & vbTab
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_OPTPROFILE) & vbTab
'
'    If Grp.IsStockOrIndex Then
'        If Grp.Und.UndType = enCtStock Then
'            sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_DIV) & vbTab
'            If Grp.Und.UseCustDivs Then
'                sDesc = sDesc & Grp.Und.DivFreqCust & vbTab
'                sDesc = sDesc & Grp.Und.DivDateCust & vbTab
'                sDesc = sDesc & Grp.Und.DivAmtCust & vbTab
'            Else
'                sDesc = sDesc & Grp.Und.DivFreq & vbTab
'                sDesc = sDesc & Grp.Und.DivDate & vbTab
'                sDesc = sDesc & Grp.Und.DivAmt & vbTab
'            End If
'        Else
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & STR_NA & vbTab
'            sDesc = sDesc & Grp.Und.Yield & vbTab
'        End If
'    Else
'        sDesc = sDesc & STR_NA & vbTab
'        sDesc = sDesc & STR_NA & vbTab
'        sDesc = sDesc & STR_NA & vbTab
'        sDesc = sDesc & STR_NA & vbTab
'    End If
'
'    sDesc = sDesc & fgDiv.Cell(flexcpTextDisplay, 1, QDC_RATE) & vbTab
'
'    Dim aExpColl As EtsMmQuotesLib.MmQvExpColl
'    If Grp.IsStockOrIndex Then
'        Set aExpColl = Grp.Und.Exp
'    Else
'        Set aExpColl = Grp.Und.Exp
'    End If
'
'    For Each aExp In aExpColl
'        If Grp.UseCustRates Then
'            sDesc = sDesc & aExp.RateCust & vbTab
'        Else
'            sDesc = sDesc & aExp.Rate & vbTab
'        End If
'    Next
'    sDesc = sDesc & vbCrLf
'
'    aLog.LogInfo sDesc
'
'    sDesc = "ATM Volas" & vbCrLf & "Expiry" & vbTab
'    For Each aExp In aExpColl
'        sDesc = sDesc & Format$(aExp.ExpiryMonth, "MMMYY") & vbTab
'    Next
'    sDesc = sDesc & vbCrLf & "Strike" & vbTab
'    For Each aExp In aExpColl
'        sDesc = sDesc & aExp.AtmStrike.Strike & vbTab
'    Next
'    sDesc = sDesc & vbCrLf & "ATM Vola" & vbTab
'    For Each aExp In aExpColl
'        sDesc = sDesc & aExp.AtmVola & vbTab
'    Next
'    sDesc = sDesc & vbCrLf
'
'    Set aExpColl = Nothing
'    aLog.LogInfo sDesc
'End Sub
'
'Public Sub MakeDataSnapshot(ByVal sFileName$, ByRef frmOwner As Form)
'    On Error GoTo EH
'    Dim aLog As New clsLogMgr, sDesc$, bUndOpened As Boolean
'    Dim aUnd As EtsMmQuotesLib.MmQvUndAtom, nIdx&
'
'    Screen.MousePointer = vbHourglass
'
'    aLog.Init sFileName, "Quotes View Data Snapshot"
'    g_Params.MakeAllParamsDescription sDesc
'    aLog.LogInfo sDesc
'
'    sDesc = frmOwner.GetCaption() & vbCrLf & String$(70, "-") & vbCrLf
'    aLog.LogInfo sDesc
'
'    If Not Grp Is Nothing Then
'        bUndOpened = (Grp.ID <> 0)
'    End If
'
'    If bUndOpened Then
'        sDesc = "Underlying Data" & vbCrLf
'
'        For nIdx = QUC_SYMBOL To QUC_LAST_COLUMN
'            sDesc = sDesc & gdUnd.Col(nIdx).Caption & vbTab
'        Next
'        sDesc = sDesc & vbCrLf
'
'        AppendUndDataSnapshot Grp.Und, sDesc
'
'        aLog.LogInfo sDesc
'
'        If Grp.IsStockOrIndex Then
'            If Not Grp.Und.SynthUnd Is Nothing Then
'                sDesc = ""
'                For Each aUnd In Grp.Und.SynthUnd
'                    AppendUndDataSnapshot aUnd, sDesc
'                Next
'                aLog.LogInfo sDesc
'            End If
'        End If
'
'        AppendUndParams aLog
'
'        sDesc = "Options Data" & vbCrLf
'
'        For nIdx = QOC_C_SYMBOL To QOC_LAST_COLUMN
'            sDesc = sDesc & gdOpt.Col(nIdx).Caption & vbTab
'        Next
'        sDesc = sDesc & vbCrLf
'        aLog.LogInfo sDesc
'
'        AppendOptionsData aLog
'    Else
'        sDesc = "Symbol: <No Symbol Selected>" & vbCrLf
'        aLog.LogInfo sDesc
'    End If
'
'    Screen.MousePointer = vbDefault
'    gCmn.MyMsgBox frmOwner, "Data snapshot was saved to a file" & vbCrLf & _
'                    App.Path & "\Logs\" & sFileName, vbInformation
'    Exit Sub
'EH:
'    Dim sErrDesc$
'    sErrDesc = Err.Description
'    On Error Resume Next
'    Screen.MousePointer = vbDefault
'    gCmn.MyMsgBox frmOwner, "Fail to save data snapshot to a file" & vbCrLf & _
'                    App.Path & "\Logs\" & sFileName & vbCrLf & _
'                    sErrDesc, vbCritical
'End Sub
'


