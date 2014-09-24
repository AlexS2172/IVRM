VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "Vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmBetas 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Stock Index Beta"
   ClientHeight    =   3690
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5895
   Icon            =   "frmBetas.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3690
   ScaleWidth      =   5895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgIndex 
      Height          =   3135
      Left            =   120
      TabIndex        =   0
      Top             =   360
      Width           =   4215
      _cx             =   7435
      _cy             =   5530
      _ConvInfo       =   1
      Appearance      =   0
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
      BackColorSel    =   -2147483635
      ForeColorSel    =   -2147483634
      BackColorBkg    =   -2147483643
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
      ExplorerBar     =   0
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
      Begin VB.Timer Timer1 
         Interval        =   500
         Left            =   7320
         Top             =   2160
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   4440
      TabIndex        =   1
      Top             =   1800
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOK 
      Height          =   300
      Left            =   4440
      TabIndex        =   2
      Top             =   1320
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&OK"
   End
   Begin ElladaFlatControls.FlatButton btnIndexAdd 
      Height          =   300
      Left            =   4440
      TabIndex        =   3
      Top             =   360
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Add"
   End
   Begin ElladaFlatControls.FlatButton btnIndexDel 
      Height          =   300
      Left            =   4440
      TabIndex        =   4
      Top             =   840
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Delete"
   End
   Begin VB.Label lblUnderList 
      BackStyle       =   0  'Transparent
      Caption         =   "Hedge Symbol list:"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   1455
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   4920
      Picture         =   "frmBetas.frx":000C
      Top             =   3240
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Menu mnuUndParam 
      Caption         =   "Underlying Params"
      Visible         =   0   'False
      Begin VB.Menu mnuAddUnd 
         Caption         =   "Add Underlying"
      End
      Begin VB.Menu mnuDeleteUnd 
         Caption         =   "Delete Underlying"
      End
   End
End
Attribute VB_Name = "frmBetas"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    IDX_SYMBOL = 0
    IDX_BETA
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_bChanged As Boolean
Private m_iItemID As Long
Private m_sIndexes As String
Private m_collIndexBeta As clsIndexBetaColl
Private m_iFirstIndexID As Long
Private m_bIsSynthetic As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal iItemID As Long, ByVal sItemSymbol As String, ByRef sIndexList As String, _
    Optional ByRef bIsSynthetic As Boolean = False, Optional ByRef collIndexBeta As clsIndexBetaColl = Nothing) As Boolean
    
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bChanged = False
    m_iItemID = iItemID
    m_sIndexes = sIndexList
    m_bIsSynthetic = bIsSynthetic
    
    If collIndexBeta Is Nothing Then
        Set m_collIndexBeta = New clsIndexBetaColl
    Else
        Set m_collIndexBeta = collIndexBeta
    End If
    
    Load Me
    Me.Caption = "Beta for '" & sItemSymbol & IIf(m_bIsSynthetic, "' synthetic root", "' underlying")
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
        
    Execute = m_bChanged
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnOK_Click()
    On Error GoTo Herr
    
    Screen.MousePointer = vbHourglass
    If m_bChanged Then SaveData
    Screen.MousePointer = vbDefault
    Hide
    Exit Sub
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    m_bChanged = False
    Me.Hide
End Sub


Private Sub btnIndexAdd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub btnIndexDel_Click()
On Error GoTo ErrHnd
             
DeleteRow fgIndex.RowSel

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub fgIndex_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim sOldID, sNewID As String, l As Long
    Dim aBeta As clsIndexBetaAtom

    On Error GoTo ErrHnd
    If Col = IDX_SYMBOL And Row > 1 Then
                                
        sOldID = fgIndex.RowData(Row)
        sNewID = Format$(fgIndex.Cell(flexcpText, Row, IDX_SYMBOL))
                
        If sOldID <> sNewID Then
            fgIndex.RowData(Row) = sNewID
            m_bChanged = True
        End If
            
    ElseIf Col = IDX_BETA And Row > 1 Then
        
        If Len(fgIndex.TextMatrix(Row, Col)) = 0 Then
            fgIndex.TextMatrix(Row, Col) = 0
        End If
        
        Set aBeta = m_collIndexBeta(Trim$(Str$(fgIndex.RowData(Row))))
        
        If aBeta Is Nothing Then
            m_bChanged = True
        ElseIf aBeta.Beta <> gCmn.ReadDbl(fgIndex.TextMatrix(Row, Col)) Then
            m_bChanged = True
        End If
    End If
    
    If m_bChanged Then imgSave.Visible = True
    
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub fgIndex_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    With fgIndex
        If NewRow > -1 And NewCol > -1 Then
            If NewRow < 2 Then
                btnIndexDel.Enabled = False
            Else
                btnIndexDel.Enabled = True
            End If
        End If
    End With
End Sub

Private Sub fgIndex_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgIndex
        Select Case KeyCode
        Case vbKeyInsert
            AddRow
            Exit Sub
        Case vbKeyDelete
            DeleteRow fgIndex.RowSel
        End Select
    End With
End Sub

Private Sub fgIndex_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim DblVal As Double, l As Long

    On Error Resume Next
    
    If Col = IDX_BETA And Row > 1 Then
        On Error GoTo BadDbl
            DblVal = CDbl(fgIndex.EditText)
    End If
    
    On Error Resume Next
    Cancel = False
    Exit Sub
BadDbl:
    Cancel = True
End Sub

Private Sub fgIndex_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With fgIndex
        If (Button And vbRightButton) And .Row > -1 And .Col > -1 Then
            If .Row < 2 Then
                mnuDeleteUnd.Enabled = False
            Else
                mnuDeleteUnd.Enabled = True
            End If
            PopupMenu mnuUndParam, , , , mnuAddUnd
        End If
    End With

End Sub

Private Sub fgIndex_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    On Error Resume Next
    With fgIndex
        If Button And vbRightButton Then
            If .MouseRow >= 2 Then
                .Row = .MouseRow
                .Col = .MouseCol
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim I As Long
    On Error Resume Next
    
    With fgIndex
        .Rows = 0
        .Cols = COLUMNS_COUNT
        .BackColorSel = GCOLOR_TABLE_SELECT2_BACK
        .ForeColorSel = GCOLOR_TABLE_SELECT2_FORE
        .HighLight = flexHighlightAlways
        .SelectionMode = flexSelectionByRow
        .FocusRect = flexFocusHeavy
        
        .Rows = 2
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExSortShow
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 2
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(IDX_SYMBOL) = True
        .MergeCol(IDX_BETA) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        I = IDX_SYMBOL
        .TextMatrix(0, I) = "Symbol"
        .TextMatrix(1, I) = "Symbol"
        
        .ColDataType(I) = flexDTString
                
        I = IDX_BETA
        .TextMatrix(0, I) = "Beta"
        .TextMatrix(1, I) = "Beta"
        .ColDataType(I) = flexDTDouble
        .ColFormat(I) = "0.000000"
                                   
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
Dim rs As ADODB.Recordset, aBeta As clsIndexBetaAtom
Dim sKey, strIndexID As String, dBeta#, b
        
    fgIndex.ColComboList(IDX_SYMBOL) = m_sIndexes
    
    strIndexID = Left(m_sIndexes, InStr(m_sIndexes, ";") - 1)
    strIndexID = Replace(strIndexID, "#", "")
    m_iFirstIndexID = gCmn.ReadLng(strIndexID)
    
    If m_collIndexBeta.Count = 0 Then
        
        If Not m_bIsSynthetic Then
            Set rs = gDBW.usp_IndexUnderlyingBeta_Get(Null, m_iItemID)
        Else
            Set rs = gDBW.usp_SyntheticIndexBeta_Get(m_iItemID)
        End If
        
        With rs
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    dBeta = gCmn.ReadDbl(.Fields("fBeta"))
                    If dBeta <> 0 Then
                        Set aBeta = m_collIndexBeta.Add(Trim$(Str$(gCmn.ReadLng(.Fields("iIndexID")))))
                        aBeta.IndexID = gCmn.ReadLng(.Fields("iIndexID"))
                        aBeta.Beta = dBeta
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    For Each aBeta In m_collIndexBeta
        With fgIndex
            .AddItem ""
                
            .RowData(.Rows - 1) = Trim$(Str$(aBeta.IndexID))
                                        
            .TextMatrix(.Rows - 1, IDX_BETA) = aBeta.Beta
            .Cell(flexcpText, .Rows - 1, IDX_SYMBOL) = aBeta.IndexID
            .Cell(flexcpAlignment, .Rows - 1, IDX_BETA) = flexAlignRightCenter
        End With
    Next
    
    If fgIndex.Rows < 3 Then
        mnuDeleteUnd.Enabled = False
        btnIndexDel.Enabled = False
    End If
    
    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
    Dim aBeta As clsIndexBetaAtom, l#
    
    If Not m_bIsSynthetic Then
        gDBW.usp_IndexUnderlyingBeta_Clear Null, m_iItemID
    End If
    m_collIndexBeta.Clear

    With fgIndex
            
        For l = 2 To .Rows - 1
            
            Set aBeta = m_collIndexBeta(Trim$(Str$(.RowData(l))))
            
            If aBeta Is Nothing Then
                Set aBeta = m_collIndexBeta.Add(Trim$(Str$(.RowData(l))))
                aBeta.Beta = gCmn.ReadDbl(.Cell(flexcpText, l, IDX_BETA))
                aBeta.IndexID = gCmn.ReadLng(.RowData(l))
            Else
                aBeta.Beta = aBeta.Beta + gCmn.ReadDbl(.Cell(flexcpText, l, IDX_BETA))
            End If
        Next
    End With
    
    For Each aBeta In m_collIndexBeta
        If Not m_bIsSynthetic Then
            gDBW.usp_IndexUnderlyingBeta_Save aBeta.IndexID, m_iItemID, aBeta.Beta
        End If
    Next
    m_collIndexBeta.Changed = True
    SaveData = True
End Function



Private Sub mnuAddUnd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub mnuDeleteUnd_Click()
On Error GoTo ErrHnd
                 
DeleteRow fgIndex.RowSel

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub AddRow()
    Dim Data As New clsSymbolWithBasket, Cancel As Boolean
    
    With fgIndex
                        
        .AddItem ""
        .TextMatrix(.Rows - 1, IDX_BETA) = 0
        .Cell(flexcpAlignment, .Rows - 1, IDX_BETA) = flexAlignRightCenter
        
        .RowData(.Rows - 1) = -1
        .TextMatrix(.Rows - 1, IDX_SYMBOL) = m_iFirstIndexID
        .Select .Rows - 1, IDX_SYMBOL
        .EditCell
    End With
    
    m_bChanged = True
    imgSave.Visible = True
    
End Sub

Private Sub DeleteRow(ByVal Row As Long)
    
    With fgIndex
        
        If Row < 2 Then Exit Sub
    
        If Row = .Rows - 1 Then
            .RemoveItem Row
            .Row = Row - 1
            .RowSel = Row - 1
        Else
            .RemoveItem Row
        End If
        
        If .RowSel < 2 Then
            mnuDeleteUnd.Enabled = False
            btnIndexDel.Enabled = False
        End If
            
    End With
    
    m_bChanged = True
    imgSave.Visible = True

End Sub

