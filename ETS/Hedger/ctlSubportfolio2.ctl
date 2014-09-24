VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Begin VB.UserControl ctlSubportfolio2 
   BackColor       =   &H00C8D0D4&
   ClientHeight    =   4140
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   8130
   ScaleHeight     =   4140
   ScaleWidth      =   8130
   Begin EgarHedgerSQL.ctlSubPortfolioSummary2 ctlSubPortfolioSummary21 
      Align           =   1  'Align Top
      Height          =   2220
      Left            =   0
      TabIndex        =   1
      Top             =   0
      Width           =   8130
      _ExtentX        =   14340
      _ExtentY        =   3916
   End
   Begin VSFlex7Ctl.VSFlexGrid grd 
      Height          =   1635
      Left            =   0
      TabIndex        =   0
      Top             =   2280
      Width           =   4815
      _cx             =   8493
      _cy             =   2884
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
      MouseIcon       =   "ctlSubportfolio2.ctx":0000
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
         Index           =   7
         Left            =   3480
         MouseIcon       =   "ctlSubportfolio2.ctx":0162
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":02B4
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   6
         Left            =   3120
         MouseIcon       =   "ctlSubportfolio2.ctx":0315
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":0467
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   5
         Left            =   2760
         MouseIcon       =   "ctlSubportfolio2.ctx":04C8
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":061A
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   4
         Left            =   2400
         MouseIcon       =   "ctlSubportfolio2.ctx":067B
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":07CD
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   3
         Left            =   2040
         MouseIcon       =   "ctlSubportfolio2.ctx":082E
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":0980
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   2
         Left            =   1680
         MouseIcon       =   "ctlSubportfolio2.ctx":09E1
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":0B33
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   1
         Left            =   1320
         MouseIcon       =   "ctlSubportfolio2.ctx":0B94
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":0CE6
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   0
         Left            =   960
         MouseIcon       =   "ctlSubportfolio2.ctx":0D47
         MousePointer    =   99  'Custom
         Picture         =   "ctlSubportfolio2.ctx":0E99
         ToolTipText     =   "Open Help"
         Top             =   120
         Width           =   150
      End
   End
   Begin EgarHedgerSQL.ctlSubPortfolioSummary4 ctlSubPortfolioSummary41 
      Height          =   1815
      Left            =   0
      TabIndex        =   2
      Top             =   0
      Visible         =   0   'False
      Width           =   8175
      _ExtentX        =   14420
      _ExtentY        =   3201
   End
End
Attribute VB_Name = "ctlSubportfolio2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Editable As Boolean

Public WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1

Public Event PositionMoved(ByVal RowID As Variant, ByVal NewHedgingSymbol As String, ByVal OldHedgingSymbol As String)

Private Enum ColIndexEnum
    IDX_TICKER
    IDX_EXPIRY
    IDX_DELTA
    IDX_STOCK_PRICE
    IDX_VEGA
    IDX_STOCK_HV
    IDX_STOCK_IV
    IDX_TRASH
    MINT_COLUMN_COUNT
End Enum

Private rsData As ADODB.Recordset
Public HedgeSymbolFilter As String
Public fRemaining As Boolean

Private Const Border = 60

Private Const TXT_COLOR_HOVER_NORM = vbBlue '&H6633&    '&H9966&
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
    ctlSubPortfolioSummary41.Height = ctlSubPortfolioSummary21.Height
    With grd
        .Left = 0
        .Top = ctlSubPortfolioSummary21.Height
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
        .Width = Max(0, ScaleWidth)
        .Height = Max(0, ScaleHeight - ctlSubPortfolioSummary21.Height)
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
    End With
    
    i = IDX_EXPIRY
    With m_cdColumns(i)
        .sName = "Expiry"
        .sCaption = "Expiry" & vbCrLf & "Date"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDate
        .sFormat = "YYYY-MM-DD"
    End With
    
    i = IDX_DELTA
    With m_cdColumns(i)
        .sName = "Delta"
        .sCaption = "Delta"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
    End With
    
    i = IDX_STOCK_PRICE
    With m_cdColumns(i)
        .sName = "StockPrice"
        .sCaption = "Stock" & vbCrLf & "Price"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
    End With
    
    i = IDX_VEGA
    With m_cdColumns(i)
    .sName = "Vega"
        .sCaption = "Vega"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
        .bVisible = False
    End With
    
    i = IDX_STOCK_HV
    With m_cdColumns(i)
        .sName = "StockHV"
        .sCaption = "Stock" & vbCrLf & "HV, %"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
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
        .bVisible = False
    End With
    
    i = IDX_TRASH
    With m_cdColumns(i)
        .sName = "Trash"
        .sCaption = "Remove"
        .sCaption2 = .sCaption
        .MergeCol = True
        .vsDataType = flexDTString
        .sFormat = ""
        .bVisible = True
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
        '.OutlineBar = flexOutlineBarSymbolsLeaf ' flexOutlineBarNone
        .ExplorerBar = flexExSort
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
'        '.OLEDragMode = flexOLEDragAutomatic
'        '.OLEDropMode = flexOLEDropAutomatic
'        .OLEDragMode = flexOLEDragManual
'        .OLEDropMode = flexOLEDropManual
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
Public Sub Clear()
    ctlSubPortfolioSummary21.Clear
    ctlSubPortfolioSummary41.Clear
    With grd
        .Rows = 2
        .Rows = 3
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
Public Sub DisplayOutputRS(rs As ADODB.Recordset, Optional ByVal HedgeSymbol As String, Optional ByVal SelectedRowID As Variant, Optional SelectedRow As Long)
Dim lRow As Long
Dim lRowCounter As Long
Dim StockSymbol As String
Dim sComboList As String
Dim Criteria As String
Dim Caption As String
    
    Editable = False
    HedgeSymbolFilter = HedgeSymbol
    grd.Redraw = flexRDNone
    ClearGrid
    Caption = PortfolioHedge.GetSubportfolioCaption(HedgeSymbol)
    
    fRemaining = Caption = "NONE"
    ctlSubPortfolioSummary41.Visible = fRemaining
    ctlSubPortfolioSummary21.Visible = Not fRemaining
    
    With grd
        If fRemaining Then
            .TextMatrix(0, .ColIndex(IDX_TRASH)) = "Add"
            .TextMatrix(1, .ColIndex(IDX_TRASH)) = "Add"
        Else
            .TextMatrix(0, .ColIndex(IDX_TRASH)) = "Remove"
            .TextMatrix(1, .ColIndex(IDX_TRASH)) = "Remove"
            ctlSubPortfolioSummary21.SetCaption Caption
        End If
    End With
    
    If rs Is Nothing Then
        AddRow
        GoTo Done
    End If
    With rs
        .Filter = PortfolioHedge.GetSubportfolioFilter(HedgeSymbol)
        If .RecordCount <> 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            lRowCounter = 0
            Do While Not .EOF
                StockSymbol = !StockSymbol
                lRow = AddRow
                lRowCounter = lRowCounter + 1
                FillCell lRow, IDX_TICKER, StockSymbol
                grd.Cell(flexcpForeColor, lRow, grd.ColIndex(IDX_TICKER)) = TXT_COLOR_HOVER_NORM
                grd.Cell(flexcpData, lRow, IDX_TICKER, lRow, grd.ColIndex(IDX_TICKER)) = .Bookmark
                grd.Cell(flexcpFontUnderline, lRow, grd.ColIndex(IDX_TICKER)) = True
                grd.Cell(flexcpFontBold, lRow, grd.ColIndex(IDX_TICKER)) = False
                If Not IsMissing(SelectedRowID) Then
                    If SelectedRowID = .Bookmark Then
                        grd.ShowCell lRow, 1
                        grd.Cell(flexcpBackColor, lRow, 0, lRow, grd.Cols - 1) = BKG_COLOR_ROW_CELECTED
                    End If
                End If
                FillCell lRow, IDX_EXPIRY, !Expiry, "UL"
                FillCell lRow, IDX_DELTA, !DeltaReal
                FillCell lRow, IDX_STOCK_PRICE, !StockPriceReal
                FillCell lRow, IDX_VEGA, !VegaReal
                FillCell lRow, IDX_STOCK_HV, !HVReal * 100
                FillCell lRow, IDX_STOCK_IV, !IV * 100
                If Caption = "NONE" Then
                    FillCell lRow, IDX_TRASH, "<--"
                Else
                    FillCell lRow, IDX_TRASH, "-->"
                End If
                With grd
                    .Cell(flexcpForeColor, lRow, .ColIndex(IDX_TRASH)) = TXT_COLOR_HOVER_NORM
                    .Cell(flexcpFontBold, lRow, .ColIndex(IDX_TRASH)) = True
                    .Cell(flexcpFontBold, lRow, .ColIndex(IDX_TRASH)) = True
                    .Cell(flexcpAlignment, lRow, .ColIndex(IDX_TRASH)) = flexAlignCenterCenter
                End With
                .MoveNext
'                DoEvents
            Loop
        Else
            grd.Rows = 3
        End If
        .Filter = ""
    End With
Done:
    ColorRows grd
    grd.Redraw = True
    AdjustHelpPictures
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ClearResults()
    On Error Resume Next
    Clear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotSubportfolioTotals(ByVal Q As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal Gain As Double, ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double, ByVal HedgeSymbol As String)
    On Error Resume Next
    If HedgeSymbol = HedgeSymbolFilter Then
        If fRemaining Then
            ctlSubPortfolioSummary41.SetTotals Q, D2, d3, Gain
            ctlSubPortfolioSummary41.SetSubTotals Q1, Q2, Q3, Q4
        Else
            ctlSubPortfolioSummary21.SetTotals Q, D2, d3, Gain
            ctlSubPortfolioSummary21.SetSubTotals Q1, Q2, Q3, Q4
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotSubportfolioTotals2(ByVal d1 As Long, ByVal D2 As Long, ByVal d3 As Double, ByVal D4 As Double, ByVal HedgeSymbol As String)
    On Error Resume Next
    If HedgeSymbol = HedgeSymbolFilter Then
        If fRemaining Then
            ctlSubPortfolioSummary41.SetTotals2 d1 - D2, D2, d3, D4
        Else
            ctlSubPortfolioSummary21.SetTotals2 d1 - D2, D2, d3, D4
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        .Redraw = flexRDNone
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
        .Redraw = True
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        .Redraw = flexRDNone
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                FillCell Row, IDX_STOCK_HV, HV * 100
                ColorCell grd, Row, IDX_STOCK_HV, DataStatus
'                If DataStatus = DATA_FEED Then
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_HV)) = BKG_COLOR_PRICE_FEED
'                Else
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_HV)) = BKG_COLOR_PRICE_MANUAL
'                End If
            End If
        Next
        .Redraw = True
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        .Redraw = flexRDNone
        For Row = 2 To .Rows - 1
            If StockSymbol = .TextMatrix(Row, .ColIndex(IDX_TICKER)) Then
                FillCell Row, IDX_STOCK_IV, IV * 100
                ColorCell grd, Row, IDX_STOCK_IV, DataStatus
'                If DataStatus = DATA_FEED Then
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_IV)) = BKG_COLOR_PRICE_FEED
'                Else
'                    .Cell(flexcpBackColor, Row, .ColIndex(IDX_STOCK_IV)) = BKG_COLOR_PRICE_MANUAL
'                End If
            End If
        Next
        .Redraw = True
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim lCol As Long
    On Error Resume Next
    lCol = grd.ColKey(Col)
    Cancel = Not (Editable And m_cdColumns(lCol).bCanEdit)
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
'Dim StockSymbol As String
'    On Error Resume Next
'    With grd
'        If .MouseRow >= 2 Then
'            If .MouseCol = .ColIndex(IDX_TICKER) Or .MouseCol = .ColIndex(IDX_TRASH) Then
'                ' Allow click
'            Else
'                ' if the click was on the active cell, start dragging
'                .Row = .MouseRow
'                .Col = .MouseCol
'                StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
'                If Len(StockSymbol) > 0 Then
'                    ' use OLEDrag method to start manual OLE drag operation
'                    ' this will fire the OLEStartDrag event, which we will use
'                    ' to fill the DataObject with the data we want to drag.
'                    .OLEDrag
'                    ' tell grid control to ignore mouse movements until the
'                    ' mouse button goes up again
'                    Cancel = True
'                End If
'            End If
'        End If
'    End With
'End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_OLEStartDrag(Data As VSFlex7Ctl.VSDataObject, AllowedEffects As Long)
'Dim StockSymbol As String
'Dim Expiry As String
'Dim RowID As Variant
'Dim S As String
'    On Error Resume Next
'    ' set contents of data object for manual drag
'    With grd
'        StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
'        Expiry = .TextMatrix(.Row, .ColIndex(IDX_EXPIRY))
'        RowID = .Cell(flexcpData, .Row, .ColIndex(IDX_TICKER))
'        S = "DRAG," & StockSymbol & "," & Expiry & "," & HedgeSymbolFilter & "," & Format$(RowID)
'    End With
'    Data.SetData S, vbCFText
'End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_OLEDragDrop(Data As VSFlex7Ctl.VSDataObject, Effect As Long, ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single)
'Dim lRow As Long
'Dim S As String
'Dim StockSymbol As String
'Dim Expiry As String
'Dim PrevHedgeSymbol As String
'Dim RowID As Variant
'Dim V As Variant
'    On Error Resume Next
'    With grd
'        ' drop text
'        If Data.GetFormat(vbCFText) Then
'            S = Data.GetData(vbCFText)
'            V = Split(S, ",")
'            If UBound(V) = 4 Then
'                If V(0) = "DRAG" Then
'                    StockSymbol = V(1)
'                    Expiry = V(2)
'                    PrevHedgeSymbol = V(3)
'                    RowID = Val(V(4))
'                    If PrevHedgeSymbol <> HedgeSymbolFilter Then
'                        RaiseEvent PositionMoved(RowID, HedgeSymbolFilter, PrevHedgeSymbol)
'                    End If
'                End If
'            End If
'        End If
'    End With
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'
Private Sub grd_Click()
Dim StockSymbol As String
Dim NewHedgeSymbol As String
Dim PrevHedgeSymbol As String
Dim RowID As Variant
    On Error Resume Next
    If Not Editable Then
        Exit Sub
    End If
    With grd
        If Not MultiSelection(grd) Then
        If .MouseRow > 1 Then
            If .MouseCol = .ColIndex(IDX_TICKER) Then
                .Row = .MouseRow
                .Col = .MouseCol
                StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
                If Len(StockSymbol) > 0 Then
                    ShowStocWindowStatic StockSymbol
                End If
            ElseIf .MouseCol = .ColIndex(IDX_TRASH) Then
                .Row = .MouseRow
                .Col = .MouseCol
                StockSymbol = .TextMatrix(.Row, .ColIndex(IDX_TICKER))
                If Len(StockSymbol) > 0 Then
                    Select Case HedgeSymbolFilter
                    Case "LONG"
                        NewHedgeSymbol = "NOT_LONG"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    Case "NOT_LONG"
                        NewHedgeSymbol = "LONG"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    Case "SHORT"
                        NewHedgeSymbol = "NOT_SHORT"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    Case "NOT_SHORT"
                        NewHedgeSymbol = "SHORT"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    Case "NONE"
                        ' do move to 'OPT' subportfolio
                        NewHedgeSymbol = "OPT"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    Case Else
                        ' do move to 'NONE' subportfolio
                        NewHedgeSymbol = "NONE"
                        PrevHedgeSymbol = HedgeSymbolFilter
                    End Select
                    RowID = .Cell(flexcpData, .Row, .ColIndex(IDX_TICKER))
                    If PrevHedgeSymbol <> NewHedgeSymbol Then
                        RaiseEvent PositionMoved(RowID, NewHedgeSymbol, PrevHedgeSymbol)
                    End If
                End If
            End If
        End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_MouseMove(Button As Integer, Shift As Integer, X As Single, y As Single)
Dim StockSymbol As String
    On Error Resume Next
    With grd
        If Editable Then
        If .MouseRow >= 2 And (.MouseCol = .ColIndex(IDX_TICKER) Or .MouseCol = .ColIndex(IDX_TRASH)) Then
            StockSymbol = .TextMatrix(.MouseRow, .ColIndex(IDX_TICKER))
            If StockSymbol <> "" Then
                .MousePointer = flexCustom
                Exit Sub
            End If
        End If
        End If
        .MousePointer = flexDefault
    End With
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
        m_cdColumns(IDX_EXPIRY).bVisible = VegaMode
        m_cdColumns(IDX_VEGA).bVisible = VegaMode
        m_cdColumns(IDX_STOCK_HV).bVisible = VegaMode
        m_cdColumns(IDX_STOCK_IV).bVisible = VegaMode
    Else
        m_cdColumns(IDX_EXPIRY).bVisible = True
        m_cdColumns(IDX_DELTA).bVisible = True
        m_cdColumns(IDX_STOCK_PRICE).bVisible = True
        m_cdColumns(IDX_VEGA).bVisible = True
        m_cdColumns(IDX_STOCK_HV).bVisible = True
        m_cdColumns(IDX_STOCK_IV).bVisible = True
    End If
    ctlSubPortfolioSummary21.SetVegaMode VegaMode
    ctlSubPortfolioSummary41.SetVegaMode VegaMode
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
    Case IDX_STOCK_HV
        ShowHelpID 106
    Case IDX_STOCK_IV
        ShowHelpID 107
    Case IDX_TRASH
        If fRemaining Then
            ShowHelpID 409
        Else
            ShowHelpID 408
        End If
    Case Else
        Debug.Assert False
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ShowHelpTipsChanged()
    On Error Resume Next
    ShowHelpTags
    ctlSubPortfolioSummary21.ShowHelpTags
    ctlSubPortfolioSummary41.ShowHelpTags
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    ColorRows grd
End Sub

