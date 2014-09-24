VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEntTraderContractsProp 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Trader's Contracts Properties"
   ClientHeight    =   7290
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   7200
   Icon            =   "frmEntTraderContractsProp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7290
   ScaleWidth      =   7200
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fg 
      Height          =   7035
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   5535
      _cx             =   9763
      _cy             =   12409
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
      BackColorSel    =   -2147483635
      ForeColorSel    =   0
      BackColorBkg    =   -2147483636
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
      SelectionMode   =   1
      GridLines       =   1
      GridLinesFixed  =   1
      GridLineWidth   =   1
      Rows            =   50
      Cols            =   2
      FixedRows       =   1
      FixedCols       =   0
      RowHeightMin    =   0
      RowHeightMax    =   0
      ColWidthMin     =   0
      ColWidthMax     =   0
      ExtendLastCol   =   0   'False
      FormatString    =   $"frmEntTraderContractsProp.frx":000C
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
      Editable        =   2
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
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   5760
      TabIndex        =   5
      Top             =   2220
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
      Left            =   5760
      TabIndex        =   4
      Top             =   1740
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
   Begin ElladaFlatControls.FlatButton btnSelectAll 
      Height          =   300
      Left            =   5760
      TabIndex        =   1
      Top             =   120
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
      Caption         =   "&Select All"
   End
   Begin ElladaFlatControls.FlatButton btnDeselectAll 
      Height          =   300
      Left            =   5760
      TabIndex        =   2
      Top             =   600
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
      Caption         =   "&Deselect All"
   End
   Begin ElladaFlatControls.FlatButton btnStrategy 
      Height          =   300
      Left            =   5760
      TabIndex        =   3
      Top             =   1080
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
      Caption         =   "S&trategies..."
   End
   Begin VB.Image imgSave 
      Height          =   240
      Left            =   6240
      Picture         =   "frmEntTraderContractsProp.frx":0048
      Top             =   6840
      Visible         =   0   'False
      Width           =   240
   End
End
Attribute VB_Name = "frmEntTraderContractsProp"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    TSR_ISSELECT
    TSR_UNDSYMBOL
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private m_bChanged As Boolean
Private m_nTraderID As Long
Private m_sTraderName As String
Private m_collTrdUnd As clsTraderContractColl
Private m_sCurrentOriginalText As String
Private m_bSaving As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal nTraderId As Long, ByVal sTraderName As String) As Boolean
    
    On Error GoTo EH
    Screen.MousePointer = vbHourglass
    m_bChanged = False
    m_nTraderID = nTraderId
    m_sTraderName = sTraderName
    m_sCurrentOriginalText = ""
    m_bSaving = False
    
    Load Me
    
    m_collTrdUnd.Clear
    
    Me.Caption = m_sTraderName & "'s contracts"
    
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
        
    Execute = m_bChanged
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnDeselectAll_Click()
    Dim i&, nCount&, aTrdCnt As clsTraderContractAtom
    On Error Resume Next
    nCount = fg.Rows - 1
    With fg
        For i = 2 To nCount
            .Cell(flexcpChecked, i, TSR_ISSELECT) = flexUnchecked
            Set aTrdCnt = .RowData(i)
            If Not aTrdCnt Is Nothing Then
                If aTrdCnt.TraderId <> -1& Then m_bChanged = True
                If aTrdCnt.TraderId = m_nTraderID Then aTrdCnt.TraderId = -1&
            End If
        Next
    End With
    If m_bChanged Then imgSave.Visible = True
End Sub

Private Sub btnOK_Click()
    On Error GoTo EH
    
    Screen.MousePointer = vbHourglass
    m_bSaving = True
    EnableControls = False
    If m_bChanged Then SaveData
    EnableControls = True
    m_bSaving = False
    Screen.MousePointer = vbDefault
    Hide
    Exit Sub
EH:
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

Private Sub btnSelectAll_Click()
    Dim i&, nCount&, aTrdCnt As clsTraderContractAtom
    On Error Resume Next
    nCount = fg.Rows - 1
    With fg
        For i = 2 To nCount
            .Cell(flexcpChecked, i, TSR_ISSELECT) = flexChecked
            Set aTrdCnt = .RowData(i)
            If Not aTrdCnt Is Nothing Then
                If aTrdCnt.TraderId <> m_nTraderID Then
                    m_bChanged = True
                    aTrdCnt.TraderId = m_nTraderID
                End If
            End If
        Next
    End With
    If m_bChanged Then imgSave.Visible = True
End Sub

Private Sub UpdateStrategyButtonState()
    On Error Resume Next
    btnStrategy.Enabled = (fg.Cell(flexcpChecked, fg.Row, TSR_ISSELECT) = flexChecked)
End Sub

Private Sub btnStrategy_Click()
    On Error Resume Next
    Dim aTrdCnt As clsTraderContractAtom
    
    If fg.Cell(flexcpChecked, fg.Row, TSR_ISSELECT) = flexChecked Then
        Set aTrdCnt = fg.RowData(fg.Row)
        If Not aTrdCnt Is Nothing Then
            frmEntTraderContractProp.Execute m_nTraderID, m_sTraderName, _
                                        aTrdCnt.ContractId, aTrdCnt.ContractSymbol
        End If
    End If
End Sub

Private Sub fg_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    Dim aTrdCnt As clsTraderContractAtom, nNewId As Long
    Dim sValue$

    On Error Resume Next
    
    With fg
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
      
            Set aTrdCnt = .RowData(Row)
            
            If Not aTrdCnt Is Nothing Then
                If Col = TSR_ISSELECT Then
                    nNewId = IIf(.Cell(flexcpChecked, Row, TSR_ISSELECT) = flexChecked, m_nTraderID, -1&)
                    aTrdCnt.TraderId = nNewId
                    m_bChanged = True
                End If
            End If
        End If
    End With
    
    If m_bChanged Then imgSave.Visible = True
    
    Exit Sub
End Sub

Private Sub fg_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    UpdateStrategyButtonState
End Sub

Private Sub fg_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    
    Cancel = True
    With fg
        .ComboList = ""
        If Col = TSR_ISSELECT Then
            Cancel = False
        End If
    End With
End Sub

Private Sub fg_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    Dim aTrdCnt As clsTraderContractAtom
    On Error Resume Next
    
    Cancel = True
    With fg
        If Col = TSR_ISSELECT Then
            Cancel = False
        End If
        
        If Not Cancel Then m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
    End With
End Sub

Private Sub fg_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal y As Single, Cancel As Boolean)
    On Error Resume Next
    With fg
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
    Set m_collTrdUnd = New clsTraderContractColl
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim i As Long
    On Error Resume Next
    
    With fg
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
        .MergeCol(TSR_ISSELECT) = True
        .MergeCol(TSR_UNDSYMBOL) = True
       
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = TSR_ISSELECT
        .TextMatrix(0, i) = "Is Trader's" & vbCrLf & "Contract"
        .TextMatrix(1, i) = "Is Trader's" & vbCrLf & "Contract"
        .ColDataType(i) = flexDTBoolean
               
        i = TSR_UNDSYMBOL
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "symbol"
        .ColDataType(i) = flexDTString
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset, aTrdCnt As clsTraderContractAtom
    Dim sKey$, nId&
                
    Set rs = gDBW.usp_BoTraderUnderlyings_Get(m_nTraderID, False)
    While Not rs.EOF
        sKey = gCmn.ReadStr(rs!iContractID)
        Set aTrdCnt = m_collTrdUnd(sKey)
        If aTrdCnt Is Nothing Then
            Set aTrdCnt = m_collTrdUnd.Add(sKey)
            aTrdCnt.TraderId = IIf(gCmn.ReadByte(rs!tiIsSelected) = 1, m_nTraderID, 0)
            aTrdCnt.ContractId = gCmn.ReadLng(rs!iContractID)
            aTrdCnt.ContractSymbol = gCmn.ReadStr(rs!vcUndSymbol)
            Set aTrdCnt = Nothing
        End If
        rs.MoveNext
    Wend
    Set rs = Nothing
                
    With fg
        For Each aTrdCnt In m_collTrdUnd
            .Redraw = flexRDNone
            
            .AddItem ""
            
            .RowData(.Rows - 1) = aTrdCnt
            .Cell(flexcpChecked, .Rows - 1, TSR_ISSELECT) = IIf(aTrdCnt.TraderId = m_nTraderID, flexChecked, flexUnchecked)
            .Cell(flexcpText, .Rows - 1, TSR_UNDSYMBOL) = aTrdCnt.ContractSymbol
        Next
    End With
    
    With fg
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
        End If
        UpdateStrategyButtonState
        .Redraw = flexRDBuffered
    End With
    

    LoadData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
    Dim aTrdCnt As clsTraderContractAtom, aStr As clsStrategyAtom, nRet&, Msg$
        
    SaveData = True
    For Each aTrdCnt In m_collTrdUnd
        DoEvents
        
        If aTrdCnt.TraderId = m_nTraderID Then
            Me.Caption = m_sTraderName & "'s contracts - Saving data. Processing '" & aTrdCnt.ContractSymbol & "'"
            gDBW.usp_BoTraderContract_Save m_nTraderID, DBWrite(aTrdCnt.ContractId, 0)
        ElseIf aTrdCnt.TraderId = -1& Then
            Me.Caption = m_sTraderName & "'s contracts - Saving data. Processing '" & aTrdCnt.ContractSymbol & "'"
            nRet = gDBW.usp_BoTraderContract_Del(m_nTraderID, DBWrite(aTrdCnt.ContractId, 0))
            If nRet <> 0 Then
                SaveData = False
                Select Case nRet
                    Case -1
                        Msg = "Can't delete '" & aTrdCnt.ContractSymbol & "' from '" & m_sTraderName & "' contracts. There are trades on this contract" & vbCrLf & "Continue execution?"
                    Case -2
                        Msg = "Can't delete '" & aTrdCnt.ContractSymbol & "' from '" & m_sTraderName & "' contracts. There are orders on this contract" & vbCrLf & "Continue execution?"
                    Case Default
                        Msg = "Can't delete '" & aTrdCnt.ContractSymbol & "' from '" & m_sTraderName & "' contracts." & vbCrLf & "Continue execution?"
                End Select
                If gCmn.MyMsgBox(Msg, vbExclamation + vbYesNo) = vbNo Then Exit Function
            End If
        End If
    Next
End Function

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_bSaving Then Cancel = True
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error GoTo EH
    Set m_collTrdUnd = Nothing
    Exit Sub
EH:
    ShowError
End Sub

Private Property Let EnableControls(ByVal blnEnabled As Boolean)
    On Error Resume Next
    Dim ctlBox As Variant
    For Each ctlBox In Me.Controls
        If TypeOf ctlBox Is ElladaFlatControls.FlatButton Then
            ctlBox.Enabled = blnEnabled
        End If
    Next
End Property

