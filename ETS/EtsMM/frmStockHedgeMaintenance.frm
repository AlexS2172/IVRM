VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.Form frmStockHedgeMaintenance 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Stock Hedge Maintenance"
   ClientHeight    =   7740
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   4665
   Icon            =   "frmStockHedgeMaintenance.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   7740
   ScaleWidth      =   4665
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   3060
      TabIndex        =   1
      Top             =   7320
      Width           =   1455
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "OK"
      Height          =   315
      Left            =   1500
      TabIndex        =   0
      Top             =   7320
      Width           =   1455
   End
   Begin VSFlex7Ctl.VSFlexGrid fg 
      Height          =   7095
      Left            =   120
      TabIndex        =   2
      Top             =   120
      Width           =   4395
      _cx             =   7752
      _cy             =   12515
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
   Begin VB.Menu mnuPopup 
      Caption         =   "<Popup>"
      Visible         =   0   'False
      Begin VB.Menu mnuCopy 
         Caption         =   "Copy"
         Shortcut        =   ^C
      End
      Begin VB.Menu mnuPrint 
         Caption         =   "Print..."
      End
   End
End
Attribute VB_Name = "frmStockHedgeMaintenance"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Public Event OnOK()
'
'Private m_frmOwner As Form
'Private m_bIsOpened As Boolean
'Private m_bDirty As Boolean
'Private m_bInitNow As Boolean
'Private m_bInProc As Boolean
'Private m_bKeyDown As Boolean
'
'Private m_Grp As New EtsMmStockHedgeLib.MmShTraderGroupColl
'
'Public Sub Execute(ByRef frmOwner As Form)
'    On Error GoTo EH
'    If m_bIsOpened Then Exit Sub
'    Screen.MousePointer = vbHourglass
'    Set m_frmOwner = frmOwner
'    If Not m_frmOwner Is Nothing Then m_frmOwner.Enabled = False
'
'    m_bIsOpened = True
'    m_bDirty = False
'    m_bKeyDown = False
'    m_bInitNow = True
'    m_bInProc = False
'
'    Load Me
'
'    FormatGrid
'    LoadData
'
'    m_bInitNow = False
'
'    Me.Show vbModeless, m_frmOwner
'    Screen.MousePointer = vbDefault
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to open stock hedge maintenance dialog."
'    On Error Resume Next
'    Unload Me
'End Sub
'
'Private Function CheckTriState(enStatus As EtsMmStockHedgeLib.MmShGroupStatusEnum) As VSFlex7Ctl.CellCheckedSettings
'    On Error Resume Next
'    If enStatus = enMmShGroupStatusAll Then
'        CheckTriState = flexTSChecked
'    ElseIf enStatus = enMmShGroupStatusChilds Then
'        CheckTriState = flexTSGrayed
'    Else
'        CheckTriState = flexTSUnchecked
'    End If
'End Function
'
'Private Function CheckState(enStatus As EtsMmStockHedgeLib.MmShGroupStatusEnum) As VSFlex7Ctl.CellCheckedSettings
'    On Error Resume Next
'    If enStatus = enMmShGroupStatusNone Then
'        CheckState = flexUnchecked
'    Else
'        CheckState = flexChecked
'    End If
'End Function
'
'Private Function StatusFromCheck(enCheck As VSFlex7Ctl.CellCheckedSettings) As EtsMmStockHedgeLib.MmShGroupStatusEnum
'    On Error Resume Next
'    If enCheck = flexChecked Or enCheck = flexTSChecked Then
'        StatusFromCheck = enMmShGroupStatusAll
'    ElseIf enCheck = flexTSGrayed Then
'        StatusFromCheck = enMmShGroupStatusChilds
'    Else
'        StatusFromCheck = enMmShGroupStatusNone
'    End If
'End Function
'
'Private Sub LoadData()
'    On Error GoTo EH
'    Dim rs As ADODB.Recordset
'
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aRowData As clsShRowData, nRow&
'
'    With fg
'        .Redraw = flexRDNone
'        .Rows = 1
'
'        m_Grp.Clear
'
'        Set rs = gDBW.usp_MmHedgeData_Get
'        Do While Not rs.EOF
'            HedgeDataReadRow rs
'            rs.MoveNext
'        Loop
'
'        For Each aTrdGrp In m_Grp
'            .AddItem ""
'            nRow = .Rows - 1
'            .RowOutlineLevel(nRow) = 0
'            .IsSubtotal(nRow) = True
'
'            Set aRowData = New clsShRowData
'            Set aRowData.TrdGrp = aTrdGrp
'
'            .RowData(nRow) = aRowData
'            .TextMatrix(nRow, 0) = aTrdGrp.Trader.Name
'            .TextMatrix(nRow, 1) = aTrdGrp.Trader.Name
'            .TextMatrix(nRow, 2) = aTrdGrp.Trader.Name
'            .Cell(flexcpChecked, nRow, 3) = CheckTriState(aTrdGrp.Status)
'            .Cell(flexcpBackColor, nRow, 0, nRow, .Cols - 1) = &HC0FFC0 'GetAlternateColor(GetSysColor(COLOR_WINDOW))
'            .Cell(flexcpFontBold, nRow, 0, nRow, .Cols - 1) = True
'            .MergeRow(nRow) = True
'
'            Set aRowData = Nothing
'
'            For Each aStgGrp In aTrdGrp.Strategy
'                For Each aUndGrp In aStgGrp.Und
'                    .AddItem ""
'                    nRow = .Rows - 1
'                    .RowOutlineLevel(nRow) = 1
'                    .IsSubtotal(nRow) = False
'
'                    Set aRowData = New clsShRowData
'                    Set aRowData.TrdGrp = aTrdGrp
'                    Set aRowData.StgGrp = aStgGrp
'                    Set aRowData.UndGrp = aUndGrp
'
'                    .RowData(nRow) = aRowData
'                    .TextMatrix(nRow, 0) = " " 'aTrdGrp.Trader.Name
'                    .TextMatrix(nRow, 1) = aStgGrp.Strategy.Name
'                    .TextMatrix(nRow, 2) = aUndGrp.Und.Symbol
'                    .Cell(flexcpChecked, nRow, 3) = CheckState(aUndGrp.Status)
'
'                    Set aRowData = Nothing
'                Next
'            Next
'        Next
'
'        .MergeCol(0) = True
'
'        If .Rows > 1 Then .Row = 1
'
'        .Cell(flexcpPictureAlignment, 1, 3, .Rows - 1, 3) = flexPicAlignCenterCenter
'        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
'
'        .AutoSize 0, .Cols - 1, False, 100
'        .Redraw = flexRDBuffered
'    End With
'
'    Exit Sub
'EH:
'    fg.Redraw = flexRDBuffered
'    gCmn.ErrorMsgBox Me, "Fail to load stock hedge maintenance settings."
'End Sub
'
'Private Sub HedgeDataReadRow(rs As ADODB.Recordset)
'    Dim aTrdGrp As EtsMmStockHedgeLib.MmShTraderGroupAtom
'    Dim aStgGrp As EtsMmStockHedgeLib.MmShStrategyGroupAtom
'    Dim aUndGrp As EtsMmStockHedgeLib.MmShUndGroupAtom
'    Dim aTrader As EtsGeneralLib.EtsTraderAtom
'    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
'    Dim aUnd As EtsGeneralLib.UndAtom
'    Dim nStrategyID&, nUndID&
'
'    Set aTrader = g_Trader(ReadLng(rs!iTraderID))
'    If Not aTrader Is Nothing Then
'        nStrategyID = ReadLng(rs!iStrategyID)
'        If nStrategyID <> 0 Then
'            Set aStrategy = g_Strategy(nStrategyID)
'        Else
'            nStrategyID = NO_STRATEGY_ID
'        End If
'
'        nUndID = ReadLng(rs!iContractID)
'        Set aUnd = g_Stock(nUndID)
'
'        If nStrategyID = NO_STRATEGY_ID And aUnd Is Nothing _
'            Or (nStrategyID = NO_STRATEGY_ID Or Not aStrategy Is Nothing) And Not aUnd Is Nothing Then
'
'            Set aTrdGrp = m_Grp(aTrader.ID)
'            If aTrdGrp Is Nothing Then
'                Set aTrdGrp = m_Grp.Add(aTrader.ID, aTrader.Name)
'                Set aTrdGrp.Trader = aTrader
'                aTrdGrp.Status = DEFAULT_TRADER_HEDGE_STATUS
'            End If
'
'            If Not aUnd Is Nothing Then
'                Set aStgGrp = aTrdGrp.Strategy(nStrategyID)
'                If aStgGrp Is Nothing Then
'                    If nStrategyID = NO_STRATEGY_ID Then
'                        Set aStrategy = New EtsGeneralLib.EtsStrategyAtom
'                        aStrategy.ID = NO_STRATEGY_ID
'                        aStrategy.Name = NO_STRATEGY_NAME
'                    End If
'
'                    Set aStgGrp = aTrdGrp.Strategy.Add(aStrategy.ID, aStrategy.Name)
'                    Set aStgGrp.Strategy = aStrategy
'                End If
'
'                Set aUndGrp = aStgGrp.Und(aUnd.ID)
'                If aUndGrp Is Nothing Then
'                    Set aUndGrp = aStgGrp.Und.Add(aUnd.ID, aUnd.Symbol)
'                    Set aUndGrp.Und = aUnd
'                    aUndGrp.Status = ReadByte(rs!tiState)
'                    Set aUndGrp = Nothing
'                End If
'
'                Set aStgGrp = Nothing
'
'            ElseIf nUndID = 0 Then
'                aTrdGrp.Status = ReadByte(rs!tiState)
'            End If
'            Set aTrdGrp = Nothing
'        End If
'
'        Set aTrader = Nothing
'    End If
'End Sub
'
'Private Sub SaveData()
'    On Error GoTo EH
'    Dim aRowData As clsShRowData, nRow&, enStatus As EtsMmStockHedgeLib.MmShGroupStatusEnum
'    If m_bInProc Then Exit Sub
'    m_bInProc = True
'
'    For nRow = 1 To fg.Rows - 1
'        Set aRowData = fg.RowData(nRow)
'        If Not aRowData Is Nothing Then
'            enStatus = StatusFromCheck(fg.Cell(flexcpChecked, nRow, 3))
'
'            If Not aRowData.UndGrp Is Nothing Then
'                If aRowData.UndGrp.Status <> enStatus Then
'                    gDBW.usp_MmHedgeData_Save _
'                        aRowData.TrdGrp.Trader.ID, _
'                        IIf(aRowData.StgGrp.Strategy.ID <> NO_STRATEGY_ID, aRowData.StgGrp.Strategy.ID, 0), _
'                        aRowData.UndGrp.Und.ID, enStatus
'                End If
'            Else
'                If aRowData.TrdGrp.Status <> enStatus Then _
'                    gDBW.usp_MmHedgeData_Save aRowData.TrdGrp.Trader.ID, 0, 0, enStatus
'            End If
'            Set aRowData = Nothing
'        End If
'    Next
'    m_bInProc = False
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox Me, "Fail to save stock hedge maintenance settings."
'End Sub
'
'Public Property Get IsOpened() As Boolean
'    On Error Resume Next
'    IsOpened = m_bIsOpened
'End Property
'
'Private Sub btnCancel_Click()
'    On Error Resume Next
'    If m_bInProc Then
'        m_bInProc = False
'        Exit Sub
'    End If
'
'    Unload Me
'End Sub
'
'Private Sub btnOK_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'
'    If m_bDirty Then
'        Screen.MousePointer = vbHourglass
'
'        fg.Enabled = False
'        btnCancel.Enabled = False
'        btnOK.Enabled = False
'
'        SaveData
'        RaiseEvent OnOK
'        Screen.MousePointer = vbDefault
'    End If
'    Unload Me
'End Sub
'
'Private Sub fg_AfterEdit(ByVal Row As Long, ByVal Col As Long)
'    On Error Resume Next
'    If m_bInProc Or Col <> 3 Then Exit Sub
'    m_bDirty = True
'End Sub
'
'Private Sub fg_KeyDown(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'        m_bKeyDown = True
'    End If
'End Sub
'
'Private Sub fg_KeyUp(KeyCode As Integer, Shift As Integer)
'    On Error Resume Next
'    If m_bKeyDown Then
'        m_bKeyDown = False
'
'        If (KeyCode = vbKeyC Or KeyCode = vbKeyInsert) And Shift = vbCtrlMask Then
'            mnuCopy_Click
'        End If
'    End If
'End Sub
'
'Private Sub fg_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
'    On Error Resume Next
'    If Button = vbRightButton Then
'        PopupMenu mnuPopup
'    End If
'End Sub
'
'Private Sub fg_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
'    On Error Resume Next
'    Cancel = (Col <> 3)
'End Sub
'
'Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
'    On Error Resume Next
'    If UnloadMode <> vbFormCode Then
'        Cancel = m_bInProc
'    Else
'        m_bInProc = False
'    End If
'End Sub
'
'Private Sub Form_Terminate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    If Not m_frmOwner Is Nothing Then
'        m_frmOwner.Enabled = True
'        m_frmOwner.SetFocus
'    End If
'
'    Set m_frmOwner = Nothing
'    m_bIsOpened = False
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    Form_Terminate
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'End Sub
'
'Private Sub FormatGrid()
'    On Error Resume Next
'    With fg
'        .Redraw = flexRDNone
'        .Rows = 1
'        .Cols = 4
'
'        .AllowBigSelection = False
'        .AllowSelection = False
'        .AllowUserFreezing = flexFreezeNone
'        .AllowUserResizing = flexResizeNone
'        .Appearance = flex3DLight
'        .AutoSizeMode = flexAutoSizeColWidth
'        .AutoSearch = flexSearchNone
'        .AutoSizeMouse = False
'        .Editable = flexEDKbdMouse
'        .Ellipsis = flexEllipsisEnd
'        .ExplorerBar = flexExNone
'        .ExtendLastCol = False
'        .FixedCols = 0
'        .FixedRows = 1
'        .FocusRect = flexFocusSolid
'        .FrozenCols = 0
'        .FrozenRows = 0
'        .OutlineBar = flexOutlineBarSymbolsLeaf
'        .OutlineCol = 0
'        .ScrollBars = flexScrollBarBoth
'        .ScrollTrack = True
'        .SelectionMode = flexSelectionFree
'        .TabBehavior = flexTabControls
'        .MergeCells = flexMergeFree
'        .MergeCompare = flexMCExact
'
'        .TextMatrix(0, 0) = "Trader"
'        .ColDataType(0) = flexDTString
'        .ColFormat(0) = ""
'
'        .TextMatrix(0, 1) = "Strategy"
'        .ColDataType(1) = flexDTString
'        .ColFormat(1) = ""
'
'        .TextMatrix(0, 2) = "Stock"
'        .ColDataType(2) = flexDTString
'        .ColFormat(2) = ""
'
'        .TextMatrix(0, 3) = "Hedge"
'        .ColDataType(3) = flexDTString
'        .ColFormat(3) = ""
'
'        .Redraw = flexRDBuffered
'    End With
'End Sub
'
'Private Sub mnuCopy_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    g_ClipMgr.CopyGridToClipboard fg
'    Screen.MousePointer = vbDefault
'End Sub
'
'Private Sub mnuPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute Me, "Stock Hedge Maintenance", "Stock Hedge Maintenance", fg
'    Screen.MousePointer = vbDefault
'End Sub
'
'
