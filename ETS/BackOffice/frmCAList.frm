VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmCAList 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Corporate Actions List"
   ClientHeight    =   5130
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6405
   Icon            =   "frmCAList.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5130
   ScaleWidth      =   6405
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgCorpActions 
      Height          =   4455
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   6135
      _cx             =   10821
      _cy             =   7858
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
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Height          =   300
      Left            =   4920
      TabIndex        =   1
      Top             =   4680
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
      Caption         =   "&Close"
   End
   Begin ElladaFlatControls.FlatButton btnRollback 
      Height          =   300
      Left            =   3360
      TabIndex        =   3
      Top             =   4680
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
      Caption         =   "&Rollback"
   End
   Begin VB.Label lblCount 
      BackStyle       =   0  'Transparent
      Caption         =   "Total: 0 Corporate Action(s)"
      Height          =   300
      Left            =   120
      TabIndex        =   2
      Top             =   4800
      Width           =   2175
   End
End
Attribute VB_Name = "frmCAList"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Enum GridColumns
    CA_UNDSYMBOL
    CA_TYPE
    CA_DATE
    CA_STATUS
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private rsList As ADODB.Recordset
Private m_Edit As Boolean

Private Const nTimeOut = 180 'sec
Private nLastErrorNumber As Long
Private sLastErrorDescription As String
Private sLastErrorSource As String
Private nLastStatus As Long
Private bBusy As Boolean
Private bCancel As Boolean

Private WithEvents m_CN As ADODB.Connection
Attribute m_CN.VB_VarHelpID = -1

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As Variant
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    DoEvents
    Load Me
    m_Edit = False
    If LoadData Then
        Screen.MousePointer = vbDefault
        Show vbModal
    End If
    Unload Me
    Execute = m_Edit
    Exit Function
Herr:
    Screen.MousePointer = vbDefault
    ShowError
End Function

Private Sub btnRollback_Click()
    On Error Resume Next
    If bBusy Then
        bCancel = True
    Else
        RollbackCA
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnClose_Click()
    On Error Resume Next
    If Not bBusy Then
        Me.Hide
        Unload Me
    End If
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
    Dim i As Long
    On Error Resume Next
    
    With fgCorpActions
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
        .FixedCols = 0
        .FixedRows = 2
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .MergeRow(1) = True
        .MergeCol(CA_UNDSYMBOL) = True
        .MergeCol(CA_TYPE) = True
        .MergeCol(CA_DATE) = True
        .MergeCol(CA_STATUS) = True
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
               
        i = CA_UNDSYMBOL
        .TextMatrix(0, i) = "Underlying" & vbCrLf & "symbol"
        .TextMatrix(1, i) = "Underlying" & vbCrLf & "symbol"
        
        .ColDataType(i) = flexDTString
                
        i = CA_TYPE
        .TextMatrix(0, i) = "Corporate" & vbCrLf & "action type"
        .TextMatrix(1, i) = "Corporate" & vbCrLf & "action type"
        
        .ColDataType(i) = flexDTString
        
        
        i = CA_DATE
        .TextMatrix(0, i) = "Effective" & vbCrLf & "date"
        .TextMatrix(1, i) = "Effective" & vbCrLf & "date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = "MM/DD/YYYY"
        
        i = CA_STATUS
        .TextMatrix(0, i) = "Corporate" & vbCrLf & "action state"
        .TextMatrix(1, i) = "Corporate" & vbCrLf & "action state"
        
        .ColDataType(i) = flexDTString
        
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        .Cell(flexcpFontBold, 0, 0, 1, LAST_COLUMN) = True
                                   
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset, sStatus As String
    
    ' clearing option grid
    fgCorpActions.Redraw = flexRDNone
    fgCorpActions.Rows = 2
    
    Set rs = gDBW.usp_CorporateAction_Get(Null, Null, Null, Null, Null)
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                fgCorpActions.AddItem ""
                
                fgCorpActions.RowData(fgCorpActions.Rows - 1) = gCmn.ReadLng(.Fields("iCorpActionID"))
                                        
                fgCorpActions.TextMatrix(fgCorpActions.Rows - 1, CA_UNDSYMBOL) = gCmn.ReadStr(.Fields("vcUnderlyingSymbol"))
                fgCorpActions.TextMatrix(fgCorpActions.Rows - 1, CA_TYPE) = gCmn.ReadStr(.Fields("vcTypeName"))
                fgCorpActions.TextMatrix(fgCorpActions.Rows - 1, CA_DATE) = gCmn.ReadDate(.Fields("dtEffectiveDate"))
                Select Case gCmn.ReadLng(.Fields("iStatusID"))
                    Case 1
                        sStatus = "Active"
                    Case 2
                        sStatus = "Complete"
                    Case 3
                        sStatus = "Incorrect"
                End Select
                
                fgCorpActions.TextMatrix(fgCorpActions.Rows - 1, CA_STATUS) = sStatus
                
                .MoveNext
            Loop
        End If
    End With
    
    lblCount.Caption = "Total: " & fgCorpActions.Rows - 2 & " corporate action(s)"
    
    If fgCorpActions.Rows > 2 Then
        fgCorpActions.Row = 2
        btnRollback.Enabled = True
    Else
        btnRollback.Enabled = False
    End If
    
    fgCorpActions.Redraw = flexRDBuffered

    LoadData = True
End Function

Private Sub fgCorpActions_BeforeEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    Cancel = True
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    InitGrid
    
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If UnloadMode = vbFormControlMenu Then
        Cancel = bBusy
    End If
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    LockWindowUpdate Me.hwnd
    
        
    With fgCorpActions
        .ColWidth(CA_UNDSYMBOL) = 0.2 * .Width - 100
        .ColWidth(CA_TYPE) = 0.35 * .Width - 100
        .ColWidth(CA_DATE) = 0.2 * .Width - 100
        .ColWidth(CA_STATUS) = 0.25 * .Width - 100
    End With
    
    btnClose.Top = ScaleHeight - btnClose.Height - 150
    btnClose.Left = ScaleWidth - btnClose.Width - 150
    
    btnRollback.Move btnClose.Left - btnClose.Width - 150, btnClose.Top
    
    lblCount.Move 150, btnClose.Top
    
    fgCorpActions.Move 150, 150, ScaleWidth - 300, btnClose.Top - 300
    
    LockWindowUpdate 0
End Sub

Private Sub m_CN_ExecuteComplete(ByVal RecordsAffected As Long, ByVal pError As ADODB.Error, adStatus As ADODB.EventStatusEnum, ByVal pCommand As ADODB.Command, ByVal pRecordset As ADODB.Recordset, ByVal pConnection As ADODB.Connection)
    On Error Resume Next
    bBusy = False
    If Not pError Is Nothing Then
        nLastErrorNumber = pError.Number
        sLastErrorDescription = pError.Description
        Exit Sub
    End If
    nLastStatus = adStatusOK
End Sub



Public Sub Idle()
    DoEvents
    Sleep 10
    If bBusy And bCancel Then
        Err.Raise vbObjectError, , "Operation calceled"
    End If
End Sub

Private Function GetLogCARollbackCommand( _
    ByVal CorpActionID As Long _
) As ADODB.Command
    Debug.Print "usp_LogCA_Rollback", CorpActionID
    Set GetLogCARollbackCommand = New ADODB.Command
    With GetLogCARollbackCommand
        Set .ActiveConnection = m_CN
        .CommandText = "usp_LogCA_Rollback"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(CorpActionID, 0)
        End With
    End With
End Function


Private Sub RollbackCA()
    Dim iAnswr As VbMsgBoxResult
    Dim tStart As Date
    Dim CM As ADODB.Command
    
    On Error GoTo Herr
    
    With fgCorpActions
    
        If fgCorpActions.Rows <= 2 Then Exit Sub
        
        iAnswr = vbYes
        iAnswr = gCmn.MyMsgBox("Rollback Corrporate action for '" & _
            .TextMatrix(.Row, CA_UNDSYMBOL) & "' underlying?", vbYesNo + vbQuestion)
        
        If iAnswr = vbYes Then
            nLastErrorNumber = 0
            sLastErrorDescription = ""
            nLastStatus = 0
               
            bBusy = True
            bCancel = False
            btnRollback.Caption = "Cancel"
            btnClose.Enabled = False
            Screen.MousePointer = vbHourglass
            
            tStart = Now
             
            Set m_CN = gDBW.Connection
            
            Set CM = GetLogCARollbackCommand(.RowData(.Row))
            With CM
                .CommandTimeout = nTimeOut
                ' Execute SQL request asynchronously
                .Execute , , adAsyncExecute
            End With

           
            On Error GoTo Failed
            ' Wait for the request completion
            Do
                If Not bBusy Then
                    Exit Do
                End If
                If DateDiff("s", tStart, Now) > nTimeOut Then
                    Err.Raise vbObject, "Corporate Actions List", "Rollback of Corporate Action is timeout expired"
                End If
                Idle
            Loop
            
            On Error GoTo Herr
            If nLastStatus = adStatusOK Then
                Set m_CN = Nothing
            
                btnRollback.Caption = "&Rollback"
                btnClose.Enabled = True
                Screen.MousePointer = vbDefault
                
                LoadData
                m_Edit = True
            Else
                Err.Raise nLastErrorNumber, "Corporate Actions List", "Rollback of Corporate Action is not available" 'sLastErrorDescription
            End If
                
        End If
    End With
    Exit Sub
Failed:
    Dim n, s
    n = Err.Number
    s = Err.Description
    On Error Resume Next
    m_CN.Cancel
    Set m_CN = Nothing
    On Error GoTo Herr
    Err.Clear
    Err.Raise n, "Corporate Actions List", s
Herr:
    ShowError
    bBusy = False
    btnRollback.Caption = "&Rollback"
    btnClose.Enabled = True
    Screen.MousePointer = vbDefault
    
    Set m_CN = Nothing
End Sub

