VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmEntCalendarProp 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Calendar properties"
   ClientHeight    =   5925
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   9855
   Icon            =   "frmEntCalendarProp.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5925
   ScaleWidth      =   9855
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VSFlex7Ctl.VSFlexGrid fgUnderlying 
      Height          =   5295
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   9615
      _cx             =   16960
      _cy             =   9340
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
      Begin MSComCtl2.DTPicker DTPicker1 
         Height          =   255
         Left            =   5760
         TabIndex        =   1
         Top             =   2280
         Visible         =   0   'False
         Width           =   975
         _ExtentX        =   1720
         _ExtentY        =   450
         _Version        =   393216
         Format          =   62455809
         CurrentDate     =   36936
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Height          =   300
      Left            =   8400
      TabIndex        =   2
      Top             =   5520
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
      Left            =   6960
      TabIndex        =   3
      Top             =   5520
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
End
Attribute VB_Name = "frmEntCalendarProp"
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
    IDX_EXPIRY_SYMBOL = 0
    IDX_EXPIRY1
    IDX_EXPIRY_DAY
    IDX_DAYS_TO_EXPIRY
    IDX_VEGA_WEIGHT1
    
    IDX_DUMMY
    MINT_CONDITIONCOL
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private mState As eState

Private msTempDate As String
Private msTempValue As String
Private msTempCond As String

' row conditions
Private Const MSTR_ADD = "A"
Private Const MSTR_EDIT = "E"
Private Const MSTR_DELETE = "D"
Private Const MSTR_NONE = ""
Private Const MSTR_NEWDATE = "<New Date>"
Private Const MSTR_NEWVALUE = "<New Value>"

Private m_ItemID As Long
Private m_ItemName As String
Private m_bChanged As Boolean
Private m_bOK As Boolean
Private rsList As ADODB.Recordset
Private m_Edit As Boolean
Private fAutoCalc As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal ItemID As Long, ByVal ItemName As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_ItemID = ItemID
    m_ItemName = ItemName
    Load Me
    Me.Caption = "Expirations Calendar " & m_ItemName
    m_bOK = False
    m_Edit = False
    If LoadData Then
'        Changed = False
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
    On Error Resume Next
    ' protection from simultaineous DTPicker1 and button click
    If Timer1.Enabled Then Exit Sub
    RowValidate
    If m_bChanged Then
        If SaveData Then Me.Hide
    Else
        Me.Hide
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    ' protection from simultaineous DTPicker1 and button click
    If Timer1.Enabled Then Exit Sub
    Me.Hide
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
Dim i As Long
    On Error Resume Next
    
    With fgUnderlying
        .Rows = 1
        .Cols = COLUMNS_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .Appearance = flex3DLight
        .Editable = flexEDKbdMouse
        .ExplorerBar = flexExNone
        .Sort = flexSortNone
        .ExtendLastCol = True
        .FixedCols = 0
        .FixedRows = 1
        
        .MergeCells = flexMergeFixedOnly
        .MergeRow(0) = True
        .Cell(flexcpAlignment, 0, 0, 0, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = IDX_EXPIRY_SYMBOL
        .TextMatrix(0, i) = "Expiry"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_EXPIRY1
        .TextMatrix(0, i) = "Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
    
        i = IDX_EXPIRY_DAY
        .TextMatrix(0, i) = "Weekday"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
    
        i = IDX_DAYS_TO_EXPIRY
        .TextMatrix(0, i) = "DTE"
        .ColDataType(i) = flexDTLong
        .ColFormat(i) = ""
        
        i = IDX_VEGA_WEIGHT1
        .TextMatrix(0, i) = "Vega weight"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
        
        .ColHidden(MINT_CONDITIONCOL) = True
    
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
Dim rsFull As ADODB.Recordset
Dim nRow As Long
Dim dd1 As Date
Dim dd5 As Date
Dim dw As Double
Dim dn As Long
Dim ID As Long
Dim Symbol As String
    With fgUnderlying
        .Redraw = flexRDNone
        .Rows = 1
    End With
    
    On Error Resume Next
    Dim bUseActual As Boolean
    Dim bIsActual As Boolean
    bUseActual = False
    Dim rsActual As ADODB.Recordset
'    Set rsActual = ActualExpiryGet(Date)
'    If Not rsActual Is Nothing Then
'        With rsActual
'            If .RecordCount > 0 Then
'                bUseActual = True
'                .MoveFirst
'                Do While Not .EOF
'                    Debug.Print !ExpiryID, !ExpiryPeriod
'                    .MoveNext
'                Loop
'            End If
'        End With
'    End If
    
    On Error GoTo 0
    
    Set rsFull = gDBW.usp_Expiry_Get(Null, Null, m_ItemID)
    ' maximum date to show: today + 15 years
    dd5 = DateSerial(Year(Date) + 15, Month(Date), Day(Date))
    With rsFull
        If .RecordCount > 0 Then
            .Sort = "dtExpiryDate"
            .MoveFirst
            Do While Not .EOF
                ID = !iExpiryID
                Symbol = !vcExpiryPeriod
                dd1 = !dtExpiryDate
                dw = DBRead(!fVegaWeight)
                bIsActual = (dd1 >= Date) And (dd1 <= dd5)
                If bIsActual Then
                    If bUseActual Then
                        With rsActual
                            .MoveFirst
                            .Find "ExpiryID=" & Format$(ID)
                            bIsActual = Not .EOF
                        End With
                    End If
                End If
                If bIsActual Then
                    With fgUnderlying
                        .AddItem ""
                        nRow = .Rows - 1
                        .RowData(nRow) = ID
                        .TextMatrix(nRow, IDX_EXPIRY_SYMBOL) = Symbol
                        .Cell(flexcpData, nRow, IDX_EXPIRY_SYMBOL) = 2
                        If dd1 > #1/1/1990# Then
                            .TextMatrix(nRow, IDX_EXPIRY1) = gCmn.FmtDate(dd1)
                            .TextMatrix(nRow, IDX_EXPIRY_DAY) = WeekdayName(Weekday(dd1), False)
                            dn = DateDiff("d", Date, dd1)
                            If dn < 0 Then dn = 0
                            .TextMatrix(nRow, IDX_DAYS_TO_EXPIRY) = dn
                        Else
                            .TextMatrix(nRow, IDX_EXPIRY1) = ""
                            .TextMatrix(nRow, IDX_EXPIRY_DAY) = ""
                            .TextMatrix(nRow, IDX_DAYS_TO_EXPIRY) = ""
                        End If
                        
                        .TextMatrix(nRow, IDX_VEGA_WEIGHT1) = dw
                        .TextMatrix(nRow, MINT_CONDITIONCOL) = MSTR_NONE
                    End With
                End If
                .MoveNext
            Loop
        Else
            fgUnderlying.AddItem ""
        End If
    End With
    With fgUnderlying
        If .Rows > 1 Then
            .Row = 1
            .Col = 0
        End If
        .AutoSize 0, LAST_COLUMN, , 100
        .Redraw = flexRDBuffered
    End With
    LoadData = True
    SetState STATE_NONE
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
Dim ID As Long
Dim iRow As Long
Dim ExpDate As Date
Dim VegaWeight As Double
    With fgUnderlying
        For iRow = 1 To .Rows - 1
            Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
            Case MSTR_EDIT
                ID = .RowData(iRow)
                ExpDate = gCmn.ReadDate(.TextMatrix(iRow, IDX_EXPIRY1))
                VegaWeight = gCmn.ReadDbl(.TextMatrix(iRow, IDX_VEGA_WEIGHT1))
                gDBW.usp_Expiry_Save ID, Null, ExpDate, VegaWeight, m_ItemID
            End Select
        Next
    End With
    SaveData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' temporary data reloading
'
Private Sub DataReload()
Dim iCurRow As Long
Dim iTopRow As Long
On Error Resume Next
    With fgUnderlying
        iCurRow = .Row
        iTopRow = .TopRow
        LoadData
        .TopRow = iTopRow
        .Row = iCurRow
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' stop the timer (used after DTPicker1 CloseUp)
'
Private Sub Timer1_Timer()
    Timer1.Enabled = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_Change()
On Error Resume Next
    fgUnderlying.Text = gCmn.FmtDate(DTPicker1.Value)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_CloseUp()
    ' starting the timer
    Timer1.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    ' close date picker when user hits escape or return
    Select Case KeyCode
    Case vbKeyEscape
        fgUnderlying.Text = DTPicker1.Tag
        DTPicker1.Visible = False
    Case vbKeyReturn
        DTPicker1.Visible = False
    Case vbKeyTab
        DTPicker1.Visible = False
        fgUnderlying.SetFocus
        SendKeys "{TAB}"
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_LostFocus()
On Error Resume Next
    Timer1.Enabled = True
    SendKeys "{ESC}"
    DTPicker1.Visible = False
    fgUnderlying.SetFocus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_BeforeRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
    If OldRow < 1 Then Exit Sub
    If NewRow <> OldRow Then
        Cancel = Not RowValidate
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_BeforeScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_ChangeEdit()
    On Error Resume Next
    RowEdit fgUnderlying.Row
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgUnderlying
        Select Case KeyCode
        ' escape handling
        Case vbKeyEscape
            If Timer1.Enabled Then Exit Sub
            If ((mState = STATE_EDIT) Or (mState = STATE_NEW)) And (.EditWindow = 0) Then
                RowUndoEdit
            End If
        End Select
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim D As Date
    On Error Resume Next
    fAutoCalc = False
    Select Case Col
    Case IDX_EXPIRY1
        Cancel = False
        fAutoCalc = True
        With fgUnderlying
            RowEdit Row
            Cancel = True
            DTPicker1.Move .CellLeft, .CellTop - 10, .CellWidth, .CellHeight + 50
            If .Text = "<New Date>" Then
                D = Date
            Else
                D = gCmn.ReadDate(.Text)
                If D = 0 Then
                    D = Date
                End If
            End If
            DTPicker1.Value = D
            DTPicker1.Tag = D
            .Text = gCmn.FmtDate(DTPicker1.Value)
            DTPicker1.Visible = True
            DTPicker1.SetFocus
        End With
    Case IDX_VEGA_WEIGHT1
        Cancel = False
    Case Else
        Cancel = True
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' determine if we can to change the current row
'
Public Function RowValidate() As Boolean
Dim ExpDate As Date
Dim dn As Long
    On Error GoTo Herr
    With fgUnderlying
        If mState = STATE_EDIT Then
            If .TextMatrix(.Row, IDX_EXPIRY1) <> "" Then
                ExpDate = gCmn.ReadDate(.TextMatrix(.Row, IDX_EXPIRY1))
                Dim ID As Long
                ID = .RowData(.Row)
                If ID <> StdExpiryID(ExpDate) Then
                    .Col = IDX_EXPIRY1
                    ShowWarning "Month/Year cannot be changed"
                    Exit Function
                End If
                
                If ExpDate > #1/1/1990# Then
                    .TextMatrix(.Row, IDX_EXPIRY1) = gCmn.FmtDate(ExpDate)
                    .TextMatrix(.Row, IDX_EXPIRY_DAY) = WeekdayName(Weekday(ExpDate), False)
                    dn = DateDiff("d", Date, ExpDate)
                    If dn < 0 Then dn = 0
                    .TextMatrix(.Row, IDX_DAYS_TO_EXPIRY) = dn
                    If fAutoCalc Then
                        .TextMatrix(.Row, IDX_VEGA_WEIGHT1) = DefaultVegaWeight(ExpDate)
                    End If
                Else
                    .TextMatrix(.Row, IDX_EXPIRY1) = ""
                    .TextMatrix(.Row, IDX_EXPIRY_DAY) = ""
                    .TextMatrix(.Row, IDX_DAYS_TO_EXPIRY) = ""
                    .TextMatrix(.Row, IDX_VEGA_WEIGHT1) = ""
                End If
            End If
            Select Case mState
            Case STATE_EDIT
                If .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_NONE Then
                    .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_EDIT
                    .Cell(flexcpForeColor, .Row, IDX_EXPIRY_SYMBOL, .Row, IDX_VEGA_WEIGHT1) = GCOLOR_TABLE_CHANGED_FORE
                End If
            End Select
            m_bChanged = True
        End If
    End With
    SetState STATE_NONE
    RowValidate = True
    Exit Function
Herr:
    ShowError "Error while validating a row"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' setting edit mode for a grid
'
Private Function RowEdit(ByVal Row As Long) As Boolean
    On Error GoTo Herr
    With fgUnderlying
        If Row < 1 Then
            Exit Function
        End If
        If mState = STATE_NONE Then
            msTempDate = .TextMatrix(Row, IDX_EXPIRY1)
            msTempValue = .TextMatrix(Row, IDX_VEGA_WEIGHT1)
            msTempCond = .TextMatrix(Row, MINT_CONDITIONCOL)
            SetState STATE_EDIT
        End If
    End With
    RowEdit = True
    Exit Function
Herr:
    ShowError "Error occured when trying to enter in edit mode"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' undoing changes
'
Private Function RowUndoEdit() As Boolean
Dim iID As Long
Dim i As Long
    On Error GoTo Herr
    With fgUnderlying
        If .Row < 1 Then
            Exit Function
        End If
        .TextMatrix(.Row, IDX_EXPIRY1) = msTempDate
        .TextMatrix(.Row, IDX_VEGA_WEIGHT1) = msTempValue
        .TextMatrix(.Row, MINT_CONDITIONCOL) = msTempCond
    End With
    SetState STATE_NONE
    RowUndoEdit = True
    Exit Function
Herr:
    ShowError "Error occured when trying to undo edit mode"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ColorCellsAll()
Dim iRow As Long
On Error Resume Next
    For iRow = 1 To fgUnderlying.Rows - 1
        ColorCells iRow
    Next iRow
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ColorCells(Optional ByVal iRow As Long = -1)
Dim iBackColorSel As Long
Dim iForeColorSel As Long
Dim iForeColor As Long
    On Error Resume Next
    With fgUnderlying
        
        If iRow = .Row Then
            Select Case mState
            Case STATE_EDIT, STATE_NEW
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                iForeColorSel = GCOLOR_TABLE_CHANGED_FORE
                iForeColor = GCOLOR_TABLE_WHITE_FORE
            Case Else
                iBackColorSel = GCOLOR_TABLE_SELECT_BACK
                If iRow >= .Rows - 0 Then '1
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
            If iRow >= .Rows - 0 Then '2
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
        .Cell(flexcpForeColor, iRow, IDX_EXPIRY_SYMBOL, iRow, IDX_VEGA_WEIGHT1) = iForeColor
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnderlying_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim dValue#
    If Col <> IDX_VEGA_WEIGHT1 Then Exit Sub
    
    With fgUnderlying
        dValue = 0
        dValue = CDbl(.EditText)
        If dValue <= 0# Then
            dValue = Val(.EditText)
            If dValue <= 0# Then
                dValue = .Cell(flexcpData, Row, 1)
            End If
        End If
        
        .Cell(flexcpData, Row, 1) = dValue
        .EditText = dValue
    End With
End Sub

