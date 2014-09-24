VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmRootList 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Underlying Option Roots"
   ClientHeight    =   3720
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   7110
   Icon            =   "frmRootList.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3720
   ScaleWidth      =   7110
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgRoots 
      Height          =   3495
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5415
      _cx             =   9551
      _cy             =   6165
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
      Begin VB.Image imgProp 
         Height          =   180
         Left            =   3360
         Picture         =   "frmRootList.frx":000C
         Top             =   240
         Visible         =   0   'False
         Width           =   180
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   5640
      TabIndex        =   1
      Top             =   1080
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
      Left            =   5640
      TabIndex        =   2
      Top             =   600
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
   Begin ElladaFlatControls.FlatButton btnSyntProp 
      Height          =   300
      Left            =   5640
      TabIndex        =   3
      Top             =   120
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
      Caption         =   "&Properties"
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   6720
      Picture         =   "frmRootList.frx":0059
      Top             =   3360
      Visible         =   0   'False
      Width           =   240
   End
   Begin VB.Menu mnuParams 
      Caption         =   "Synthetic Params"
      Visible         =   0   'False
      Begin VB.Menu mnuSyntheticPar 
         Caption         =   "Synthetic Params"
      End
   End
End
Attribute VB_Name = "frmRootList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

' form state constants
Private Enum eState
    STATE_NONE
    STATE_LOAD
    STATE_EDIT
    STATE_NEW
    STATE_DELETE
End Enum

Private Enum GridColumns
    IDX_ROOTSYMBOL = 0
    IDX_UNDTYPE
    IDX_FITTYPE
    IDX_LOTSIZE
    IDX_SYNT_PAR       '  Uncomment if SU works correctly
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private mState As eState

' row conditions
Private Const MSTR_ADD = "A"
Private Const MSTR_EDIT = "E"
Private Const MSTR_DELETE = "D"
Private Const MSTR_NONE = ""

Private m_ItemID As Long
Private m_ItemName As String
Private m_bOK As Boolean
Private rsList As ADODB.Recordset
Private m_Edit As Boolean
Private m_sContracts As String
Private m_sIndexes As String
Private m_ContractTypeID As Long

Private m_Roots As New clsOptRootColl

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal ItemID As Long, ByVal ContractTypeID&, ByVal ItemName As String, _
        ByRef sContracts As String, ByRef sIndexes As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_ItemID = ItemID
    m_ContractTypeID = ContractTypeID
    m_ItemName = ItemName
    m_sContracts = sContracts
    m_sIndexes = sIndexes
    Load Me
    Me.Caption = "Option Roots for " & m_ItemName
    m_bOK = False
    m_Edit = False
    If LoadData Then
        Screen.MousePointer = vbDefault
        m_Edit = True
        Show vbModal
    End If
    Unload Me
    Execute = m_bOK
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnOK_Click()
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_bOK = SaveData
    Screen.MousePointer = vbDefault
    If m_bOK Then Hide
    Exit Sub
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub


Private Sub btnSyntProp_Click()
On Error Resume Next
    With fgRoots
        If frmSyntUndPar.Execute(m_Roots(.RowData(.Row)), m_sContracts, m_sIndexes) Then imgSave.Visible = True         '  Uncomment if SU works correctly
    End With
End Sub

Private Sub fgRoots_AfterEdit(ByVal Row As Long, ByVal Col As Long)
Dim Root As clsOptRootAtom, Root2 As clsOptRootAtom
Dim aRoot As clsOptRootAtom
    On Error Resume Next
    If Col = IDX_UNDTYPE And Row > 1 Then
        
        Set Root = m_Roots(fgRoots.RowData(Row))
        
        If fgRoots.Cell(flexcpText, Row, IDX_UNDTYPE) = 0 Then
            fgRoots.Cell(flexcpPicture, Row, IDX_SYNT_PAR) = Nothing    '  Uncomment if SU works correctly
                        
            If Root.IsSynthetic Then
                Root.IsSynthetic = False
                Root.IsChanged = True
            End If
        Else
            If Not Root.IsSynthetic Then
                Root.IsSynthetic = True
                Root.IsBasket = True
                Root.IsChanged = True
                If Root.SynthParams.Count = 0 Then
                    If frmSyntUndPar.Execute(Root, m_sContracts, m_sIndexes) Then imgSave.Visible = True
                End If
                                
            End If
            
            fgRoots.Cell(flexcpPicture, Row, IDX_SYNT_PAR) = imgProp.Picture                            '  Uncomment if SU works correctly
            fgRoots.Cell(flexcpPictureAlignment, Row, IDX_SYNT_PAR) = flexPicAlignRightCenter           '  Uncomment if SU works correctly
        End If
    ElseIf Col = IDX_FITTYPE And Row > 1 Then
        Set Root = m_Roots(fgRoots.RowData(Row))
        Dim dCount As Long: dCount = 0
        If Root.IsFittingEnabled <> IIf(fgRoots.TextMatrix(Row, Col) <> 0, True, False) Then
            Root.IsFittingEnabled = IIf(fgRoots.TextMatrix(Row, Col) <> 0, True, False)
            For Each aRoot In m_Roots
                If aRoot.IsFittingEnabled Then dCount = dCount + 1
            Next
            If dCount > 0 Then
                btnOK.Enabled = True
            Else
                btnOK.Enabled = False
            End If
            Root.IsChanged = True
        End If
    ElseIf Col = IDX_LOTSIZE And Row > 1 Then
                    
        Set Root = m_Roots(fgRoots.RowData(Row))
        
        If Root.LotSize <> fgRoots.TextMatrix(Row, Col) Then
            Root.LotSize = fgRoots.TextMatrix(Row, Col)
            Root.IsChanged = True
        End If
        
    ElseIf Col = IDX_ROOTSYMBOL And Row > 1 Then
                    
        Set Root = m_Roots(fgRoots.RowData(Row))
        
        If Root.Symbol <> fgRoots.TextMatrix(Row, Col) Then
            
            For Each Root2 In m_Roots
                If Root2.RootID <> Root.RootID Then
                    If Root2.Symbol = fgRoots.TextMatrix(Row, Col) And Root2.NewRootID = 0& Then
                        Root.NewRootID = Root2.RootID
                        Root2.CanMove = False
                        fgRoots.RemoveItem Row
                        Exit For
                    End If
                End If
            Next
        
            Root.Symbol = fgRoots.TextMatrix(Row, Col)
            Root.IsChanged = True
        End If
        
    End If
    
    If Root.IsChanged Then imgSave.Visible = True
End Sub

Private Sub fgRoots_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    With fgRoots
        If NewRow > -1 And NewCol > -1 Then
            If .Cell(flexcpText, NewRow, IDX_UNDTYPE) = 0 Then
                btnSyntProp.Enabled = False
            Else
                 btnSyntProp.Enabled = True   ' uncomment if SU works correctly
            End If
        End If
    End With
End Sub

Private Sub fgRoots_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    With fgRoots
        If .Cell(flexcpText, .Row, IDX_UNDTYPE) = 0 Then
            btnSyntProp.Enabled = False
        Else
             btnSyntProp.Enabled = True '  uncomment if SU works correctly
        End If
    End With
End Sub

Private Sub fgRoots_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgRoots
        Select Case KeyCode
        Case vbKeyReturn
            If .Row > 1 And .Col = IDX_SYNT_PAR And .Cell(flexcpText, .Row, IDX_UNDTYPE) = 1 Then                                 '  Uncomment if SU works correctly
                If frmSyntUndPar.Execute(m_Roots(.RowData(.Row)), m_sContracts, m_sIndexes) Then imgSave.Visible = True           '  Uncomment if SU works correctly
            End If
        End Select
    End With
End Sub

Private Sub fgRoots_MouseUp(Button As Integer, Shift As Integer, X As Single, y As Single)
    On Error Resume Next
    With fgRoots
        If (Button And vbRightButton) And .Row > -1 And .Col > -1 Then
            If .Cell(flexcpText, .Row, IDX_UNDTYPE) = 0 Then
                mnuSyntheticPar.Enabled = False
            Else
                mnuSyntheticPar.Enabled = True
            End If
            PopupMenu mnuParams, , , , mnuSyntheticPar
        End If
    End With
End Sub

Private Sub fgRoots_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    
    Select Case Col
        Case IDX_ROOTSYMBOL
            Cancel = False
        Case IDX_SYNT_PAR       '  Uncomment if SU works correctly
            Cancel = True
        Case IDX_UNDTYPE
            Cancel = (m_ContractTypeID <> 2) 'only stock can have synthetic root
        Case IDX_LOTSIZE
            Cancel = False
        Case IDX_FITTYPE
            Cancel = False
        Case Else
            Cancel = True
    End Select
End Sub

Private Sub fgRoots_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
Dim Row As Long

On Error Resume Next
    With fgRoots
        ' Opening property window
        If (Button And vbLeftButton) Then
            If X >= (.Cell(flexcpLeft, , .Cols - 1) + .Cell(flexcpWidth, , .Cols - 1) - imgProp.Width) Then
                Row = .MouseRow
                
                If .Cell(flexcpText, Row, IDX_UNDTYPE) = 1 Then
                    
                    If frmSyntUndPar.Execute(m_Roots(.RowData(Row)), m_sContracts, m_sIndexes) Then imgSave.Visible = True
                        
                End If
            End If
        ElseIf (Button And vbRightButton) Then
            If .MouseRow >= 2 Then
                .Row = .MouseRow
                .Col = .MouseCol
            End If
        End If
    End With
End Sub

Private Sub fgRoots_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim IntVal As Integer
Dim Root, Root2 As clsOptRootAtom
    On Error Resume Next
    
    If Col = IDX_LOTSIZE And Row > 1 Then
        On Error GoTo BadInt
            IntVal = CInt(fgRoots.EditText)
            If IntVal <= 0 Then GoTo BadInt
    ElseIf Col = IDX_ROOTSYMBOL And Row > 1 Then
        If Len(fgRoots.EditText) < 1 Then
            Cancel = True
            Exit Sub
        End If
        
        Set Root = m_Roots(fgRoots.RowData(Row))
        
        For Each Root2 In m_Roots
            If Root2.RootID <> Root.RootID Then
                If Root2.Symbol = fgRoots.EditText Then
                    
                    If Not Root.CanMove Then
                        Cancel = True
                        Exit Sub
                    End If
                    
                    If gCmn.MyMsgBox("Do you want to move all options of this root to different root?", vbYesNo) <> vbYes Then
                        Cancel = True
                        Exit Sub
                    End If
                    
                    Exit For
                End If
            End If
        Next
    End If
    On Error Resume Next
    Cancel = False
    Exit Sub
BadInt:
    Cancel = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    Timer1.Enabled = False
    Timer1.Interval = 1
    
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim i As Long
    On Error Resume Next
    
    'btnSyntProp.Enabled = False ' Delete if SU works correctly
    
    With fgRoots
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
        .MergeCol(IDX_ROOTSYMBOL) = True
        .MergeCol(IDX_UNDTYPE) = True
        .MergeCol(IDX_FITTYPE) = True
        .MergeCol(IDX_LOTSIZE) = True
        .MergeCol(IDX_SYNT_PAR) = True      '  Uncomment if SU works correctly
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = IDX_ROOTSYMBOL
        .TextMatrix(0, i) = "Symbol"
        .TextMatrix(1, i) = "Symbol"
        .ColDataType(i) = flexDTString
                
        i = IDX_FITTYPE
        .TextMatrix(0, i) = "Use when" & vbCrLf & "Fitting to IV"
        .TextMatrix(1, i) = "Use when" & vbCrLf & "Fitting to IV"
        .ColDataType(i) = flexDTBoolean
                       
        i = IDX_UNDTYPE
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "Type"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "Type"
        .ColDataType(i) = flexDTString
        .ColComboList(i) = "#0;Common|#1;Synthetic"        '  Unchange back if SU works correctly
               
        i = IDX_LOTSIZE
        .TextMatrix(0, i) = "LotSize /" & vbCrLf & "Contract multiplier"
        .TextMatrix(1, i) = "LotSize /" & vbCrLf & "Contract multiplier"
        .ColWidth(i) = 1500
        .EditWidth(i) = 1500
        .ColDataType(i) = flexDTString
        .ColFormat(i) = "0"
        
        i = IDX_SYNT_PAR                                            '  Uncomment if SU works correctly
        .TextMatrix(0, i) = "Synthetic" & vbCrLf & "Params"         '  Uncomment if SU works correctly
        .TextMatrix(1, i) = "Synthetic" & vbCrLf & "Params"         '  Uncomment if SU works correctly
        .ColDataType(i) = flexDTString                              '  Uncomment if SU works correctly
        .ColFormat(i) = "0"                                         '  Uncomment if SU works correctly
            
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here load Option Roots Properties from DB
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset
    Dim nRow As Long
    Dim sKey As String
    Dim Root As clsOptRootAtom
        
    Set rs = gDBW.usp_OptionRoot_Get(Null, m_ItemID)
    
    m_Roots.Clear
    With rs
        If .RecordCount > 0 Then
            
            .MoveFirst
            Do While Not .EOF
                sKey = Str$(gCmn.ReadLng(.Fields("iRootID")))
                Set Root = m_Roots.Add(sKey)
                Root.IsChanged = False
                        
                Root.Symbol = gCmn.ReadStr(.Fields("vcSymbol"))
                Root.RootID = gCmn.ReadLng(.Fields("iRootID"))
                Root.UnderlyingID = gCmn.ReadLng(.Fields("iUnderlyingID"))
                Root.LotSize = gCmn.ReadLng(.Fields("iLotSize"))
                Root.Cash = gCmn.ReadDbl(.Fields("fCash"))
                Root.synthSkew = gCmn.ReadDbl(.Fields("fSyntheticSkew"))
                Root.synthKurt = gCmn.ReadDbl(.Fields("fSyntheticKurt"))
                Root.synthYield = gCmn.ReadDbl(.Fields("fSyntheticYield"))
                Root.IsBasket = gCmn.ReadByte(.Fields("tiSyntheticIsBasket"))
                                         
                Root.IsSynthetic = gCmn.ReadByte(.Fields("tiIsSynthetic"))
                Root.IsFittingEnabled = gCmn.ReadByte(.Fields("tiIsFitEnabled"))
                
                .MoveNext
            Loop
        End If
    End With
    
    nRow = 2
    For Each Root In m_Roots
                        
        With fgRoots
            .AddItem ""
                        
            .RowData(nRow) = Str$(Root.RootID)
            
            .TextMatrix(nRow, IDX_ROOTSYMBOL) = Root.Symbol
            .TextMatrix(nRow, IDX_FITTYPE) = Root.IsFittingEnabled
                                               
            .TextMatrix(nRow, IDX_LOTSIZE) = Root.LotSize
            .Cell(flexcpAlignment, nRow, IDX_LOTSIZE) = flexAlignRightCenter
            
            If Root.IsSynthetic Then
                .Cell(flexcpText, nRow, IDX_UNDTYPE) = 1                                            '  Uncomment if SU works correctly
                .Cell(flexcpPicture, nRow, IDX_SYNT_PAR) = imgProp.Picture                          '  Uncomment if SU works correctly
                .Cell(flexcpPictureAlignment, nRow, IDX_SYNT_PAR) = flexPicAlignRightCenter         '  Uncomment if SU works correctly
            Else
                .Cell(flexcpText, nRow, IDX_UNDTYPE) = 0
            End If
        End With
        nRow = nRow + 1
    Next
    
    With fgRoots
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
        End If
        .Redraw = flexRDBuffered
    End With
    
    
    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Here save Option Roots Properties to DB
Private Function SaveData() As Boolean
    Dim Root As clsOptRootAtom, SyntPar As clsSyntParAtom, aBeta As clsIndexBetaAtom, rs As ADODB.Recordset
                        
    For Each Root In m_Roots
        If Root.IsChanged Then
        
            If Root.NewRootID > 0& Then
                Set rs = gDBW.usp_OptionRootMove_Save(Root.RootID, Root.NewRootID)
                  
                With rs
                    If .RecordCount > 0 Then
                        Dim sErrMsg As String
                        
                        sErrMsg = "Can't move roots options because there exists pairs of duplicated options : "
                        sErrMsg = sErrMsg & vbCrLf
                        .MoveFirst
                        Do While Not .EOF And Len(sErrMsg) < 1000
                            sErrMsg = sErrMsg & "'"
                            sErrMsg = sErrMsg & gCmn.ReadStr(.Fields("vcOptSymbol1"))
                            sErrMsg = sErrMsg & "' and '"
                            sErrMsg = sErrMsg & gCmn.ReadStr(.Fields("vcOptSymbol2"))
                            sErrMsg = sErrMsg & "'" & vbCrLf
                            .MoveNext
                        Loop
                        
                        If Not rs.EOF Then
                            sErrMsg = sErrMsg & "..."
                        End If
                                           
                        gCmn.MyMsgBox sErrMsg, vbCritical
                    End If
                End With
            Else
                gDBW.usp_OptionRoot_Save Root.RootID, Root.Symbol, Root.LotSize, Root.IsFittingEnabled, Root.IsSynthetic, _
                        Root.Cash, Root.synthSkew, Root.synthKurt, Root.synthYield, Root.IsBasket
            
            End If
        End If
        
        If Root.SynthParams.Changed Then
        
            gDBW.usp_SyntheticParams_Del Root.RootID, Null
            
            If Root.IsSynthetic Then
                For Each SyntPar In Root.SynthParams
                    gDBW.usp_SyntheticParam_Save Root.RootID, SyntPar.UnderlyingID, SyntPar.Weight, SyntPar.StrikeSettlement
                Next
            End If
            
        End If
        If Root.SyntheticIndexBetas.Changed Then
        
            gDBW.usp_SyntheticIndexBeta_Del Root.RootID, Null
            
            If Root.IsSynthetic Then
                For Each aBeta In Root.SyntheticIndexBetas
                    gDBW.usp_SyntheticIndexBeta_Save Root.RootID, aBeta.IndexId, aBeta.Beta
                Next
            End If
            
        End If
    Next
    
    SaveData = True
End Function

Private Sub mnuSyntheticPar_Click()
On Error Resume Next
    Timer1.Enabled = True  ' uncomment if SU work correctly
End Sub

Private Sub Timer1_Timer()
    On Error Resume Next
    Timer1.Enabled = False
    With fgRoots
        If frmSyntUndPar.Execute(m_Roots(.RowData(.Row)), m_sContracts, m_sIndexes) Then imgSave.Visible = True
    End With
End Sub

