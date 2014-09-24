VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmUnderlyingParams 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Underlying Parameters"
   ClientHeight    =   6570
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   9285
   Icon            =   "frmUnderlyingParams.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6570
   ScaleWidth      =   9285
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin ElladaFlatControls.FlatButton btnClose 
      Cancel          =   -1  'True
      Default         =   -1  'True
      Height          =   315
      Left            =   7440
      TabIndex        =   1
      Top             =   6120
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   556
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Close"
   End
   Begin VSFlex7Ctl.VSFlexGrid fgUnderlying 
      Height          =   5895
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   9015
      _cx             =   15901
      _cy             =   10398
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
      BackColorFixed  =   -2147483633
      ForeColorFixed  =   -2147483630
      BackColorSel    =   -2147483637
      ForeColorSel    =   0
      BackColorBkg    =   16777215
      BackColorAlternate=   -2147483643
      GridColor       =   -2147483633
      GridColorFixed  =   -2147483632
      TreeColor       =   -2147483632
      FloodColor      =   192
      SheetBorder     =   -2147483642
      FocusRect       =   1
      HighLight       =   1
      AllowSelection  =   -1  'True
      AllowBigSelection=   -1  'True
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
End
Attribute VB_Name = "frmUnderlyingParams"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Option Explicit

Private Enum GridColumns
    IDX_SYMBOL = 0
    IDX_HTB
    IDX_NAME
    
    IDX_DUMMY
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private Sub btnClose_Click()
    On Error Resume Next
    Hide
End Sub

Private Sub fgUnderlying_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error GoTo EH
    Dim btIsHTB As Byte
    With fgUnderlying
        Select Case Col
        Case IDX_HTB
            btIsHTB = IIf(.Cell(flexcpChecked, Row, Col) = flexChecked, 1, 0)
            If .Cell(flexcpData, Row, IDX_SYMBOL) = 1 Then
                gDBW.usp_Index_Save .RowData(Row), , , , btIsHTB
            ElseIf .Cell(flexcpData, Row, IDX_SYMBOL) = 2 Then
                gDBW.usp_Stock_Save .RowData(Row), , , , btIsHTB
            End If
        End Select
    End With
    Exit Sub
EH:
    gCmn.ErrorMsgBox "Error occured while saving underlying data"
End Sub

Private Sub fgUnderlying_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = Col <> IDX_HTB
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    
    InitGrid
    LoadData
    
    gCmn.CustomizeForm Me
End Sub

Private Sub InitGrid()
    On Error Resume Next
    Dim i As Long
    
    With fgUnderlying
        .Rows = 1
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .AutoResize = False
        .AutoSizeMouse = False
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusRaised
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = IDX_SYMBOL
        .TextMatrix(0, i) = "Symbol"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_NAME
        .TextMatrix(0, i) = "Name"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
    
        i = IDX_HTB
        .TextMatrix(0, i) = "Is HTB"
        .ColDataType(i) = flexDTBoolean
    End With
End Sub

Private Sub LoadData()
    On Error GoTo EH
    Dim rs As ADODB.Recordset
    Dim nRow&
    
    With fgUnderlying
        .Redraw = flexRDNone
        .Rows = 1
        Set rs = gDBW.usp_Index_Get
        While Not rs.EOF
            
            .AddItem ""
            nRow = .Rows - 1
            .RowData(nRow) = gCmn.ReadLng(rs!iContractID)
            .TextMatrix(nRow, IDX_SYMBOL) = gCmn.ReadStr(rs!vcSymbol)
            .TextMatrix(nRow, IDX_NAME) = gCmn.ReadStr(rs!vcContractName)
            .Cell(flexcpData, nRow, IDX_SYMBOL) = 1
            .Cell(flexcpChecked, nRow, IDX_HTB) = IIf(gCmn.ReadByte(rs!bIsHTB) <> 0, 1, 0)
            
            rs.MoveNext
        Wend
        Set rs = Nothing
        
        Set rs = gDBW.usp_Stock_Get
        While Not rs.EOF
            
            .AddItem ""
            nRow = .Rows - 1
            .RowData(nRow) = gCmn.ReadLng(rs!iContractID)
            .TextMatrix(nRow, IDX_SYMBOL) = gCmn.ReadStr(rs!vcSymbol)
            .TextMatrix(nRow, IDX_NAME) = gCmn.ReadStr(rs!vcContractName)
            .Cell(flexcpData, nRow, IDX_SYMBOL) = 2
            .Cell(flexcpChecked, nRow, IDX_HTB) = IIf(gCmn.ReadByte(rs!bIsHTB) <> 0, 1, 0)
            
            rs.MoveNext
        Wend
        If .Rows > 1 Then
            .Row = 1
            .Col = 0
            .Sort = flexSortStringAscending
        End If
        .AutoSize 0, LAST_COLUMN, , 100
        .Redraw = flexRDBuffered
    End With
    
    Set rs = Nothing
    Exit Sub
EH:
    fgUnderlying.Redraw = flexRDBuffered
    Set rs = Nothing
    gCmn.ErrorMsgBox "Error occured while loading underlying data"
End Sub
