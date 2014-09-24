VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Begin VB.UserControl ctlIndexOptions 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   ClientHeight    =   5895
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9000
   ScaleHeight     =   5895
   ScaleWidth      =   9000
   Begin VSFlex7Ctl.VSFlexGrid grd 
      Height          =   4875
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   7695
      _cx             =   13573
      _cy             =   8599
      _ConvInfo       =   1
      Appearance      =   0
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
      BackColor       =   6710886
      ForeColor       =   3407769
      BackColorFixed  =   6710886
      ForeColorFixed  =   16777215
      BackColorSel    =   6710886
      ForeColorSel    =   16777215
      BackColorBkg    =   6710886
      BackColorAlternate=   6710886
      GridColor       =   8421504
      GridColorFixed  =   8421504
      TreeColor       =   8421504
      FloodColor      =   192
      SheetBorder     =   8421504
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
      BackColorFrozen =   6710886
      ForeColorFrozen =   16777215
      WallPaperAlignment=   9
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   3
         Left            =   3840
         MouseIcon       =   "ctlIndexOptions.ctx":0000
         MousePointer    =   99  'Custom
         Picture         =   "ctlIndexOptions.ctx":0152
         ToolTipText     =   "Open Help"
         Top             =   240
         Visible         =   0   'False
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   2
         Left            =   2880
         MouseIcon       =   "ctlIndexOptions.ctx":01B3
         MousePointer    =   99  'Custom
         Picture         =   "ctlIndexOptions.ctx":0305
         ToolTipText     =   "Open Help"
         Top             =   240
         Visible         =   0   'False
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   1
         Left            =   1920
         MouseIcon       =   "ctlIndexOptions.ctx":0366
         MousePointer    =   99  'Custom
         Picture         =   "ctlIndexOptions.ctx":04B8
         ToolTipText     =   "Open Help"
         Top             =   240
         Visible         =   0   'False
         Width           =   150
      End
      Begin VB.Image imgSdHelp 
         Appearance      =   0  'Flat
         Height          =   150
         Index           =   0
         Left            =   960
         MouseIcon       =   "ctlIndexOptions.ctx":0519
         MousePointer    =   99  'Custom
         Picture         =   "ctlIndexOptions.ctx":066B
         ToolTipText     =   "Open Help"
         Top             =   240
         Visible         =   0   'False
         Width           =   150
      End
   End
   Begin VB.Menu mnuContext 
      Caption         =   "<Context>"
      Begin VB.Menu mnuColumns 
         Caption         =   "&Columns"
      End
      Begin VB.Menu mnuBar0 
         Caption         =   "-"
      End
      Begin VB.Menu mnuHide 
         Caption         =   "&Hide"
      End
   End
End
Attribute VB_Name = "ctlIndexOptions"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Editable As Boolean
Public Event IndexPriceChanged(ByVal IndexSymbol As String, ByVal IndexPrice As Double, ContractsAmount As Long, ByVal DataStatus As enmDATA_STATUS)

Private Enum ColIndexEnum
    IDX_INDEX = 0
    IDX_PRICE
    IDX_HV
    IDX_IV
    MINT_COLUMN_COUNT
End Enum

Private m_cdColumns(0 To MINT_COLUMN_COUNT - 1) As New clsColumnDefinition

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
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
    
    i = IDX_INDEX
    With m_cdColumns(i)
        .sName = "Index"
        .sCaption = "Index Symbol"
        .sCaption2 = m_cdColumns(i).sCaption
        .MergeCol = True
        .vsDataType = flexDTDispatch
        .sFormat = ""
    End With
    
    i = IDX_PRICE
    With m_cdColumns(i)
        .sName = "Price"
        .sCaption = "Index price"
        .sCaption2 = m_cdColumns(i).sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
    End With
    
    i = IDX_HV
    With m_cdColumns(i)
        .sName = "HV"
        .sCaption = "Index HV, %"
        .sCaption2 = m_cdColumns(i).sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
    End With
    
    i = IDX_IV
    With m_cdColumns(i)
        .sName = "IV"
        .sCaption = "Index IV, %"
        .sCaption2 = m_cdColumns(i).sCaption
        .MergeCol = True
        .vsDataType = flexDTDouble
        .sFormat = "#,###.##"
    End With
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FormatGrid()
Dim i As Long
Dim w As Single
    w = 6960
    With grd
        .Rows = 3 + 3
        .Cols = MINT_COLUMN_COUNT
        .FixedRows = 2
        .FixedCols = 0
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .ExplorerBar = flexExNone
        For i = 0 To MINT_COLUMN_COUNT - 1
            .ColWidth(i) = w / 4
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
'        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
'        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .Cell(flexcpFontBold, 0, 0, 1, .Cols - 1) = False
        .Editable = flexEDKbdMouse
        .AllowUserResizing = flexResizeNone
        .AutoResize = True
        .AllowSelection = False
        .HighLight = flexHighlightNever
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
Public Sub DisplayOutputRS(rs As ADODB.Recordset, ByVal HedgeSymbol As String)
Dim lRow As Long
Dim lCol As Long
    Editable = False
    ClearGrid
    If rs Is Nothing Then
        AddRow
        Exit Sub
    End If
    With rs
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If !IndexSymbol <> "NONE" Then
                    lRow = AddRow
                    FillCell lRow, IDX_INDEX, !IndexSymbol
                    FillCell lRow, IDX_PRICE, !IndexPrice
                    FillCell lRow, IDX_HV, gPortfolioHedge.GetIndexHV(!IndexSymbol) * 100
                    FillCell lRow, IDX_IV, gPortfolioHedge.GetIndexIV(!IndexSymbol) * 100
                End If
                .MoveNext
            Loop
        Else
            grd.Rows = 3 + 3
        End If
    End With
    AdjustHelpPictures
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetVegaMode(ByVal VegaMode As Boolean)
Dim Msg As String
    If VegaMode Then
        Msg = "Equivalent" & vbCrLf & "Index Vega"
    Else
        Msg = "Equivalent" & vbCrLf & "Index Position"
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
Dim lRow As Long
Dim IndexSymbol
    On Error Resume Next
    With grd
        .Rows = 2
        .Rows = 6
    End With
    If gPortfolioHedge Is Nothing Then Exit Sub
    lRow = 1
    For Each IndexSymbol In Array("SPX", "NDX", "OEX", "DJX")
        lRow = lRow + 1
        FillCell lRow, IDX_INDEX, IndexSymbol
        FillCell lRow, IDX_PRICE, gPortfolioHedge.GetStockPrice(IndexSymbol)
        FillCell lRow, IDX_HV, gPortfolioHedge.GetIndexHV(IndexSymbol) * 100
        FillCell lRow, IDX_IV, gPortfolioHedge.GetIndexIV(IndexSymbol) * 100
    Next
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
'        If AddRow Mod 2 = 0 Then
'            .Cell(flexcpBackColor, AddRow, 0, AddRow, .Cols - 1) = &HEEEEEE
'        Else
'            .Cell(flexcpBackColor, AddRow, 0, AddRow, .Cols - 1) = &HFFFFFF
'        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FillCell(ByVal lRow As Long, ByVal iColl As ColIndexEnum, ByVal Data As Variant)
Dim lCol As Long
    With grd
        lCol = .ColIndex(iColl)
        If IsNull(Data) Then
            .TextMatrix(lRow, lCol) = ""
        Else
            .TextMatrix(lRow, lCol) = Data
        End If
    End With
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    With grd
'        If Button = 2 And .MouseRow > -1 And .MouseCol > -1 Then
'            .Col = .MouseCol
'            .Row = .MouseRow
'            PopupMenu mnuContext
'        End If
'    End With
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuColumns_Click()
Dim i As Long
    On Error Resume Next
    With grd
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
    On Error Resume Next
    With grd
        If .Row > -1 Then
            If .Col > -1 Then
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
    lCol = grd.ColKey(Col)
    Cancel = Not (Editable And m_cdColumns(lCol).bCanEdit)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyPressEdit(ByVal Row As Long, ByVal Col As Long, KeyAscii As Integer)
    On Error Resume Next
    Select Case grd.ColKey(Col)
    Case IDX_PRICE
        If Not IsNumericKey(KeyAscii) Then
            KeyAscii = 0
        End If
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_CellChanged(ByVal Row As Long, ByVal Col As Long)
Dim IndexSymbol As String
Dim IndexPrice As Double
Dim lCol As Long
Dim ContractsAmount As Long
    On Error Resume Next
    If Editable And Row >= 2 Then
        With grd
            IndexSymbol = .TextMatrix(Row, .ColIndex(IDX_INDEX))
            lCol = .ColKey(Col)
            Select Case lCol
            Case IDX_PRICE
                IndexPrice = ScanDouble(.TextMatrix(Row, .ColIndex(IDX_PRICE)))
                RaiseEvent IndexPriceChanged(IndexSymbol, IndexPrice, ContractsAmount, DATA_MANUAL)
                FillCell Row, IDX_PRICE, IndexPrice
                '.Cell(flexcpBackColor, Row, .ColIndex(IDX_PRICE)) = BKG_COLOR_PRICE_MANUAL
                .Select Row, Col
            End Select
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexPriceData(ByVal IndexSymbol As String, ByVal IndexPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Dim ContractsAmount As Long
Dim Row As Long
    Editable = False
    With grd
        For Row = 2 To .Rows - 1
            If IndexSymbol = .TextMatrix(Row, .ColIndex(IDX_INDEX)) Then
                RaiseEvent IndexPriceChanged(IndexSymbol, IndexPrice, ContractsAmount, DataStatus)
                FillCell Row, IDX_PRICE, IndexPrice
            End If
        Next
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexHVData(ByVal IndexSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        For Row = 2 To .Rows - 1
            If IndexSymbol = .TextMatrix(Row, .ColIndex(IDX_INDEX)) Then
                FillCell Row, IDX_HV, HV * 100
            End If
        Next
    End With
    Editable = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexIVData(ByVal IndexSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
Dim Row As Long
    Editable = False
    With grd
        For Row = 2 To .Rows - 1
            If IndexSymbol = .TextMatrix(Row, .ColIndex(IDX_INDEX)) Then
                FillCell Row, IDX_IV, IV * 100
            End If
        Next
    End With
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
Private Sub imgSDHelp_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case IDX_INDEX
        ShowHelpID 302
    Case IDX_PRICE
        ShowHelpID 305
    Case IDX_HV
        ShowHelpID 303
    Case IDX_IV
        ShowHelpID 304
    Case Else
        Debug.Assert False
    End Select
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

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_GotFocus()
'    On Error Resume Next
'    Set g_grd = grd
'End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_LostFocus()
'    On Error Resume Next
'    Set g_grd = Nothing
'End Sub

