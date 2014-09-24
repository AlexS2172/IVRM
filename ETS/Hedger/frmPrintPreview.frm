VERSION 5.00
Object = "{A8561640-E93C-11D3-AC3B-CE6078F7B616}#1.0#0"; "VSPRINT7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmPrintPreview 
   Caption         =   "Print Preview"
   ClientHeight    =   7920
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10305
   BeginProperty Font 
      Name            =   "Tahoma"
      Size            =   8.25
      Charset         =   204
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   Icon            =   "frmPrintPreview.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   7920
   ScaleWidth      =   10305
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.ComboBox cbxZoom1 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   3840
      Style           =   2  'Dropdown List
      TabIndex        =   10
      Top             =   0
      Width           =   1455
   End
   Begin VB.Timer tmrMakereport 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   9720
      Top             =   0
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   7665
      Width           =   10305
      _ExtentX        =   18177
      _ExtentY        =   450
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
   End
   Begin VSPrinter7LibCtl.VSPrinter vsPrinter 
      Height          =   7350
      Left            =   0
      TabIndex        =   1
      Top             =   360
      Width           =   10305
      _cx             =   18177
      _cy             =   12965
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      MousePointer    =   0
      BackColor       =   -2147483643
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   11.25
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
      _ConvInfo       =   1
      AutoRTF         =   -1  'True
      Preview         =   -1  'True
      DefaultDevice   =   0   'False
      PhysicalPage    =   -1  'True
      AbortWindow     =   -1  'True
      AbortWindowPos  =   0
      AbortCaption    =   "Printing ..."
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
      SmallChangeHorz =   300
      SmallChangeVert =   300
      Track           =   0   'False
      ProportionalBars=   -1  'True
      Zoom            =   38.8591800356506
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
      NavBar          =   1
      NavBarColor     =   -2147483633
      ExportFormat    =   0
      URL             =   ""
      Navigation      =   3
      NavBarMenuText  =   "Whole &Page|Page &Width|&Two Pages|Thumb&nail"
   End
   Begin ElladaFlatControls.FlatButton btnPageSetup 
      Height          =   285
      Left            =   5520
      TabIndex        =   2
      Top             =   0
      Width           =   1170
      _ExtentX        =   2064
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MouseIcon       =   "frmPrintPreview.frx":014A
      MousePointer    =   99
      Caption         =   "Page Setup.."
   End
   Begin ElladaFlatControls.FlatButton btnPrint 
      Height          =   285
      Left            =   6840
      TabIndex        =   3
      Top             =   0
      Width           =   1170
      _ExtentX        =   2064
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Tahoma"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      MouseIcon       =   "frmPrintPreview.frx":02AC
      MousePointer    =   99
      Caption         =   "Print.."
   End
   Begin ElladaFlatControls.FlatButton btnMove 
      Height          =   285
      Index           =   0
      Left            =   0
      TabIndex        =   4
      Top             =   0
      Width           =   375
      _ExtentX        =   661
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "|<"
   End
   Begin ElladaFlatControls.FlatButton btnMove 
      Height          =   285
      Index           =   1
      Left            =   480
      TabIndex        =   5
      Top             =   0
      Width           =   375
      _ExtentX        =   661
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "<"
   End
   Begin ElladaFlatControls.FlatButton btnMove 
      Height          =   285
      Index           =   2
      Left            =   1920
      TabIndex        =   6
      Top             =   0
      Width           =   375
      _ExtentX        =   661
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   ">"
   End
   Begin ElladaFlatControls.FlatButton btnMove 
      Height          =   285
      Index           =   3
      Left            =   2400
      TabIndex        =   7
      Top             =   0
      Width           =   375
      _ExtentX        =   661
      _ExtentY        =   503
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   ">|"
   End
   Begin VB.Label lblZoom 
      Alignment       =   1  'Right Justify
      Caption         =   "Zoom :"
      Height          =   255
      Left            =   2880
      TabIndex        =   9
      Top             =   30
      Width           =   855
   End
   Begin VB.Label lblPage 
      Alignment       =   2  'Center
      Caption         =   "Page 1 of 1"
      Height          =   255
      Left            =   720
      TabIndex        =   8
      Top             =   30
      Width           =   1335
   End
End
Attribute VB_Name = "frmPrintPreview"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Public Enum PrintTask
    PRINT_NOTHING
    PRINT_PORTFOLIO
    PRINT_PORTFOLIO_SUMMARY
    PRINT_SHORT_SUBPORTFOLIO
    PRINT_LONG_SUBPORTFOLIO
    PRINT_STOCKS
End Enum

Private fVegaMode As Boolean
Private m_PrintWhat As PrintTask
Private m_ShowHiddenRows As Boolean
Private pgW As Double

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ExecutePrint(ByVal PrintWhat As PrintTask)
    m_PrintWhat = PrintWhat
    m_ShowHiddenRows = True
    tmrMakereport.Interval = 100 'msec
    tmrMakereport.Enabled = True
    Me.Show vbModal
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub tmrMakereport_Timer()
    On Error Resume Next
    tmrMakereport.Enabled = False
    ReDisplay
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ReDisplay()
    Screen.MousePointer = vbHourglass
    ExecutePrintEx m_PrintWhat
    Screen.MousePointer = vbDefault
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintEx(ByVal PrintWhat As PrintTask)
    On Error GoTo Herr
    'm_PrintWhat = PRINT_NOTHING
    If gPortfolioHedge Is Nothing Then
        Exit Sub
    End If
    fVegaMode = gPortfolioHedge.VegaMode
'    If gPortfolioHedge.IsEmptyPortfolio Then
'        gCmn.MyMsgBox "Portfolio is empty.", vbExclamation
'        Exit Sub
'    End If
    Select Case PrintWhat
    Case PRINT_NOTHING
        '
    Case PRINT_PORTFOLIO
        ExecutePrintPortfolio
    Case PRINT_PORTFOLIO_SUMMARY
        ExecutePrintPortfolioSummary
    Case PRINT_SHORT_SUBPORTFOLIO
        ExecutePrintSubPortfolio "SHORT", 0
    Case PRINT_LONG_SUBPORTFOLIO
        ExecutePrintSubPortfolio "LONG", 1
    Case PRINT_STOCKS
        ExecutePrintStocks
    Case Else
        '
    End Select
    Exit Sub
Herr:
    ShowError
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
Private Sub ExecutePrintPortfolio1()
    With gPortfolioHedge.rsPortfolioData
        If .RecordCount <> 0 Then

        End If
    End With
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
    sFormat = ">+" & (pgW / 4) * 1.25 & "|>" & (pgW / 4) * 0.75
    sHeader = "|"
    sBody = ""
    sBody = sBody & "Unhedged deviation, $" & "|" & Format$(D2, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "Hedged deviation, $" & "|" & Format$(d3, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "Gain, %" & "|" & Format$(Gain * 100, "#,##0.#0;(#,##0.#0)") & ";"
    If fVegaMode Then
        sBody = sBody & "Original number of contracts"
    Else
        sBody = sBody & "Original number of stocks"
    End If
    sBody = sBody & "|" & Format$(HedgedPositions + NonHedgedPositions, "#,##0;(#,##0)") & ";"
    If fVegaMode Then
        sBody = sBody & "Hedged contracts"
    Else
        sBody = sBody & "Hedged stocks"
    End If
    sBody = sBody & "|" & Format$(HedgedPositions, "#,##0;(#,##0)") & ";"
    sBody = sBody & "Hedged position, $" & "|" & Format$(EquivalentHedgedPositions, "#,##0.#0;(#,##0.#0)") & ";"
    If fVegaMode Then
        sBody = sBody & "Index vega" & "|" & Format$(-Q, "#,##0.#0;(#,##0.#0)") & ";"

    Else
        sBody = sBody & "Index position, $" & "|" & Format$(-Q, "#,##0.#0;(#,##0.#0)") & ";"
    End If
    sBody = sBody & "Remaining position, $" & "|" & Format$(EquivalentNonHedgedPositions, "#,##0.#0;(#,##0.#0)") & ";"
    
    AddGrid 2, sFormat, sHeader, sBody
    AddQbyIndexGrid1 -Q1, -Q2, -Q3, -Q4

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddQbyIndexGrid1(ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double)
Dim sFormat As String
Dim sHeader As String
Dim sBody As String
    sFormat = ">+" & pgW / 8 & "|>+" & pgW / 5 & "|>+" & pgW / 8 & "|>+" & pgW / 8 & "|>+" & pgW / 8
    If fVegaMode Then
        sHeader = "Index Symbol|Equivalent Index Vega|Index Price|Index HV, %|Index IV, %"
    Else
        sHeader = "Index Symbol|Equivalent Index Position, $|Index Price|Index HV, %|Index IV, %"
    End If
    sBody = ""
    sBody = sBody & "SPX" & "|" & _
        Format$(Q1, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("SPX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "NDX" & "|" & _
        Format$(Q2, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("NDX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "OEX" & "|" & _
        Format$(Q3, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("OEX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "DJX" & "|" & _
        Format$(Q4, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("DJX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    AddTitle "Index Positions"
    AddGrid 5, sFormat, sHeader, sBody
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ExecutePrintSubPortfolio1(ByVal HedgeSymbol As String)
    If gPortfolioHedge.HedgeMode = "" Then
        HedgeSymbol = "OPT"
    End If
    If gPortfolioHedge.rsPositions Is Nothing Then
        Exit Sub
    End If

Dim Caption As String
    Caption = gPortfolioHedge.GetSubportfolioCaption(HedgeSymbol)
'    With gPortfolioHedge.rsPositions.Clone
'        .Filter = gPortfolioHedge.GetSubportfolioFilter(HedgeSymbol)
'        If .RecordCount <> 0 Then
'            .MoveFirst
'            Do While Not .EOF
'                .MoveNext
'            Loop
'        End If
'    End With
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
    
    sFormat = ">+" & (pgW / 4) * 1.25
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
        sBody0 = sBody0 & sBody(i) & ";"
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
    
    sFormat = sFormat & "|>" & (pgW / 4) * 0.75
    sHeader = sHeader & "|" & HedgeCaption
    
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
    
    sBody(1) = sBody(1) & "|" & Format$(D2, "#,##0.#0;(#,##0.#0)")
    sBody(2) = sBody(2) & "|" & Format$(d3, "#,##0.#0;(#,##0.#0)")
    sBody(3) = sBody(3) & "|" & Format$(Gain * 100, "#,##0.#0;(#,##0.#0)")
    If HedgeSymbol = "ALL" Then
        sBody(4) = sBody(4) & "|" & Format$(HedgedPositions, "#,##0;(#,##0)")
    Else
        sBody(4) = sBody(4) & "|" & Format$(HedgedPositions + NonHedgedPositions, "#,##0;(#,##0)")
    End If
    sBody(5) = sBody(5) & "|" & Format$(HedgedPositions, "#,##0;(#,##0)")
    sBody(6) = sBody(6) & "|" & Format$(EquivalentHedgedPositions, "#,##0.#0;(#,##0.#0)")
    sBody(7) = sBody(7) & "|" & Format$(-Q, "#,##0.#0;(#,##0.#0)")
    If HedgeSymbol = "ALL" Then
        sBody(8) = sBody(8) & "|" & Format$(0, "#,##0.#0;(#,##0.#0)")
    Else
        sBody(8) = sBody(8) & "|" & Format$(EquivalentNonHedgedPositions, "#,##0.#0;(#,##0.#0)")
    End If
    sBody(9) = sBody(9) & "|" & Format$(-Q1, "#,##0.#0;(#,##0.#0)")
    sBody(10) = sBody(10) & "|" & Format$(-Q2, "#,##0.#0;(#,##0.#0)")
    sBody(11) = sBody(11) & "|" & Format$(-Q3, "#,##0.#0;(#,##0.#0)")
    sBody(12) = sBody(12) & "|" & Format$(-Q4, "#,##0.#0;(#,##0.#0)")

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddIndexGrid()
Dim sFormat As String
Dim sHeader As String
Dim sBody As String
    sFormat = ">+" & pgW / 8 & "|>+" & pgW / 8 & "|>+" & pgW / 8 & "|>+" & pgW / 8
    sHeader = "Index symbol|Index price|Index HV, %|Index IV, %"
    sBody = ""
    sBody = sBody & "SPX" & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("SPX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("SPX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "NDX" & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("NDX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("NDX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "OEX" & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("OEX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("OEX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
    sBody = sBody & "DJX" & "|" & _
        Format$(gPortfolioHedge.GetStockPrice("DJX"), "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexHV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & "|" & _
        Format$(gPortfolioHedge.GetIndexIV("DJX") * 100, "#,##0.#0;(#,##0.#0)") & ";"
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
Dim R As Long
Dim c As Long
Dim fFixed2 As Boolean
    
    sFormat = ""
    sHeader = ""
    sBody = ""
    
    fFixed2 = False
    'fgGrid.GetSelection Row1, Col1, Row2, Col2
    'If Row1 = Row2 And Col1 = Col2 Then
        Row1 = 1
        Row2 = fgGrid.Rows - 1
        Col1 = 0
        Col2 = fgGrid.Cols - 1
        If fgGrid.FixedRows = 2 Then
            fFixed2 = True
        End If
    'End If
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
                 sFormat = sFormat & "|"
            End If
            If Len(sHeader) > 0 Then
                 sHeader = sHeader & "|"
            End If
            sFormat = sFormat & ">+" & pgW / (Col2 - Col1 + 1)
            sHeader = sHeader & fgGrid.TextMatrix(0, c)
            If fFixed2 Then
                If fgGrid.TextMatrix(0, c) <> fgGrid.TextMatrix(1, c) Then
                    sHeader = sHeader & " " & fgGrid.TextMatrix(1, c)
                End If
            End If
        End If
    Next
    sFormat = sFormat & ";"
    sHeader = sHeader & ";"
    
    sHeader = Replace(sHeader, vbCrLf, " ")
    
    Dim sLineBody As String
    Dim sCellBody As String
    For R = Row1 To Row2
        If m_ShowHiddenRows Or Not fgGrid.RowHidden(R) Then
            If fFixed2 And R = 1 Then GoTo Skip
            sLineBody = ""
            For c = Col1 To Col2
                If Not fgGrid.ColHidden(c) Then
                    If Len(sLineBody) > 0 Then
                         sLineBody = sLineBody & "|"
                    End If
                    If fgGrid.Cell(flexcpChecked, R, c) = flexNoCheckbox Then
                        sCellBody = fgGrid.Cell(flexcpTextDisplay, R, c)
                        If sCellBody = "" Then
                            sCellBody = " "
                        End If
                    Else
                        If fgGrid.Cell(flexcpChecked, R, c) = flexUnchecked Or fgGrid.Cell(flexcpChecked, R, c) = flexTSUnchecked Then
                            sCellBody = " "
                        Else
                            sCellBody = "+"
                        End If
                    End If
                    sLineBody = sLineBody & sCellBody
                End If
            Next
            sBody = sBody & sLineBody & ";"
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
    With vsPrinter
        .Clear
        .Header = ""
        .Footer = ""
        pgW = .PageWidth - .MarginLeft - .MarginRight
        .MarginHeader = .MarginTop - ScaleY(10, vbPoints, vbTwips)
        .MarginFooter = .MarginBottom - ScaleY(10, vbPoints, vbTwips)
        .StartDoc
        .HdrFontSize = 8
        .FontSize = 8
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddCaption(ByVal sTitle As String)
    On Error Resume Next
    With vsPrinter
        .FontBold = False
        .FontSize = 14
        .Paragraph = ""
        .TextAlign = taCenterTop
        .Paragraph = sTitle
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddTitle(ByVal sTitle As String)
    On Error Resume Next
    With vsPrinter
        .FontBold = True
        .FontSize = 10
        .Paragraph = ""
        .TextAlign = taLeftTop
        .Paragraph = sTitle
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddSubTitle(ByVal sTitle As String)
    On Error Resume Next
    With vsPrinter
        .FontBold = False
        .FontSize = 8
        .TextAlign = taLeftTop
        .Paragraph = ""
        .Paragraph = sTitle
    End With
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
    With vsPrinter
        .FontBold = False
        If nCols > 15 Then
            .FontSize = 6
        Else
            .FontSize = 8
        End If
        .TextAlign = taLeftTop
        .Paragraph = ""
        .StartTable
        .AddTable sFormat, sHeader, sBody, RGB(192, 192, 192)
        .EndTable
    End With
End Sub
        
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AddEndDoc()
Dim dtNow As Date
Dim i As Long
    With vsPrinter
        .EndDoc
        .FontBold = False
        .FontSize = 8
        dtNow = Now
        For i = 1 To .PageCount
            .StartOverlay i
            .TextAlign = taLeftTop
            .CurrentX = .MarginLeft
            .CurrentY = .MarginHeader
            .FontBold = True
            .Text = APP_TITLE
            .FontBold = False
            .TextAlign = taRightTop
            .Text = Format$(dtNow, "General Date")
            .CurrentX = .MarginLeft
            .CurrentY = .PageHeight - .MarginBottom
            .TextAlign = taRightTop
            .Paragraph = "Page " & i & " of " & .PageCount
            .EndOverlay
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnPageSetup_Click()
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    If vsPrinter.PrintDialog(pdPageSetup) Then
        ReDisplay
    End If
    Screen.MousePointer = vbDefault
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnPrint_Click()
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    vsPrinter.PrintDoc True
    Screen.MousePointer = vbDefault
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cbxZoom1_Click()
    On Error Resume Next
    With cbxZoom1
        If .ListIndex >= 0 Then
            If .ItemData(.ListIndex) <> zmPercentage Then
                vsPrinter.ZoomMode = .ItemData(.ListIndex)
            Else
                vsPrinter.Zoom = Val(.Text)
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error GoTo Herr
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    
    Me.Caption = "Print Preview - " & APP_TITLE
    
    btnPageSetup.Enabled = (Printers.Count > 0)
    btnPrint.Enabled = (Printers.Count > 0)
    
    With vsPrinter
        .Clear
        .PhysicalPage = True
        .NavBar = vpnbNone
        .Track = True
        .FontName = "Tahoma"
        .HdrFontName = "Tahoma"
    End With
    
    LoadSettings
    InitZoomCombo
    
    UpdateStatus
    Exit Sub
Herr:
    ShowError "Error loading Print Priview form."
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    SaveSettings
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Resize()
    On Error Resume Next
    With vsPrinter
        .Width = Max(0, Me.ScaleWidth - 2 * .Left)
        .Height = Max(0, Me.ScaleHeight - .Top - sbStatus.Height)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadSettings()
    On Error Resume Next
    With vsPrinter
        .ZoomMode = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintZoomMode", .ZoomMode)
        If .ZoomMode = zmPercentage Then
            .Zoom = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintZoom", .Zoom)
        End If
        .PaperSize = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintPaperSize", pprLetter)
        .Orientation = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintOrientation", orPortrait)
        .MarginLeft = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginLeft", .MarginLeft)
        .MarginRight = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginRight", .MarginRight)
        .MarginTop = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginTop", .MarginTop)
        .MarginBottom = GetStrKeyValueEx(APP_REG_KEY & "Settings", "PrintMarginBottom", .MarginBottom)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveSettings()
    On Error Resume Next
    With vsPrinter
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintZoomMode", .ZoomMode
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintZoom", .Zoom
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintPaperSize", .PaperSize
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintOrientation", .Orientation
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginLeft", .MarginLeft
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginRight", .MarginRight
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginTop", .MarginTop
        SetStrKeyValue HKEY_CURRENT_USER, APP_REG_KEY & "Settings", "PrintMarginBottom", .MarginBottom
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitZoomCombo()
    On Error Resume Next
    Dim i As Long
    
    With cbxZoom1
        .Clear
        .AddItem "Thumbnail"
        .ItemData(.ListCount - 1) = zmThumbnail
        
        .AddItem "Two Pages"
        .ItemData(.ListCount - 1) = zmTwoPages
    
        .AddItem "Whole Page"
        .ItemData(.ListCount - 1) = zmWholePage
        
        .AddItem "Page Width"
        .ItemData(.ListCount - 1) = zmPageWidth
        
        .AddItem "Stretch"
        .ItemData(.ListCount - 1) = zmStretch
    
        .AddItem "25%"
        .ItemData(.ListCount - 1) = zmPercentage
        
        .AddItem "50%"
        .ItemData(.ListCount - 1) = zmPercentage
    
        .AddItem "75%"
        .ItemData(.ListCount - 1) = zmPercentage
    
        .AddItem "100%"
        .ItemData(.ListCount - 1) = zmPercentage
    
        .AddItem "150%"
        .ItemData(.ListCount - 1) = zmPercentage
    
        .AddItem "200%"
        .ItemData(.ListCount - 1) = zmPercentage
    
        .AddItem "400%"
        .ItemData(.ListCount - 1) = zmPercentage
        
        For i = 0 To .ListCount - 1
            If .ItemData(i) = vsPrinter.ZoomMode Then
                If vsPrinter.ZoomMode <> zmPercentage Then
                    .ListIndex = i
                    Exit For
                ElseIf Val(.List(i)) = Round(vsPrinter.Zoom) Then
                    .ListIndex = i
                    Exit For
                End If
            End If
        Next
        If .ListIndex < 0 Then .ListIndex = 2
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnMove_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case 0 ' First
        vsPrinter.PreviewPage = 1
    Case 1 ' Prev
        vsPrinter.PreviewPage = vsPrinter.PreviewPage - 1
    Case 2 ' Next
        vsPrinter.PreviewPage = vsPrinter.PreviewPage + 1
    Case 3 ' last
        vsPrinter.PreviewPage = vsPrinter.PageCount
    End Select
    UpdateStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UpdateStatus()
    On Error Resume Next
    lblPage.Caption = "Page " & vsPrinter.PreviewPage & " of " & vsPrinter.PageCount
    btnMove(0).Enabled = (vsPrinter.PageCount > 0 And vsPrinter.PreviewPage > 1)
    btnMove(1).Enabled = btnMove(0).Enabled
    btnMove(2).Enabled = (vsPrinter.PageCount > vsPrinter.PreviewPage)
    btnMove(3).Enabled = btnMove(2).Enabled
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub vsPrinter_AfterUserPage()
    On Error Resume Next
    UpdateStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub vsPrinter_BeforeUserZoom(NewZoom As Integer)
    On Error Resume Next
    NewZoom = vsPrinter.Zoom
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub vsPrinter_EndDoc()
    On Error Resume Next
    UpdateStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub vsPrinter_EndPage()
    On Error Resume Next
    UpdateStatus
End Sub

