VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "VSFLEX7.OCX"
Object = "{86CF1D34-0C5F-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCT2.OCX"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmUndList 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Underlying list"
   ClientHeight    =   7260
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   12960
   Icon            =   "frmUndList.frx":0000
   LinkTopic       =   "Form1"
   MinButton       =   0   'False
   ScaleHeight     =   7260
   ScaleWidth      =   12960
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VSFlex7Ctl.VSFlexGrid fgUnd 
      Height          =   6615
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   12735
      _cx             =   22463
      _cy             =   11668
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
         Format          =   173539329
         CurrentDate     =   36936
      End
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Height          =   300
      Left            =   11520
      TabIndex        =   2
      Top             =   6840
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
   Begin ElladaFlatControls.FlatButton btnAllActive 
      Height          =   300
      Left            =   7200
      TabIndex        =   4
      Top             =   6840
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
      Caption         =   "Set ALL &Active"
   End
   Begin ElladaFlatControls.FlatButton btnAllInactive 
      Height          =   300
      Left            =   8640
      TabIndex        =   5
      Top             =   6840
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
      Caption         =   "Set ALL &Inactive"
   End
   Begin ElladaFlatControls.FlatButton btnActivePos 
      Height          =   300
      Left            =   10080
      TabIndex        =   6
      Top             =   6840
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
      Caption         =   "Active with &Pos"
   End
   Begin VB.Label lblCount 
      BackStyle       =   0  'Transparent
      Caption         =   "Total: 0 Underlying(s)"
      Height          =   300
      Left            =   120
      TabIndex        =   3
      Top             =   6840
      Width           =   3855
   End
End
Attribute VB_Name = "frmUndList"
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
    IDX_IS_ACTIVE = 0
    IDX_SYMBOL
    
    IDX_SYMBOL_TYPE
    IDX_OPTION_STYLE
    IDX_OPTION_CALENDAR
    
    IDX_DIV_FREQ
    IDX_DIV_DATE
    IDX_DIV_AMNT
    IDX_USE2
    IDX_DIV_FREQ2
    IDX_DIV_DATE2
    IDX_DIV_AMNT2
    
    IDX_DUMMY
    MINT_CONDITIONCOL
    COLUMNS_COUNT
    LAST_COLUMN = COLUMNS_COUNT - 1
End Enum

Private mState As eState

' row conditions
Private Const MSTR_ADD = "A"
Private Const MSTR_EDIT = "E"
Private Const MSTR_DELETE = "D"
Private Const MSTR_NONE = ""
Private Const MSTR_NEWDATE = "<New Date>"
Private Const MSTR_NEWVALUE = "<New Value>"

Private m_bChanged As Boolean
Private m_bOK As Boolean
Private rsList As ADODB.Recordset
Private m_Edit As Boolean

Private m_tempbChange As Boolean
Private m_tempState As String

Private m_frmCustDivs As New frmCustomDivs



'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute() As Variant
    On Error GoTo Herr
    Screen.MousePointer = vbHourglass
    DoEvents
    Load Me
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

Private Sub btnActivePos_Click()
    On Error Resume Next
    SetState STATE_LOAD
    
    Call gDBW.usp_UnderlyingActive_Save(0, 2)
    
    LoadData
End Sub

Private Sub btnAllActive_Click()
    On Error Resume Next

    SetState STATE_LOAD
    
    Call gDBW.usp_UnderlyingActive_Save(0, 1)
    
    LoadData
End Sub

Private Sub btnAllInactive_Click()
    On Error Resume Next
    SetState STATE_LOAD
    
    Call gDBW.usp_UnderlyingActive_Save(0, 0)
    
    LoadData
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnClose_Click()
    On Error Resume Next
    ' protection from simultaineous DTPicker1 and button click
    If Timer1.Enabled Then Exit Sub
    If m_bChanged Then
        If SaveData Then Me.Hide
    Else
        Me.Hide
    End If
End Sub


Private Sub fgUnd_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
Dim DivDate As Date, DivAmnt As Double
    On Error Resume Next
    With fgUnd
        If Col = IDX_DIV_DATE2 And .EditText <> "" Then
            DivDate = gCmn.ReadDate(.EditText)
            .EditText = gCmn.FmtDate(DivDate)
        ElseIf Col = IDX_DIV_AMNT2 Then
            DivAmnt = gCmn.ReadDbl(.EditText)
            .EditText = gCmn.FmtDbl(DivAmnt)
            
            ' value validating
            If DivAmnt < 0 Then
                .Col = IDX_DIV_AMNT2
                ShowWarning "Div Amount cannot be less than 0"
                Cancel = True
                Exit Sub
            End If
        End If
        
        Select Case Col
        Case IDX_IS_ACTIVE, IDX_USE2, IDX_DIV_FREQ2, IDX_DIV_AMNT2, IDX_OPTION_STYLE, IDX_OPTION_CALENDAR
            .TextMatrix(Row, MINT_CONDITIONCOL) = MSTR_EDIT
            ColorCells Row
            m_bChanged = True
        End Select
        Cancel = False
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    gCmn.CustomizeForm Me
    g_Params.SetIcon WND_MAIN, Me.hWnd, False

    InitGrid
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub InitGrid()
Dim s As String
Dim sCustom As String
Dim SC As String
Dim i As Long
    On Error Resume Next
    s = "#0;<NONE>|#1;Annually|#2;Semiannually|#4;Quarterly|#12;Monthly"
    sCustom = "#0;<NONE>|#1;Annually|#2;Semiannually|#4;Quarterly|#12;Monthly|#24;Custom Stream"
    SC = "#1;3rd Sat|#2;3rd Fri|#3;3rd Thu"
    
    With fgUnd
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
        .MergeCol(IDX_IS_ACTIVE) = True
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
        
        i = IDX_IS_ACTIVE
        .TextMatrix(0, i) = "Active"
        .TextMatrix(1, i) = "Active"
        .ColDataType(i) = flexDTBoolean
        
        i = IDX_SYMBOL
        .TextMatrix(0, i) = "Equity"
        .TextMatrix(1, i) = "Symbol"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_SYMBOL_TYPE
        .TextMatrix(0, i) = "Equity"
        .TextMatrix(1, i) = "Type"
        .ColDataType(i) = flexDTString
        .ColFormat(i) = ""
        
        i = IDX_OPTION_STYLE
        .TextMatrix(0, i) = "Option"
        .TextMatrix(1, i) = "Style"
        .ColDataType(i) = flexDTString
        .ColComboList(i) = "#0;Euro|#1;Amer"
        
        i = IDX_OPTION_CALENDAR
        .TextMatrix(0, i) = "Option"
        .TextMatrix(1, i) = "Calendar"
        .ColDataType(i) = flexDTString
        .ColComboList(i) = SC
        
        i = IDX_DIV_FREQ
        .TextMatrix(0, i) = "Market dividends"
        .TextMatrix(1, i) = "Div Freq"
        .ColComboList(i) = s
        
        i = IDX_DIV_DATE
        .TextMatrix(0, i) = "Market dividends"
        .TextMatrix(1, i) = "Div Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
        
        i = IDX_DIV_AMNT
        .TextMatrix(0, i) = "Market dividends"
        .TextMatrix(1, i) = "Div Amnt"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = ""
    
        i = IDX_USE2
        .TextMatrix(0, i) = "Use" & vbCrLf & "Manual"
        .TextMatrix(1, i) = "Use" & vbCrLf & "Manual"
        .ColDataType(i) = flexDTBoolean
        
        i = IDX_DIV_FREQ2
        .TextMatrix(0, i) = "Manual dividends"
        .TextMatrix(1, i) = "Div Freq"
        .AutoSize (i)
        .ColComboList(i) = sCustom
        
        i = IDX_DIV_DATE2
        .TextMatrix(0, i) = "Manual dividends"
        .TextMatrix(1, i) = "Div Date"
        .ColDataType(i) = flexDTDate
        .ColFormat(i) = ""
        
        i = IDX_DIV_AMNT2
        .TextMatrix(0, i) = "Manual dividends"
        .TextMatrix(1, i) = "Div Amnt"
        .ColDataType(i) = flexDTDouble
        .ColFormat(i) = ""
        
        .ColHidden(MINT_CONDITIONCOL) = True
    
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadData() As Boolean
    Dim rs As ADODB.Recordset
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
    Dim IsIndex As String
    Dim CalendarID As Long
    Dim IsAmerican As Boolean
    Dim s As String, nContractTypeID&, btIsActive As Byte

    Set rs = gDBW.usp_ExpiryCalendar_Get
    s = ""
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = DBRead(!iExpCalendarID)
                Symbol = DBRead(!vcExpCalendarName, "")
                s = s & "#" & Format$(ID) & ";" & Symbol & "|"
                .MoveNext
            Loop
        End If
    End With

    With fgUnd
        .ColComboList(IDX_OPTION_CALENDAR) = s
        .Redraw = flexRDNone
        .Rows = 2
    End With
    
    Set rs = gDBW.usp_Underlying_Get
    
    With rs
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                ID = DBRead(!iContractID, 0)
                Symbol = DBRead(!vcSymbol, "")
                nContractTypeID = DBRead(!iContractTypeID, 0)
                btIsActive = IIf(DBRead(!tiIsActive, 0) <> 0, 1, 0)
                If nContractTypeID = TYPE_STOCK Then
                    IsIndex = "Stock"
                    df1 = DBRead(!iDivFreq, 0)
                    dd1 = DBRead(!dtDivDate, 0#)
                    da1 = DBRead(!fDivAmt, 0)
                    f2 = DBRead(!tiIsDivCustom, enDivMarket)
                    df2 = DBRead(!iDivFreqCustom, 0)
                    dd2 = DBRead(!dtDivDateCustom, 0#)
                    da2 = DBRead(!fDivAmtCustom, 0)
                Else
                    If DBRead(!tiIsBasket) = 0 Then
                        IsIndex = "Index"
                    Else
                        IsIndex = "Basket"
                    End If
                    df1 = 0
                    dd1 = 0
                    da1 = 0
                    f2 = enDivMarket
                    df2 = 0
                    dd2 = 0
                    da2 = 0
                End If
                
                IsAmerican = DBRead(!tiCalcOptionType, 0) <> 0
                CalendarID = DBRead(!iExpCalendarID, 0)
                
                With fgUnd
                    .AddItem ""
                    nRow = .Rows - 1
                    .RowData(nRow) = ID
                    .Cell(flexcpChecked, nRow, IDX_IS_ACTIVE) = btIsActive
                    .TextMatrix(nRow, IDX_SYMBOL) = Symbol
                    .Cell(flexcpData, nRow, IDX_SYMBOL) = 2
                    .TextMatrix(nRow, IDX_SYMBOL_TYPE) = IsIndex
                    
                    .Cell(flexcpText, nRow, IDX_OPTION_STYLE) = IIf(IsAmerican, 1, 0)
                    .Cell(flexcpText, nRow, IDX_OPTION_CALENDAR) = CalendarID
                    
                    .Cell(flexcpText, nRow, IDX_DIV_FREQ) = df1
                    If df1 > 0 Then
                        If dd1 > #1/1/1990# Then
                            .TextMatrix(nRow, IDX_DIV_DATE) = dd1
                        End If
                        If da1 > 0 Then
                            .TextMatrix(nRow, IDX_DIV_AMNT) = da1
                        End If
                    End If
                    .Cell(flexcpChecked, nRow, IDX_USE2) = IIf(f2 <> enDivMarket, 1, 0)
                    If f2 <> enDivCustomStream Then
                        .Cell(flexcpText, nRow, IDX_DIV_FREQ2) = df2
                        If df2 > 0 Then
                            If dd2 > #1/1/1990# Then
                                .TextMatrix(nRow, IDX_DIV_DATE2) = dd2
                            End If
                            If da2 > 0 Then
                                .TextMatrix(nRow, IDX_DIV_AMNT2) = da2
                            End If
                        End If
                    Else
                        .Cell(flexcpText, nRow, IDX_DIV_FREQ2) = 24
                    End If
                        
                    
                    .TextMatrix(nRow, MINT_CONDITIONCOL) = MSTR_NONE
                End With
                .MoveNext
            Loop
        End If
    End With
    
    With fgUnd
        If .Rows > 2 Then
            .Row = 2
            .Col = 0
        End If
        .Redraw = flexRDBuffered
    End With
    
    lblCount.Caption = "Total: " & fgUnd.Rows - 2 & " underlying(s)"
    
    LoadData = True
    SetState STATE_NONE
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveData() As Boolean
    Dim btActive As Byte
    Dim nOptionStyle&
    Dim nOptionCalendar&
    Dim ID As Long
    Dim iRow As Long
    Dim bUse2 As Byte
    Dim DivAmnt As Double
    Dim DivFreq As Long
    Dim DivDate As Date
    Dim IsIndex As String
    
    With fgUnd
        For iRow = 2 To .Rows - 1
            Select Case .TextMatrix(iRow, MINT_CONDITIONCOL)
            Case MSTR_EDIT
                ID = .RowData(iRow)
                btActive = IIf(.Cell(flexcpChecked, iRow, IDX_IS_ACTIVE) = flexChecked, 1, 0)
                nOptionStyle = gCmn.ReadLng(.TextMatrix(iRow, IDX_OPTION_STYLE))
                nOptionCalendar = gCmn.ReadLng(.TextMatrix(iRow, IDX_OPTION_CALENDAR))
                bUse2 = IIf(.Cell(flexcpChecked, iRow, IDX_USE2) = flexChecked, 1, 0)
                DivFreq = gCmn.ReadLng(.TextMatrix(iRow, IDX_DIV_FREQ2))
                DivAmnt = gCmn.ReadDbl(.TextMatrix(iRow, IDX_DIV_AMNT2))
                DivDate = gCmn.ReadDate(.TextMatrix(iRow, IDX_DIV_DATE2))
                If bUse2 = 1 And DivFreq = 24 Then
                    DivAmnt = 0
                    DivFreq = 0
                    DivDate = 0
                    bUse2 = 2
                End If
                If .TextMatrix(iRow, IDX_SYMBOL_TYPE) = "Stock" Then
                    If bUse2 = 2 Then
                        gDBW.usp_Stock_Save ID, , , , , , , , , , , bUse2, nOptionStyle, , nOptionCalendar, btActive
                    Else
                        gDBW.usp_Stock_Save ID, , , , , , , , DivFreq, DivDate, DivAmnt, bUse2, nOptionStyle, , nOptionCalendar, btActive
                    End If
                    
                Else
                    gDBW.usp_Index_Save ID, , , , , nOptionStyle, , nOptionCalendar, , btActive
                End If
            End Select
        Next
    End With
    SaveData = True
End Function


Private Sub Form_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    LockWindowUpdate Me.hWnd
    
        
    btnClose.Top = ScaleHeight - btnClose.Height - 150
    btnClose.Left = ScaleWidth - btnClose.Width - 150
    
    btnActivePos.Move btnClose.Left - btnClose.Width - 150, btnClose.Top
    btnAllInactive.Move btnActivePos.Left - btnActivePos.Width - 150, btnClose.Top
    btnAllActive.Move btnAllInactive.Left - btnAllInactive.Width - 150, btnClose.Top
    
    lblCount.Move 150, btnClose.Top
    
    
    
    fgUnd.Move 150, 150, ScaleWidth - 300, btnClose.Top - 300
    
    LockWindowUpdate 0
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' stop the timer (used after DTPicker1 CloseUp)
'
Private Sub Timer1_Timer()
    On Error Resume Next
    Timer1.Enabled = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_Change()
On Error Resume Next
    fgUnd.Text = gCmn.FmtDate(DTPicker1.Value)
    fgUnd.TextMatrix(fgUnd.Row, MINT_CONDITIONCOL) = MSTR_EDIT
    ColorCells fgUnd.Row
    m_bChanged = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_CloseUp()
    ' starting the timer
    On Error Resume Next
    Timer1.Enabled = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_KeyDown(KeyCode As Integer, Shift As Integer)
On Error Resume Next
    ' close date picker when user hits escape or return
    Select Case KeyCode
    Case vbKeyEscape
        m_bChanged = m_tempbChange
        fgUnd.TextMatrix(fgUnd.Row, MINT_CONDITIONCOL) = m_tempState
        fgUnd.Text = DTPicker1.Tag
        
        ColorCells fgUnd.Row
        
        DTPicker1.Visible = False
    Case vbKeyReturn
        DTPicker1.Visible = False
    Case vbKeyTab
        DTPicker1.Visible = False
        fgUnd.SetFocus
        SendKeys "{TAB}"
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DTPicker1_LostFocus()
On Error Resume Next
    Timer1.Enabled = True
    DTPicker1.Visible = False
    fgUnd.SetFocus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnd_BeforeRowColChange(ByVal OldRow As Long, ByVal OldCol As Long, ByVal NewRow As Long, ByVal NewCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnd_BeforeScroll(ByVal OldTopRow As Long, ByVal OldLeftCol As Long, ByVal NewTopRow As Long, ByVal NewLeftCol As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnd_BeforeUserResize(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
On Error Resume Next
    DTPicker1.Visible = False
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    With fgUnd
        Select Case KeyCode
        ' escape handling
        Case vbKeyEscape
            If Timer1.Enabled Then Exit Sub
            If .EditWindow = 0 Then
                Me.Hide
            End If
        End Select
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub fgUnd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim D As Date
    Dim bCustomStream As Boolean
    
    If gCmn.ReadLng(fgUnd.TextMatrix(Row, IDX_DIV_FREQ2)) = 24 Then
        If Col = IDX_DIV_DATE2 Or Col = IDX_DIV_AMNT2 Then
            m_frmCustDivs.Init fgUnd.RowData(Row), fgUnd.TextMatrix(Row, IDX_SYMBOL)
            m_frmCustDivs.Show vbModal, Me
            Cancel = True
            Exit Sub
        End If
    End If
    
    Select Case Col
        Case IDX_IS_ACTIVE, IDX_USE2, IDX_DIV_FREQ2, IDX_DIV_AMNT2, IDX_OPTION_STYLE, IDX_OPTION_CALENDAR
            Cancel = False
            
        Case IDX_DIV_DATE2
            Cancel = False
            With fgUnd
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
                
                m_tempbChange = m_bChanged
                m_tempState = .TextMatrix(.Row, MINT_CONDITIONCOL)
                
                DTPicker1.Value = D
                DTPicker1.Tag = .Text
                .Text = gCmn.FmtDate(DTPicker1.Value)
                DTPicker1.Visible = True
                DTPicker1.SetFocus
            End With
        Case Else
            Cancel = True
    End Select
End Sub

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
    For iRow = 1 To fgUnd.Rows - 1
        ColorCells iRow
    Next
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ColorCells(Optional ByVal iRow As Long = -1)
Dim iBackColorSel As Long
Dim iForeColorSel As Long
Dim iForeColor As Long
    On Error Resume Next
    With fgUnd
        
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
        
        .Cell(flexcpForeColor, iRow, 0, iRow, .Cols - 1) = iForeColor
    End With
End Sub

