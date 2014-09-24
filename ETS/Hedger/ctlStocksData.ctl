VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlStocksData 
   Appearance      =   0  'Flat
   BackColor       =   &H00C8D0D4&
   ClientHeight    =   2445
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   3975
   ScaleHeight     =   2445
   ScaleWidth      =   3975
   Begin VSFlex7Ctl.VSFlexGrid grd 
      Height          =   2055
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3735
      _cx             =   6588
      _cy             =   3625
      _ConvInfo       =   1
      Appearance      =   2
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
      BackColorFixed  =   13160660
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   6710886
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
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
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   5
         Left            =   2760
         MouseIcon       =   "ctlStocksData.ctx":0000
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":0152
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   4
         Left            =   2400
         MouseIcon       =   "ctlStocksData.ctx":01B3
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":0305
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   3
         Left            =   2040
         MouseIcon       =   "ctlStocksData.ctx":0366
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":04B8
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   2
         Left            =   1680
         MouseIcon       =   "ctlStocksData.ctx":0519
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":066B
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   0
         Left            =   960
         MouseIcon       =   "ctlStocksData.ctx":06CC
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":081E
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   1
         Left            =   1320
         MouseIcon       =   "ctlStocksData.ctx":087F
         MousePointer    =   99  'Custom
         Picture         =   "ctlStocksData.ctx":09D1
         ToolTipText     =   "Open Help"
         Top             =   0
         Width           =   150
      End
   End
End
Attribute VB_Name = "ctlStocksData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Editable As Boolean
Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1
Public Cancel As Boolean

Private Enum ColIndexEnum
    IDX_TICKER = 0
    IDX_STOCK_PRICE
    IDX_STOCK_HV
    IDX_STOCK_IV
    
    IDX_CORR_SPX
    IDX_CORR_NDX
    IDX_CORR_OEX
    IDX_CORR_DJX
    
    IDX_VCORR_SPX
    IDX_VCORR_NDX
    IDX_VCORR_OEX
    IDX_VCORR_DJX
    
    MINT_COLUMN_COUNT
End Enum

Private Const TXT_COLOR_HOVER_NORM = &H6633&    '&H9966&
Private Const TXT_COLOR_HOVER_WARN = vbRed
Private Const BKG_COLOR_CHILD_NORM = &HC0FFFF
Private Const BKG_COLOR_CHILD_WARN = &HC0C0FF

Private m_cdColumns(0 To MINT_COLUMN_COUNT - 1) As New clsColumnDefinition

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    Set PortfolioHedge = gPortfolioHedge
    With grd
        .Left = 0
        .Top = 0
    End With
    Editable = False
    LoadSettings
    FormatGrid
    ApplyGridLayout
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    On Error Resume Next
    With grd
        .Width = Max(0, ScaleWidth - 2 * .Left)
        .Height = Max(0, ScaleHeight - 2 * .Top)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadSettings()
Dim i As Long
    
    For i = 0 To MINT_COLUMN_COUNT - 1
        With m_cdColumns(i)
            .bVisible = True
            .lIndex = i
            .bCanChangeOrder = False
            .bCanEdit = False
            Set .imgHelp = Nothing
        End With
    Next
    
    i = IDX_TICKER
    m_cdColumns(i).sName = "StockSymbol"
    m_cdColumns(i).sCaption = "Underlying" & vbCrLf & "Symbol"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDispatch
    m_cdColumns(i).sFormat = ""
    Set m_cdColumns(i).imgHelp = imgSdHelp(i)
    
    i = IDX_STOCK_PRICE
    m_cdColumns(i).sName = "StockPrice"
    m_cdColumns(i).sCaption = "Stock" & vbCrLf & "Price"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#,###.##"
    m_cdColumns(i).bCanEdit = False
    Set m_cdColumns(i).imgHelp = imgSdHelp(i)
    
    i = IDX_STOCK_HV
    m_cdColumns(i).sName = "StockHV"
    m_cdColumns(i).sCaption = "Stock" & vbCrLf & "HV,%"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    Set m_cdColumns(i).imgHelp = imgSdHelp(i)
    
    i = IDX_STOCK_IV
    m_cdColumns(i).sName = "StockIV"
    m_cdColumns(i).sCaption = "Stock" & vbCrLf & "IV,%"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    Set m_cdColumns(i).imgHelp = imgSdHelp(i)
    
    i = IDX_CORR_SPX
    m_cdColumns(i).sName = "Corr1"
    m_cdColumns(i).sCaption = "Price Corr"
    m_cdColumns(i).sCaption2 = "SPX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_CORR_NDX
    m_cdColumns(i).sName = "Corr2"
    m_cdColumns(i).sCaption = "Price Corr"
    m_cdColumns(i).sCaption2 = "NDX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_CORR_OEX
    m_cdColumns(i).sName = "Corr3"
    m_cdColumns(i).sCaption = "Price Corr"
    m_cdColumns(i).sCaption2 = "OEX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_CORR_DJX
    m_cdColumns(i).sName = "Corr4"
    m_cdColumns(i).sCaption = "Price Corr"
    m_cdColumns(i).sCaption2 = "DJX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    Set m_cdColumns(i).imgHelp = imgSdHelp(4)
    
    i = IDX_VCORR_SPX
    m_cdColumns(i).sName = "VCorr1"
    m_cdColumns(i).sCaption = "Voly Corr"
    m_cdColumns(i).sCaption2 = "SPX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_VCORR_NDX
    m_cdColumns(i).sName = "VCorr2"
    m_cdColumns(i).sCaption = "Voly Corr"
    m_cdColumns(i).sCaption2 = "NDX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_VCORR_OEX
    m_cdColumns(i).sName = "VCorr3"
    m_cdColumns(i).sCaption = "Voly Corr"
    m_cdColumns(i).sCaption2 = "OEX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_VCORR_DJX
    m_cdColumns(i).sName = "VCorr4"
    m_cdColumns(i).sCaption = "Voly Corr"
    m_cdColumns(i).sCaption2 = "DJX"
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    m_cdColumns(i).bCanEdit = False
    Set m_cdColumns(i).imgHelp = imgSdHelp(5)

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FormatGrid()
Dim i As Long
    With grd
        .Rows = 3
        .Cols = MINT_COLUMN_COUNT
        .FixedRows = 2
        .FixedCols = 0
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .ExplorerBar = flexExSortShow
        For i = 0 To MINT_COLUMN_COUNT - 1
            .ColHidden(i) = Not m_cdColumns(i).bVisible
            .TextMatrix(0, i) = m_cdColumns(i).sCaption
            .TextMatrix(1, i) = m_cdColumns(i).sCaption2
            .MergeCol(i) = m_cdColumns(i).MergeCol
            .ColKey(i) = i
            .ColDataType(i) = m_cdColumns(i).vsDataType
            .ColFormat(i) = m_cdColumns(i).sFormat
            If Not m_cdColumns(i).imgHelp Is Nothing Then
                m_cdColumns(i).imgHelp.Top = .RowPos(0)
            End If
        Next
        .Cell(flexcpAlignment, 0, 0, 1, .Cols - 1) = flexAlignCenterCenter
'        .Cell(flexcpBackColor, 0, 0, 1, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 1, .Cols - 1) = False
        .Editable = flexEDKbdMouse
        .AllowUserResizing = flexResizeColumns
        .AutoResize = True
        .AllowSelection = True
        .HighLight = flexHighlightWithFocus
    End With
    AdjustHelpPictures
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub HideHelpTags()
    On Error Resume Next
    Dim i As Long
    For i = imgSdHelp.LBound To imgSdHelp.UBound
        imgSdHelp(i).Visible = False
    Next
    For i = 0 To MINT_COLUMN_COUNT - 1
        If Not m_cdColumns(i).imgHelp Is Nothing Then
            m_cdColumns(i).imgHelp.Visible = False
        End If
    Next
    DoEvents
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpTags()
    On Error Resume Next
    Dim i As Long
    For i = imgSdHelp.LBound To imgSdHelp.UBound
        imgSdHelp(i).Visible = g_bShowHelpTags
    Next
    For i = 0 To MINT_COLUMN_COUNT - 1
        If Not m_cdColumns(i).imgHelp Is Nothing Then
            m_cdColumns(i).imgHelp.Visible = IIf(g_bShowHelpTags, m_cdColumns(i).bVisible, False)
        End If
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ApplyGridLayout()
Dim i As Long
Dim lIdx As Long
    With grd
        For i = 0 To MINT_COLUMN_COUNT - 1
            lIdx = .ColIndex(i)
            .ColHidden(lIdx) = Not m_cdColumns(i).bVisible
            .ColPosition(lIdx) = m_cdColumns(i).lIndex
        Next
    End With
    ShowHelpTags
    AdjustHelpPictures
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotSymbolsChanged(rsChangedSymbols As ADODB.Recordset)
    On Error Resume Next
    If Not rsChangedSymbols Is Nothing Then
        With rsChangedSymbols
            If .RecordCount <> 0 Then
                grd.Redraw = flexRDNone
                .MoveFirst
                Do While Not .EOF
                    Debug.Print !Supported
                    If !Supported Then
                        ' Symbol came to list
                        AppendSymbol !StockSymbol
                    Else
                        ' Symbol gone from list
                        RemoveSymbol !StockSymbol
                    End If
                    .MoveNext
                    DoEvents
                Loop
                ColorRows grd
                grd.Redraw = True
            End If
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AppendSymbol(ByVal StockSymbol As String)
Dim StockPrice As Double
Dim StockHV As Double
Dim StockIV As Double
Dim Corr(1 To 4) As Double
Dim VCorr(1 To 4) As Double
Dim fCorr(1 To 4) As Boolean
Dim fNoCorr As Boolean
Dim lRow As Long
    With grd
        If Not PortfolioHedge.CheckStockSymbol(StockSymbol) Then
            GoTo Skip
        End If
        With PortfolioHedge
            fCorr(1) = .GetStockIndexCorr2(StockSymbol, "SPX", Corr(1), VCorr(1))
            fCorr(2) = .GetStockIndexCorr2(StockSymbol, "NDX", Corr(2), VCorr(2))
            fCorr(3) = .GetStockIndexCorr2(StockSymbol, "OEX", Corr(3), VCorr(3))
            fCorr(4) = .GetStockIndexCorr2(StockSymbol, "DJX", Corr(4), VCorr(4))
            StockPrice = .GetStockPrice(StockSymbol)
            StockHV = .GetStockHV(StockSymbol)
            StockIV = .GetStockIV(StockSymbol)
        End With
        
        lRow = AddRow
        'grd.Cell(flexcpBackColor, lRow, 0, lRow, grd.Cols - 1) = IIf(lRow Mod 2 > 0, c0, c1)
        
        FillCell lRow, IDX_TICKER, StockSymbol
        FillCell lRow, IDX_STOCK_HV, StockHV * 100
        FillCell lRow, IDX_STOCK_IV, StockIV * 100
        FillCell lRow, IDX_STOCK_PRICE, StockPrice
        
        If fCorr(1) Then
            FillCell lRow, IDX_CORR_SPX, Corr(1)
            FillCell lRow, IDX_VCORR_SPX, VCorr(1)
        End If
        If fCorr(2) Then
            FillCell lRow, IDX_CORR_NDX, Corr(2)
            FillCell lRow, IDX_VCORR_NDX, VCorr(2)
        End If
        If fCorr(3) Then
            FillCell lRow, IDX_CORR_OEX, Corr(3)
            FillCell lRow, IDX_VCORR_OEX, VCorr(3)
        End If
        If fCorr(4) Then
            FillCell lRow, IDX_CORR_DJX, Corr(4)
            FillCell lRow, IDX_VCORR_DJX, VCorr(4)
        End If
    End With
Skip:
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub RemoveSymbol(ByVal StockSymbol As String)
Dim Row As Long
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            .RemoveItem Row
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayStockData(Optional ByVal StocksFilter As enmStocksFilter = STOCKS_SUPPORTED)
Dim lRow As Long
Dim StockSymbol As String
Dim StockPrice As Double
Dim StockHV As Double
Dim StockIV As Double
Dim rsStocks As ADODB.Recordset
Dim rsStockData As ADODB.Recordset
Dim Count As Long
    
    Cancel = False
    
    If PortfolioHedge Is Nothing Then
        Debug.Assert False
        ClearGrid
        Exit Sub
    End If
    Dim Sql As String
    
    Select Case StocksFilter
    Case STOCKS_SUPPORTED

#If ETS Then
        Set rsStockData = usp_StockReportDH_Get("")
#Else
        Sql = "EXEC usp_StockReportDH_Get"
'        Sql = "SELECT S.StockSymbol, P.ClosePrice, H.HV, H.IV, C.CorrSPX, C.VCorrSPX, C.CorrNDX, C.VCorrNDX, C.CorrOEX, C.VCorrOEX, C.CorrDJX, C.VCorrDJX" & _
            " FROM ((SupportedStockSymbols AS S LEFT JOIN SymbolPrices AS P ON S.StockSymbol = P.StockSymbol) LEFT JOIN StockHV180 AS H ON S.StockSymbol = H.StockSymbol) LEFT JOIN StockIndexCorr180 AS C ON S.StockSymbol = C.StockSymbol" & _
            " WHERE (S.Supported<>0);"
        
        Set rsStockData = OpenRSDynamic(Sql)
        rsStockData.Filter = "Supported=True"
#End If

    Case STOCKS_PORTFOLIO

#If ETS Then
        Dim SymbolList As String
        SymbolList = ""
        With PortfolioHedge
            With .GetSymbols(.rsPortfolioData)
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        If Len(SymbolList) > 0 Then
                            SymbolList = SymbolList & ","
                        End If
                        SymbolList = SymbolList & "'" & !StockSymbol & "'"
                        .MoveNext
                    Loop
                End If
            End With
        End With
        If Len(SymbolList) > 0 Then
            Set rsStockData = usp_StockReportDH_Get(SymbolList)
        Else
            Set rsStockData = New ADODB.Recordset
            With rsStockData
                .Fields.Append "Symbol", adVarChar, 20
                .Open
            End With
        End If
#Else
        With PortfolioHedge
            .SelectStockSymbols .GetSymbols(.rsPortfolioData)
        End With
        
        Sql = "EXEC usp_StockReportDH_Get"
'        Sql = "SELECT S.StockSymbol, P.ClosePrice, H.HV, H.IV, C.CorrSPX, C.VCorrSPX, C.CorrNDX, C.VCorrNDX, C.CorrOEX, C.VCorrOEX, C.CorrDJX, C.VCorrDJX" & _
            " FROM ((SupportedStockSymbols AS S LEFT JOIN SymbolPrices AS P ON S.StockSymbol=P.StockSymbol) LEFT JOIN StockHV180 AS H ON S.StockSymbol=H.StockSymbol) LEFT JOIN StockIndexCorr180 AS C ON S.StockSymbol=C.StockSymbol" & _
            " WHERE (S.Selected<>0);"
        
        Set rsStockData = OpenRSDynamic(Sql)
        rsStockData.Filter = "Selected=True"
#End If
    
    Case Else
        ClearGrid
        Exit Sub
    End Select
    
    If rsStockData Is Nothing Then
        ClearGrid
        Exit Sub
    End If
    
    HideHelpTags
    Dim c0 As Long
    Dim c1 As Long
    c0 = GCOLOR_TABLE_ODD_ROW_GRAY
    c1 = GCOLOR_TABLE_EVEN_ROW_GRAY
    
    Count = 0
    With rsStockData
        If .RecordCount <> 0 Then
            Editable = False
            grd.Rows = 2
            grd.Redraw = flexRDNone
            grd.ExplorerBar = flexExNone
            .Sort = "Symbol"
            .MoveFirst
            Do While Not .EOF
                Count = Count + 1
                lRow = AddRow
                grd.Cell(flexcpBackColor, lRow, 0, lRow, grd.Cols - 1) = IIf(lRow Mod 2 > 0, c0, c1)
                FillCell lRow, IDX_TICKER, !Symbol
                FillCell lRow, IDX_STOCK_HV, !HV * 100
                FillCell lRow, IDX_STOCK_IV, !IV * 100
                FillCell lRow, IDX_STOCK_PRICE, !PriceClose
                FillCell lRow, IDX_CORR_SPX, !CorrSPX
                FillCell lRow, IDX_VCORR_SPX, !VCorrSPX
                FillCell lRow, IDX_CORR_NDX, !CorrNDX
                FillCell lRow, IDX_VCORR_NDX, !VCorrNDX
                FillCell lRow, IDX_CORR_OEX, !CorrOEX
                FillCell lRow, IDX_VCORR_OEX, !VCorrOEX
                FillCell lRow, IDX_CORR_DJX, !CorrDJX
                FillCell lRow, IDX_VCORR_DJX, !VCorrDJX
                If (Count Mod 100) = 0 Then
                    grd.Redraw = True
                    DoEvents
                    grd.Redraw = flexRDNone
                End If
                If Cancel Then Exit Do
                .MoveNext
            Loop
        Else
            grd.Rows = 2
            grd.Rows = 3
            grd.Cell(flexcpBackColor, 2, 0, 2, grd.Cols - 1) = IIf(2 Mod 2 > 0, c0, c1)
        End If
        .Filter = ""
    End With
    grd.Redraw = True
    AdjustHelpPictures
    ShowHelpTags
    grd.ExplorerBar = flexExSortShow
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ClearGrid()
    With grd
        .Rows = 2
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function AddRow() As Long
    With grd
        .Rows = .Rows + 1
        AddRow = .Rows - 1
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FillCell(ByVal lRow As Long, ByVal iColl As ColIndexEnum, ByVal Value As Variant, Optional ByVal NullValue As String = "")
Dim lCol As Long
    With grd
        lCol = .ColIndex(iColl)
        If IsNull(Value) Then
            .TextMatrix(lRow, lCol) = NullValue
        Else
            .TextMatrix(lRow, lCol) = Value
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim lCol As Long
    On Error Resume Next
    grd.ComboList = ""
    lCol = grd.ColKey(Col)
    Cancel = Not (Editable And m_cdColumns(lCol).bCanEdit)
    If Not Cancel Then
        '
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyPressEdit(ByVal Row As Long, ByVal Col As Long, KeyAscii As Integer)
    On Error Resume Next
    Select Case grd.ColKey(Col)
    Case IDX_TICKER
    Case Else
        If Not IsNumericKey(KeyAscii) Then
            KeyAscii = 0
        End If
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_CellChanged(ByVal Row As Long, ByVal Col As Long)
Dim StockSymbol As String
Dim IndexSymbol As String
Dim StockPrice As Double
Dim StockHV As Double
Dim StockIV As Double
Dim Corr As Double
Dim VCorr As Double
Dim lCol As Long
Dim SubRow As Long
    On Error Resume Next
    If Editable And Row >= 2 Then
        With grd
            lCol = .ColKey(Col)
            StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER))
            If StockSymbol = "" Then
                SubRow = Row - 1
                Do While SubRow >= 2
                    StockSymbol = .TextMatrix(SubRow, .ColIndex(IDX_TICKER))
                    If "" <> StockSymbol Then
                        Exit Do
                    End If
                    SubRow = SubRow - 1
                Loop
            End If
            If StockSymbol = "" Then
                Exit Sub
            End If
            Select Case lCol
            Case IDX_STOCK_PRICE
                StockPrice = ScanDouble(.TextMatrix(Row, Col))
                PortfolioHedge.SetStockPrice StockSymbol, StockPrice, DATA_MANUAL
            Case IDX_STOCK_HV
                StockHV = ScanDouble(.TextMatrix(Row, Col)) / 100
                PortfolioHedge.SetStockHVData StockSymbol, StockHV, DATA_MANUAL
            Case IDX_STOCK_IV
                StockIV = ScanDouble(.TextMatrix(Row, Col)) / 100
                PortfolioHedge.SetStockIVData StockSymbol, StockIV, DATA_MANUAL
            
            Case IDX_CORR_SPX
                IndexSymbol = "SPX"
                Corr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_CORR_SPX)))
                PortfolioHedge.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, DATA_MANUAL
            Case IDX_CORR_NDX
                IndexSymbol = "NDX"
                Corr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_CORR_NDX)))
                PortfolioHedge.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, DATA_MANUAL
            Case IDX_CORR_OEX
                IndexSymbol = "OEX"
                Corr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_CORR_OEX)))
                PortfolioHedge.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, DATA_MANUAL
            Case IDX_CORR_DJX
                IndexSymbol = "DJX"
                Corr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_CORR_DJX)))
                PortfolioHedge.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, DATA_MANUAL
            
            Case IDX_VCORR_SPX
                IndexSymbol = "SPX"
                VCorr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_VCORR_SPX)))
                PortfolioHedge.SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr, DATA_MANUAL
            Case IDX_VCORR_NDX
                IndexSymbol = "NDX"
                VCorr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_VCORR_NDX)))
                PortfolioHedge.SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr, DATA_MANUAL
            Case IDX_VCORR_OEX
                IndexSymbol = "OEX"
                VCorr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_VCORR_OEX)))
                PortfolioHedge.SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr, DATA_MANUAL
            Case IDX_VCORR_DJX
                IndexSymbol = "DJX"
                VCorr = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_VCORR_DJX)))
                PortfolioHedge.SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr, DATA_MANUAL
            
            End Select
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            FillCell Row, IDX_STOCK_HV, HV * 100
            ColorCell grd, Row, IDX_STOCK_HV, DataStatus
        End If
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            FillCell Row, IDX_STOCK_IV, IV * 100
            ColorCell grd, Row, IDX_STOCK_IV, DataStatus
        End If
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            FillCell Row, IDX_STOCK_PRICE, StockPrice
            ColorCell grd, Row, IDX_STOCK_PRICE, DataStatus
        End If
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIndexCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
Dim Col1 As Long
    Editable = False
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            Select Case IndexSymbol
            Case "SPX"
                Col1 = IDX_CORR_SPX
            Case "NDX"
                Col1 = IDX_CORR_NDX
            Case "OEX"
                Col1 = IDX_CORR_OEX
            Case "DJX"
                Col1 = IDX_CORR_DJX
            Case Else
                GoTo Skip
            End Select
            FillCell Row, Col1, Corr
            ColorCell grd, Row, Col1, DataStatus
        End If
    End With
Skip:
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIndexVCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
Dim Col2 As Long
    Editable = False
    With grd
        Row = .FindRow(StockSymbol, , .ColIndex(IDX_TICKER))
        If Row > 1 Then
            Select Case IndexSymbol
            Case "SPX"
                Col2 = IDX_VCORR_SPX
            Case "NDX"
                Col2 = IDX_VCORR_NDX
            Case "OEX"
                Col2 = IDX_VCORR_OEX
            Case "DJX"
                Col2 = IDX_VCORR_DJX
            Case Else
                GoTo Skip
            End Select
            FillCell Row, Col2, VCorr
            ColorCell grd, Row, Col2, DataStatus
        End If
    End With
Skip:
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGrid() As VSFlexGrid
    On Error Resume Next
    Set GetGrid = grd
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
    On Error Resume Next
    ClearGrid
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub imgSDHelp_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case IDX_TICKER
        ShowHelpID 101
    Case IDX_STOCK_PRICE
        ShowHelpID 104
    Case IDX_STOCK_HV
        ShowHelpID 106
    Case IDX_STOCK_IV
        ShowHelpID 107
    Case 4
        ShowHelpID 312
    Case 5
        ShowHelpID 313
    Case Else
        Debug.Assert False
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ShowHelpTipsChanged()
    On Error Resume Next
    ShowHelpTags
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AdjustHelpPictures()
    On Error Resume Next
    Dim i As Long
    With grd
        For i = 0 To MINT_COLUMN_COUNT - 1
            If Not m_cdColumns(i).imgHelp Is Nothing Then
                m_cdColumns(i).imgHelp.Left = .ColPos(.ColIndex(i)) + .ColWidth(.ColIndex(i)) - m_cdColumns(i).imgHelp.Width - ScaleX(.GridLineWidth, vbPixels, vbTwips)
            End If
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long)
    On Error Resume Next
    If OldLeftCol <> NewLeftCol Then AdjustHelpPictures
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    With grd
        Dim lIdx As Long
        lIdx = .ColKey(Position)
'        If lIdx <> SD_IDX_TICKER Then
            m_cdColumns(lIdx).lIndex = Position
'        End If
    End With
    AdjustHelpPictures
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    AdjustHelpPictures
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyToClipboard(ByVal All As Boolean) As Boolean
    On Error GoTo Herr
    If MultiSelection(grd) Then
        CopyToClipboard = CopyGridToClipboard(grd, All)
    Else
        CopyToClipboard = CopyGridToClipboard(grd, True)
    End If
    Exit Function
Herr:
    ShowError "Copy the grid content to the clipboard failed due to the following error:"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_GotFocus()
    On Error Resume Next
    Debug.Print "grd_GotFocus"
    Set g_grd = grd
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_LostFocus()
    On Error Resume Next
    Debug.Print "grd_LostFocus"
    Set g_grd = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_BeforeSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    grd.ExplorerBar = flexExNone
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    ColorRows grd
    grd.ExplorerBar = flexExSortShow
End Sub

