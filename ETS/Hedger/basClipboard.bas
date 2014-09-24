Attribute VB_Name = "basClipboard"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Option Compare Text

Public Enum CopyTask
    COPY_NOTHING
    COPY_PORTFOLIO
    COPY_PORTFOLIO_SUMMARY
    COPY_SHORT_SUBPORTFOLIO
    COPY_LONG_SUBPORTFOLIO
    COPY_STOCKS
End Enum

Private fVegaMode As Boolean
Private m_ShowHiddenRows As Boolean
Private Buffer As String
Private CellSeparator As String
Private RowSeparator As String

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ExecuteCopy(ByVal CopyWhat As CopyTask)
    m_ShowHiddenRows = True
    CellSeparator = vbTab
    RowSeparator = vbCrLf
    On Error GoTo Herr
    If gPortfolioHedge Is Nothing Then
        Exit Sub
    End If
    fVegaMode = gPortfolioHedge.VegaMode
    Select Case CopyWhat
    Case COPY_PORTFOLIO
        ExecutePrintPortfolio
    Case COPY_PORTFOLIO_SUMMARY
        ExecutePrintPortfolioSummary
    Case COPY_SHORT_SUBPORTFOLIO
        ExecutePrintSubPortfolio "SHORT", 0
    Case COPY_LONG_SUBPORTFOLIO
        ExecutePrintSubPortfolio "LONG", 1
    Case COPY_STOCKS
        ExecutePrintStocks
    End Select
    Exit Sub
Herr:
    ShowError "Copy to Clipboard failed."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintPortfolio()
    AddStartDoc
    If fVegaMode Then
        AddCaption "Porfolio data (Vega hedging)"
    Else
        AddCaption "Porfolio data (Delta hedging)"
    End If
    AddSubTitle "Portfolio: " & gPortfolioHedge.PortfolioName
    AddTitle "Portfolio Positions"
    AddGridReport frmMain.ctlPortfolio1.GetGrid
    AddEndDoc
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintStocks()
    On Error Resume Next
    AddStartDoc
    AddCaption "Stocks and Indexes data"
    AddSubTitle "Portfolio: " & gPortfolioHedge.PortfolioName
    AddTitle "Indexes data"
    AddGridReport frmMain.ctlStocks1.GetGrid(0)
    AddTitle "Stocks data"
    AddGridReport frmMain.ctlStocks1.GetGrid(1)
    AddEndDoc
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintSubPortfolio(ByVal HedgeSymbol As String, ByVal HedgeSymbolIndex As Integer)
Dim Caption As String
    If gPortfolioHedge.HedgeMode = "" Then
        HedgeSymbol = "OPT"
        HedgeSymbolIndex = 0
    End If
    Caption = gPortfolioHedge.GetSubportfolioCaption(HedgeSymbol)
    AddStartDoc
    If fVegaMode Then
        AddCaption "Subportfolio data (Vega hedging)"
    Else
        AddCaption "Subportfolio data (Delta hedging)"
    End If
    AddSubTitle "Portfolio: " & gPortfolioHedge.PortfolioName
    AddTotalsGrid1 HedgeSymbol
    AddTitle "Hedged Positions"
    AddGridReport frmMain.ctlSubportfolios21.GetGrid(HedgeSymbolIndex)
    AddEndDoc
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddTotalsGrid1(ByVal HedgeSymbol As String)
Dim sFormat As String
Dim sHeader As String
Dim sBody As String
Dim HedgeSymbol2 As String
Dim HedgeCaption As String

    If HedgeSymbol = "OPT" Then



        HedgeSymbol2 = "NONE"
        HedgeCaption = "Optimal Hedge"
    Else
        HedgeSymbol2 = "NOT_" & HedgeSymbol
        If HedgeSymbol = "SHORT" Then
            HedgeCaption = "Short hedge"
        Else
            HedgeCaption = "Long hedge"
        End If
    End If
    AddTitle HedgeCaption


    Dim Q As Double
    Dim D2 As Double
    Dim d3 As Double
    Dim Gain As Double
    Dim Q1 As Double
    Dim Q2 As Double
    Dim Q3 As Double
    Dim Q4 As Double
    Dim OriginalPositionsCount As Long
    Dim HedgedPositions As Long
    Dim NonHedgedPositions As Long
    Dim EquivalentPositions As Double
    Dim EquivalentHedgedPositions As Double
    Dim EquivalentNonHedgedPositions As Double

    With gPortfolioHedge
        .CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
        .CalculateSubportfolioTotals2 HedgeSymbol, HedgedPositions, EquivalentHedgedPositions
        .CalculateSubportfolioTotals2 HedgeSymbol2, NonHedgedPositions, EquivalentNonHedgedPositions
    End With
    sHeader = CellSeparator
    sBody = ""
    sBody = sBody & "Unhedged deviation, $" & CellSeparator & Format$(D2, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "Hedged deviation, $" & CellSeparator & Format$(d3, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "Gain, %" & CellSeparator & Format$(Gain * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    If fVegaMode Then
        sBody = sBody & "Original number of contracts"
    Else
        sBody = sBody & "Original number of stocks"
    End If
    sBody = sBody & CellSeparator & Format$(HedgedPositions + NonHedgedPositions, "#,##0;(#,##0)") & RowSeparator
    If fVegaMode Then
        sBody = sBody & "Hedged contracts"
    Else
        sBody = sBody & "Hedged stocks"
    End If
    sBody = sBody & CellSeparator & Format$(HedgedPositions, "#,##0;(#,##0)") & RowSeparator
    sBody = sBody & "Hedged position, $" & CellSeparator & Format$(EquivalentHedgedPositions, "#,##0.#0;(#,##0.#0)") & RowSeparator
    If fVegaMode Then
        sBody = sBody & "Index vega" & CellSeparator & Format$(-Q, "#,##0.#0;(#,##0.#0)") & RowSeparator

    Else
        sBody = sBody & "Index position, $" & CellSeparator & Format$(-Q, "#,##0.#0;(#,##0.#0)") & RowSeparator
    End If
    sBody = sBody & "Remaining position, $" & CellSeparator & Format$(EquivalentNonHedgedPositions, "#,##0.#0;(#,##0.#0)") & RowSeparator

    AddGrid 2, sFormat, sHeader, sBody
    AddQbyIndexGrid1 -Q1, -Q2, -Q3, -Q4

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddQbyIndexGrid1(ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double)
Dim sFormat As String
Dim sHeader As String
Dim sBody As String
    If fVegaMode Then
        sHeader = "Index Symbol" & CellSeparator & "Equivalent Index Vega" & CellSeparator & "Index Price" & CellSeparator & "Index HV, %" & CellSeparator & "Index IV, %"
    Else
        sHeader = "Index Symbol" & CellSeparator & "Equivalent Index Position, $" & CellSeparator & "Index Price" & CellSeparator & "Index HV, %" & CellSeparator & "Index IV, %"
    End If
    sBody = ""
    sBody = sBody & "SPX" & CellSeparator & _
        Format$(Q1, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("SPX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "NDX" & CellSeparator & _
        Format$(Q2, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("NDX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "OEX" & CellSeparator & _
        Format$(Q3, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("OEX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "DJX" & CellSeparator & _
        Format$(Q4, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("DJX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    AddTitle "Index Positions"
    AddGrid 5, sFormat, sHeader, sBody
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintPortfolioSummary()
Dim sFormat As String
Dim sHeader As String
Dim sBody0 As String
Dim sBody(1 To 12) As String
    On Error Resume Next
    AddStartDoc
    AddSubTitle ""
    If fVegaMode Then
        AddCaption "Hedge summary (Vega hedging)"
    Else
        AddCaption "Hedge summary (Delta hedging)"
    End If
    AddSubTitle "Portfolio: " & gPortfolioHedge.PortfolioName
    
    AddTitle "Calculated values"
    
    sBody(1) = "Unhedged deviation, $"
    sBody(2) = "Hedged deviation, $"
    sBody(3) = "Gain, %"
    If fVegaMode Then
        sBody(4) = "Original number of contracts"
    Else
        sBody(4) = "Original number of stocks"
    End If
    If fVegaMode Then
        sBody(5) = "Hedged contracts"
    Else
        sBody(5) = "Hedged stocks"
    End If
    sBody(6) = "Hedged position, $"
    If fVegaMode Then
        sBody(7) = "Index vega"
    Else
        sBody(7) = "Index position, $"
    End If
    sBody(8) = "Remaining position, $"
    
    If fVegaMode Then
        sBody(9) = "SPX equivalent vega"
        sBody(10) = "NDX equivalent vega"
        sBody(11) = "OEX equivalent vega"
        sBody(12) = "DJX equivalent vega"
    Else
        sBody(9) = "SPX position, $"
        sBody(10) = "NDX position, $"
        sBody(11) = "OEX position, $"
        sBody(12) = "DJX position, $"
    End If
    
    sHeader = ""
    If gPortfolioHedge.HedgeMode = "" Then
        AddTotalsGrid "ALL", sFormat, sHeader, sBody
        AddTotalsGrid "OPT", sFormat, sHeader, sBody
    Else
        AddTotalsGrid "ALL", sFormat, sHeader, sBody
        AddTotalsGrid "SHORT", sFormat, sHeader, sBody
        AddTotalsGrid "LONG", sFormat, sHeader, sBody
    End If
    
    sBody0 = ""
    Dim i As Long
    For i = 1 To 12
        sBody0 = sBody0 & sBody(i) & RowSeparator
    Next
    AddGrid 2, sFormat, sHeader, sBody0
    
    AddIndexGrid
    AddEndDoc
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddTotalsGrid(ByVal HedgeSymbol As String, sFormat As String, sHeader As String, sBody() As String)
Dim HedgeSymbol2 As String
Dim HedgeCaption As String
    
    If HedgeSymbol = "ALL" Then
        HedgeSymbol2 = "NONE"
        HedgeCaption = "Original"
    ElseIf HedgeSymbol = "OPT" Then
        HedgeSymbol2 = "NONE"
        HedgeCaption = "Hedge"
    Else
        HedgeSymbol2 = "NOT_" & HedgeSymbol
        If HedgeSymbol = "SHORT" Then
            HedgeCaption = "Short hedge"
        Else
            HedgeCaption = "Long hedge"
        End If
    End If
    
    sHeader = sHeader & CellSeparator & HedgeCaption
    
    Dim Q As Double
    Dim D2 As Double
    Dim d3 As Double
    Dim Gain As Double
    Dim Q1 As Double
    Dim Q2 As Double
    Dim Q3 As Double
    Dim Q4 As Double
    Dim OriginalPositionsCount As Long
    Dim HedgedPositions As Long
    Dim NonHedgedPositions As Long
    Dim EquivalentPositions As Double
    Dim EquivalentHedgedPositions As Double
    Dim EquivalentNonHedgedPositions As Double
    
    With gPortfolioHedge
        .CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
        .CalculateSubportfolioTotals2 HedgeSymbol, HedgedPositions, EquivalentHedgedPositions
        .CalculateSubportfolioTotals2 HedgeSymbol2, NonHedgedPositions, EquivalentNonHedgedPositions
    End With
    
    sBody(1) = sBody(1) & CellSeparator & Format$(D2, "#,##0.#0;(#,##0.#0)")
    sBody(2) = sBody(2) & CellSeparator & Format$(d3, "#,##0.#0;(#,##0.#0)")
    sBody(3) = sBody(3) & CellSeparator & Format$(Gain * 100, "#,##0.#0;(#,##0.#0)")
    If HedgeSymbol = "ALL" Then
        sBody(4) = sBody(4) & CellSeparator & Format$(HedgedPositions, "#,##0;(#,##0)")
    Else
        sBody(4) = sBody(4) & CellSeparator & Format$(HedgedPositions + NonHedgedPositions, "#,##0;(#,##0)")
    End If
    sBody(5) = sBody(5) & CellSeparator & Format$(HedgedPositions, "#,##0;(#,##0)")
    sBody(6) = sBody(6) & CellSeparator & Format$(EquivalentHedgedPositions, "#,##0.#0;(#,##0.#0)")
    sBody(7) = sBody(7) & CellSeparator & Format$(-Q, "#,##0.#0;(#,##0.#0)")
    If HedgeSymbol = "ALL" Then
        sBody(8) = sBody(8) & CellSeparator & Format$(0, "#,##0.#0;(#,##0.#0)")
    Else
        sBody(8) = sBody(8) & CellSeparator & Format$(EquivalentNonHedgedPositions, "#,##0.#0;(#,##0.#0)")
    End If
    sBody(9) = sBody(9) & CellSeparator & Format$(-Q1, "#,##0.#0;(#,##0.#0)")
    sBody(10) = sBody(10) & CellSeparator & Format$(-Q2, "#,##0.#0;(#,##0.#0)")
    sBody(11) = sBody(11) & CellSeparator & Format$(-Q3, "#,##0.#0;(#,##0.#0)")
    sBody(12) = sBody(12) & CellSeparator & Format$(-Q4, "#,##0.#0;(#,##0.#0)")

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddIndexGrid()
Dim sFormat As String
Dim sHeader As String
Dim sBody As String
    sHeader = "Index symbol" & CellSeparator & "Index price" & CellSeparator & "Index HV, %" & CellSeparator & "Index IV, %"
    sBody = ""
    sBody = sBody & "SPX" & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("SPX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "NDX" & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("NDX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "OEX" & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("OEX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    sBody = sBody & "DJX" & CellSeparator & _
        Format$(gPortfolioHedge.GetStockPrice("DJX"), "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexHV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & CellSeparator & _
        Format$(gPortfolioHedge.GetIndexIV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & RowSeparator
    AddTitle "Index data"
    AddGrid 5, sFormat, sHeader, sBody
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ScanGrid(fgGrid As VSFlexGrid, nCols As Long, sFormat As String, sHeader As String, sBody As String) As Boolean
Dim Row1 As Long
Dim Row2 As Long
Dim Col1 As Long
Dim Col2 As Long
Dim r As Long
Dim c As Long
Dim fFixed2 As Boolean

    sFormat = ""
    sHeader = ""
    sBody = ""

    fFixed2 = False
'    fgGrid.GetSelection Row1, Col1, Row2, Col2
'    If Row1 = Row2 And Col1 = Col2 Then
        Row1 = 1
        Row2 = fgGrid.Rows - 1
        Col1 = 0
        Col2 = fgGrid.Cols - 1
        If fgGrid.FixedRows = 2 Then
            fFixed2 = True
        End If
'    End If
    If Col1 = 0 Then
        ' Try to avoid empty column if outline is used
        If fgGrid.TextMatrix(0, 0) = " " Then
            Col1 = 1
        End If
    End If
    If Col2 = 8 Then
        ' Try to avoid empty column if outline is used
        If fgGrid.TextMatrix(0, Col2) = "Remove" Then
            Col2 = Col2 - 1
        End If
    End If
    nCols = Col2 - Col1 + 1

    For c = Col1 To Col2
        If Not fgGrid.ColHidden(c) Then
            If Len(sFormat) > 0 Then
                 sFormat = sFormat & CellSeparator
            End If
            If Len(sHeader) > 0 Then
                 sHeader = sHeader & CellSeparator
            End If
            sHeader = sHeader & fgGrid.TextMatrix(0, c)
            If fFixed2 Then
                If fgGrid.TextMatrix(0, c) <> fgGrid.TextMatrix(1, c) Then
                    sHeader = sHeader & " " & fgGrid.TextMatrix(1, c)
                End If
            End If
        End If
    Next
    sFormat = sFormat & RowSeparator
    sHeader = sHeader & RowSeparator

    sHeader = Replace(sHeader, vbCrLf, " ")

    Dim sLineBody As String
    Dim sCellBody As String
    For r = Row1 To Row2
        If m_ShowHiddenRows Or Not fgGrid.RowHidden(r) Then
            If fFixed2 And r = 1 Then GoTo Skip
            sLineBody = ""
            For c = Col1 To Col2
                If Not fgGrid.ColHidden(c) Then
                    If Len(sLineBody) > 0 Then
                         sLineBody = sLineBody & CellSeparator
                    End If
                    If fgGrid.Cell(flexcpChecked, r, c) = flexNoCheckbox Then
                        sCellBody = fgGrid.Cell(flexcpTextDisplay, r, c)
                        If sCellBody = "" Then
                            sCellBody = " "
                        End If
                    Else
                        If fgGrid.Cell(flexcpChecked, r, c) = flexUnchecked Or fgGrid.Cell(flexcpChecked, r, c) = flexTSUnchecked Then
                            sCellBody = " "
                        Else
                            sCellBody = "+"
                        End If
                    End If
                    sLineBody = sLineBody & sCellBody
                End If
            Next
            sBody = sBody & sLineBody & RowSeparator
        End If
Skip:
    Next
    ScanGrid = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintGrid(fgGrid As VSFlexGrid, ByVal sTitle As String)
    On Error Resume Next
    AddStartDoc
    AddTitle sTitle
    AddGridReport fgGrid
    AddEndDoc
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddStartDoc()
    On Error Resume Next
    Buffer = ""
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let Paragraph(ByVal vNewValue As Variant)
    Buffer = Buffer & vNewValue & vbCrLf
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddCaption(ByVal sTitle As String)
    On Error Resume Next
    Paragraph = ""
    Paragraph = sTitle
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddTitle(ByVal sTitle As String)
    On Error Resume Next
    Paragraph = ""
    Paragraph = sTitle
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddSubTitle(ByVal sTitle As String)
    On Error Resume Next
    Paragraph = ""
    Paragraph = sTitle
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddGridReport(fgGrid As VSFlexGrid)
Dim nCols As Long
Dim sFormat As String, sHeader As String, sBody As String
    If ScanGrid(fgGrid, nCols, sFormat, sHeader, sBody) Then
        AddGrid nCols, sFormat, sHeader, sBody
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddGrid(ByVal nCols As Long, ByVal sFormat As String, ByVal sHeader As String, ByVal sBody As String)
    Paragraph = ""
    Paragraph = sHeader
    Paragraph = sBody
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddEndDoc()
    With Clipboard
        .Clear
        .SetText Buffer
    End With
End Sub

