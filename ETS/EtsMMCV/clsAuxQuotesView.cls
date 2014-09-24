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
Public gdOpt As clsGridDef

Public geOpt As clsGridEx

Public fgUnd As VSFlex7Ctl.VSFlexGrid
Public fgDiv As VSFlex7Ctl.VSFlexGrid
Public fgVol As VSFlex7Ctl.VSFlexGrid
Public fgOpt As VSFlex7Ctl.VSFlexGrid

Private m_GridLock(GT_QUOTES_UNDERLYING To GT_QUOTES_OPTIONS) As New clsGridLock

Public RealTime As Boolean
Public RealTimeConnected As Boolean

Public SortColOrder As VSFlex7Ctl.SortSettings
Public SortColKey As Long

Public CalcModelVisible As Boolean
Public ExpCalendarVisible As Boolean
Public DividendsVisible As Boolean
Public VolaVisible As Boolean
Public RatesVisible As Boolean

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
        .ScrollBars = flexScrollBarNone
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
        .ColWidthMin = 150
    End With
    
    With fgVol
        .Rows = 2
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
        .SelectionMode = flexSelectionListBox
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightAlways
        .ColWidthMin = 150
    End With
End Sub

Public Sub FormatUndGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
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
        
        For i = 0 To QUC_LAST_COLUMN
            nIdx = gdUnd.Idx(i)
            If nIdx <> QUC_NONE Then
                .TextMatrix(0, i + 1) = gdUnd.Col(nIdx).Caption
                .ColFormat(i + 1) = gdUnd.Col(nIdx).Format
                .ColDataType(i + 1) = gdUnd.Col(nIdx).DataType
                .ColKey(i + 1) = nIdx
                .ColWidth(i + 1) = IIf(gdUnd.Col(nIdx).Width > 0, gdUnd.Col(nIdx).Width, -1)
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
        
        For i = QDC_TRADES To QDC_RATE_VAL
            .TextMatrix(0, i) = gdDiv.Col(i).Caption
            .ColFormat(i) = gdDiv.Col(i).Format
            .ColDataType(i) = gdDiv.Col(i).DataType
        Next
        
        .ColComboList(QDC_TRADES) = "#0;<All>|#1;Today's|#2;OpenPos|#3;NetPos|#7;Manual|#4;Simulated|#5;Today's & Sim|#6;OpenPos & Sim|#8;Manual & Sim"
        .TextMatrix(1, QDC_TRADES) = CStr(QV.TradeFilter.Data(enFtTrades))
        
        .TextMatrix(1, QDC_MODEL) = g_Params.CalcModelName(g_Params.CalcModel)
        .ColComboList(QDC_STYLE) = "#0;European|#1;American"
        .TextMatrix(1, QDC_STYLE) = "1"
        
        .ColEditMask(QDC_DATE) = "99/99/9999"
        
        
        sComboList = "#0;<All>"
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
        Next
        If Len(sComboList) > 0 Then
            .ColComboList(QDC_FLT_TRADER) = sComboList
            .TextMatrix(1, QDC_FLT_TRADER) = IIf(g_CurTraderID <> 0, CStr(g_CurTraderID), CStr(QV.TradeFilter.Data(enFtTrader)))
        End If
        
        sComboList = "#0;<All>"
        For Each aStrategy In g_Strategy
            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
        Next
        If Len(sComboList) > 0 Then
            .ColComboList(QDC_FLT_STRATEGY) = sComboList
            .TextMatrix(1, QDC_FLT_STRATEGY) = CStr(QV.TradeFilter.Data(enFtStrategy))
        End If
        
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
    
        '.Cols = QVC_LAST_COLUMN - 1
        .Cols = 1
        
        .TextMatrix(0, QVC_VOLA) = gdVol.Col(QVC_VOLA).Caption
        .TextMatrix(1, QVC_VOLA) = "ATM Vola"
        .ColFormat(QVC_VOLA) = gdVol.Col(QVC_VOLA).Format
        .ColDataType(QVC_VOLA) = gdVol.Col(QVC_VOLA).DataType
       
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        .ColAlignment(0) = flexAlignCenterCenter
        .RowHeight(1) = 250

        m_GridLock(GT_QUOTES_VOLA).UnlockRedraw
    End With
End Sub

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

        For i = 1 To nRows
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
            Or RatesVisible Or CalcModelVisible Or ExpCalendarVisible Then
            
            If Grp.Und.ID <> 0 Then
                bValidDivs = Grp.Und.Dividend.IsValidDivs(Grp.Und.Dividend.DivType)
            Else
                bValidDivs = False
            End If
            
            nCols = .Cols - 1
            For i = 0 To nCols
                If i < QDC_CALENDAR Then
                    .ColHidden(i) = Not CalcModelVisible
                    
                ElseIf i < QDC_UNDPROFILE Then
                    .ColHidden(i) = Not ExpCalendarVisible
                    
                ElseIf i <= QDC_DIV Then
                    .ColHidden(i) = False
                    
                ElseIf i < QDC_RATE Then
                    .ColHidden(i) = Not (DividendsVisible _
                            And (Grp.ID = 0 Or Grp.ID <> 0 And Grp.ContractType = enCtStock) _
                            And (i <= QDC_FREQ Or i > QDC_FREQ And bValidDivs))
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
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    
    With fgVol
        m_GridLock(GT_QUOTES_VOLA).LockRedraw
        
        aFont.Name = gdVol.Font.Name
        aFont.Size = gdVol.Font.Size
        aFont.Bold = gdVol.Font.Bold
        aFont.Italic = gdVol.Font.Italic
        aFont.Strikethrough = gdVol.Font.Strikethrough
        aFont.Underline = gdVol.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 4
        nRows = .Rows - 1
        For i = QVC_VOLA_VAL To nCols
            .Cell(flexcpBackColor, 1, i) = IIf(gdVol.Col(QVC_VOLA_VAL).BackColor <> 0, gdVol.Col(QVC_VOLA_VAL).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i) = IIf(gdVol.Col(QVC_VOLA_VAL).ForeColor <> 0, gdVol.Col(QVC_VOLA_VAL).ForeColor, RGB(1, 1, 1))
        Next
        If .Cols > QVC_VOLA_VAL Then
            .Cell(flexcpBackColor, 1, .Cols - 1) = IIf(gdVol.Col(QVC_VOLA_DATECALC).BackColor <> 0, gdVol.Col(QVC_VOLA_DATECALC).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, .Cols - 1) = IIf(gdVol.Col(QVC_VOLA_DATECALC).ForeColor <> 0, gdVol.Col(QVC_VOLA_DATECALC).ForeColor, RGB(1, 1, 1))
        End If
    
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
