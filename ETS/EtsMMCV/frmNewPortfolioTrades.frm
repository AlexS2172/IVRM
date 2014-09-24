VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.Form frmNewPortfolioTrades 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "New Portfolio Trades"
   ClientHeight    =   5385
   ClientLeft      =   2760
   ClientTop       =   3750
   ClientWidth     =   9015
   Icon            =   "frmNewPortfolioTrades.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5385
   ScaleWidth      =   9015
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.ComboBox cbxStrategy 
      Height          =   315
      Left            =   2340
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   1
      Top             =   300
      Width           =   2115
   End
   Begin VB.ComboBox cbxClearingBroker 
      Height          =   315
      Left            =   6780
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   3
      Top             =   300
      Width           =   2115
   End
   Begin VB.ComboBox cbxBroker 
      Height          =   315
      Left            =   4560
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   2
      Top             =   300
      Width           =   2115
   End
   Begin VB.ComboBox cbxTrader 
      Height          =   315
      Left            =   120
      Sorted          =   -1  'True
      Style           =   2  'Dropdown List
      TabIndex        =   0
      Top             =   300
      Width           =   2115
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   7440
      TabIndex        =   6
      Top             =   4920
      Width           =   1455
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "Create"
      Height          =   315
      Left            =   5880
      TabIndex        =   5
      Top             =   4920
      Width           =   1455
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTrd 
      Height          =   4035
      Left            =   120
      TabIndex        =   4
      Top             =   720
      Width           =   8775
      _cx             =   15478
      _cy             =   7117
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
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Strategy:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   2340
      TabIndex        =   11
      Top             =   60
      Width           =   855
   End
   Begin VB.Label lblTradesCount 
      BackStyle       =   0  'Transparent
      Caption         =   "0 trades(s)"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   10
      Top             =   4860
      Width           =   2175
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Trader:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   9
      Top             =   60
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Broker:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   4560
      TabIndex        =   8
      Top             =   45
      Width           =   855
   End
   Begin VB.Label lblTitle 
      BackStyle       =   0  'Transparent
      Caption         =   "Clearing Broker:"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   4
      Left            =   6780
      TabIndex        =   7
      Top             =   60
      Width           =   1215
   End
End
Attribute VB_Name = "frmNewPortfolioTrades"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_mbResult As VbMsgBoxResult
Private m_hComplete As Long
Private m_GridLock As New clsGridLock
Private geTrd As New clsGridEx
Attribute geTrd.VB_VarHelpID = -1
Private m_gdTrd As New clsGridDef
Private m_sCurrentOriginalText As String

Private m_nTraderID As Long
Private m_nStrategyID As Long
Private m_nBrokerID As Long
Private m_nClearingBrokerID As Long
Private m_bInitNow As Boolean
Private m_Trades As EtsGeneralLib.MmTradeInfoColl

Public Function Execute(ByRef collTrades As EtsGeneralLib.MmTradeInfoColl, _
                        ByRef frmOwner As Form) As VbMsgBoxResult
    On Error GoTo EH
    Dim bRestoreEnabled As Boolean
    
    Screen.MousePointer = vbHourglass
    m_mbResult = vbCancel
    m_bInitNow = True
    Load Me
    LoadRegEntries
    Set m_Trades = collTrades
    
    If Not frmOwner Is Nothing Then
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            
            InitCombos
            ShowTrades
            
            m_bInitNow = False
            
            Screen.MousePointer = vbDefault
            Me.Show vbModeless, frmOwner
        
            While m_hComplete <> 0 And MsgWaitForMultipleObjects(1, m_hComplete, 0, INFINITE, QS_ALLEVENTS) <> WAIT_OBJECT_0
                Sleep 0
                DoEvents
            Wend
            
            If m_hComplete <> 0 Then
                CloseHandle m_hComplete
                m_hComplete = 0
            End If
            
            If bRestoreEnabled Then frmOwner.Enabled = True
        Else
            ShowTrades
            m_bInitNow = False
            Screen.MousePointer = vbDefault
            Me.Show vbModal
        End If
    Else
        ShowTrades
        m_bInitNow = False
        Screen.MousePointer = vbDefault
        Me.Show vbModal
    End If
    
    If m_mbResult = vbOK Then SaveRegEntries
    
    Set m_Trades = Nothing
    Unload Me
    Execute = m_mbResult
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open expression editor window."
    On Error Resume Next
    Set m_Trades = Nothing
    If bRestoreEnabled Then frmOwner.Enabled = True
    Unload Me
End Function

Private Sub btnCancel_Click()
    On Error Resume Next
    m_mbResult = vbCancel
    
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub btnOk_Click()
    On Error Resume Next
    m_mbResult = vbOK
    
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub cbxBroker_Click()
    On Error Resume Next
    If Not m_bInitNow Then
        If cbxBroker.ListIndex > -1 Then
            m_nBrokerID = cbxBroker.ItemData(cbxBroker.ListIndex)
        Else
            m_nBrokerID = 0
        End If
        TradesUpdateCommissions
    End If
End Sub

Private Sub cbxClearingBroker_Click()
    On Error Resume Next
    If Not m_bInitNow Then
        If cbxClearingBroker.ListIndex > -1 Then
            m_nClearingBrokerID = cbxClearingBroker.ItemData(cbxClearingBroker.ListIndex)
        Else
            m_nClearingBrokerID = 0
        End If
        TradesUpdateCommissions
    End If
End Sub

Private Sub cbxStrategy_Click()
    On Error Resume Next
    If Not m_bInitNow Then
        If cbxStrategy.ListIndex > -1 Then
            m_nStrategyID = cbxStrategy.ItemData(cbxStrategy.ListIndex)
        Else
            m_nStrategyID = 0
        End If
        TradesUpdateStrategy
    End If
End Sub

Private Sub cbxTrader_Click()
    On Error Resume Next
    If Not m_bInitNow Then
        If cbxTrader.ListIndex > -1 Then
            m_nTraderID = cbxTrader.ItemData(cbxTrader.ListIndex)
        Else
            m_nTraderID = 0
        End If
        TradesUpdateTrader
    End If
End Sub

Private Sub fgTrd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&, sValue$, nValue&, aTrd As EtsGeneralLib.MmTradeInfoAtom, nCol&
    
    With fgTrd
        nIdx = .ColKey(Col)
        If nIdx = TLC_QTY Then
            sValue = .TextMatrix(Row, Col)
            Set aTrd = .RowData(Row)
            If Not aTrd Is Nothing And sValue <> m_sCurrentOriginalText Then
                nValue = ReadLng(sValue)
                If aTrd.Quantity <> nValue Then
                    aTrd.Quantity = nValue
                    
                    nCol = .ColIndex(TLC_TOT_PRICE)
                    If nCol >= 0 Then .TextMatrix(Row, nCol) = aTrd.TotalPrice
                    
                    TradeCalcCommission aTrd
                    
                    nCol = .ColIndex(TLC_COMMISSION)
                    If nCol >= 0 Then .TextMatrix(Row, nCol) = aTrd.TotalCommission
                End If
            End If
            Set aTrd = Nothing
        End If
    End With
End Sub

Private Sub fgTrd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim nIdx&
    
    With fgTrd
        nIdx = .ColKey(Col)
        Cancel = (nIdx <> TLC_QTY)
        If Not Cancel Then
            m_sCurrentOriginalText = .TextMatrix(Row, Col)
        End If
    End With
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_GridLock.Init fgTrd
    geTrd.Init fgTrd
    g_DefGrids(GT_TRADES_LIST).CopyTo m_gdTrd
    FormatGrid
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_hComplete <> 0 Then SetEvent m_hComplete
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If m_hComplete <> 0 Then
        CloseHandle m_hComplete
        m_hComplete = 0
    End If
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    Dim i&, nIdx&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        .Rows = 1
        .Cols = 1
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight ' flexFlat
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortAndMove
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusHeavy
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionByRow
        .TabBehavior = flexTabControls
    
        .Cols = TLC_LAST_COLUMN
        
        nIdx = 0
        For i = TLC_TYPE To TLC_COMMISSION
            If i < TLC_DATE Or i > TLC_CLR_BROKER Then
                .ColKey(nIdx) = i
                .ColDataType(nIdx) = m_gdTrd.Col(i).DataType
                .ColFormat(nIdx) = m_gdTrd.Col(i).Format
                .TextMatrix(0, nIdx) = m_gdTrd.Col(i).Caption
                nIdx = nIdx + 1
            End If
        Next
        .Cols = nIdx
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        geTrd.AdjustSortImages
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub ShowTrades()
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, nRow&, i&
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    Dim aBroker As EtsGeneralLib.EtsBrokerAtom
    Dim aClearingBroker As EtsGeneralLib.EtsBrokerAtom
    
    With fgTrd
        m_GridLock.LockRedraw
        .Rows = 1
        
        Set aTrader = g_Trader(m_nTraderID)
        Set aStrategy = g_Strategy(m_nStrategyID)
        Set aBroker = g_Broker(m_nBrokerID)
        Set aClearingBroker = g_ClearingBroker(m_nClearingBrokerID)
        
        For Each aTrd In m_Trades
            
            Set aTrd.Trader = aTrader
            Set aTrd.Strategy = aStrategy
            Set aTrd.Broker = aBroker
            Set aTrd.ClearingBroker = aClearingBroker
            
            TradeCalcCommission aTrd
            
            nRow = .Rows
            .AddItem ""
            
            .RowData(nRow) = aTrd
            
            TradeUpdate nRow
        Next
        
        Set aTrader = Nothing
        Set aStrategy = Nothing
        Set aBroker = Nothing
        Set aClearingBroker = Nothing
        
        i = -1
        i = .ColIndex(TLC_UNDERLYING)
        If i >= 0 Then
            .Col = i
            .Row = 1
            .Sort = flexSortGenericAscending
        End If
        geTrd.ShowSortImage i, 1
        
        .Row = 1
        .Col = 1
        
        i = .ColIndex(TLC_QTY)
        If i > -1 Then .Cell(flexcpFontBold, 1, i, .Rows - 1, i) = True
        
        .AutoSize 0, .Cols - 1, False, 0
        
        m_GridLock.UnlockRedraw
    End With
    
    lblTradesCount.Caption = CStr(m_Trades.Count) & " trade(s)"
End Sub

Private Sub TradesUpdateTrader()
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    
    For Each aTrd In m_Trades
        Set aTrd.Trader = g_Trader(m_nTraderID)
    Next
End Sub

Private Sub TradeCalcCommission(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim dRate#, dClearingRate#, nCommType&
    
    dRate = 0#
    dClearingRate = 0#
    
    With aTrd
        If Not .Broker Is Nothing Or Not .ClearingBroker Is Nothing Then
            If .ContractType = enCtOption Or .ContractType = enCtFutOption Then
                
                If .Price > g_Params.CommissionLowPremiumLimit Then
                    nCommType = 0
                Else
                    nCommType = 1
                End If
                
                Select Case nCommType
                    Case 0 ' Regular
                        dRate = .Broker.OptRegComm
                        dClearingRate = .ClearingBroker.OptRegComm
                        
                    Case 1 ' LowPrem
                        dRate = .Broker.LowPremComm
                        dClearingRate = .ClearingBroker.LowPremComm
                        
                    Case 2 ' Spread
                        dRate = .Broker.OptSpreadComm
                        dClearingRate = .ClearingBroker.OptSpreadComm
                End Select
                
            Else
                dRate = .Broker.UndComm
                dClearingRate = .ClearingBroker.UndComm
            End If
            
            .BrokerCommission = .Quantity * .LotSize * dRate
            .ClearingBrokerCommission = .Quantity * .LotSize * dClearingRate
        Else
            .BrokerCommission = 0#
            .ClearingBrokerCommission = 0#
        End If
    End With
End Sub

Private Sub TradesUpdateCommissions()
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aBroker As EtsGeneralLib.EtsBrokerAtom
    Dim aClearingBroker As EtsGeneralLib.EtsBrokerAtom
    Dim nCol&, nRow&
    
    Set aBroker = g_Broker(m_nBrokerID)
    Set aClearingBroker = g_ClearingBroker(m_nClearingBrokerID)
    
    With fgTrd
        m_GridLock.LockRedraw
    
        nCol = .ColIndex(TLC_COMMISSION)
        If nCol >= 0 Then
            For nRow = 1 To .Rows - 1
                Set aTrd = .RowData(nRow)
                
                If Not aTrd Is Nothing Then
                    Set aTrd.Broker = aBroker
                    Set aTrd.ClearingBroker = aClearingBroker
                    
                    TradeCalcCommission aTrd
                    .TextMatrix(nRow, nCol) = aTrd.TotalCommission
                End If
                
                Set aTrd = Nothing
            Next
        End If

        m_GridLock.UnlockRedraw
    End With

    Set aBroker = Nothing
    Set aClearingBroker = Nothing
End Sub

Private Sub TradesUpdateStrategy()
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    Set aStrategy = g_Strategy(m_nStrategyID)
    For Each aTrd In m_Trades
        Set aTrd.Strategy = aStrategy
    Next
    Set aStrategy = Nothing
End Sub

Private Sub TradeUpdate(ByVal nRow As Long)
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, nCol&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        Set aTrd = .RowData(nRow)
        If Not aTrd Is Nothing Then
            
            nCol = .ColIndex(TLC_TYPE)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.GetTradeTypeString
            
            nCol = .ColIndex(TLC_UNDERLYING)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Und.Symbol
            
            nCol = .ColIndex(TLC_SYMBOL)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Symbol
            
            If aTrd.ContractType = enCtOption Then
                nCol = .ColIndex(TLC_OPT_TYPE)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = IIf(aTrd.Opt.OptType = enOtCall, "C", "P")
            
                nCol = .ColIndex(TLC_EXPIRY)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Opt.Expiry
            
                nCol = .ColIndex(TLC_STRIKE)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Opt.Strike
            Else
                nCol = .ColIndex(TLC_OPT_TYPE)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = ""
            
                nCol = .ColIndex(TLC_EXPIRY)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = ""
            
                nCol = .ColIndex(TLC_STRIKE)
                If nCol >= 0 Then .TextMatrix(nRow, nCol) = ""
            End If
            
            nCol = .ColIndex(TLC_BS)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.IsBuy
            
            nCol = .ColIndex(TLC_PRICE)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Price
            
            nCol = .ColIndex(TLC_QTY)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Quantity
            
            nCol = .ColIndex(TLC_LOTSIZE)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.LotSize
            
            nCol = .ColIndex(TLC_TOT_PRICE)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.TotalPrice
            
            nCol = .ColIndex(TLC_COMMISSION)
            If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.TotalCommission
            
            Set aTrd = Nothing
        End If
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub InitCombos()
    On Error Resume Next
    FillTraderCombo m_nTraderID
    FillStrategyCombo m_nStrategyID
    FillBrokerCombo cbxBroker, g_Broker, m_nBrokerID
    FillBrokerCombo cbxClearingBroker, g_ClearingBroker, m_nClearingBrokerID
End Sub

Private Sub FillBrokerCombo(ByRef aCbx As VB.ComboBox, ByRef aBrokerColl As EtsGeneralLib.EtsBrokerColl, ByRef nValue As Long)
    On Error Resume Next
    Dim aBroker As EtsGeneralLib.EtsBrokerAtom
    
    With aCbx
        .Clear
        
        .AddItem "<None>"
        .ItemData(.NewIndex) = 0
        If nValue = 0 Then .ListIndex = 0
        
        For Each aBroker In aBrokerColl
            .AddItem aBroker.Name
            .ItemData(.NewIndex) = aBroker.ID
            If nValue = aBroker.ID Then .ListIndex = .NewIndex
        Next
        
        If .ListCount > 0 Then
            If .ListIndex < 0 Then
                .ListIndex = 0
            End If
            nValue = .ItemData(.ListIndex)
        Else
            nValue = 0
        End If
    End With
End Sub

Private Sub FillTraderCombo(ByRef nValue As Long)
    On Error Resume Next
    Dim aTrader As EtsGeneralLib.EtsTraderAtom
    
    With cbxTrader
        .Clear
        
        For Each aTrader In g_Trader
            .AddItem aTrader.Name
            .ItemData(.NewIndex) = aTrader.ID
            If nValue = aTrader.ID Then .ListIndex = .NewIndex
        Next
        
        If .ListCount > 0 Then
            If .ListIndex < 0 Then
                .ListIndex = 0
            End If
            nValue = .ItemData(.ListIndex)
        Else
            nValue = 0
        End If
    End With
End Sub

Private Sub FillStrategyCombo(ByRef nValue As Long)
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    With cbxStrategy
        .Clear
        
        .AddItem "<None>"
        .ItemData(.NewIndex) = 0
        If nValue = 0 Then .ListIndex = .NewIndex
        
        For Each aStrategy In g_Strategy
            .AddItem aStrategy.Name
            .ItemData(.NewIndex) = aStrategy.ID
            If nValue = aStrategy.ID Then .ListIndex = .NewIndex
        Next
        
        If .ListCount > 0 Then
            If .ListIndex < 0 Then
                .ListIndex = 0
            End If
            nValue = .ItemData(.ListIndex)
        Else
            nValue = 0
        End If
    End With
End Sub

Private Sub LoadRegEntries()
    On Error Resume Next
    If g_CurTraderID = 0 Then
        m_nTraderID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "NewPortfolioTrades", "TraderID", 0)
    Else
        m_nTraderID = g_CurTraderID
    End If
    m_nStrategyID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "NewPortfolioTrades", "StrategyID", 0)
    m_nBrokerID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "NewPortfolioTrades", "BrokerID", 0)
    m_nClearingBrokerID = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "NewPortfolioTrades", "ClearingBrokerID", 0)
End Sub

Private Sub SaveRegEntries()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "NewPortfolioTrades", "TraderID", m_nTraderID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "NewPortfolioTrades", "StrategyID", m_nStrategyID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "NewPortfolioTrades", "BrokerID", m_nBrokerID
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "NewPortfolioTrades", "ClearingBrokerID", m_nClearingBrokerID
End Sub

