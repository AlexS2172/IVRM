VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmSyntUndPar 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Synthetic Underlying Params"
   ClientHeight    =   4680
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   5895
   Icon            =   "frmSyntUndPar.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4680
   ScaleWidth      =   5895
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.TextBox txtYield 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   3000
      TabIndex        =   12
      Top             =   960
      Width           =   1335
   End
   Begin VB.CheckBox chkIsBasket 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Basket Dividends"
      ForeColor       =   &H80000008&
      Height          =   495
      Left            =   3000
      TabIndex        =   11
      Top             =   240
      Width           =   1335
   End
   Begin VB.TextBox txtKurtosis 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   1560
      TabIndex        =   9
      Top             =   960
      Width           =   1335
   End
   Begin VB.TextBox txtSkew 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   1560
      TabIndex        =   7
      Top             =   360
      Width           =   1335
   End
   Begin VB.TextBox txtCash 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   120
      TabIndex        =   5
      Top             =   360
      Width           =   1335
   End
   Begin VSFlex7Ctl.VSFlexGrid fgParams 
      Height          =   3135
      Left            =   120
      TabIndex        =   0
      Top             =   1440
      Width           =   4215
      _cx             =   7435
      _cy             =   5530
      _ConvInfo       =   1
      Appearance      =   0
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
      Top             =   2880
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
      Top             =   2400
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&OK"
   End
   Begin ElladaFlatControls.FlatButton btnParamAdd 
      Height          =   300
      Left            =   4440
      TabIndex        =   3
      Top             =   1440
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Add"
   End
   Begin ElladaFlatControls.FlatButton btnParamDel 
      Height          =   300
      Left            =   4440
      TabIndex        =   4
      Top             =   1920
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Delete"
   End
   Begin ElladaFlatControls.FlatButton btnBeta 
      Height          =   300
      Left            =   4440
      TabIndex        =   14
      Top             =   3360
      Visible         =   0   'False
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      Enabled         =   0   'False
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Index Beta..."
   End
   Begin VB.Label lblYield 
      BackStyle       =   0  'Transparent
      Caption         =   "Yield (%) :"
      Height          =   255
      Left            =   3000
      TabIndex        =   13
      Top             =   720
      Width           =   1215
   End
   Begin VB.Label lblKurt 
      BackStyle       =   0  'Transparent
      Caption         =   "Kurtosis :"
      Height          =   255
      Left            =   1560
      TabIndex        =   10
      Top             =   720
      Width           =   1215
   End
   Begin VB.Label lblSkew 
      BackStyle       =   0  'Transparent
      Caption         =   "Skew :"
      Height          =   255
      Left            =   1560
      TabIndex        =   8
      Top             =   120
      Width           =   1215
   End
   Begin VB.Label lblCashAmount 
      BackStyle       =   0  'Transparent
      Caption         =   "Cash Amount :"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   120
      Width           =   1215
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   5520
      Picture         =   "frmSyntUndPar.frx":000C
      Top             =   4320
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
Attribute VB_Name = "frmSyntUndPar"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    TSR_UNDSYMBOL = 0
    TSR_STRATEGY
    TSR_SETTLEMENT
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_bRootChanged As Boolean
Private m_bParamsChanged As Boolean
Private m_bBetaChanged As Boolean

Private m_OptionRoot As clsOptRootAtom
Private m_sContracts As String
Private m_sIndexes As String
Private m_SyntheticIndexBetas As clsIndexBetaColl

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByRef OptRoot As clsOptRootAtom, ByRef sContracts As String, _
    ByRef sIndexes As String) As Boolean
    
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bRootChanged = False
    m_bParamsChanged = False
    m_bBetaChanged = False
    Set m_OptionRoot = OptRoot
    m_sContracts = sContracts
    m_sIndexes = sIndexes

    Load Me
    Me.Caption = "Synthetic Underlying Params for " & m_OptionRoot.Symbol
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
        
    Execute = m_bRootChanged Or m_bParamsChanged Or m_bBetaChanged
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnOK_Click()
    On Error GoTo Herr
    
    Screen.MousePointer = vbHourglass
    If m_bRootChanged Or m_bParamsChanged Or m_bBetaChanged Then SaveData
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
    m_bRootChanged = False
    m_bParamsChanged = False
    m_bBetaChanged = False
    Me.Hide
End Sub


Private Sub btnParamAdd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub btnDel_Click()
On Error GoTo ErrHnd
             
DeleteRow fgParams.RowSel

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub btnParamDel_Click()
On Error GoTo ErrHnd
             
DeleteRow fgParams.RowSel

Exit Sub
ErrHnd:
    ShowError

End Sub

Private Sub chkIsBasket_Click()
    On Error Resume Next
    
    Dim IsBasket As Boolean
    IsBasket = (chkIsBasket.Value <> 0)
    
    txtYield.Enabled = Not IsBasket
    lblYield.Enabled = Not IsBasket
    
    If IsBasket <> m_OptionRoot.IsBasket Then
        m_bRootChanged = True
        imgSave.Visible = True
    End If

End Sub

Private Sub fgParams_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim Param As clsSyntParAtom, sOldID, sNewID As String, l As Long

    On Error GoTo ErrHnd
    
    If Row > 1 Then
        Select Case Col
            Case TSR_UNDSYMBOL
                sOldID = fgParams.RowData(Row).ContractId
                sNewID = Format$(fgParams.Cell(flexcpText, Row, TSR_UNDSYMBOL))
                
                If sOldID <> sNewID Then
                    fgParams.RowData(Row).ContractId = sNewID
                    GetRowDataByID fgParams.RowData(Row)
                    m_bParamsChanged = True
                End If
            Case TSR_STRATEGY
                If Len(fgParams.TextMatrix(Row, Col)) = 0 Then
                    fgParams.TextMatrix(Row, Col) = 0
                End If
        
                Set Param = m_OptionRoot.SynthParams(Str$(fgParams.RowData(Row).ContractId))
        
                If Param Is Nothing Then
                    m_bParamsChanged = True
                ElseIf Param.Weight <> fgParams.TextMatrix(Row, Col) Then
                    m_bParamsChanged = True
                End If
            Case TSR_SETTLEMENT
                If Len(fgParams.TextMatrix(Row, Col)) = 0 Then
                    fgParams.TextMatrix(Row, Col) = "--"
                End If
        
                Set Param = m_OptionRoot.SynthParams(Str$(fgParams.RowData(Row).ContractId))
        
                If Param Is Nothing Then
                    m_bParamsChanged = True
                ElseIf Param.StrikeSettlement <> IIf(fgParams.TextMatrix(Row, Col) <> "--", fgParams.TextMatrix(Row, Col), BAD_DOUBLE_VALUE) Then
                    m_bParamsChanged = True
                End If
        End Select
    End If
    
    If m_bParamsChanged Then imgSave.Visible = True
    
    Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub fgParams_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    With fgParams
        If NewRow > -1 And NewCol > -1 Then
            If NewRow < 2 Then
                btnParamDel.Enabled = False
            Else
                btnParamDel.Enabled = True
            End If
        End If
    End With
End Sub

Private Sub fgParams_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgParams
        Select Case KeyCode
        Case vbKeyInsert
            AddRow
            Exit Sub
        Case vbKeyDelete
            DeleteRow fgParams.RowSel
        End Select
    End With
End Sub

Private Sub fgParams_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim DblVal As Double, l As Long

    On Error Resume Next
    If Row > 1 Then
        Select Case Col
            Case TSR_STRATEGY
                On Error GoTo BadDbl
                DblVal = CDbl(fgParams.EditText)
            Case TSR_SETTLEMENT
                On Error GoTo BadDbl
                Dim s As String
                Dim s1 As String
                Dim f As Double
                
                s = fgParams.EditText
                If Len(s) <> 0 And s <> "--" Then
                    f = Abs(gCmn.Str2Num(s))
                    While f > 1
                        f = f / 100
                    Wend
                    s1 = Format(f, "#,##0.0000%")
                    If s1 <> s Then
                        fgParams.EditText = f
                    End If
                End If
        End Select
    End If
    
    
    On Error Resume Next
    Cancel = False
    Exit Sub
BadDbl:
    Cancel = True
End Sub

Private Sub fgParams_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With fgParams
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

Private Sub fgParams_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    On Error Resume Next
    With fgParams
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
    Set m_SyntheticIndexBetas = New clsIndexBetaColl
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim i As Long
    On Error Resume Next
    
    With fgParams
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
        .MergeCol(TSR_UNDSYMBOL) = True
        .MergeCol(TSR_STRATEGY) = True
        .MergeCol(TSR_SETTLEMENT) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        i = TSR_UNDSYMBOL
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "symbol"
        
        .ColDataType(i) = flexDTString
                
        i = TSR_STRATEGY
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "weight"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "weight"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = "0.000000"
                                   
        i = TSR_SETTLEMENT
        .TextMatrix(0, i) = "Strike settlement" & vbCrLf & "allocation %"
        .TextMatrix(1, i) = "Strike settlement" & vbCrLf & "allocation %"
        .ColDataType(i) = vbDouble
        .ColFormat(i) = "#,##0.#0%"
                                   
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
Dim rs As ADODB.Recordset, Param As clsSyntParAtom
Dim sKey As String
    
    fgParams.ColComboList(TSR_UNDSYMBOL) = m_sContracts
    
    If m_OptionRoot.SynthParams.Count = 0 Then
        Set rs = gDBW.usp_SyntheticParams_Get(m_OptionRoot.RootID)
    
        With rs
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    Set Param = m_OptionRoot.SynthParams.Add(Str$(gCmn.ReadLng(.Fields("iUnderlyingID"))))
                    Param.UnderlyingID = gCmn.ReadLng(.Fields("iUnderlyingID"))
                    Param.Weight = gCmn.ReadDbl(.Fields("fWeight"))
                    Param.StrikeSettlement = gCmn.ReadDbl(.Fields("fStrikeSettlement"))
                    .MoveNext
                Loop
            End If
        End With
        
    End If
        
    txtCash.Text = gCmn.FmtWideDbl(m_OptionRoot.Cash)
    txtSkew.Text = gCmn.FmtDbl(m_OptionRoot.synthSkew)
    txtKurtosis.Text = gCmn.FmtDbl(m_OptionRoot.synthKurt)
    txtYield.Text = gCmn.FmtDbl(m_OptionRoot.synthYield * 100)
    
    If m_OptionRoot.IsBasket Then
        chkIsBasket.Value = 1
    Else
        chkIsBasket.Value = 0
    End If
        
    If m_OptionRoot.SynthParams.Count = 0 Then
        m_bParamsChanged = True
        imgSave.Visible = True
        sKey = Str$(m_OptionRoot.UnderlyingID)
        Set Param = m_OptionRoot.SynthParams.Add(sKey)
        Param.UnderlyingID = m_OptionRoot.UnderlyingID
        Param.Weight = 1
        Param.StrikeSettlement = BAD_DOUBLE_VALUE
    Else
        m_bParamsChanged = False
        imgSave.Visible = False
    End If
    
    For Each Param In m_OptionRoot.SynthParams
        
        With fgParams
            .AddItem ""
            
            .RowData(.Rows - 1) = New clsSymbolWithBasket
            .RowData(.Rows - 1).ContractId = Param.UnderlyingID
           
            GetRowDataByID .RowData(.Rows - 1)
            
            .TextMatrix(.Rows - 1, TSR_STRATEGY) = Param.Weight
            .TextMatrix(.Rows - 1, TSR_SETTLEMENT) = IIf(Param.StrikeSettlement <> BAD_DOUBLE_VALUE, Param.StrikeSettlement, "--")
            .Cell(flexcpText, .Rows - 1, TSR_UNDSYMBOL) = Param.UnderlyingID
            .Cell(flexcpAlignment, .Rows - 1, TSR_STRATEGY) = flexAlignRightCenter
            
            m_OptionRoot.SynthParams.Changed = True
        End With
    Next
    
    With fgParams
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
        End If
        .Redraw = flexRDBuffered
    End With
    
m_bParamsChanged = m_OptionRoot.SynthParams.Changed
    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
    Dim aBeta As clsIndexBetaAtom
    Dim Param As clsSyntParAtom, l As Long
    
    With fgParams
        If .Rows < 3 Then Err.Raise vbObjectError, "Synthetic underlying params", "Empty Underlying list."
             
        If Not IsNumeric(txtCash.Text) Then Err.Raise vbObjectError, "Synthetic underlying params", "Invalid Cash Amount value."
        m_OptionRoot.Cash = gCmn.ReadDbl(txtCash.Text)
        
        If Not IsNumeric(txtSkew.Text) Then Err.Raise vbObjectError, "Synthetic underlying params", "Invalid Synthetic Skew value."
        m_OptionRoot.synthSkew = gCmn.ReadDbl(txtSkew.Text)
        
        If Not IsNumeric(txtKurtosis.Text) Then Err.Raise vbObjectError, "Synthetic underlying params", "Invalid Synthetic Kurtosis value."
        m_OptionRoot.synthKurt = gCmn.ReadDbl(txtKurtosis.Text)
                       
        m_OptionRoot.IsBasket = (chkIsBasket.Value <> 0)
            
        m_OptionRoot.SynthParams.Clear
        For l = 2 To .Rows - 1
            Dim Data As clsSymbolWithBasket
            Set Data = .RowData(l)
            
            If Data.IsIndex And Not Data.IsBasket Then
                m_OptionRoot.IsBasket = False
                Exit For
            End If
        Next
        
        If Not m_OptionRoot.IsBasket And chkIsBasket.Value Then Err.Raise vbObjectError, "Synthetic underlying params", "Synthetic underlying contains not basket index." & vbCrLf & "Use Yield value instead of basket dividends."
        
        If m_OptionRoot.IsBasket Then
            m_OptionRoot.synthYield = 0#
        Else
            If Not IsNumeric(txtYield.Text) Then Err.Raise vbObjectError, "Synthetic underlying params", "Invalid Synthetic Yield value."
            m_OptionRoot.synthYield = gCmn.ReadDbl(txtYield.Text) * 0.01
        End If
        
        For l = 2 To .Rows - 1
            
            Set Param = m_OptionRoot.SynthParams(Str$(.RowData(l).ContractId))
            
            If Param Is Nothing Then
                Set Param = m_OptionRoot.SynthParams.Add(Str$(.RowData(l).ContractId))
                Param.UnderlyingID = .Cell(flexcpText, l, TSR_UNDSYMBOL)
                Param.Weight = .Cell(flexcpText, l, TSR_STRATEGY)
                If .Cell(flexcpText, l, TSR_SETTLEMENT) <> "" And .Cell(flexcpText, l, TSR_SETTLEMENT) <> "--" Then
                    Param.StrikeSettlement = .Cell(flexcpText, l, TSR_SETTLEMENT)
                Else
                    Param.StrikeSettlement = BAD_DOUBLE_VALUE
                End If
            Else
                Param.Weight = Param.Weight + gCmn.ReadDbl(.Cell(flexcpText, l, TSR_STRATEGY))
                Param.StrikeSettlement = Param.StrikeSettlement + gCmn.ReadDbl(.Cell(flexcpText, l, TSR_SETTLEMENT))
            End If
            
        Next
        
        If m_bBetaChanged Then
            m_OptionRoot.SyntheticIndexBetas.Changed = True
            m_OptionRoot.SyntheticIndexBetas.Clear
            
            For Each aBeta In m_SyntheticIndexBetas
                m_OptionRoot.SyntheticIndexBetas.Add Trim$(Str$(aBeta.IndexId)), aBeta
            Next
        
        End If
        
        m_OptionRoot.SynthParams.Changed = m_bParamsChanged
        m_OptionRoot.IsChanged = m_bRootChanged

    End With
    
    m_bRootChanged = False
    m_bBetaChanged = False
    m_bParamsChanged = False
    
    SaveData = True
End Function


Private Sub GetRowDataByID(RowData As clsSymbolWithBasket)
Dim i As Long, s As String, rsUnderlyings As ADODB.Recordset
On Error GoTo ErrHnd
                 
    Set rsUnderlyings = gDBW.usp_ContractSymbolAndIsBasket_Get(RowData.ContractId)
    '-------------------------------------
    
    With rsUnderlyings
        
        If .RecordCount > 0 Then
            .MoveFirst
            RowData.ContractId = !iContractID
            RowData.Symbol = !vcSymbol
            RowData.IsIndex = IIf(!iContractTypeID = 1, True, False)
            RowData.IsBasket = IIf(DBRead(!tiIsBasket, 0) = 1, True, False)
        End If
    End With
            
Exit Sub
ErrHnd:
    ShowError
End Sub



Private Sub Form_Unload(Cancel As Integer)
    On Error GoTo EH
    Screen.MousePointer = vbHourglass
    If m_bRootChanged Or m_bParamsChanged Or m_bBetaChanged Then SaveData
EX:
    Screen.MousePointer = vbDefault
    Set m_SyntheticIndexBetas = Nothing
    Exit Sub
EH:
    ShowError
    GoTo EX
End Sub

Private Sub mnuAddUnd_Click()
On Error GoTo ErrHnd
             
    AddRow

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub mnuDeleteUnd_Click()
On Error GoTo ErrHnd
                 
DeleteRow fgParams.RowSel

Exit Sub
ErrHnd:
    ShowError
End Sub

Private Sub AddRow()
    Dim Data As New clsSymbolWithBasket, Cancel As Boolean
    
    With fgParams
                        
        .AddItem ""
        .TextMatrix(.Rows - 1, TSR_SETTLEMENT) = "--"
        .TextMatrix(.Rows - 1, TSR_STRATEGY) = 1
        .Cell(flexcpText, .Rows - 1, TSR_UNDSYMBOL) = m_OptionRoot.UnderlyingID
        .Cell(flexcpAlignment, .Rows - 1, TSR_STRATEGY) = flexAlignRightCenter
        
        .RowData(.Rows - 1) = New clsSymbolWithBasket
        .RowData(.Rows - 1).ContractId = m_OptionRoot.UnderlyingID
        GetRowDataByID .RowData(.Rows - 1)
        
        .Select .Rows - 1, TSR_UNDSYMBOL
        .EditCell
    End With
    
    m_bParamsChanged = True
    imgSave.Visible = True
    
End Sub

Private Sub DeleteRow(ByVal Row As Long)
    
    With fgParams
        
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
            btnParamDel.Enabled = False
        End If
            
    End With
    
    m_bParamsChanged = True
    imgSave.Visible = True

End Sub

Private Sub txtCash_Change()
On Error Resume Next
    m_bRootChanged = True
    imgSave.Visible = True
End Sub

Private Sub txtCash_GotFocus()
    gCmn.SelectText txtCash
End Sub

Private Sub txtCash_Validate(Cancel As Boolean)
On Error Resume Next
    If Not IsNumeric(txtCash.Text) Then
        txtCash.SetFocus
        ShowWarning "Invalid Cash Amount value."
        Cancel = True
    End If
End Sub

Private Sub txtSkew_Change()
On Error Resume Next
    m_bRootChanged = True
    imgSave.Visible = True
End Sub

Private Sub txtSkew_GotFocus()
    gCmn.SelectText txtSkew
End Sub

Private Sub txtSkew_Validate(Cancel As Boolean)
On Error Resume Next
    If Not IsNumeric(txtSkew.Text) Then
        txtSkew.SetFocus
        ShowWarning "Invalid Synthetic Skew value."
        Cancel = True
    End If
End Sub

Private Sub txtKurtosis_Change()
On Error Resume Next
    m_bRootChanged = True
    imgSave.Visible = True
End Sub

Private Sub txtKurtosis_GotFocus()
    gCmn.SelectText txtKurtosis
End Sub

Private Sub txtKurtosis_Validate(Cancel As Boolean)
On Error Resume Next
    If Not IsNumeric(txtKurtosis.Text) Then
        txtKurtosis.SetFocus
        ShowWarning "Invalid Synthetic Kurtosis value."
        Cancel = True
    End If
End Sub


Private Sub txtYield_Change()
On Error Resume Next
    m_bRootChanged = True
    imgSave.Visible = True
End Sub

Private Sub txtYield_GotFocus()
    gCmn.SelectText txtSkew
End Sub

Private Sub txtYield_Validate(Cancel As Boolean)
On Error Resume Next
    If Not IsNumeric(txtSkew.Text) Then
        txtSkew.SetFocus
        ShowWarning "Invalid Synthetic Skew value."
        Cancel = True
    End If
End Sub


Private Sub btnBeta_Click()
    Dim aBeta As clsIndexBetaAtom
    On Error GoTo Herr
    
    If Not m_SyntheticIndexBetas.Changed Then
        m_SyntheticIndexBetas.Clear
        For Each aBeta In m_OptionRoot.SyntheticIndexBetas
            m_SyntheticIndexBetas.Add Trim$(Str$(aBeta.IndexId)), aBeta
        Next
    End If
    
    If frmBetas.Execute(m_OptionRoot.RootID, m_OptionRoot.Symbol, m_sIndexes, True, m_SyntheticIndexBetas) Then
        m_bBetaChanged = True
        imgSave.Visible = True
    End If

Herr:
    ShowError
End Sub
