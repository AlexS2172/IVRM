VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Begin VB.UserControl ctlPortfolioHedge 
   BackColor       =   &H00C8D0D4&
   ClientHeight    =   5250
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8895
   ScaleHeight     =   5250
   ScaleWidth      =   8895
   Begin VSFlex7Ctl.VSFlexGrid grd 
      Height          =   4875
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   8415
      _cx             =   14843
      _cy             =   8599
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
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   5
         Left            =   6480
         MouseIcon       =   "ctlPortfolioHedge.ctx":0000
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":0152
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   4
         Left            =   5640
         MouseIcon       =   "ctlPortfolioHedge.ctx":01B3
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":0305
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   3
         Left            =   4680
         MouseIcon       =   "ctlPortfolioHedge.ctx":0366
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":04B8
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   2
         Left            =   3720
         MouseIcon       =   "ctlPortfolioHedge.ctx":0519
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":066B
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   1
         Left            =   2760
         MouseIcon       =   "ctlPortfolioHedge.ctx":06CC
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":081E
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   0
         Left            =   1800
         MouseIcon       =   "ctlPortfolioHedge.ctx":087F
         MousePointer    =   99  'Custom
         Picture         =   "ctlPortfolioHedge.ctx":09D1
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<Context>"
      Begin VB.Menu mnuColumns 
         Caption         =   "Columns"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuBar0 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuHide 
         Caption         =   "Hide column"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuBar1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuInsertRow 
         Caption         =   "Insert Row"
      End
      Begin VB.Menu mnuDeleteRow 
         Caption         =   "Delete Row"
      End
      Begin VB.Menu mnuBar2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPaste 
         Caption         =   "Paste"
         Visible         =   0   'False
      End
   End
End
Attribute VB_Name = "ctlPortfolioHedge"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Editable As Boolean

Public Event StockSymbolChanged(ByVal RowID As Variant, StockSymbol As String, StockPrice As Double, StockIV As Double)
Public Event StockOptionExpiryChanged(ByVal RowID As Variant, OptionExpiry As Variant)
Public Event StockDeltaChanged(ByVal RowID As Variant, StockDelta As Double)
Public Event StockPriceChanged(ByVal RowID As Variant, StockPrice As Double)
Public Event StockVegaChanged(ByVal RowID As Variant, StockVega As Double)
Public Event StockHVChanged(ByVal RowID As Variant, StockHV As Double)
Public Event StockIVChanged(ByVal RowID As Variant, StockIV As Double)

Public Event DeleteRow(ByVal RowID As Variant)
Public Event InsertRow(RowID As Variant)

Public Event Copy()
Public Event Paste()

Public fNetPositions As Boolean

Private Enum ColIndexEnum
    IDX_TICKER = 0
    IDX_EXPIRY
    IDX_DELTA
    IDX_STOCK_PRICE
    IDX_VEGA
    IDX_STOCK_IV
    MINT_COLUMN_COUNT
End Enum

Private SortColumn As ColIndexEnum
Private SortReverse As Boolean
Private fSilent As Boolean
Private fShowLog As Boolean

Private Const EMPTY_ROWS = 1

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
    If Not PortfolioHedge Is Nothing Then
        SetVegaMode PortfolioHedge.VegaMode, PortfolioHedge.ShortPortfolio
    End If
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
            Set .imgHelp = imgSdHelp(i)
        End With
    Next
    
    i = IDX_TICKER
    With m_cdColumns(i)
        .sName = "Security"
        .sCaption = "Underlying" & vbCrLf & "Symbol"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDispatch
        .sFormat = ""
        .bCanEdit = True
    End With
    
    i = IDX_EXPIRY
    With m_cdColumns(i)
        .sName = "Expiry"
        .sCaption = "Expiry" & vbCrLf & "Date"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDate
        .sFormat = "YYYY-MM-DD"
        .bCanEdit = True
    End With
    
    i = IDX_DELTA
    With m_cdColumns(i)
        .sName = "Delta"
        .sCaption = "Delta"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
        .bCanEdit = True
    End With
    
    i = IDX_STOCK_PRICE
    With m_cdColumns(i)
        .sName = "StockPrice"
        .sCaption = "Stock" & vbCrLf & "Price"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
        .bCanEdit = True
    End With
    
    i = IDX_VEGA
    With m_cdColumns(i)
        .sName = "Vega"
        .sCaption = "Vega"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
        .bCanEdit = True
        .bVisible = False
    End With
    
    i = IDX_STOCK_IV
    With m_cdColumns(i)
        .sName = "StockIV"
        .sCaption = "Stock" & vbCrLf & "IV, %"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
        .bCanEdit = True
        .bVisible = False
    End With
    
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
        '//////////////////////////////////////////
        ' If use own sorting :                  '/
        .ExplorerBar = flexExSort              '/
        SortColumn = IDX_TICKER               '/
        SortReverse = False                  '/
        '/////////////////////////////////////
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
Private Sub grd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
'    With grd
'        SortColumn = .ColKey(Col)
'        SortReverse = Order <> 1
'    End With
'    '/////////////////
'    ReDisplayData  '/
'    '///////////////
    With grd
        Dim i As Long
        For i = .Rows - 1 To 2 Step -1
            If .TextMatrix(i, .ColIndex(IDX_TICKER)) = "" Then
                Dim RowID As Variant
                RowID = .Cell(flexcpData, i, .ColIndex(IDX_TICKER))
                RaiseEvent DeleteRow(RowID)
                .RemoveItem i
            Else
                '
            End If
        Next
        AppendRow
    End With
    ColorRows grd
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ReDisplayData()
    DisplayOutputRS PortfolioHedge.rsPortfolioData, False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DisplayOutputRS(rs As ADODB.Recordset, ByVal Reset As Boolean)
Dim lRow As Long
Dim StockSymbol As String
Dim Delta As Double
Dim Vega As Double
Dim lUnsupported As Long
Dim Criteria As String
    Editable = False
    lUnsupported = 0
    ClearGrid
    
    If Reset Then
        SortColumn = IDX_TICKER
        SortReverse = False
    End If
    
    Select Case SortColumn
    Case IDX_TICKER
        Criteria = "StockSymbol"
    Case IDX_EXPIRY
        Criteria = "Expiration"
    Case IDX_DELTA
        Criteria = "Delta"
    Case IDX_VEGA
        Criteria = "Vega"
    Case IDX_STOCK_PRICE
        Criteria = "StockPrice"
    Case IDX_STOCK_IV
        Criteria = "IV"
    Case Else
        Criteria = "StockSymbol"
    End Select
    If SortReverse Then
        Criteria = Criteria & " desc"
    End If
    
    With rs.Clone
        If .RecordCount <> 0 Then
'            If .RecordCount < 200 Then
                grd.Redraw = flexRDNone
                grd.ExplorerBar = flexExNone
'            End If
            .Sort = Criteria
            .MoveFirst
            Do While Not .EOF
                If Len(!StockSymbol) > 0 Then
                    lRow = AddRow
                    FillCell lRow, IDX_TICKER, !StockSymbol
                    grd.Cell(flexcpData, lRow, IDX_TICKER, lRow, IDX_TICKER) = .Bookmark
                    FillCell lRow, IDX_EXPIRY, !Expiration, "UL"
                    FillCell lRow, IDX_DELTA, !Delta
                    FillCell lRow, IDX_STOCK_PRICE, !StockPrice
                    FillCell lRow, IDX_VEGA, !Vega
                    FillCell lRow, IDX_STOCK_IV, !IV * 100
                    If !Unsupported Then
                        With grd
                            'BKG_COLOR_CHILD_WARN
                            .Cell(flexcpBackColor, lRow, 0, lRow, .Cols - 1) = IIf(lRow Mod 2 > 0, GCOLOR_TABLE_ODD_ROW_RED, GCOLOR_TABLE_EVEN_ROW_RED)
                        End With
                        If lUnsupported = 0 Then
                            lUnsupported = lRow
                        End If
                    End If
                End If
                .MoveNext
                If (Count Mod 100) = 0 Then
                    grd.Redraw = True
                    DoEvents
                    grd.Redraw = flexRDNone
                End If
            Loop
        End If
    End With
    If EMPTY_ROWS > 0 Then
        For lRow = 1 To EMPTY_ROWS
            InsertRow
        Next
    End If
    If lUnsupported <> 0 Then
        With grd
            .ShowCell lUnsupported, .ColIndex(IDX_TICKER)
        End With
    End If
    grd.Redraw = True
    AdjustHelpPictures
    Editable = True
    ColorRows grd
    grd.ExplorerBar = flexExSort
    fShowLog = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ScanStocksRS(rs As ADODB.Recordset)
Dim lRow As Long
    ClearGrid
    With rs
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                lRow = AddRow
                FillCell lRow, IDX_TICKER, !StockSymbol
                FillCell lRow, IDX_EXPIRY, !Expiration, "UL"
                FillCell lRow, IDX_DELTA, !Delta
                FillCell lRow, IDX_DELTA, !Vega
                If Not IsNull(!StockSymbol) Then
                    FillCell lRow, IDX_STOCK_PRICE, !StockPrice
                    FillCell lRow, IDX_STOCK_IV, !IV * 100
                End If
                .MoveNext
'                DoEvents
            Loop
        Else
            grd.Rows = 3
        End If
        If EMPTY_ROWS > 0 Then
            For lRow = 1 To EMPTY_ROWS
                InsertRow
            Next
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ClearGrid()
    With grd
        .Rows = 2
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function AddRow() As Long
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
Private Sub grd_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With grd
        If Button = 2 And .MouseRow > 1 And .MouseCol > -1 Then
            If Not MultiSelection(grd) Then
                .Col = .MouseCol
                .Row = .MouseRow
                If .Col = .MouseCol Then
                    If .Row = .MouseRow Then
                        PopupMenu mnuContext ', , X, Y
                    End If
                End If
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuColumns_Click()
Dim i As Long
    On Error Resume Next
    With grd
        For i = 0 To .Cols - 1
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
    On Error Resume Next
    With grd
        If .Row > -1 Then
            If .Col > -1 Then
                'm_cdColumns(.ColKey(.Col)).bVisible = False
                .ColHidden(.Col) = True
            End If
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
    Cancel = Not ((Not fNetPositions) And Editable And m_cdColumns(lCol).bCanEdit)
    If Not Cancel Then
    
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_CellChanged(ByVal Row As Long, ByVal Col As Long)
Dim StockSymbol As String
Dim OptionExpiry As Variant
Dim NewDate As Date
Dim StockDelta As Double
Dim StockPrice As Double
Dim StockVega As Double
Dim StockIV As Double
Dim RowID As Variant
Dim lCol As Long
    On Error Resume Next
    If fNetPositions Then
        Exit Sub
    End If
    If Not Editable Then
        Exit Sub
    End If
    If Row < 2 Then
        Exit Sub
    End If
    With grd
        lCol = .ColKey(Col)
        StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER))
'        If Len(StockSymbol) = 0 Then
'            Exit Sub
'        End If
        RowID = .Cell(flexcpData, Row, .ColIndex(IDX_TICKER))
'        If IsEmpty(RowID) Then
'            Exit Sub
'        End If
        Select Case lCol
        Case IDX_TICKER
            StockSymbol = .TextMatrix(Row, Col)
            RaiseEvent StockSymbolChanged(RowID, StockSymbol, StockPrice, StockIV)
            FillCell Row, IDX_TICKER, StockSymbol
            FillCell Row, IDX_STOCK_PRICE, StockPrice
            FillCell Row, IDX_STOCK_IV, StockIV * 100
'            ColorCell grd, Row, IDX_STOCK_PRICE, DATA_NORMAL
'            ColorCell grd, Row, IDX_STOCK_IV, DATA_NORMAL
'            .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_PRICE)) = BKG_COLOR_PRICE_NORMAL
'            .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_IV)) = BKG_COLOR_PRICE_NORMAL
            If Not PortfolioHedge.CheckStockSymbol(StockSymbol) Then
'                gCmn.MyMsgBox "This Symbol is not found in the list of supported stocks.", vbCritical
                PortfolioHedge.LogSave "Unsupported Symbol '" & StockSymbol & "'"
                If Not fShowLog Then
                    fShowLog = True
                    frmDataLog.ShowHistory
                End If
                With grd
                    'BKG_COLOR_CHILD_WARN
                    .Cell(flexcpBackColor, Row, 0, Row, .Cols - 1) = IIf(Row Mod 2 > 0, GCOLOR_TABLE_ODD_ROW_RED, GCOLOR_TABLE_EVEN_ROW_RED)
                    .ShowCell Row, .ColIndex(IDX_TICKER)
                End With
            Else
                With grd
                    .Cell(flexcpBackColor, Row, 0, Row, .Cols - 1) = IIf(Row Mod 2 > 0, GCOLOR_TABLE_ODD_ROW_GRAY, GCOLOR_TABLE_EVEN_ROW_GRAY)
                End With
            End If
        Case IDX_EXPIRY
            OptionExpiry = .TextMatrix(Row, Col)
            If UCase(OptionExpiry) = "UL" Or OptionExpiry = "" Then
                OptionExpiry = Null
            Else
                If ScanDate(OptionExpiry, NewDate) Then
                    OptionExpiry = NewDate
                End If
            End If
            RaiseEvent StockOptionExpiryChanged(RowID, OptionExpiry)
            FillCell Row, IDX_EXPIRY, OptionExpiry, "UL"
        Case IDX_DELTA
            StockDelta = ScanDouble(.TextMatrix(Row, Col))
            RaiseEvent StockDeltaChanged(RowID, StockDelta)
            FillCell Row, IDX_DELTA, StockDelta
        Case IDX_STOCK_PRICE
            StockPrice = ScanDouble(.TextMatrix(Row, Col))
            If Len(StockSymbol) > 0 Then
                RaiseEvent StockPriceChanged(RowID, StockPrice)
            Else
                FillCell Row, IDX_STOCK_PRICE, StockPrice
            End If
        Case IDX_VEGA
            StockVega = ScanDouble(.TextMatrix(Row, Col))
            RaiseEvent StockVegaChanged(RowID, StockVega)
            FillCell Row, IDX_VEGA, StockVega
        Case IDX_STOCK_IV
            StockIV = ScanDouble(.TextMatrix(Row, Col)) / 100
            If Len(StockSymbol) > 0 Then
                RaiseEvent StockIVChanged(RowID, StockIV)
            Else
                FillCell Row, IDX_STOCK_IV, StockIV
            End If
        End Select
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuDeleteRow_Click()
Dim RowID As Variant
Dim lRow As Long
    On Error Resume Next
    With grd
        lRow = .Row
        If (Not fNetPositions) And Editable And lRow >= 2 Then
            If Not MultiSelection(grd) Then
                RowID = .Cell(flexcpData, lRow, .ColIndex(IDX_TICKER))
                RaiseEvent DeleteRow(RowID)
                .RemoveItem lRow
            End If
        End If
    End With
    ColorRows grd
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuInsertRow_Click()
Dim RowID As Variant
Dim lRow As Long
    On Error Resume Next
    With grd
        If (Not fNetPositions) And Editable Then
            If Not MultiSelection(grd) Then
                lRow = .Row
                If lRow > 1 Then
                    InsertRow lRow '+ 1
                End If
            End If
        End If
    End With
    PortfolioHedge.GetExpiry
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InsertRow(Optional ByVal lRow As Long = 0)
Dim RowID As Variant
    On Error Resume Next
    With grd
        RaiseEvent InsertRow(RowID)
        If lRow > 1 Then
            .AddItem "", lRow
        Else
            lRow = .Rows
            .AddItem "", lRow
        End If
        .Cell(flexcpData, lRow, .ColIndex(IDX_TICKER)) = RowID
    End With
    ColorRows grd
End Sub
    
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub AppendRow()
Dim RowID As Variant
Dim lRow As Long
    On Error Resume Next
    With grd
        RaiseEvent InsertRow(RowID)
        lRow = .Rows
        .AddItem "", lRow
        .Cell(flexcpData, lRow, .ColIndex(IDX_TICKER)) = RowID
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyPress(KeyAscii As Integer)
    On Error Resume Next
    If KeyAscii = vbMenuAccelCtrlC Then
        mnuCopy_Click
    ElseIf KeyAscii = vbMenuAccelCtrlV Then
        mnuPaste_Click
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyPressEdit(ByVal Row As Long, ByVal Col As Long, KeyAscii As Integer)
    On Error Resume Next
    Select Case KeyAscii
    Case vbMenuAccelCtrlC
    Case vbMenuAccelCtrlV
    Case Else
        Select Case grd.ColKey(Col)
        Case IDX_DELTA, IDX_STOCK_PRICE, IDX_VEGA, IDX_STOCK_IV
            If Not IsNumericKey(KeyAscii) Then
                KeyAscii = 0
            End If
        End Select
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyDown(KeyCode As Integer, Shift As Integer)
Static fReenter As Boolean
    On Error Resume Next
    If fReenter Then
        Exit Sub
    End If
    fReenter = True
'    If KeyCode = vbKeyDelete And (Shift And vbShiftMask) > 0 And mnuDeleteRow.Enabled Then
    If KeyCode = vbKeyDelete And mnuDeleteRow.Enabled Then
        mnuDeleteRow_Click
'    ElseIf KeyCode = vbKeyInsert And (Shift And vbShiftMask) > 0 And mnuInsertRow.Enabled Then
    ElseIf KeyCode = vbKeyInsert And mnuInsertRow.Enabled Then
        mnuInsertRow_Click
    ElseIf KeyCode = vbKeyDown And mnuInsertRow.Enabled Then
        If grd.Row = grd.Rows - 1 Then
            InsertRow
'            ColorRows grd
        End If
    End If
    fReenter = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuCopy_Click()
    On Error Resume Next
    RaiseEvent Copy
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuPaste_Click()
    On Error Resume Next
    RaiseEvent Paste
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the text buffer
'
Public Function CopyToText(Buffer As String, ByVal All As Boolean) As Boolean
    On Error GoTo Herr
    CopyToText = SaveGridToText(grd, Buffer, All)
    Exit Function
Herr:
    ShowError "Copy the grid content to the clipboard failed due to the following error:"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyToClipboard(ByVal All As Boolean) As Boolean
    On Error GoTo Herr
    CopyToClipboard = CopyGridToClipboard(grd, All)
    Exit Function
Herr:
    ShowError "Copy the grid content to the clipboard failed due to the following error:"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyFromClipboard() As Boolean
Dim Buffer As String
    On Error GoTo Herr
    fSilent = True
    fShowLog = False
    With Clipboard
        If .GetFormat(vbCFText) Then
            Buffer = .GetText
            CopyFromClipboard = LoadGridFromText1(Buffer)
        End If
    End With
    fSilent = False
    ColorRows grd
    Exit Function
Herr:
    fSilent = False
    ShowError "Copy the grid content from the clipboard failed due to the following error:"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Parse text buffer and fill grid
'
Private Function LoadGridFromText1(ByVal Buffer As String) As Boolean
Dim Lines As Variant
Dim Fields As Variant
Dim Line1 As Variant
Dim Field As Variant
Dim i As Long
Dim j As Long
Dim r1 As Long
Dim c1 As Long
Dim r2 As Long
Dim c2 As Long
    If Len(Buffer) > 0 Then
        Lines = Split(Buffer, vbCrLf)
        grd.GetSelection r1, c1, r2, c2
        If r1 < 2 Then
            Exit Function
            r1 = 2
            c1 = 0
        End If
        If c1 < 0 Then
            Exit Function
            c1 = 0
        End If
        i = r1
        For Each Line1 In Lines
            If Len(Line1) > 0 Then
                Fields = Split(Line1, vbTab)
                i = i + 1
                If grd.Rows < i Then
                    AppendRow
                End If
                j = c1
                For Each Field In Fields
Try:
                    j = j + 1
                    If j > grd.Cols Then
                        GoTo Skip
                    End If
                    If grd.ColHidden(j - 1) Then
                        GoTo Try
                    End If
                    grd.TextMatrix(i - 1, j - 1) = Field
Skip:
                Next
            End If
        Next
        grd.Select r1, c1, r2, c2
        LoadGridFromText1 = True
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    If Len(StockSymbol) = 0 Then
    Else
    With grd
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                FillCell Row, IDX_STOCK_PRICE, StockPrice
                ColorCell grd, Row, IDX_STOCK_PRICE, DataStatus
'                If DataStatus = DATA_FEED Then
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_PRICE)) = BKG_COLOR_PRICE_FEED
'                Else
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_PRICE)) = BKG_COLOR_PRICE_MANUAL
'                End If
            End If
        Next
    End With
    End If
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIVData(ByVal StockSymbol As String, ByVal StockIV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                FillCell Row, IDX_STOCK_IV, StockIV * 100
                ColorCell grd, Row, IDX_STOCK_IV, DataStatus
'                If DataStatus = DATA_FEED Then
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_IV)) = BKG_COLOR_PRICE_FEED
'                Else
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_IV)) = BKG_COLOR_PRICE_MANUAL
'                End If
            End If
        Next
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_VegaModeChanged(ByVal NewVegaMode As Boolean)
    On Error Resume Next
    SetVegaMode NewVegaMode, PortfolioHedge.ShortPortfolio
    ApplyGridLayout
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ShortPortfolioModeChanged(ByVal NewShortPortfolioMode As Boolean)
    On Error Resume Next
    SetVegaMode PortfolioHedge.VegaMode, NewShortPortfolioMode
    ApplyGridLayout
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetVegaMode(ByVal VegaMode As Boolean, ByVal ShortPortfolioMode As Boolean)
    On Error Resume Next
    If ShortPortfolioMode Then
        m_cdColumns(IDX_DELTA).bVisible = Not VegaMode
        m_cdColumns(IDX_STOCK_PRICE).bVisible = Not VegaMode
        m_cdColumns(IDX_VEGA).bVisible = VegaMode
        m_cdColumns(IDX_STOCK_IV).bVisible = VegaMode
    Else
        m_cdColumns(IDX_DELTA).bVisible = True
        m_cdColumns(IDX_STOCK_PRICE).bVisible = True
        m_cdColumns(IDX_VEGA).bVisible = True
        m_cdColumns(IDX_STOCK_IV).bVisible = True
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGrid() As VSFlexGrid
    On Error Resume Next
    Set GetGrid = grd
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub imgSDHelp_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case IDX_TICKER
        ShowHelpID 101
    Case IDX_EXPIRY
        ShowHelpID 102
    Case IDX_DELTA
        ShowHelpID 103
    Case IDX_STOCK_PRICE
        ShowHelpID 104
    Case IDX_VEGA
        ShowHelpID 105
    Case IDX_STOCK_IV
        ShowHelpID 107
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

