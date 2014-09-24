VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmWtdVegaSettings 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Weighted Vega Settings"
   ClientHeight    =   6255
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   12390
   Icon            =   "frmWtdVegaSettings.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   6255
   ScaleWidth      =   12390
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.CommandButton btnRecalculate 
      Caption         =   "Recalculate"
      Height          =   315
      Left            =   3060
      TabIndex        =   1
      Top             =   90
      Width           =   1395
   End
   Begin MSComctlLib.ProgressBar pbProgress 
      Height          =   240
      Left            =   120
      TabIndex        =   7
      Top             =   5760
      Visible         =   0   'False
      Width           =   1695
      _ExtentX        =   2990
      _ExtentY        =   423
      _Version        =   393216
      Appearance      =   1
      Scrolling       =   1
   End
   Begin VB.TextBox txtMaturity 
      Height          =   315
      Left            =   2100
      TabIndex        =   0
      Top             =   90
      Width           =   855
   End
   Begin VSFlex7Ctl.VSFlexGrid fgData 
      Height          =   4815
      Left            =   120
      TabIndex        =   2
      Top             =   840
      Width           =   12135
      _cx             =   21405
      _cy             =   8493
      _ConvInfo       =   1
      Appearance      =   1
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
      BackColorBkg    =   -2147483636
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
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   9240
      TabIndex        =   3
      Top             =   5760
      Width           =   1455
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   10800
      TabIndex        =   4
      Top             =   5760
      Width           =   1455
   End
   Begin MSForms.TabStrip tsType 
      Height          =   255
      Left            =   120
      TabIndex        =   8
      Top             =   600
      Width           =   1335
      ListIndex       =   0
      Size            =   "2355;450"
      Items           =   "Tab1;Tab2;"
      TipStrings      =   ";;"
      Names           =   "Tab1;Tab2;"
      NewVersion      =   -1  'True
      TabsAllocated   =   2
      Tags            =   ";;"
      TabData         =   2
      Accelerator     =   ";;"
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
      TabState        =   "3;3"
   End
   Begin VB.Label lblProgress 
      BackStyle       =   0  'Transparent
      Caption         =   "Progress..."
      Height          =   255
      Left            =   1860
      TabIndex        =   6
      Top             =   4230
      Visible         =   0   'False
      Width           =   1635
   End
   Begin VB.Label lblMaturityTitle 
      Alignment       =   1  'Right Justify
      BackStyle       =   0  'Transparent
      Caption         =   "Reference Maturity, days :"
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   1935
   End
   Begin VB.Menu mnuPopup 
      Caption         =   "<Popup>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
         Enabled         =   0   'False
         Shortcut        =   ^C
         Visible         =   0   'False
      End
      Begin VB.Menu mnuPrint 
         Caption         =   "Print..."
         Enabled         =   0   'False
      End
   End
End
Attribute VB_Name = "frmWtdVegaSettings"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Event OnOK()

Private m_frmOwner As Form
Private m_bIsOpened As Boolean
Private m_bDirty As Boolean
Private m_bInitNow As Boolean
Private m_bInProc As Boolean

Private m_nMaturity As Long
Private m_nTotalExpCount As Long
Private m_bKeyDown As Boolean
Private m_Tab As Long


Public Sub Execute(ByRef frmOwner As Form)
    On Error GoTo EH
    If m_bIsOpened Then Exit Sub
    Screen.MousePointer = vbHourglass
    Set m_frmOwner = frmOwner
    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
    
    m_bIsOpened = True
    m_bDirty = False
    m_bKeyDown = False
    m_bInitNow = True
    m_bInProc = False

    Load Me
    
    FormatGrid
    LoadData
    
    m_bInitNow = False
    
    Me.Show vbModeless, m_frmOwner
    Screen.MousePointer = vbDefault
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open Weighted Vega Settings dialog."
    On Error Resume Next
    Unload Me
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = True
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
End Sub

Public Property Get IsOpened() As Boolean
    On Error Resume Next
    IsOpened = m_bIsOpened
End Property

Private Sub SaveData()
    On Error Resume Next
    Dim bDirty As Boolean, nRow&, dValue#
    Dim aExpMonth As EtsGeneralLib.EtsExpMonthAtom
    
    m_bInProc = True

    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = m_nTotalExpCount

    lblProgress.Caption = "Saving data..."
    lblProgress.Visible = True
    pbProgress.Visible = True
    DoEvents

    If m_nMaturity <> g_ExpCalendar.Maturity Then
        Err.Clear
        gDBW.usp_WtdVegaMaturity_Save m_nMaturity
        If Err.Number <> 0 Then
            gCmn.ErrorMsgBox Me, "Fail to save weighted vega settings."
            GoTo EX
        End If
        g_ExpCalendar.Maturity = m_nMaturity
    End If

    With fgData
                    
        For nRow = 1 To .Rows - 1
            Set aExpMonth = .Cell(flexcpData, nRow, 1)
       
            If Not aExpMonth Is Nothing Then
                If aExpMonth.IsDirty Then
                    dValue = 0
                    dValue = .ValueMatrix(nRow, 1)
                                
                    If dValue > 0 And aExpMonth.VegaWeight <> dValue Then
                        Err.Clear
                        gDBW.usp_ExpiryVegaWeightSave_Save aExpMonth.Expiry, dValue
                        If Err.Number <> 0 Then
                                gCmn.ErrorMsgBox Me, "Fail to save weighted vega settings."
                                GoTo EX
                        End If
                        aExpMonth.VegaWeight = dValue
                    End If
                                
                    aExpMonth.IsDirty = False
                End If
                            
                Set aExpMonth = Nothing
                IncProgress pbProgress
                DoEvents
            End If
        Next
                    
    End With

    m_bDirty = False

EX:
    Set aExpMonth = Nothing
    
    lblProgress.Visible = False
    pbProgress.Visible = False
    m_bInProc = False
End Sub

Private Sub btnCancel_Click()
    On Error Resume Next
    If m_bInProc Then
        m_bInProc = False
        Exit Sub
    End If
    
    Unload Me
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    If m_bDirty Then
        Screen.MousePointer = vbHourglass
        
        txtMaturity.Enabled = False
        fgData.Enabled = False
        btnCancel.Enabled = False
        btnOK.Enabled = False
        btnRecalculate.Enabled = False
        
        SaveData
        RaiseEvent OnOK
        Screen.MousePointer = vbDefault
    End If
    Unload Me
End Sub

Private Sub btnRecalculate_Click()
    On Error Resume Next
    Dim nRow&, dValue#
    Dim aExpMonth As EtsGeneralLib.EtsExpMonthAtom
    
    Dim dtNow As Date
    dtNow = GetNewYorkTime
    
    m_bInProc = True
    
    txtMaturity.Enabled = False
    fgData.Enabled = False
    btnRecalculate.Enabled = False
    btnOK.Enabled = False

    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = m_nTotalExpCount

    lblProgress.Caption = "Recalculation..."
    lblProgress.Visible = True
    pbProgress.Visible = True
    DoEvents

    With fgData
        .Redraw = flexRDNone
        For nRow = 1 To .Rows - 1
            Set aExpMonth = .Cell(flexcpData, nRow, 1)
                    
            If Not aExpMonth Is Nothing Then
                If aExpMonth.Expiry > Date Then
                    dValue = Sqr(m_nMaturity / Abs(DateDiff("d", aExpMonth.Expiry, dtNow)))
                    .TextMatrix(nRow, 1) = dValue
                            
                     If aExpMonth.VegaWeight <> dValue Then
                         aExpMonth.IsDirty = True
                         m_bDirty = True
                     End If
                End If
                        
                IncProgress pbProgress
                        
                DoEvents
                If Not m_bInProc Then GoTo EX
                        
                Set aExpMonth = Nothing
            End If
        Next
                
        .Redraw = flexRDBuffered
    End With

EX:
    fgData.Redraw = flexRDBuffered
    lblProgress.Visible = False
    pbProgress.Visible = False
    btnRecalculate.Enabled = True
    btnOK.Enabled = True
    txtMaturity.Enabled = True
    fgData.Enabled = True
    
    btnRecalculate.SetFocus
    
    m_bInProc = False
End Sub

Private Sub fgData_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
        m_bKeyDown = True
    End If
End Sub

Private Sub fgData_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown Then
        m_bKeyDown = False
    
        If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
            mnuCopy_Click
        End If
    End If
End Sub

Private Sub fgData_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
'    If Button = vbRightButton Then
'        PopupMenu mnuPopup
'    End If
End Sub

Private Sub fgData_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim aExpMonth As EtsGeneralLib.EtsExpMonthAtom
    Set aExpMonth = fgData.Cell(flexcpData, Row, Col)
    Cancel = aExpMonth Is Nothing
    Set aExpMonth = Nothing
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If UnloadMode <> vbFormCode Then
        Cancel = m_bInProc
    Else
        m_bInProc = False
    End If
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not m_frmOwner Is Nothing Then
        m_frmOwner.Enabled = True
        m_frmOwner.SetFocus
    End If
    
    Set m_frmOwner = Nothing
    m_bIsOpened = False
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Private Sub fgData_ValidateEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim dValue#, aExpMonth As EtsGeneralLib.EtsExpMonthAtom
    
    With fgData
        Set aExpMonth = .Cell(flexcpData, Row, Col)
        If Not aExpMonth Is Nothing Then
            dValue = 0
            dValue = CDbl(.EditText)
            If dValue <= 0# Then
                dValue = val(.EditText)
                If dValue <= 0# Then
                    dValue = aExpMonth.VegaWeight
                End If
            End If
            
            If aExpMonth.VegaWeight <> dValue Then
                m_bDirty = True
                .Cell(flexcpData, 0, Col) = True
                aExpMonth.IsDirty = True
            End If
            .EditText = dValue
        End If
    End With
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
End Sub

Private Sub LoadData()
    On Error GoTo EH
    Dim nRow&
    Dim aExpMonth As EtsGeneralLib.EtsExpMonthAtom
    
    m_nMaturity = g_ExpCalendar.Maturity
    txtMaturity.Text = Format$(g_ExpCalendar.Maturity, "#,##0")
    
    m_nTotalExpCount = 0
    
    With fgData
        .Redraw = flexRDNone
        
        .Cols = 2
        .Rows = 1
        
        .TextMatrix(0, 0) = "Date"
        .ColDataType(0) = flexDTString
        .ColFormat(0) = ""
    
        .TextMatrix(0, 1) = "Vega"
        .ColDataType(1) = flexDTDouble
        .ColFormat(1) = "#,##0.0000"
            
        m_nTotalExpCount = g_ExpCalendar.Count
        
        nRow = 1
        For Each aExpMonth In g_ExpCalendar
            .AddItem UCase$(Format$(aExpMonth.Expiry, "MMM DD YYYY"))
            
            .TextMatrix(nRow, 1) = aExpMonth.VegaWeight
            .Cell(flexcpData, nRow, 1) = aExpMonth
            
            nRow = nRow + 1
        Next
        
        .ColAlignment(0) = flexAlignCenterCenter
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        
        .AutoSize 0, .Cols - 1, False, 100
        .Redraw = flexRDBuffered
    End With

    Exit Sub
EH:
    fgData.Redraw = flexRDBuffered
    gCmn.ErrorMsgBox Me, "Fail to load weighted vega settings."
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    
    With fgData
        .Redraw = flexRDNone
        .Rows = 1
        .Cols = 10
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 1
        .FixedRows = 1
        .FocusRect = flexFocusHeavy
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
        .Redraw = flexRDBuffered
    End With
End Sub

Private Sub mnuCopy_Click()
    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    g_ClipMgr.CopyGridToClipboard fgData
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuPrint_Click()
    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute Me, "Weighted Vega", "Weighted Vega", fgData
'    Screen.MousePointer = vbDefault
End Sub

Private Sub tsType_Click(Index As Long)
 Dim t As Integer
 
 If Index <> m_Tab Then
    m_Tab = IIf(m_Tab = 1, 0, 1)
    With fgData
        For t = 1 To .Cols - 1
            If .ColHidden(t) = False Then .ColHidden(t) = True Else .ColHidden(t) = False
        Next
    End With
 End If
End Sub

Private Sub txtMaturity_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bInitNow Then Exit Sub
    
    Dim nMaturity&

    If KeyCode = vbKeyReturn Then
        nMaturity = 0&
        nMaturity = Abs(CLng(txtMaturity.Text))
        If nMaturity > 0& Then
            If m_nMaturity <> nMaturity Then
                m_nMaturity = nMaturity
                btnRecalculate_Click
                txtMaturity.SetFocus
            End If
        End If
        txtMaturity.Text = Format$(m_nMaturity, "#,##0")
        
    ElseIf KeyCode = vbKeyEscape Then
        txtMaturity.Text = Format$(m_nMaturity, "#,##0")
    End If
    
End Sub

Private Sub txtMaturity_LostFocus()
    On Error Resume Next
    txtMaturity_KeyDown vbKeyReturn, 0
End Sub
