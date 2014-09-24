VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmFiltersEdit 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Filters"
   ClientHeight    =   2610
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   4485
   Icon            =   "frmFiltersEdit.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   2610
   ScaleWidth      =   4485
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   3000
      TabIndex        =   0
      Top             =   2160
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
   Begin ElladaFlatControls.FlatButton btnApply 
      Height          =   300
      Left            =   1560
      TabIndex        =   1
      Top             =   2160
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
      Caption         =   "&Apply"
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   120
      TabIndex        =   2
      Top             =   2160
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
   Begin VSFlex7Ctl.VSFlexGrid grd 
      Height          =   1995
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   4215
      _cx             =   7435
      _cy             =   3519
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
      FormatString    =   $"frmFiltersEdit.frx":000C
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
   Begin VB.Image imgProp 
      Height          =   180
      Left            =   4320
      Picture         =   "frmFiltersEdit.frx":0048
      Top             =   2160
      Visible         =   0   'False
      Width           =   180
   End
   Begin VB.Menu mnuContext 
      Caption         =   "Context"
      Visible         =   0   'False
      Begin VB.Menu mnuAdd 
         Caption         =   "&Add"
      End
      Begin VB.Menu mnuDelete 
         Caption         =   "&Delete"
      End
      Begin VB.Menu mnuEdit 
         Caption         =   "&Edit"
      End
   End
End
Attribute VB_Name = "frmFiltersEdit"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Edit Filters List
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

' column numbers
Private Const MINT_IDCOL = 0
Private Const MINT_CONDITIONCOL = 1
Private Const MINT_COL1 = 2

Private msTempID As String
Private msTempCol1 As String
Private msTempCond As String

' row conditions
Private Const MSTR_ADD = "A"
Private Const MSTR_EDIT = "E"
Private Const MSTR_DELETE = "D"
Private Const MSTR_NONE = ""
Private Const MSTR_NEWCOL1 = "<New Value>"

' variables for current values
Private miID As Long
Private msName As String

Private mbDirty As Boolean
Private mState As eState

Private rsRuleItemTypes As ADODB.Recordset

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal id As Long, ByVal Name As String) As Boolean
    On Error GoTo Herr
    mbDirty = False
    miID = id
    msName = Name
    Load Me
    Me.Caption = "Filters for rule '" & msName & "'"
    DataLoad
    Show vbModal
    Unload Me
    Execute = mbDirty
    Exit Function
Herr:
    ShowError
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Activate()
    On Error Resume Next
    grd.SetFocus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    gCmn.CustomizeForm Me
    With grd
        .Clear
        .Rows = 1
        .Cols = 3
        .TextMatrix(0, MINT_COL1) = "Value"
        .Cell(flexcpAlignment, 0, MINT_COL1, 0, MINT_COL1) = flexAlignCenterCenter
        .ExtendLastCol = True
        .ColHidden(MINT_IDCOL) = True
        .ColHidden(MINT_CONDITIONCOL) = True
        .ColWidth(MINT_COL1) = 2000
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnApply_Click()
    On Error Resume Next
    btnApply.SetFocus
    If DataSave Then
        DataReload
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    Me.Hide
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    Me.ValidateControls
    btnOK.SetFocus
    If btnApply.Enabled Then
        If DataSave Then
            Me.Hide
        End If
    Else
        Me.Hide
    End If
End Sub

'//////////////////
' menu events   '/
'////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuAdd_Click()
    On Error Resume Next
    With grd
        .Row = .Rows - 2
        .EditCell
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuDelete_Click()
    On Error Resume Next
    RowDelete
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mnuEdit_Click()
    On Error Resume Next
    With grd
        .EditCell
    End With
End Sub

'//////////////////
' grid events   '/
'////////////////

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub grd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_AfterRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long)
    On Error Resume Next
    If NewRow <> OldRow Then
        ColorCells NewRow
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_BeforeMouseDown(ByVal Button As Integer, ByVal Shift As Integer, ByVal X As Single, ByVal Y As Single, Cancel As Boolean)
    On Error Resume Next
    With grd
        ' Opening context menu
        If (Button And vbRightButton) Then
            If (.EditWindow = 0) Then
                If (.MouseRow > 0) And (.MouseRow < .Rows - 1) Then
                    .Row = .MouseRow
                    PopupMenu mnuContext
                End If
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_BeforeRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long, Cancel As Boolean)
    On Error Resume Next
    If OldRow < 1 Then Exit Sub
    If NewRow <> OldRow Then
        Cancel = Not RowValidate
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_ChangeEdit()
    On Error Resume Next
    RowEdit
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_CellChanged(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    RowEdit
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub grd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With grd
        Select Case KeyCode
        ' escape handling
        Case vbKeyEscape
            If ((mState = STATE_EDIT) Or (mState = STATE_NEW)) And (.EditWindow = 0) Then
                RowUndoEdit
            End If
        ' insert handling
        Case vbKeyInsert
            If (mState = STATE_NONE) Then
                mnuAdd_Click
            End If
        ' delete handling
        Case vbKeyDelete
            If (mState = STATE_NONE) Then
                RowDelete
            End If
        Case 93
            PopupMenu mnuContext, , .CellLeft + .Left, .CellTop + .CellHeight + .Top
        Case vbKeyF10
            If Shift = vbShiftMask Then PopupMenu mnuContext, , .CellLeft + .Left, .CellTop + .CellHeight + .Top
        End Select
    End With
End Sub

'//////////////////////
' private function  '/
'////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' determine if we can change the current row
'
Public Function RowValidate() As Boolean
Dim iRow As Long
Dim v1 As Variant
    With grd
        If (mState = STATE_EDIT) Or (mState = STATE_NEW) Then
            v1 = .TextMatrix(.Row, MINT_COL1)
            
            ' value validating
            If Len(v1) = 0 Then
                .Col = MINT_COL1
                ShowMessage "Value is empty"
                Exit Function
            End If
            
            Select Case mState
            Case STATE_EDIT
                If .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_NONE Then
                    .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_EDIT
                    .Cell(flexcpForeColor, .Row, MINT_COL1, .Row, MINT_COL1) = GCOLOR_TABLE_CHANGED_FORE
                End If
            Case STATE_NEW
                .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_ADD
                .Cell(flexcpForeColor, .Row, MINT_COL1, .Row, MINT_COL1) = GCOLOR_TABLE_NEW_FORE
                
                .Rows = .Rows + 1
                .TextMatrix(.Rows - 1, MINT_IDCOL) = ""
                .TextMatrix(.Rows - 1, MINT_COL1) = MSTR_NEWCOL1
                .TextMatrix(.Rows - 1, MINT_CONDITIONCOL) = MSTR_NONE
                .RowHidden(.Rows - 1) = True
            End Select
            btnApply.Enabled = True
        End If
    End With
    SetState STATE_NONE
    RowValidate = True
Exit Function
Herr:
    ShowError "Error while validating a row"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' setting edit mode for a grid
'
Private Function RowEdit() As Boolean
    On Error GoTo Herr
    With grd
        If mState = STATE_NONE Then
            msTempID = .TextMatrix(.Row, MINT_IDCOL)
            msTempCol1 = .TextMatrix(.Row, MINT_COL1)
            msTempCond = .TextMatrix(.Row, MINT_CONDITIONCOL)
        
            If (.Row = .Rows - 2) Then
                SetState STATE_NEW
                .RowHidden(.Rows - 1) = False
                ColorCells .Rows - 1
            Else
                SetState STATE_EDIT
            End If
        End If
    End With
    RowEdit = True
Exit Function
Herr:
    ShowError "Error occured when trying to enter in edit mode"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' undoing changes
'
Private Function RowUndoEdit() As Boolean
Dim iID As Long
Dim i As Long
    On Error GoTo Herr
    
    With grd
        .TextMatrix(.Row, MINT_IDCOL) = msTempID
        .TextMatrix(.Row, MINT_COL1) = msTempCol1
        .TextMatrix(.Row, MINT_CONDITIONCOL) = msTempCond
        If .Row = .Rows - 2 Then
            .RowHidden(.Rows - 1) = True
        End If
    End With
    SetState STATE_NONE
    
    RowUndoEdit = True
Exit Function
Herr:
    ShowError "Error occured when trying to undo edit mode"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' setting control state
'
Private Sub SetState(ByVal X As eState)
    On Error Resume Next
    mState = X
    If (mState = STATE_LOAD) Then
        Screen.MousePointer = vbHourglass
    Else
        Screen.MousePointer = vbDefault
    End If
    ColorCellsAll
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' deleting data row
'
Private Sub RowDelete()
    On Error GoTo Herr
    With grd
        If .Row >= .Rows - 2 Then Exit Sub
        If gCmn.MyMsgBox("Do you want to delete this row?", vbYesNo) = vbYes Then

            Select Case .TextMatrix(.Row, MINT_CONDITIONCOL)
            Case MSTR_EDIT, MSTR_NONE
                .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_DELETE
                .RowHidden(.Row) = True
            Case MSTR_ADD
                .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_NONE
                .RowHidden(.Row) = True
            End Select
            btnApply.Enabled = True
        End If
    End With
Exit Sub
Herr:
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ColorCellsAll()
Dim iRow As Long
    On Error Resume Next
    For iRow = 1 To grd.Rows - 1
        ColorCells iRow
    Next iRow
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ColorCells(Optional ByVal iRow As Long = -1)
Dim iBackColorSel As Long
Dim iForeColorSel As Long
Dim iForeColor As Long
    On Error Resume Next
    With grd
        
        If iRow = .Row Then
            Select Case mState
            Case STATE_EDIT, STATE_NEW
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                iForeColorSel = GCOLOR_TABLE_CHANGED_FORE
                iForeColor = GCOLOR_TABLE_WHITE_FORE
            Case Else
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                If iRow >= .Rows - 2 Then
                    iForeColorSel = GCOLOR_TABLE_DELETED_FORE
                    iForeColor = GCOLOR_TABLE_DELETED_FORE
                Else
                    Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
                    Case MSTR_ADD
                        iForeColorSel = GCOLOR_TABLE_NEW_FORE
                        iForeColor = GCOLOR_TABLE_NEW_FORE
                    Case MSTR_EDIT
                        iForeColorSel = GCOLOR_TABLE_CHANGED_FORE
                        iForeColor = GCOLOR_TABLE_CHANGED_FORE
                    Case Else
                        iForeColorSel = GCOLOR_TABLE_CAPTION_FORE
                        iForeColor = GCOLOR_TABLE_WHITE_FORE
                    End Select
                End If
            End Select
        Else
            If iRow >= .Rows - 2 Then
                iForeColor = GCOLOR_TABLE_DELETED_FORE
            Else
                Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
                Case MSTR_ADD
                    iForeColor = GCOLOR_TABLE_NEW_FORE
                Case MSTR_EDIT
                    iForeColor = GCOLOR_TABLE_CHANGED_FORE
                Case Else
                    iForeColor = GCOLOR_TABLE_WHITE_FORE
                End Select
            End If
        End If
        
        If iRow = .Row Then
            .BackColorSel = iBackColorSel
            .ForeColorSel = iForeColorSel
        End If
        .Cell(flexcpForeColor, iRow, MINT_COL1, iRow, MINT_COL1) = iForeColor
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' temporary data reloading
'
Private Sub DataReload()
Dim iCurRow As Long
Dim iTopRow As Long
    On Error Resume Next
    With grd
        iCurRow = .Row
        iTopRow = .TopRow
        DataLoad
        .TopRow = iTopRow
        .Row = iCurRow
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' loading data from the database
'
Private Function DataLoad() As Boolean
Dim rs As ADODB.Recordset
Dim i As Long
    On Error GoTo Herr
    grd.Rows = 1
    Set rs = gDBW.usp_BookRuleItemMask_Get(miID)
    If Not rs Is Nothing Then
        With rs
            i = 0
            If .RecordCount > 0 Then
                grd.Rows = .RecordCount + 1
                .MoveFirst
                Do While Not .EOF
                    i = i + 1
                    grd.RowHidden(i) = False
                    grd.TextMatrix(i, MINT_IDCOL) = DBRead(!iBookRuleItemMaskID, 0)
                    grd.TextMatrix(i, MINT_COL1) = DBRead(!vcMaskName, "")
                    grd.TextMatrix(i, MINT_CONDITIONCOL) = MSTR_NONE
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    With grd
        .Rows = .Rows + 1
        .TextMatrix(.Rows - 1, MINT_IDCOL) = ""
        .TextMatrix(.Rows - 1, MINT_COL1) = MSTR_NEWCOL1
        .TextMatrix(.Rows - 1, MINT_CONDITIONCOL) = MSTR_NONE

        .Rows = .Rows + 1
        .TextMatrix(.Rows - 1, MINT_IDCOL) = ""
        .TextMatrix(.Rows - 1, MINT_COL1) = MSTR_NEWCOL1
        .TextMatrix(.Rows - 1, MINT_CONDITIONCOL) = MSTR_NONE
        .RowHidden(.Rows - 1) = True
        .Row = 1
    End With
Done:
    btnApply.Enabled = False
    SetState STATE_NONE
    DataLoad = True
    Exit Function
Herr:
    ShowError "Error loading data"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' saving data
'
Private Function DataSave() As Boolean
Dim iRow As Long
Dim vId As Variant
Dim v1 As Variant
Dim v2 As Variant
Dim iRet As Long
    On Error GoTo Herr
    With grd
        ' deleting
        For iRow = 1 To .Rows - 1
            If .TextMatrix(iRow, MINT_CONDITIONCOL) = MSTR_DELETE Then
                vId = gCmn.ReadLng(.TextMatrix(iRow, MINT_IDCOL))
                ' deleting data
                iRet = gDBW.usp_BookRuleItemMask_Del(vId)
            End If
        Next iRow
        ' adding and updating
        For iRow = 1 To .Rows - 1
            Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
            Case MSTR_ADD
                vId = Null
                v1 = .TextMatrix(iRow, MINT_COL1)
                ' adding data
                iRet = gDBW.usp_BookRuleItemMask_Save(vId, miID, v1)
            Case MSTR_EDIT
                vId = gCmn.ReadLng(.TextMatrix(iRow, MINT_IDCOL))
                v1 = .TextMatrix(iRow, MINT_COL1)
                ' updating data
                iRet = gDBW.usp_BookRuleItemMask_Save(vId, miID, v1)
            End Select
        Next
    End With
    DataSave = True
    Exit Function
Herr:
    ShowError "Error occured while saving data"
End Function

