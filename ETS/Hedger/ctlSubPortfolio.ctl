VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Begin VB.UserControl ctlSubPortfolio 
   Appearance      =   0  'Flat
   BackColor       =   &H00C8D0D4&
   BorderStyle     =   1  'Fixed Single
   ClientHeight    =   4005
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   6690
   ScaleHeight     =   4005
   ScaleWidth      =   6690
   Begin VSFlex7Ctl.VSFlexGrid grdPortfolioHedge 
      Height          =   2235
      Left            =   0
      TabIndex        =   0
      Top             =   360
      Width           =   5775
      _cx             =   10186
      _cy             =   3942
      _ConvInfo       =   1
      Appearance      =   2
      BorderStyle     =   0
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
      MouseIcon       =   "ctlSubPortfolio.ctx":0000
      BackColor       =   -2147483643
      ForeColor       =   -2147483640
      BackColorFixed  =   13160660
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   8421504
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   3
      HighLight       =   2
      AllowSelection  =   0   'False
      AllowBigSelection=   0   'False
      AllowUserResizing=   0
      SelectionMode   =   0
      GridLines       =   1
      GridLinesFixed  =   1
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
      ExplorerBar     =   7
      PicturesOver    =   0   'False
      FillStyle       =   0
      RightToLeft     =   0   'False
      PictureType     =   0
      TabBehavior     =   1
      OwnerDraw       =   0
      Editable        =   0
      ShowComboButton =   -1  'True
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
   Begin EgarHedger.ctlSubPortfolioSummary ctlSubPortfolioSummary1 
      Align           =   1  'Align Top
      Height          =   255
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   6690
      _ExtentX        =   11800
      _ExtentY        =   450
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<Context>"
      Begin VB.Menu mnuColumns 
         Caption         =   "Columns"
      End
      Begin VB.Menu mnuBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHide 
         Caption         =   "Hide column"
      End
      Begin VB.Menu mnuBar1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuInsertRow 
         Caption         =   "Insert Row"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuDeleteRow 
         Caption         =   "Delete Row"
         Visible         =   0   'False
      End
   End
End
Attribute VB_Name = "ctlSubPortfolio"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Editable As Boolean
Public IndexSymbolFilter As String
Public rsData As ADODB.Recordset

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Event StockHedgingIndexChanged(ByVal RowID As Variant, HedgingIndexSymbol As String, StockBeta As Double, StockCorr As Double)
Public Event StockHedgingIndicesListAdvise(ByVal StockSymbol As String, HedgingIndexSymbolList As String)
Public Event StockMoved(ByVal RowID As Variant, ByVal StockSymbol As String, ByVal PrevHedgingIndexSymbol As String, ByVal NewHedgingIndexSymbol As String, SelectedRow As Long)

Public fNetPositions As Boolean

Private Enum ColIndexEnum
    IDX_OUTLINE = 0
    IDX_TICKER
    IDX_DELTA
    IDX_STOCK_PRICE
    IDX_HEDGING_INDEX
    IDX_CORR
    IDX_BETA
    MINT_COLUMN_COUNT
End Enum

Private SortColumn As ColIndexEnum
Private SortReverse As Boolean

Private Const Border = 60

Private Const TXT_COLOR_HOVER_NORM = &H6633&    '&H9966&
Private Const TXT_COLOR_HOVER_WARN = vbRed
Private Const BKG_COLOR_CHILD_NORM = &HC0FFFF
Private Const BKG_COLOR_CHILD_WARN = &HC0C0FF
Private Const BKG_COLOR_ROW_CELECTED = &HFFFF&

Private m_cdColumns(0 To MINT_COLUMN_COUNT - 1) As New clsColumnDefinition

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    With grdPortfolioHedge
        .Left = 0
        .Top = ctlSubPortfolioSummary1.Height
    End With
    Editable = False
    LoadSettings
    FormatGrid
    ApplyGridLayout
    fNetPositions = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
    With grdPortfolioHedge
        .Width = Max(0, ScaleWidth)
        .Height = Max(0, ScaleHeight - ctlSubPortfolioSummary1.Height)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadSettings()
Dim i As Long
    
    For i = 0 To MINT_COLUMN_COUNT - 1
        m_cdColumns(i).bVisible = True
        m_cdColumns(i).lIndex = i
        m_cdColumns(i).bCanChangeOrder = False
        m_cdColumns(i).bCanEdit = False
    Next
    
    i = IDX_OUTLINE
    m_cdColumns(i).sName = "Outline"
    m_cdColumns(i).sCaption = " "
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTEmpty
    m_cdColumns(i).sFormat = ""
    m_cdColumns(i).bCanChangeOrder = False
    
    i = IDX_TICKER
    m_cdColumns(i).sName = "Security"
    m_cdColumns(i).sCaption = "Underlying" & vbCrLf & "Symbol"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDispatch
    m_cdColumns(i).sFormat = ""
    
    i = IDX_DELTA
    m_cdColumns(i).sName = "Delta"
    m_cdColumns(i).sCaption = "Delta"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#,###.##"
    
    i = IDX_STOCK_PRICE
    m_cdColumns(i).sName = "StockPrice"
    m_cdColumns(i).sCaption = "Stock" & vbCrLf & "Price"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#,###.##"
    
    i = IDX_HEDGING_INDEX
    m_cdColumns(i).sName = "HedgingIndex"
    m_cdColumns(i).sCaption = "Hedging" & vbCrLf & "Index"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTString
    m_cdColumns(i).sFormat = ""
    m_cdColumns(i).bCanEdit = False
    
    i = IDX_BETA
    m_cdColumns(i).sName = "Beta"
    m_cdColumns(i).sCaption = "Beta"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"
    
    i = IDX_CORR
    m_cdColumns(i).sName = "Corr"
    m_cdColumns(i).sCaption = "Corr"
    m_cdColumns(i).sCaption2 = m_cdColumns(i).sCaption
    m_cdColumns(i).MergeCol = True
    m_cdColumns(i).vsDataType = flexDTDouble
    m_cdColumns(i).sFormat = "#.##"

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FormatGrid()
Dim i As Long
Dim sComboList As String
    sComboList = "SPX|NDX|OEX|DJX|"
    With grdPortfolioHedge
        .Rows = 3
        .Cols = MINT_COLUMN_COUNT
        .FixedRows = 2
        .FixedCols = 0
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .OutlineBar = flexOutlineBarSymbolsLeaf ' flexOutlineBarNone
        .ExplorerBar = flexExSortShowAndMove
        For i = 0 To MINT_COLUMN_COUNT - 1
            .ColHidden(i) = Not m_cdColumns(i).bVisible
            .TextMatrix(0, i) = m_cdColumns(i).sCaption
            .TextMatrix(1, i) = m_cdColumns(i).sCaption2
            .MergeCol(i) = m_cdColumns(i).MergeCol
            .ColKey(i) = i
            .ColDataType(i) = m_cdColumns(i).vsDataType
            .ColFormat(i) = m_cdColumns(i).sFormat
        Next
        .ColWidth(IDX_OUTLINE) = 200
        .Cell(flexcpAlignment, 0, 0, 1, .Cols - 1) = flexAlignCenterCenter
'        .Cell(flexcpBackColor, 0, 0, 1, .Cols - 1) = GCOLOR_TABLE_CAPTION_BACK
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 1, .Cols - 1) = False
        .Editable = flexEDKbdMouse
        .AllowUserResizing = flexResizeColumns
        .AutoResize = True
        '.OLEDragMode = flexOLEDragAutomatic
        '.OLEDropMode = flexOLEDropAutomatic
        .OLEDragMode = flexOLEDragManual
        .OLEDropMode = flexOLEDropManual
    End With
    
    SortColumn = IDX_TICKER
    SortReverse = False
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ApplyGridLayout()
Dim i As Long
Dim lIdx As Long
    With grdPortfolioHedge
        For i = 0 To MINT_COLUMN_COUNT - 1
            lIdx = .ColIndex(i)
            .ColHidden(lIdx) = Not m_cdColumns(i).bVisible
            .ColPosition(lIdx) = m_cdColumns(i).lIndex
        Next
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset, Optional ByVal IndexSymbol As String, Optional ByVal SelectedStockSymbol As String, Optional SelectedRow As Long)
Dim lRow As Long
Dim lRowCounter As Long
Dim StockSymbol As String
Dim Delta As Double
Dim sComboList As String
Dim Criteria As String
Dim d1 As Double
    d1 = 0
    Editable = False
    Set rsData = rs
    IndexSymbolFilter = IndexSymbol
    'grdPortfolioHedge.Redraw = flexRDNone
    ClearGrid
    ctlSubPortfolioSummary1.SetCaption IndexSymbol
    
    With rs
        If Len(IndexSymbol) > 0 Then
            If IndexSymbol = "NONE" Then
                .Filter = "HedgingIndexSymbol=null"
            Else
                .Filter = "HedgingIndexSymbol='" & IndexSymbolFilter & "'"
            End If
        End If
        If .RecordCount > 0 Then
            
            Select Case SortColumn
            Case IDX_TICKER
                Criteria = "StockSymbol"
            Case IDX_DELTA
                Criteria = "Delta"
            Case IDX_STOCK_PRICE
                Criteria = "StockPrice"
            Case IDX_HEDGING_INDEX
                Criteria = "HedgingIndexSymbol"
            Case IDX_BETA
                Criteria = "Beta"
            Case IDX_CORR
                Criteria = "Corr"
            Case Else
                Criteria = "StockSymbol"
            End Select
            If SortReverse Then
                Criteria = Criteria & " desc"
            End If
            
            .Sort = Criteria
            .MoveFirst
            lRowCounter = 0
            Do While Not .EOF
                StockSymbol = !StockSymbol
                Delta = !Delta
                
                lRow = AddRow
'                With grdPortfolioHedge
'                    If lRowCounter Mod 2 = 0 Then
'                        .Cell(flexcpBackColor, AddRow, 0, AddRow, .Cols - 1) = &HEEEEEE
'                    Else
'                        .Cell(flexcpBackColor, AddRow, 0, AddRow, .Cols - 1) = &HFFFFFF
'                    End If
'                End With
                lRowCounter = lRowCounter + 1
                
                grdPortfolioHedge.IsSubtotal(lRow) = True
                grdPortfolioHedge.RowOutlineLevel(lRow) = 0

                d1 = d1 + 1
                
                FillCell lRow, IDX_TICKER, StockSymbol
                If Len(SelectedStockSymbol) > 0 Then
                    If SelectedStockSymbol = StockSymbol Then
                        SelectedRow = lRow
                        grdPortfolioHedge.ShowCell SelectedRow, 1
                        grdPortfolioHedge.Cell(flexcpBackColor, SelectedRow, 0, SelectedRow, grdPortfolioHedge.Cols - 1) = BKG_COLOR_ROW_CELECTED
                    End If
                End If
                grdPortfolioHedge.Cell(flexcpData, lRow, IDX_TICKER, lRow, IDX_TICKER) = .Bookmark
                FillCell lRow, IDX_DELTA, Delta
                FillCell lRow, IDX_STOCK_PRICE, !StockPrice
                
                RaiseEvent StockHedgingIndicesListAdvise(!StockSymbol, sComboList)
                grdPortfolioHedge.Cell(flexcpData, lRow, IDX_HEDGING_INDEX, lRow, IDX_HEDGING_INDEX) = sComboList
                FillCell lRow, IDX_HEDGING_INDEX, !HedgingIndexSymbol
                FillCell lRow, IDX_BETA, !Beta
                FillCell lRow, IDX_CORR, !Corr
                If !Corr < PortfolioHedge.CorrelationBarrier Then
                    With grdPortfolioHedge
                        .Cell(flexcpBackColor, lRow, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_WARN
                        .Cell(flexcpBackColor, lRow, IDX_BETA) = BKG_COLOR_CHILD_WARN
                        .Cell(flexcpBackColor, lRow, IDX_CORR) = BKG_COLOR_CHILD_WARN
                    End With
                End If
                'Subrows
                Dim lSubRow As Long
                Dim Beta As Double
                Dim Corr As Double
                With PortfolioHedge.rsStockData.Clone
                    .Filter = "StockSymbol='" & StockSymbol & "'"
                    If .RecordCount > 0 Then
                        .Sort = "Corr desc"
                        .MoveFirst
                        Do While Not .EOF
                            IndexSymbol = !IndexSymbol
                            If IndexSymbol <> IndexSymbolFilter Then
                                Beta = !Beta
                                Corr = !Corr
                                With PortfolioHedge.rsIndexOptions.Clone
                                    If .RecordCount > 0 Then
                                        .MoveFirst
                                    End If
                                    .Find "IndexSymbol='" & IndexSymbol & "'", 0, adSearchForward, adBookmarkFirst
                                    If Not .EOF Then
                                        If !Selected Then
                                            lSubRow = AddRow
                                            
                                            With grdPortfolioHedge
                                                .IsSubtotal(lSubRow) = True
                                                .RowOutlineLevel(lSubRow) = 1
                                                .CellBorder .BackColorBkg, 0, 0, 1, 1, 0, 0
                                                .Cell(flexcpBackColor, lSubRow, IDX_OUTLINE) = .BackColorBkg
                                                .Cell(flexcpBackColor, lSubRow, IDX_TICKER) = .BackColorBkg
                                                .Cell(flexcpBackColor, lSubRow, IDX_DELTA) = .BackColorBkg
                                                .Cell(flexcpBackColor, lSubRow, IDX_STOCK_PRICE) = .BackColorBkg
                                            End With
                                            
                                            FillCell lSubRow, IDX_HEDGING_INDEX, IndexSymbol
                                            FillCell lSubRow, IDX_BETA, Beta
                                            FillCell lSubRow, IDX_CORR, Corr
                                            
                                            With grdPortfolioHedge
                                                ' special decoration for Hedging Index Symbol
                                                .Cell(flexcpData, lSubRow, .ColIndex(IDX_HEDGING_INDEX)) = lRow
                                                .Cell(flexcpFontUnderline, lSubRow, .ColIndex(IDX_HEDGING_INDEX)) = True
                                                .Cell(flexcpFontBold, lSubRow, .ColIndex(IDX_HEDGING_INDEX)) = True
                                                If Corr < PortfolioHedge.CorrelationBarrier Then
                                                    .Cell(flexcpForeColor, lSubRow, .ColIndex(IDX_HEDGING_INDEX)) = TXT_COLOR_HOVER_WARN
                                                    .Cell(flexcpBackColor, lSubRow, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_WARN
                                                    .Cell(flexcpBackColor, lSubRow, IDX_BETA) = BKG_COLOR_CHILD_WARN
                                                    .Cell(flexcpBackColor, lSubRow, IDX_CORR) = BKG_COLOR_CHILD_WARN
                                                Else
                                                    .Cell(flexcpForeColor, lSubRow, .ColIndex(IDX_HEDGING_INDEX)) = TXT_COLOR_HOVER_NORM
                                                    .Cell(flexcpBackColor, lSubRow, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_NORM
                                                    .Cell(flexcpBackColor, lSubRow, IDX_BETA) = BKG_COLOR_CHILD_NORM
                                                    .Cell(flexcpBackColor, lSubRow, IDX_CORR) = BKG_COLOR_CHILD_NORM
                                                End If
                                            End With
                                        End If
                                    End If
                                End With
                            End If
                            .MoveNext
                        Loop
                    End If
                End With
                
                grdPortfolioHedge.IsCollapsed(lRow) = flexOutlineCollapsed
                
                .MoveNext
            Loop
        Else
            grdPortfolioHedge.Rows = 3
        End If
        .Filter = ""
    End With
    grdPortfolioHedge.Redraw = True
    
    ' Update summary values
Dim StockCount As Long
Dim EquivalentIndexPosition As Double
Dim NonHedgedDeviation As Double
Dim HedgedDeviation As Double
Dim Correlation As Double
    
    PortfolioHedge.CalculateIndexPosition _
        StockCount, EquivalentIndexPosition, IndexSymbolFilter
        
    PortfolioHedge.CalculateIndexVariations _
        NonHedgedDeviation, HedgedDeviation, Correlation, IndexSymbolFilter
        
    PortfolioHedge.SetIndexVariation _
        NonHedgedDeviation, HedgedDeviation, Correlation, IndexSymbolFilter
    
    ctlSubPortfolioSummary1.SetTotals EquivalentIndexPosition, NonHedgedDeviation, HedgedDeviation, Correlation
    
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateSummary()
Dim StockCount As Long
Dim EquivalentIndexPosition As Double
Dim NonHedgedDeviation As Double
Dim HedgedDeviation As Double
Dim Correlation As Double
    
    StockCount = 0
    EquivalentIndexPosition = 0
    NonHedgedDeviation = 0
    HedgedDeviation = 0
    Correlation = 0
    
    If IndexSymbolFilter <> "" Then
        PortfolioHedge.CalculateIndexPosition _
            StockCount, EquivalentIndexPosition, IndexSymbolFilter
            
        If IndexSymbolFilter <> "NONE" Then
            PortfolioHedge.CalculateIndexVariations _
                NonHedgedDeviation, HedgedDeviation, Correlation, IndexSymbolFilter
                
            PortfolioHedge.SetIndexVariation _
                NonHedgedDeviation, HedgedDeviation, Correlation, IndexSymbolFilter
        End If
    
    End If
    
    ctlSubPortfolioSummary1.SetTotals EquivalentIndexPosition, NonHedgedDeviation, HedgedDeviation, Correlation
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateTotals(rs As ADODB.Recordset)
Dim StockSymbol As String
Dim Delta As Double
Dim d1 As Double
Dim d2 As Double
Dim d3 As Double
Dim d4 As Double
    d1 = 0
    d2 = 0
    d3 = 0
    d4 = 0
    With rs.Clone
        If Len(IndexSymbolFilter) > 0 Then
            If IndexSymbolFilter = "NONE" Then
                .Filter = "HedgingIndexSymbol=null"
            Else
                .Filter = "HedgingIndexSymbol='" & IndexSymbolFilter & "'"
            End If
        End If
        If .RecordCount > 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            StockSymbol = ""
            Do While Not .EOF
                If StockSymbol <> !StockSymbol Then
                    StockSymbol = !StockSymbol
                    Delta = 0
                    With rs.Clone
                        .Filter = "StockSymbol='" & StockSymbol & "'"
                        If .RecordCount > 0 Then
                            .MoveFirst
                            Do While Not .EOF
                                If Not IsNull(!Delta) Then
                                    Delta = Delta + !Delta
                                End If
                                .MoveNext
                            Loop
                        End If
                    End With
                    d1 = d1 + 1
                    
                End If
                .MoveNext
            Loop
        Else
            '
        End If
    End With
    
    '!!!
    'd1 = 0
    d2 = 0
    d3 = 0
    d4 = 0
    
    ctlSubPortfolioSummary1.SetTotals d1, d2, d3, d4

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ScanStocksRS(rs As ADODB.Recordset)
Dim lRow As Long
    ClearGrid
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                lRow = AddRow
                FillCell lRow, IDX_TICKER, !StockSymbol
                FillCell lRow, IDX_DELTA, !Delta
                FillCell lRow, IDX_STOCK_PRICE, !StockPrice
                FillCell lRow, IDX_HEDGING_INDEX, !HedgingIndexSymbol
                FillCell lRow, IDX_BETA, !Beta
                FillCell lRow, IDX_CORR, !Corr
                .MoveNext
            Loop
        Else
            grdPortfolioHedge.Rows = 3
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearGrid()
    With grdPortfolioHedge
        .Rows = 2
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function AddRow() As Long
    With grdPortfolioHedge
        .Rows = .Rows + 1
        AddRow = .Rows - 1
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FillCell(ByVal lRow As Long, ByVal iColl As ColIndexEnum, ByVal Value As Variant, Optional ByVal NullValue As String = "")
Dim lCol As Long
    With grdPortfolioHedge
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
Private Sub grdPortfolioHedge_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    With grdPortfolioHedge
        If Button = 2 And .MouseRow > -1 And .MouseCol > -1 Then
            .Col = .MouseCol
            .Row = .MouseRow
            PopupMenu mnuContext ', , X, Y
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuColumns_Click()
Dim i As Long
    With grdPortfolioHedge
        For i = 0 To .Cols - 1
            Debug.Print i, .ColKey(i), .ColHidden(i)
            m_cdColumns(.ColKey(i)).lIndex = i
            m_cdColumns(.ColKey(i)).bVisible = Not .ColHidden(i)
        Next
    End With
    If frmMLColsManager.Execute(MINT_COLUMN_COUNT, m_cdColumns) Then
        ApplyGridLayout
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuHide_Click()
    With grdPortfolioHedge
        If .Row > -1 Then
            If .Col > -1 Then
                Debug.Print "Hide", .Col
                'm_cdColumns(.ColKey(.Col)).bVisible = False
                .ColHidden(.Col) = True
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim sComboList As String
Dim StockSymbol As String
Dim lCol As Long
    On Error Resume Next
    grdPortfolioHedge.ComboList = ""
    lCol = grdPortfolioHedge.ColKey(Col)
    Cancel = Not (Editable And m_cdColumns(lCol).bCanEdit)
    If Not Cancel Then
        If lCol = IDX_HEDGING_INDEX Then
            StockSymbol = grdPortfolioHedge.TextMatrix(Row, IDX_TICKER)
            sComboList = grdPortfolioHedge.Cell(flexcpData, Row, lCol)
            'sComboList = "SPX|NDX|OEX|DJX|"
            RaiseEvent StockHedgingIndicesListAdvise(StockSymbol, sComboList)
            grdPortfolioHedge.ComboList = sComboList
            Debug.Print sComboList
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_CellChanged(ByVal Row As Long, ByVal Col As Long)
Dim StockSymbol As String
Dim HedgingIndexSymbol As String
Dim StockBeta As Double
Dim StockCorr As Double
Dim RowID As Variant
Dim lCol As Long
Dim lRow As Long
Dim lSubRow As Long
    On Error Resume Next
    lRow = grdPortfolioHedge.Row
    If Editable And lRow >= 2 Then
        lCol = grdPortfolioHedge.ColKey(grdPortfolioHedge.Col)
        StockSymbol = grdPortfolioHedge.TextMatrix(lRow, grdPortfolioHedge.ColIndex(IDX_TICKER))
        RowID = grdPortfolioHedge.Cell(flexcpData, lRow, grdPortfolioHedge.ColIndex(IDX_TICKER))
        If IsEmpty(RowID) Then
            Exit Sub
        End If
        Select Case lCol
        Case IDX_HEDGING_INDEX
            HedgingIndexSymbol = grdPortfolioHedge.TextMatrix(lRow, lCol)
            RaiseEvent StockHedgingIndexChanged(RowID, HedgingIndexSymbol, StockBeta, StockCorr)
            DoEvents
            If IndexSymbolFilter = "" Or HedgingIndexSymbol = IndexSymbolFilter Then
                FillCell lRow, IDX_HEDGING_INDEX, HedgingIndexSymbol
                FillCell lRow, IDX_BETA, StockBeta
                FillCell lRow, IDX_CORR, StockCorr
            Else
                grdPortfolioHedge.RemoveItem lRow
                ' Also remove subrows
                lSubRow = lRow
                Do While lSubRow < grdPortfolioHedge.Rows
                    If grdPortfolioHedge.RowOutlineLevel(lSubRow) > 0 Then
                        grdPortfolioHedge.RemoveItem lSubRow
                    Else
                        Exit Do
                    End If
                Loop
            End If
        End Select
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
Dim StockSymbol As String
    With grdPortfolioHedge
        If .MouseRow >= 2 And .MouseCol = .ColIndex(IDX_HEDGING_INDEX) Then
            StockSymbol = .TextMatrix(.MouseRow, .ColIndex(IDX_TICKER))
            If StockSymbol = "" Then
                .MousePointer = flexCustom
                Exit Sub
            End If
        End If
        .MousePointer = flexDefault
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_AfterSort(ByVal Col As Long, Order As Integer)
    With grdPortfolioHedge
        SortColumn = .ColKey(Col)
        SortReverse = Order <> 1
    End With
    '////////////////////////////////////////////
    DisplayOutputRS rsData, IndexSymbolFilter '/
    '//////////////////////////////////////////
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_Click()
Dim StockSymbol As String
Dim NewIndexSymbol As String
Dim PrevIndexSymbol As String
Dim RowID As Variant
Dim lRow As Long
Dim SelectedRow As Long
    With grdPortfolioHedge
        If .MouseRow < 2 Then
        Else
            If .MouseCol = .ColIndex(IDX_HEDGING_INDEX) Then
                .Row = .MouseRow
                .Col = .MouseCol
                StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
                If StockSymbol = "" Then
                    ' saved parent stock row
                    lRow = .Cell(flexcpData, .Row, .ColIndex(IDX_HEDGING_INDEX))
                    If lRow > 0 Then
                        StockSymbol = .TextMatrix(lRow, .ColIndex(IDX_TICKER))
                        If Len(StockSymbol) > 0 Then
                            NewIndexSymbol = .TextMatrix(.Row, .ColIndex(IDX_HEDGING_INDEX))
                            ' do move
                            PrevIndexSymbol = IndexSymbolFilter
                            RowID = .Cell(flexcpData, lRow, .ColIndex(IDX_TICKER))
                            If PrevIndexSymbol <> NewIndexSymbol Then
                                RaiseEvent StockMoved(RowID, StockSymbol, PrevIndexSymbol, NewIndexSymbol, SelectedRow)
                            End If
    '                        If SelectedRow > 1 Then
    '                            .ShowCell SelectedRow, 1
    '                            .Cell(flexcpBackColor, SelectedRow, 0, SelectedRow, .Cols - 1) = BKG_COLOR_ROW_CELECTED
    '                        End If
                        End If
                    End If
                End If
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal Y As Single, Cancel As Boolean)
Dim StockSymbol As String
    With grdPortfolioHedge
        If .MouseRow < 2 Then
'            Debug.Assert False
        Else
            If .MouseCol >= 1 Then
                ' if the click was on the active cell, start dragging
                .Row = .MouseRow
                .Col = .MouseCol
                StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
                If Len(StockSymbol) > 0 Then
                    ' use OLEDrag method to start manual OLE drag operation
                    ' this will fire the OLEStartDrag event, which we will use
                    ' to fill the DataObject with the data we want to drag.
                    .OLEDrag
                    ' tell grid control to ignore mouse movements until the
                    ' mouse button goes up again
                    Cancel = True
                End If
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_OLEStartDrag(Data As VSFlex7Ctl.VSDataObject, AllowedEffects As Long)
Dim StockSymbol As String
Dim RowID As Variant
Dim s As String
    ' set contents of data object for manual drag
    With grdPortfolioHedge
        StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
        RowID = .Cell(flexcpData, .Row, .ColIndex(IDX_TICKER))
        s = "DRAG," & StockSymbol & "," & IndexSymbolFilter & "," & Format$(RowID)
    End With
    Data.SetData s, vbCFText
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grdPortfolioHedge_OLEDragDrop(Data As VSFlex7Ctl.VSDataObject, Effect As Long, ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal Y As Single)
Dim lRow As Long
Dim s As String
Dim StockSymbol As String
Dim PrevIndexSymbol As String
Dim RowID As Variant
Dim SelectedRow As Long
Dim v As Variant
    With grdPortfolioHedge
        ' drop text
        SelectedRow = -1
        If Data.GetFormat(vbCFText) Then
            s = Data.GetData(vbCFText)
            v = Split(s, ",")
            If UBound(v) = 3 Then
                If v(0) = "DRAG" Then
                    StockSymbol = v(1)
                    PrevIndexSymbol = v(2)
                    RowID = Val(v(3))
                    If PrevIndexSymbol <> IndexSymbolFilter Then
                        RaiseEvent StockMoved(RowID, StockSymbol, PrevIndexSymbol, IndexSymbolFilter, SelectedRow)
                    End If
                End If
            End If
'            If SelectedRow > 1 Then
'                .ShowCell SelectedRow, 1
'                .Cell(flexcpBackColor, SelectedRow, 0, SelectedRow, .Cols - 1) = BKG_COLOR_ROW_CELECTED
'            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grdPortfolioHedge
        .Redraw = flexRDNone
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                FillCell Row, IDX_STOCK_PRICE, StockPrice
                If DataStatus = DATA_FEED Then
                    .Cell(flexcpBackColor, Row, IDX_STOCK_PRICE) = BKG_COLOR_PRICE_FEED
                Else
                    .Cell(flexcpBackColor, Row, IDX_STOCK_PRICE) = BKG_COLOR_PRICE_MANUAL
                End If
            End If
        Next
        .Redraw = True
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockIndexBetaCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal Beta As Double, ByVal DateSet As Date, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
Dim SubRow As Long
    Editable = False
    With grdPortfolioHedge
        .Redraw = flexRDNone
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                If IndexSymbol = .TextMatrix(Row, .ColIndex(IDX_HEDGING_INDEX)) Then
                    FillCell Row, IDX_BETA, Beta
                    FillCell Row, IDX_CORR, Corr
                    If Corr < PortfolioHedge.CorrelationBarrier Then
                        .Cell(flexcpBackColor, Row, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_WARN
                    Else
                        .Cell(flexcpBackColor, Row, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_NORM
                    End If
                    If DataStatus = DATA_FEED Then
                        .Cell(flexcpBackColor, Row, IDX_BETA) = BKG_COLOR_PRICE_FEED
                        .Cell(flexcpBackColor, Row, IDX_CORR) = BKG_COLOR_PRICE_FEED
                    Else
                        .Cell(flexcpBackColor, Row, IDX_BETA) = BKG_COLOR_PRICE_MANUAL
                        .Cell(flexcpBackColor, Row, IDX_CORR) = BKG_COLOR_PRICE_MANUAL
                    End If
                Else
                    SubRow = Row + 1
                    Do While SubRow < .Rows
                        If "" <> .TextMatrix(SubRow, .ColIndex(IDX_TICKER)) Then
                            Exit Do
                        End If
                        If IndexSymbol = .TextMatrix(SubRow, .ColIndex(IDX_HEDGING_INDEX)) Then
                            FillCell SubRow, IDX_BETA, Beta
                            FillCell SubRow, IDX_CORR, Corr
                            If Corr < PortfolioHedge.CorrelationBarrier Then
                                .Cell(flexcpForeColor, SubRow, .ColIndex(IDX_HEDGING_INDEX)) = TXT_COLOR_HOVER_WARN
                                .Cell(flexcpBackColor, SubRow, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_WARN
                            Else
                                .Cell(flexcpForeColor, SubRow, .ColIndex(IDX_HEDGING_INDEX)) = TXT_COLOR_HOVER_NORM
                                .Cell(flexcpBackColor, SubRow, IDX_HEDGING_INDEX) = BKG_COLOR_CHILD_NORM
                            End If
                            If DataStatus = DATA_FEED Then
                                .Cell(flexcpBackColor, SubRow, IDX_BETA) = BKG_COLOR_PRICE_FEED
                                .Cell(flexcpBackColor, SubRow, IDX_CORR) = BKG_COLOR_PRICE_FEED
                            Else
                                .Cell(flexcpBackColor, SubRow, IDX_BETA) = BKG_COLOR_PRICE_MANUAL
                                .Cell(flexcpBackColor, SubRow, IDX_CORR) = BKG_COLOR_PRICE_MANUAL
                            End If
                        End If
                        SubRow = SubRow + 1
                    Loop
                End If
            End If
        Next
        .Redraw = True
    End With
    Editable = True
End Function

