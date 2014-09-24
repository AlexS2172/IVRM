VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomct2.ocx"
Begin VB.Form frmComponentsDividends 
   BackColor       =   &H80000005&
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Index Components Dividends"
   ClientHeight    =   6780
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   13215
   Icon            =   "frmComponentsDividends.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6780
   ScaleWidth      =   13215
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VSFlex7Ctl.VSFlexGrid fgUnderlying 
      Height          =   6135
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   12975
      _cx             =   22886
      _cy             =   10821
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
         Format          =   163708929
         CurrentDate     =   36936
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   10920
      TabIndex        =   2
      Top             =   6360
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
      Default         =   -1  'True
      Height          =   300
      Left            =   9480
      TabIndex        =   3
      Top             =   6360
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
   Begin ElladaFlatControls.FlatButton btnTotal 
      Height          =   300
      Left            =   2160
      TabIndex        =   6
      Top             =   6360
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
      Caption         =   "Recalculate"
   End
   Begin VB.Label lblSum1 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Sum :"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   6360
      Width           =   855
   End
   Begin VB.Label lblSum2 
      Alignment       =   1  'Right Justify
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      BackStyle       =   0  'Transparent
      BorderStyle     =   1  'Fixed Single
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1080
      TabIndex        =   4
      Top             =   6360
      Width           =   975
   End
End
Attribute VB_Name = "frmComponentsDividends"
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
    IDX_SYMBOL = 0
    IDX_WEIGHT
    IDX_DIV_FREQ
    IDX_DIV_DATE
    IDX_DIV_AMNT
    IDX_USE2
    IDX_DIV_FREQ2
    IDX_DIV_DATE2
    IDX_DIV_AMNT2
    
    IDX_DIV_FREQ3
    IDX_DIV_DATE3
    IDX_DIV_AMNT3
    
    IDX_DUMMY
    MINT_CONDITIONCOL
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private mState As eState

Private msTempCond As String
Private msTempWeight As String
Private msLastWeight As Double

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

Dim m_wsum As Double

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(ByVal ItemID As Long, ByVal ItemName As String) As Boolean
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    m_ItemID = ItemID
    m_ItemName = ItemName
    Load Me
    Me.Caption = m_ItemName & " Index Components Dividends"
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
        .MergeCol(IDX_WEIGHT) = True
        .MergeCol(IDX_USE2) = True
        .Cell(flexcpAlignment, 0, 0, 1, LAST_COLUMN) = flexAlignCenterCenter
        
        .FocusRect = flexFocusLight
        .FrozenCols = 0
        .FrozenRows = 0
        .GridLines = flexGridFlat
        .GridLinesFixed = flexGridFlat
        .TabBehavior = flexTabCells
        .BackColorFixed = GCOLOR_TABLE_CAPTION_BACK
        .ForeColorFixed = GCOLOR_TABLE_CAPTION_FORE
        
        i = IDX_SYMBOL
        .TextMatrix(0, i) = "Symbol"
        .TextMatrix(1, i) = "Symbol"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_WEIGHT
        .TextMatrix(0, i) = "Weight, %"
        .TextMatrix(1, i) = "Weight, %"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
        
        i = IDX_DIV_FREQ
        .TextMatrix(0, i) = "Market Dividends"
        .TextMatrix(1, i) = "Div Freq"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_DIV_DATE
        .TextMatrix(0, i) = "Market Dividends"
        .TextMatrix(1, i) = "Div Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
        
        i = IDX_DIV_AMNT
        .TextMatrix(0, i) = "Market Dividends"
        .TextMatrix(1, i) = "Div Amnt"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
    
        i = IDX_USE2
        .TextMatrix(0, i) = "Use"
        .TextMatrix(1, i) = "Manual"
        .ColDataType(i) = flexDTBoolean
        .ColFormat(i) = "->;<-"
        
        i = IDX_DIV_FREQ2
        .TextMatrix(0, i) = "Manual Dividends"
        .TextMatrix(1, i) = "Div Freq"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .AutoSize (i)
        
        i = IDX_DIV_DATE2
        .TextMatrix(0, i) = "Manual Dividends"
        .TextMatrix(1, i) = "Div Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
        
        i = IDX_DIV_AMNT2
        .TextMatrix(0, i) = "Manual Dividends"
        .TextMatrix(1, i) = "Div Amnt"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.00"
        
        i = IDX_DIV_FREQ3
        .TextMatrix(0, i) = "Index Dividends"
        .TextMatrix(1, i) = "Div Freq"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        .AutoSize (i)
        
        i = IDX_DIV_DATE3
        .TextMatrix(0, i) = "Index Dividends"
        .TextMatrix(1, i) = "Div Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
        
        i = IDX_DIV_AMNT3
        .TextMatrix(0, i) = "Index Dividends"
        .TextMatrix(1, i) = "Div Amnt"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = "0.0000"
        
        .ColHidden(MINT_CONDITIONCOL) = True
    
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
Dim rs As ADODB.Recordset
Dim rsFull As ADODB.Recordset
Dim rsSelected As ADODB.Recordset
Dim nRow As Long
Dim df1 As Long
Dim da1 As Double
Dim dd1 As Date
Dim df2 As Long
Dim da2 As Double
Dim dd2 As Date
    Dim f2 As EtsGeneralLib.EtsDivTypeEnum
Dim ID As Long
Dim Symbol As String
    With fgUnderlying
        .Redraw = flexRDNone
        .Rows = 2
    End With
    m_wsum = 0
    Set rsFull = gDBW.usp_Stock_Get
    Set rsSelected = gDBW.usp_IndexDefinition_Get(m_ItemID)
    
    With rsSelected
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = !iStockID
                Symbol = ""
                df1 = 0
                dd1 = 0
                da1 = 0
                f2 = enDivMarket
                df2 = 0
                dd2 = 0
                da2 = 0
                Dim Weight As Double
                Weight = DBRead(!fWeight, 0) * 100
                m_wsum = m_wsum + Weight
                With rsFull
                    If .RecordCount > 0 Then
                        .MoveFirst
                        .Find "iContractID=" & Format$(ID)
                        If .EOF Then
                            Debug.Assert False
                        Else
                            Symbol = DBRead(!vcSymbol, "")
                            df1 = DBRead(!iDivFreq, 0)
                            dd1 = DBRead(!dtDivDate, 0#)
                            da1 = DBRead(!fDivAmt, 0)
                            f2 = DBRead(!tiDivCustom, enDivMarket)
                            df2 = DBRead(!iDivFreqCustom, 0)
                            dd2 = DBRead(!dtDivDateCustom, 0#)
                            da2 = DBRead(!fDivAmtCustom, 0)
                        End If
                    End If
                End With
                
                With fgUnderlying
                    .AddItem ""
                    nRow = .Rows - 1
                    .RowData(nRow) = ID
                    .TextMatrix(nRow, IDX_SYMBOL) = Symbol
                    .Cell(flexcpData, nRow, IDX_SYMBOL) = 2
                    
                    .TextMatrix(nRow, IDX_WEIGHT) = Weight
                    
                    If df1 > 0 Then
                        .TextMatrix(nRow, IDX_DIV_FREQ) = DivFreqName(df1)
                        If dd1 > #1/1/1990# Then
                            .TextMatrix(nRow, IDX_DIV_DATE) = dd1
                        End If
                        If da1 > 0 Then
                            .TextMatrix(nRow, IDX_DIV_AMNT) = da1
                        End If
                    End If
                    
                    .TextMatrix(nRow, IDX_USE2) = f2 <> enDivMarket
                    
                    .Cell(flexcpAlignment, nRow, IDX_USE2) = flexAlignCenterCenter
                    
                    If f2 = enDivCustomPeriodical Then
                        If df2 > 0 Then
                            .TextMatrix(nRow, IDX_DIV_FREQ2) = DivFreqName(df2)
                            If dd2 > #1/1/1990# Then
                                .TextMatrix(nRow, IDX_DIV_DATE2) = dd2
                            End If
                            If da2 > 0 Then
                                .TextMatrix(nRow, IDX_DIV_AMNT2) = da2
                            End If
                        End If
                    ElseIf f2 = enDivCustomStream Then
                        If df2 > 0 Then
                            .TextMatrix(nRow, IDX_DIV_FREQ2) = "Custom Stream"
                            If dd2 > #1/1/1990# Then
                                .TextMatrix(nRow, IDX_DIV_DATE2) = ""
                            End If
                            If da2 > 0 Then
                                .TextMatrix(nRow, IDX_DIV_AMNT2) = ""
                            End If
                        End If
                    End If
                    

                    If f2 = enDivCustomPeriodical Then
                        If df2 > 0 Then
'                            .Cell(flexcpFontBold, nRow, IDX_DIV_FREQ2, nRow, IDX_DIV_AMNT2) = True
                            .TextMatrix(nRow, IDX_DIV_FREQ3) = DivFreqName(df2)
                            If dd2 > #1/1/1990# Then
                                .TextMatrix(nRow, IDX_DIV_DATE3) = dd2
                            End If
                            If da2 > 0 Then
                                .TextMatrix(nRow, IDX_DIV_AMNT3) = Weight * da2 / 100
                            End If
                        End If
                    ElseIf f2 = enDivMarket Then
'                        .Cell(flexcpFontBold, nRow, IDX_DIV_FREQ, nRow, IDX_DIV_AMNT) = True
                        If df1 > 0 Then
                            .TextMatrix(nRow, IDX_DIV_FREQ3) = DivFreqName(df1)
                            If dd1 > #1/1/1990# Then
                                .TextMatrix(nRow, IDX_DIV_DATE3) = dd1
                            End If
                            If da1 > 0 Then
                                .TextMatrix(nRow, IDX_DIV_AMNT3) = Weight * da1 / 100
                            End If
                        End If
                    Else
                       .TextMatrix(nRow, IDX_DIV_FREQ3) = "Custom Stream"
                       .TextMatrix(nRow, IDX_DIV_AMNT3) = ""
                       .TextMatrix(nRow, IDX_DIV_DATE3) = ""
                    End If
                    
                    .TextMatrix(nRow, MINT_CONDITIONCOL) = MSTR_NONE
                End With
                .MoveNext
            Loop
        Else
            fgUnderlying.AddItem ""
        End If
    End With
    With fgUnderlying
        If .Rows > 1 Then
            .Row = 2
            .Col = 0
            .Sort = flexSortStringAscending
        End If
        .Redraw = flexRDBuffered
    End With
    '//////////////
    lblSum2.Caption = Format$(m_wsum, "0.00")
    msLastWeight = 0
    '//////////////
    LoadData = True
    SetState STATE_NONE
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
Dim ID As Long
Dim iRow As Long
Dim Weight As Double
    With fgUnderlying
        For iRow = 2 To .Rows - 1
            Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
            Case MSTR_EDIT
                ID = .RowData(iRow)
                Weight = gCmn.ReadDbl(.TextMatrix(iRow, IDX_WEIGHT))
                gDBW.usp_IndexDefinition_Save m_ItemID, ID, Weight / 100
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
    If OldRow < 2 Then Exit Sub
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
Private Sub fgUnderlying_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Select Case Col
    Case IDX_WEIGHT
        RowRecalc
    Case IDX_USE2
        RowEdit Row
    End Select
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
    Debug.Print "StartEdit"
    Select Case Col
    Case IDX_WEIGHT
        Cancel = False
    Case Else
        Cancel = True
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function RowRecalc() As Boolean
Dim Weight As Double
Dim DivAmnt As Double
Dim Amnt As Double
Dim bUse2 As Boolean
    On Error GoTo Herr
    With fgUnderlying
        Weight = gCmn.ReadDbl(.TextMatrix(.Row, IDX_WEIGHT))
        bUse2 = .TextMatrix(.Row, IDX_USE2)
        If bUse2 Then
            DivAmnt = gCmn.ReadDbl(.TextMatrix(.Row, IDX_DIV_AMNT2))
        Else
            DivAmnt = gCmn.ReadDbl(.TextMatrix(.Row, IDX_DIV_AMNT))
        End If
        Amnt = Weight * DivAmnt / 100
        If Amnt > 0 Then
            .TextMatrix(.Row, IDX_DIV_AMNT3) = Amnt
        Else
            .TextMatrix(.Row, IDX_DIV_AMNT3) = ""
        End If
        '//////////////
        m_wsum = m_wsum + Weight - msLastWeight
        msLastWeight = Weight
        lblSum2.Caption = Format$(m_wsum, "0.00")
        '//////////////
    End With
    Exit Function
Herr:
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' determine if we can to change the current row
'
Public Function RowValidate() As Boolean
Dim Weight As Double
Dim DivAmnt As Double
Dim Amnt As Double
Dim bUse2 As Boolean
    On Error GoTo Herr
    With fgUnderlying
        If mState = STATE_EDIT Then
            
            Weight = gCmn.ReadDbl(.TextMatrix(.Row, IDX_WEIGHT))
            .TextMatrix(.Row, IDX_WEIGHT) = gCmn.FmtDbl(Weight)
            
            bUse2 = .TextMatrix(.Row, IDX_USE2)
            If bUse2 Then
                DivAmnt = gCmn.ReadDbl(.TextMatrix(.Row, IDX_DIV_AMNT2))
            Else
                DivAmnt = gCmn.ReadDbl(.TextMatrix(.Row, IDX_DIV_AMNT))
            End If
            Amnt = Weight * DivAmnt / 100
            If Amnt > 0 Then
                .TextMatrix(.Row, IDX_DIV_AMNT3) = Amnt
            Else
                .TextMatrix(.Row, IDX_DIV_AMNT3) = ""
            End If
            
            '//////////////
            m_wsum = m_wsum + Weight - msLastWeight
            msLastWeight = Weight
            lblSum2.Caption = Format$(m_wsum, "0.00")
            '//////////////
            
            ' value validating
            
            If Weight < 0 Then
                .Col = IDX_WEIGHT
                ShowWarning "Weight cannot be less than 0"
                Exit Function
            End If
            
            Select Case mState
            Case STATE_EDIT
                If .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_NONE Then
                    .TextMatrix(.Row, MINT_CONDITIONCOL) = MSTR_EDIT
                    .Cell(flexcpForeColor, .Row, IDX_WEIGHT, .Row, IDX_WEIGHT) = GCOLOR_TABLE_CHANGED_FORE
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
'
Private Sub btnTotal_Click()
Dim Weight As Double
Dim wsum As Double
Dim i As Long
    On Error GoTo Herr
    wsum = 0
    With fgUnderlying
        For i = 2 To .Rows - 1
            Weight = gCmn.ReadDbl(.TextMatrix(i, IDX_WEIGHT))
            wsum = wsum + Weight
        Next
    End With
    On Error Resume Next
    '//////////////
    m_wsum = wsum
    msLastWeight = 0
    lblSum2.Caption = Format$(m_wsum, "0.00")
    '//////////////
    Exit Sub
Herr:
    ShowError
End Sub

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
            msTempWeight = .TextMatrix(Row, IDX_WEIGHT)
            msLastWeight = msTempWeight
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
        If .Row < 2 Then
            Exit Function
        End If
        .TextMatrix(.Row, IDX_WEIGHT) = msTempWeight
        msLastWeight = 0
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
                If iRow >= .Rows - 0 Then '2
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
        .Cell(flexcpForeColor, iRow, IDX_WEIGHT, iRow, IDX_WEIGHT) = iForeColor
    End With
End Sub

