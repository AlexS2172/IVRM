VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.UserControl ctlTradeView 
   ClientHeight    =   7350
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9630
   FillColor       =   &H8000000E&
   KeyPreview      =   -1  'True
   ScaleHeight     =   7350
   ScaleWidth      =   9630
   Begin VB.Timer tmrShow 
      Enabled         =   0   'False
      Interval        =   10
      Left            =   840
      Top             =   4680
   End
   Begin VSFlex7Ctl.VSFlexGrid fgFlt 
      Height          =   495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   7575
      _cx             =   13361
      _cy             =   873
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
   Begin VSFlex7Ctl.VSFlexGrid fgTrd 
      Height          =   2535
      Left            =   0
      TabIndex        =   1
      Top             =   600
      Width           =   7575
      _cx             =   13361
      _cy             =   4471
      _ConvInfo       =   1
      Appearance      =   1
      BorderStyle     =   1
      Enabled         =   -1  'True
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
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
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxTradeNew 
         Caption         =   "New Trade..."
      End
      Begin VB.Menu mnuCtxTradeEdit 
         Caption         =   "Edit Trade..."
      End
      Begin VB.Menu mnuCtxTradeDelete 
         Caption         =   "Delete Trade(s)..."
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxPositionTransfer 
         Caption         =   "Position Transfer..."
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxTradeExercise 
         Caption         =   "Exercise Trade..."
      End
      Begin VB.Menu mnuCtxTradeExpiry 
         Caption         =   "Expiry Trade..."
      End
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDefTrdSort 
         Caption         =   "Default Trades Sorting"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator7 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxHideCol 
         Caption         =   "Hide Column"
      End
      Begin VB.Menu mnuCtxAutosizeCol 
         Caption         =   "Autosize Column"
      End
      Begin VB.Menu mnuCtxAutosizeGrid 
         Caption         =   "Autosize Grid"
      End
      Begin VB.Menu mnuCtxSeparator8 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
   End
End
Attribute VB_Name = "ctlTradeView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False

Public Event OnSetCaption()
Public Event OnStateChange()

Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As VB.Label
Public lblStatus As VB.Label

Private m_gdFlt As New clsGridDef
Private m_gdTrd As New clsGridDef

Private WithEvents geTrd As clsGridEx
Attribute geTrd.VB_VarHelpID = -1

Private m_bInProc As Boolean
Private m_bShutDown As Boolean
Private m_sCurrentOriginalText As String

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_TRADES_FILTER To GT_TRADES_LIST) As Boolean
Private m_GridLock(GT_TRADES_FILTER To GT_TRADES_LIST) As New clsGridLock
Private m_nTrades As Long
Private m_AuxTradeView As EtsMmGeneralLib.MmTradeView


Public m_frmOwner As Form

Public Function Init() As Boolean
    On Error GoTo EH
    
    mnuCtxTradeNew.Caption = "New Trade..." & vbTab & "Ins"
    mnuCtxTradeDelete.Caption = "Delete Trade(s)..." & vbTab & "Del"
    mnuCtxCopy.Caption = "Copy Grid" & vbTab & "Ctrl+C"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_TRADES_FILTER) = False
    m_bKeyDown(GT_TRADES_LIST) = False
    m_nTrades = 0
    
    Set m_AuxTradeView = New EtsMmGeneralLib.MmTradeView
    Set m_AuxTradeView.EtsMain = g_Main
    Set m_AuxTradeView.TradeChannel = g_TradeChannel.TradeChannel
    
    m_GridLock(GT_TRADES_FILTER).Init fgFlt
    m_GridLock(GT_TRADES_LIST).Init fgTrd
    
    m_AuxTradeView.TradeViewColumnsOrder = m_gdTrd.IdxCopy
    fgTrd.FlexDataSource = m_AuxTradeView
    
    InitColumns
    InitGrids
    
    Set geTrd = New clsGridEx
    geTrd.Init fgTrd
    
    FormatFltGrid
    FormatFltColumns
    
    FormatTrdGrid
    FormatTrdColumns
    
    InitFltData
    
    Set TradeChannel = g_TradeChannel
    
    fgFlt.Col = TFC_UNDERLYING
    fgFlt.Row = 1
    
    Set frmLayout = New frmGridLayout
    Init = True
    
    AdjustCaption
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to init trades view."
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Select Case nType
        Case TYPE_UNDERLYING
            m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) = nValue
        Case TYPE_GROUP
            m_AuxTradeView.TradeViewFilter(TFC_GROUP) = nValue
        Case TYPE_TRADER_GROUP
            m_AuxTradeView.TradeViewFilter(TFC_TRADER_GROUP) = nValue
        Case TYPE_TRADER
            m_AuxTradeView.TradeViewFilter(TFC_TRADER) = nValue
        Case TYPE_STRATEGY
            m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) = nValue
        Case TYPE_POSITION
            m_AuxTradeView.TradeViewFilter(TFC_POSTYPE) = nValue
    End Select
    
    FilterUpdate
    
    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdate()
    On Error Resume Next
    Dim nValue&
    
    With fgFlt
        m_GridLock(GT_TRADES_FILTER).LockRedraw
        
        nValue = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING)
        If g_Underlying(nValue) Is Nothing Then nValue = 0
        m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) = nValue
        .TextMatrix(1, TFC_UNDERLYING) = nValue
        
        .TextMatrix(1, TFC_TYPE) = m_AuxTradeView.TradeViewFilter(TFC_TYPE)

        nValue = m_AuxTradeView.TradeViewFilter(TFC_GROUP)
        If g_UnderlyingGroup(nValue) Is Nothing Then nValue = 0
        m_AuxTradeView.TradeViewFilter(TFC_GROUP) = nValue
        .TextMatrix(1, TFC_GROUP) = nValue
        
        nValue = m_AuxTradeView.TradeViewFilter(TFC_TRADER_GROUP)
        If g_TraderGroup(nValue) Is Nothing Then nValue = 0
        m_AuxTradeView.TradeViewFilter(TFC_TRADER_GROUP) = nValue
        .TextMatrix(1, TFC_TRADER_GROUP) = nValue
        
        nValue = m_AuxTradeView.TradeViewFilter(TFC_TRADER)
        If g_Trader(nValue) Is Nothing Then nValue = 0
        m_AuxTradeView.TradeViewFilter(TFC_TRADER) = nValue
        .TextMatrix(1, TFC_TRADER) = nValue
        
        nValue = m_AuxTradeView.TradeViewFilter(TFC_STRATEGY)
        If g_Strategy(nValue) Is Nothing Then nValue = 0
        m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) = nValue
        .TextMatrix(1, TFC_STRATEGY) = nValue
        
        .TextMatrix(1, TFC_POSTYPE) = m_AuxTradeView.TradeViewFilter(TFC_POSTYPE)
       
        .AutoSize 0, .Cols - 1, , 100
       
        m_GridLock(GT_TRADES_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, nValue&
    Dim aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom
    
    With fgFlt
        m_GridLock(GT_TRADES_FILTER).LockRedraw
        
        sValue = "0"
        nValue = 0
        sComboList = g_Params.UnderlyingComboList
        If Len(sComboList) > 0 Then
            sComboList = "#0;<All>|" & sComboList
        Else
            sComboList = "#0;<All>"
        End If
        
        Set aUnd = g_Underlying(m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING))
        If Not aUnd Is Nothing Then
            nValue = aUnd.ID
            sValue = Trim$(Str$(aUnd.ID))
            Set aUnd = Nothing
        End If
        .ColComboList(TFC_UNDERLYING) = sComboList
        .TextMatrix(1, TFC_UNDERLYING) = sValue
        m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) = nValue
    
        .ColComboList(TFC_TYPE) = "#0;<All>|#1;Today's|#2;Simulated|#3;Manual"
        .TextMatrix(1, TFC_TYPE) = "0"
        
        .ColComboList(TFC_POSTYPE) = "#0;Trades|#1;Positions|#2;<All>"
        .TextMatrix(1, TFC_POSTYPE) = "0"
    
    
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aUndGroup In g_UnderlyingGroup
            sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
            If aUndGroup.ID = m_AuxTradeView.TradeViewFilter(TFC_GROUP) Then
                nValue = aUndGroup.ID
                sValue = Trim$(Str$(aUndGroup.ID))
            End If
        Next
        .ColComboList(TFC_GROUP) = sComboList
        .TextMatrix(1, TFC_GROUP) = sValue
        m_AuxTradeView.TradeViewFilter(TFC_GROUP) = nValue
    
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        
        For Each aTraderGroup In g_TraderGroup
            If aTraderGroup.ID <> 0 Then
                        sComboList = sComboList & "|#" & Trim$(Str$(aTraderGroup.ID)) & ";" & aTraderGroup.Name
            End If
            If aTraderGroup.ID = m_AuxTradeView.TradeViewFilter(TFC_TRADER_GROUP) Then
                nValue = aTraderGroup.ID
                sValue = Trim$(Str$(aTraderGroup.ID))
            End If
        Next
        .ColComboList(TFC_TRADER_GROUP) = sComboList
        .TextMatrix(1, TFC_TRADER_GROUP) = sValue
        m_AuxTradeView.TradeViewFilter(TFC_TRADER_GROUP) = nValue
    
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
            If aTrader.ID = m_AuxTradeView.TradeViewFilter(TFC_TRADER) Then
                nValue = aTrader.ID
                sValue = Trim$(Str$(nValue))
            End If
        Next
        .ColComboList(TFC_TRADER) = sComboList
        .TextMatrix(1, TFC_TRADER) = sValue
        m_AuxTradeView.TradeViewFilter(TFC_TRADER) = nValue
        
        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>|#" & Trim$(Str$(NO_STRATEGY_ID)) & ";" & NO_STRATEGY_NAME
        If m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) = NO_STRATEGY_ID Then
            nValue = NO_STRATEGY_ID
            sValue = NO_STRATEGY_NAME
        End If
        
        For Each aStrategy In g_Strategy
            sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
            If aStrategy.ID = m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) Then
                nValue = aStrategy.ID
                sValue = Trim$(Str$(aStrategy.ID))
            End If
        Next
        .ColComboList(TFC_STRATEGY) = sComboList
        .TextMatrix(1, TFC_STRATEGY) = sValue
        m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) = nValue
    
        m_GridLock(GT_TRADES_FILTER).UnlockRedraw
    End With
End Sub

Private Sub TradesShow(ByVal bReload As Boolean)
    On Error Resume Next
    Dim i&, nCount&, aTrd As EtsGeneralLib.MmTradeInfoAtom, nRow&, nOldRow&, nOldCol&
    
    m_bInProc = True
    RaiseEvent OnStateChange
        
    'Screen.MousePointer = vbArrow
    DoEvents
    
    lblStatus.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Visible = True
    lblProcess.Caption = IIf(bReload, "Data loading...", "Filter applying...")
    lblProcess.Refresh
    lblProcess.Visible = True
    
    With fgTrd
        m_GridLock(GT_TRADES_LIST).LockRedraw
        
        nOldRow = .Row
        nOldCol = .Col
        
        m_nTrades = 0
        
        If bReload Then
'            .Rows = 1
            m_AuxTradeView.Refresh
            nCount = TradeChannel.Trades.Count
            pbProgress.Max = nCount
            
'            For Each aTrd In g_TradeChannel.Trades
'                If Not aTrd Is Nothing Then
'                    If aTrd.IsPosition = 0 Then
'                        .AddItem ""
'                        nRow = .Rows - 1
'
'                        .RowData(nRow) = aTrd
'                        .TextMatrix(nRow, TLC_KEY) = aTrd.TradeID
'
'                        TradeUpdate nRow
'
'                        If CheckTradeFilter(aTrd) Then
'                            .RowHidden(nRow) = False
'                            m_nTrades = m_nTrades + 1
'                        Else
'                            .RowHidden(nRow) = True
'                        End If
'                    End If
'
'                    Set aTrd = Nothing
'                End If
'
'                DoEvents
'                If m_bShutDown Then Exit Sub
'                IncProgress pbProgress
'            Next
            
            If m_bShutDown Then Exit Sub
            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh
            i = -1
            i = .ColIndex(TLC_DATE)
            If i >= 0 Then
                .Col = i
                .Row = 1
                .Sort = flexSortGenericDescending
            End If
            geTrd.ShowSortImage i, -1
            
            FormatTrdColumns
        Else
            nCount = .Rows - 1
            pbProgress.Max = nCount
            
'            For nRow = 1 To nCount
'                Set aTrd = .RowData(nRow)
'                If Not aTrd Is Nothing Then
'                    If CheckTradeFilter(aTrd) Then
'                        .RowHidden(nRow) = False
'                        m_nTrades = m_nTrades + 1
'                    Else
'                        .RowHidden(nRow) = True
'                        .IsSelected(nRow) = True
'                    End If
'                    Set aTrd = Nothing
'                Else
'                    Debug.Assert False
'                End If
'
'                DoEvents
'                If m_bShutDown Then Exit Sub
'                IncProgress pbProgress
'            Next
            
            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh
        End If
        Dim aDataSource As VSFlex7Ctl.IVSFlexDataSource
        Set aDataSource = m_AuxTradeView
        m_nTrades = aDataSource.GetRecordCount
    
        If nOldRow > 0 And m_nTrades > 0 Then
            If .RowHidden(nOldRow) Then
                For nRow = nOldRow To nCount
                    If Not .RowHidden(nRow) Then
                        nOldRow = nRow
                        Exit For
                    End If
                Next
            
                If .RowHidden(nOldRow) Then
                    For nRow = nOldRow To 1 Step -1
                        If Not .RowHidden(nRow) Then
                            nOldRow = nRow
                            Exit For
                        End If
                    Next
                End If
            
                If .RowHidden(nOldRow) Then nOldRow = -1
            End If
            
            .Row = nOldRow
            If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
            .SetFocus
        Else
            If m_nTrades > 0 Then
                .Row = -1
                
                For nRow = 1 To nCount
                    If Not .RowHidden(nRow) Then
                        .Row = nRow
                        Exit For
                    End If
                Next
                
                If nOldCol > 0 Then .Col = nOldCol Else .Col = 1
                .SetFocus
            Else
                .Row = -1
                .Col = -1
                
                fgFlt.SetFocus
            End If
        End If
        
      lblStatus.Caption = Format$(m_nTrades, "#,##0") & " trade(s)"
        
      .FlexDataSource = Nothing
      m_AuxTradeView.TradeViewColumnsOrder = m_gdTrd.IdxCopy
      m_AuxTradeView.Sort .ColIndex(TLC_DATE), enSortDescending
      .FlexDataSource = m_AuxTradeView
      FormatTrdColumns
      m_GridLock(GT_TRADES_LIST).UnlockRedraw
    End With
    
    DoEvents
    If m_bShutDown Then Exit Sub
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True
    
    m_bInProc = False
    RaiseEvent OnStateChange
    Screen.MousePointer = vbDefault
End Sub

'Private Sub TradeUpdate(ByVal nRow As Long)
'    On Error Resume Next
'    Dim nCol&
'    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, sValue$
'
'    With fgTrd
'        m_GridLock(GT_TRADES_LIST).LockRedraw
'
'        Set aTrd = m_AuxTradeView.RowData(nRow)
'        If Not aTrd Is Nothing Then
'            Dim i&, nIdx&
'            i = 0
'            nIdx = m_gdTrd.Idx(0)
'            While nIdx >= 0 And i <= TLC_LAST_COLUMN
'                nCol = i + 1
'                Select Case nIdx
'                    Case TLC_TYPE
'                        .TextMatrix(nRow, nCol) = aTrd.GetTradeTypeString
'
'                    Case TLC_UNDERLYING
'                        .TextMatrix(nRow, nCol) = aTrd.Und.Symbol
'
'                    Case TLC_SYMBOL
'                        .TextMatrix(nRow, nCol) = aTrd.Symbol
'
'                    Case TLC_OPT_TYPE
'                        Select Case aTrd.ContractType
'                            Case enCtOption
'                                .TextMatrix(nRow, nCol) = IIf(aTrd.Opt.OptType = enOtCall, "C", "P")
'                            Case enCtFutOption
'                                .TextMatrix(nRow, nCol) = IIf(aTrd.FutOpt.OptType = enOtCall, "C", "P")
'                            Case Else
'                                .TextMatrix(nRow, nCol) = ""
'                        End Select
'
'                    Case TLC_EXPIRY
'                        Select Case aTrd.ContractType
'                            Case enCtOption
'                                .TextMatrix(nRow, nCol) = aTrd.Opt.Expiry
'                            Case enCtFutOption
'                                .TextMatrix(nRow, nCol) = aTrd.FutOpt.Expiry
'                            Case Else
'                                .TextMatrix(nRow, nCol) = ""
'                        End Select
'
'                    Case TLC_STRIKE
'                        Select Case aTrd.ContractType
'                            Case enCtOption
'                                .TextMatrix(nRow, nCol) = aTrd.Opt.Strike
'                            Case enCtFutOption
'                                .TextMatrix(nRow, nCol) = aTrd.FutOpt.Strike
'                            Case Else
'                                .TextMatrix(nRow, nCol) = ""
'                        End Select
'
'                    Case TLC_BS
'                        .TextMatrix(nRow, nCol) = aTrd.IsBuy
'
'                    Case TLC_PRICE
'                        .TextMatrix(nRow, nCol) = aTrd.Price
'
'                    Case TLC_QTY
'                        .TextMatrix(nRow, nCol) = aTrd.Quantity
'
'                    Case TLC_LOTSIZE
'                        .TextMatrix(nRow, nCol) = aTrd.LotSize
'
'                    Case TLC_TOT_PRICE
'                        .TextMatrix(nRow, nCol) = aTrd.TotalPrice
'
'                    Case TLC_DATE
'                        .TextMatrix(nRow, nCol) = aTrd.TradeDate
'
'                    Case TLC_TRADER
'                        sValue = ""
'                        sValue = aTrd.Trader.Name
'                        .TextMatrix(nRow, nCol) = sValue
'
'                    Case TLC_STRATEGY
'                        sValue = ""
'                        If Not aTrd.Strategy Is Nothing Then sValue = aTrd.Strategy.Name
'                        .TextMatrix(nRow, nCol) = sValue
'
'                    Case TLC_BROKER
'                        sValue = ""
'                        If Not aTrd.Broker Is Nothing Then sValue = aTrd.Broker.Name
'                        .TextMatrix(nRow, nCol) = sValue
'
'                    Case TLC_CLR_BROKER
'                        sValue = ""
'                        If Not aTrd.ClearingBroker Is Nothing Then sValue = aTrd.ClearingBroker.Name
'                        .TextMatrix(nRow, nCol) = sValue
'
'                    Case TLC_COMMISSION
'                        .TextMatrix(nRow, nCol) = aTrd.TotalCommission
'
'                    Case TLC_MATCHED
'                        .TextMatrix(nRow, nCol) = aTrd.GetTradeMatchedString
'
'                    Case TLC_MARKED
'                        .TextMatrix(nRow, nCol) = (aTrd.Mark <> 0)
'
'                    Case TLC_FUT_ROOT
'                        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then
'                            .TextMatrix(nRow, nCol) = aTrd.FutRoot.Symbol
'                        Else
'                            .TextMatrix(nRow, nCol) = ""
'                        End If
'
'                    Case TLC_FUTURES
'                        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then
'                            .TextMatrix(nRow, nCol) = aTrd.Fut.Symbol
'                        Else
'                            .TextMatrix(nRow, nCol) = ""
'                        End If
'
'                    Case TLC_FUT_MATURITY
'                        If aTrd.ContractType = enCtFuture Or aTrd.ContractType = enCtFutOption Then
'                            .TextMatrix(nRow, nCol) = aTrd.Fut.MaturityDate
'                        Else
'                            .TextMatrix(nRow, nCol) = ""
'                        End If
'                    Case TLC_STOCKREFPRICE
'                        If aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtFutOption Then
'                            .TextMatrix(nRow, nCol) = aTrd.SpotReference
'                        Else
'                            .TextMatrix(nRow, nCol) = ""
'                        End If
'                    Case TLC_TRADED_IV
'                        If aTrd.TradedIV <> 0 Then
'                            .TextMatrix(nRow, nCol) = aTrd.TradedIV * 100#
'                        Else
'                            .TextMatrix(nRow, nCol) = ""
'                        End If
'
'                End Select
'
'                i = i + 1
'                nIdx = m_gdTrd.Idx(i)
'            Wend
'
'            Set aTrd = Nothing
'        End If
'
'        m_GridLock(GT_TRADES_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Function NoFilter() As Boolean
'    On Error Resume Next
'    NoFilter = Not (m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> 0 _
'                    Or m_AuxTradeView.TradeViewFilter(TFC_TYPE) <> 0 _
'                    Or m_AuxTradeView.TradeViewFilter(TFC_GROUP) <> 0 _
'                    Or m_AuxTradeView.TradeViewFilter(TFC_TRADER) <> 0 _
'                    Or m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) <> 0)
'End Function
'
'Private Function CheckTradeFilter(ByRef aTrd As EtsGeneralLib.MmTradeInfoAtom) As Boolean
'    On Error Resume Next
'    Dim bMatched As Boolean, nValue&
'
'    If Not NoFilter Then
'        nValue = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING)
'        bMatched = (nValue = 0 Or nValue = aTrd.UndID)
'
'        If bMatched Then
'            ' "#0;<All>|#1;Today's|#2;All Except Today's|#3;Simulated Only"
'            nValue = m_AuxTradeView.TradeViewFilter(TFC_TYPE)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                Select Case nValue
'                    Case 1 ' Today's
'                        bMatched = (Int(aTrd.TradeDate) = Int(Date))
'
'                    Case 2 ' Simulated
'                        bMatched = (aTrd.Status = enTsSimulated)
'
'                    Case 3 ' Manual
'                        bMatched = (aTrd.Status = enTsManual)
'
'                End Select
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_AuxTradeView.TradeViewFilter(TFC_GROUP)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = Not g_UnderlyingGroup(nValue).Und(aTrd.UndID) Is Nothing
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_AuxTradeView.TradeViewFilter(TFC_TRADER)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = (aTrd.Trader.ID = nValue)
'            End If
'        End If
'
'        If bMatched Then
'            nValue = m_AuxTradeView.TradeViewFilter(TFC_STRATEGY)
'            bMatched = (nValue = 0)
'            If Not bMatched Then
'                bMatched = (aTrd.StrategyID = IIf(nValue > 0, nValue, 0))
'            End If
'        End If
'
'        CheckTradeFilter = bMatched
'    Else
'        CheckTradeFilter = True
'    End If
'End Function

Private Sub CheckSelectedRows()
    On Error Resume Next
    
    If fgTrd.Rows < 1 Then Exit Sub
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    m_AuxTradeView.ClearSelected
    
    For i = 0 To fgTrd.SelectedRows - 1
        Set aTrd = m_AuxTradeView.RowData(fgTrd.SelectedRow(i))
        If Not aTrd Is Nothing Then
            m_AuxTradeView.SetTradeAsSelected (aTrd.TradeID)
            Set aTrd = Nothing
        End If
    Next
        
End Sub
Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_TRADES_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_TRADES_LIST).CopyTo m_gdTrd
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = TFC_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarNone
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
    End With
    
    With fgTrd
        .Rows = 1
        .Cols = TLC_COLUMN_COUNT
        
        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .Appearance = flex3DLight
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
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
        .SelectionMode = flexSelectionListBox
        .TabBehavior = flexTabControls
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&
    
    With fgFlt
        m_GridLock(GT_TRADES_FILTER).LockRedraw
    
        nCols = TFC_COLUMN_COUNT
        .Cols = nCols
        
        For i = TFC_UNDERLYING To TFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        
        m_GridLock(GT_TRADES_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont
    
    With fgFlt
        m_GridLock(GT_TRADES_FILTER).LockRedraw
    
        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline
        
        Set .Font = aFont
        
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = TFC_UNDERLYING To TFC_LAST_COLUMN
            .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).BackColor <> 0, m_gdFlt.Col(i).BackColor, RGB(1, 1, 1))
            .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdFlt.Col(i).ForeColor <> 0, m_gdFlt.Col(i).ForeColor, RGB(1, 1, 1))
        Next

        If m_gdFlt.HorizLinesVisible Then
            If m_gdFlt.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdFlt.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdFlt.GridLinesColor
        
        .BackColorSel = m_gdFlt.BackColorSel
        .ForeColorSel = m_gdFlt.ForeColorSel
        
        .AutoSize 0, .Cols - 1, , 100
        
        m_GridLock(GT_TRADES_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatTrdGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&
    
    With fgTrd
        m_GridLock(GT_TRADES_LIST).LockRedraw
    
        nCols = 0
        For i = 0 To TLC_LAST_COLUMN
            If m_gdTrd.Idx(i) <> TLC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next
        
        If nCols = 0 Then
            m_gdTrd.Idx(0) = TLC_UNDERLYING
            nCols = 1
        End If
        
        nCols = nCols + 1 ' for key column
        .Cols = nCols
        
        .ColDataType(TLC_KEY) = flexDTString
        .ColHidden(TLC_KEY) = True
        .ColKey(TLC_KEY) = TLC_NONE
        
        For i = 0 To TLC_LAST_COLUMN
            nIdx = m_gdTrd.Idx(i)
            If nIdx <> TLC_NONE Then
                .ColDataType(i + 1) = m_gdTrd.Col(nIdx).DataType
                .ColFormat(i + 1) = m_gdTrd.Col(nIdx).Format
                .ColKey(i + 1) = nIdx
                .TextMatrix(0, i + 1) = m_gdTrd.Col(nIdx).Caption
                .ColWidth(i + 1) = IIf(m_gdTrd.Col(nIdx).Width > 0, m_gdTrd.Col(nIdx).Width, -1)
            Else
                Exit For
            End If
        Next
        
        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geTrd.AdjustSortImages
        
        m_GridLock(GT_TRADES_LIST).UnlockRedraw
    End With
End Sub

Private Sub FormatTrdColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont

    With fgTrd
        m_GridLock(GT_TRADES_LIST).LockRedraw
        
        aFont.Name = m_gdTrd.Font.Name
        aFont.Size = m_gdTrd.Font.Size
        aFont.Bold = m_gdTrd.Font.Bold
        aFont.Italic = m_gdTrd.Font.Italic
        aFont.Strikethrough = m_gdTrd.Font.Strikethrough
        aFont.Underline = m_gdTrd.Font.Underline
        
        Set .Font = aFont
    
        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> TLC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdTrd.Col(nIdx).BackColor <> 0, m_gdTrd.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdTrd.Col(nIdx).ForeColor <> 0, m_gdTrd.Col(nIdx).ForeColor, RGB(1, 1, 1))

                If nRows > 1 Then
                    If nIdx = TLC_BS Or nIdx = TLC_OPT_TYPE Or nIdx = TLC_MATCHED Or nIdx = TLC_MARKED Then
                        .ColAlignment(i) = flexAlignCenterCenter
                    Else
                        .ColAlignment(i) = flexAlignGeneral
                    End If
                End If
            End If
        Next

        If m_gdTrd.HorizLinesVisible Then
            If m_gdTrd.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdTrd.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdTrd.GridLinesColor

        .BackColorSel = m_gdTrd.BackColorSel
        .ForeColorSel = m_gdTrd.ForeColorSel
        
        m_GridLock(GT_TRADES_LIST).UnlockRedraw
    End With
End Sub

Private Sub fgFlt_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sValue$, nValue&
    
    With fgFlt
        sValue = Trim$(.TextMatrix(Row, Col))
        If m_sCurrentOriginalText <> sValue Then
            nValue = CLng(sValue)
            If Err.Number <> 0 Then nValue = val(sValue)
            If Err.Number = 0 Then
                m_AuxTradeView.TradeViewFilter(Col) = nValue
                TradesShow False
            
                .AutoSize 0, .Cols - 1, , 100
            Else
                FilterUpdate
            End If
        End If
    End With
End Sub

Private Sub fgFlt_DblClick()
    On Error Resume Next
    
    With fgFlt
        m_enMenuGrid = GT_TRADES_FILTER
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick False
    End With
End Sub

Private Sub fgFlt_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridClipOrTradeOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_TRADES_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_TRADES_FILTER) Then
        m_bKeyDown(GT_TRADES_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_TRADES_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                mnuCtxTradeNew_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
            
        End Select
    End If
End Sub

Private Sub fgFlt_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgFlt
            m_enMenuGrid = GT_TRADES_FILTER
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgFlt_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Cancel = True
    
    If IsDblClickHandled Then Exit Sub
    
    With fgFlt
        If Not m_bInProc And Not m_bShutDown And Col >= TFC_UNDERLYING And Col <= TFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgTrd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub
    
    nOldPos = Col - TLC_TYPE
    nNewPos = Position - TLC_TYPE
    nIdx = m_gdTrd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then
                m_gdTrd.Idx(i) = m_gdTrd.Idx(i - 1)
            End If
        Next
        m_gdTrd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then
                m_gdTrd.Idx(i) = m_gdTrd.Idx(i + 1)
            End If
        Next
        m_gdTrd.Idx(nNewPos) = nIdx
    End If
End Sub

Private Sub fgTrd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&
    
    If Col >= 0 And Row < 0 Then
        With fgTrd
            nIdx = .ColKey(Col)
            If nIdx >= TLC_TYPE And nIdx <= TLC_LAST_COLUMN Then
                m_gdTrd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgTrd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nOldIdx&, nNewIdx&
    
    nOldIdx = fgTrd.ColKey(Col)
    nNewIdx = fgTrd.ColKey(Position)
    
    If nOldIdx = TLC_NONE Or nNewIdx = TLC_NONE Then
        Position = Col
    ElseIf Not m_gdTrd.Col(nOldIdx).CanChangeOrder Or Not m_gdTrd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgTrd_Click()
    CheckSelectedRows
End Sub

Private Sub fgTrd_DblClick()
    On Error Resume Next
    With fgTrd
        m_enMenuGrid = GT_TRADES_LIST
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows
        
        HandleGridDblClick True
    End With
End Sub

Private Sub fgTrd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) _
        Or Shift = 0 And (KeyCode = vbKeyReturn Or KeyCode = vbKeyDelete) Then
        m_bKeyDown(GT_TRADES_LIST) = True
    End If
End Sub

Private Sub fgTrd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    
    CheckSelectedRows
    
    If m_bKeyDown(GT_TRADES_LIST) Then
        m_bKeyDown(GT_TRADES_LIST) = False
        With fgTrd
            m_enMenuGrid = GT_TRADES_LIST
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With
        
        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                mnuCtxTradeNew_Click
            
            Case KeyCode = vbKeyReturn And Shift = 0
                mnuCtxTradeEdit_Click
                
            Case KeyCode = vbKeyDelete And Shift = 0
                mnuCtxTradeDelete_Click
                
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click
            
            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click
            
            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click
        
            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                mnuCtxAutosizeGrid_Click
    
            Case KeyCode = vbKeyA And Shift = vbCtrlMask
                mnuCtxAutosizeCol_Click
        
        End Select
    End If
End Sub

Private Sub fgTrd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgTrd
            m_enMenuGrid = GT_TRADES_LIST
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            
            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                And .SelectedRows <= 1 Then
                .Row = m_nMenuGridRow
            End If
            ShowPopup
        End With
    End If
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom

    'mnuCtxTradeNew          "New Trade..."
    'mnuCtxTradeEdit         "Edit Trade..."
    'mnuCtxTradeDelete       "Delete Trade..."
    'mnuCtxPositionTransfer  "Position Transfer..."
    'mnuCtxOrderNewStock     "New Stock Order..."
    'mnuCtxOrderNewOption    "New Option Order..."
    'mnuCtxTradeExercise     "Exercise Trade..."
    'mnuCtxTradeExpiry       "Expiry Trade..."
    'mnuCtxDefTrdSort        "Default Trades Sorting"
    'mnuCtxRefresh           "Refresh"
    'mnuCtxCopy              "Copy Grid"
    'mnuCtxPrint             "Print Grid..."
    'mnuCtxHideCol           "Hide Column"
    'mnuCtxAutosizeCol       "Autosize Column"
    'mnuCtxAutosizeGrid      "Autosize Grid"
    'mnuCtxGridLayout        "Grid Layout..."

    mnuCtxRefresh.Enabled = Not m_bInProc
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxDefTrdSort.Enabled = Not m_bInProc And fgTrd.ColIndex(TLC_DATE) >= 0
    mnuCtxTradeExercise.Enabled = False
    mnuCtxTradeExpiry.Enabled = False
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc
    
    Select Case m_enMenuGrid
        Case GT_TRADES_FILTER
            mnuCtxTradeNew.Enabled = (Not m_bInProc And m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> 0 _
                                        And m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxTradeEdit.Enabled = False
            mnuCtxTradeEdit.Caption = "Edit Trade..." & vbTab & "Enter"
            mnuCtxTradeDelete.Enabled = False
            
            mnuCtxPositionTransfer.Enabled = False
            
            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            
            PopupMenu mnuCtx, , , , mnuCtxTradeEdit
        
        Case GT_TRADES_LIST
            With fgTrd
                If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                    Set aTrd = m_AuxTradeView.RowData(m_nMenuGridRow)
                End If
                
                mnuCtxTradeEdit.Caption = "Edit " & IIf(aTrd.IsPosition, "Position", "Trade") & "..." & vbTab & "Enter"
                mnuCtxTradeDelete.Caption = "Delete " & IIf(aTrd.IsPosition, "Position", "Trade") & "..." & vbTab & "Del"
            
                If Not m_bInProc And (Not aTrd Is Nothing Or m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> 0) _
                                            And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                            And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                
                    Dim bEnabled As Boolean, i&, nRow&
                    
                    If Not aTrd Is Nothing Then
                        bEnabled = True
                    Else
                        bEnabled = False
                    End If
                    If .Rows > 1 And bEnabled Then
                        If .SelectedRows > 1 Then
                            mnuCtxTradeNew.Enabled = False
                            mnuCtxTradeEdit.Enabled = False
                            mnuCtxPositionTransfer.Enabled = False
                            
                            For i = 0 To .SelectedRows - 1
                                nRow = .SelectedRow(i)
                                If Not .RowHidden(nRow) Then
                                    Set aTrd = m_AuxTradeView.RowData(nRow)
                                    
                                    If Not aTrd Is Nothing Then
                                        bEnabled = (g_Params.ActualTradesEditable Or aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated)
                                        Set aTrd = Nothing
                                    Else
                                        bEnabled = False
                                    End If
                                    
                                    If Not bEnabled Then
                                        Exit For
                                    End If
                                End If
                            Next
                            
                            mnuCtxTradeDelete.Enabled = bEnabled
                            mnuCtxTradeDelete.Caption = "Delete " & IIf(aTrd.IsPosition, "Positions", "Trades") & "..." & vbTab & "Del"
                        Else
                            mnuCtxTradeNew.Enabled = IIf(aTrd.IsPosition, False, True)
                            mnuCtxTradeEdit.Enabled = True
                            mnuCtxPositionTransfer.Enabled = IIf(aTrd.IsPosition, False, True)
                            
                            If Not g_Params.ActualTradesEditable And (aTrd.Status = enTsRealtime Or aTrd.Status = enTsMatchedRealtime) Then
                                mnuCtxTradeEdit.Caption = "Trade Properties..." & vbTab & "Enter"
                                mnuCtxTradeDelete.Enabled = False
                            Else
                                mnuCtxTradeDelete.Enabled = True
                            End If
                        
                            mnuCtxTradeExercise.Enabled = aTrd.Status <> enTsSimulated _
                                                            And (aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtFutOption) _
                                                            And aTrd.Price <> 0# And IIf(aTrd.IsPosition, False, True)
                            mnuCtxTradeExpiry.Enabled = mnuCtxTradeExercise.Enabled
                        End If
                    Else
                        mnuCtxTradeNew.Enabled = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> 0
                        mnuCtxTradeEdit.Enabled = False
                        mnuCtxTradeDelete.Enabled = False
                        mnuCtxPositionTransfer.Enabled = False
                    End If
                Else
                    mnuCtxTradeNew.Enabled = False
                    mnuCtxTradeEdit.Enabled = False
                    mnuCtxTradeDelete.Enabled = False
                    mnuCtxPositionTransfer.Enabled = False
                End If
                
                mnuCtxHideCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdTrd.Idx(1) <> TLC_NONE)
                mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
                mnuCtxAutosizeGrid.Enabled = True
                
                Set aTrd = Nothing
                
                PopupMenu mnuCtx, , , , mnuCtxTradeEdit
            End With
            
    End Select
End Sub

Private Sub ResetMenuData()
    On Error Resume Next
    m_enMenuGrid = -1
    m_nMenuGridCol = -1
    m_nMenuGridRow = -1
    m_nMenuGridCols = 0
    m_nMenuGridRows = 0
End Sub

Private Sub frmLayout_OnApply(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    SetGridLayout enGridType, gdGrid
End Sub

Private Sub frmLayout_OnOK(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    SetGridLayout enGridType, gdGrid
    Screen.MousePointer = vbDefault
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    
    Select Case enGridType
        Case GT_TRADES_FILTER
            gdGrid.CopyTo m_gdFlt
            
            m_GridLock(GT_TRADES_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_TRADES_FILTER).UnlockRedraw
            
        Case GT_TRADES_LIST
            gdGrid.CopyTo m_gdTrd
            m_AuxTradeView.TradeViewColumnsOrder = m_gdTrd.IdxCopy
            m_GridLock(GT_TRADES_LIST).LockRedraw
            
            fgTrd.FlexDataSource = Nothing
            fgTrd.FlexDataSource = m_AuxTradeView
            FormatTrdGrid
            'TradesUpdate False
            FormatTrdColumns
            m_GridLock(GT_TRADES_LIST).UnlockRedraw
            RaiseEvent OnStateChange
    
    End Select
    
    
    UserControl_Resize
End Sub

Private Sub geTrd_BeforeSort(ByVal Col As Long, Order As Integer)

    Select Case Order
        Case flexSortGenericAscending, flexSortNumericAscending, flexSortStringNoCaseAscending, flexSortStringAscending
             m_AuxTradeView.Sort Col, enSortAscending
             
        Case flexSortGenericDescending, flexSortNumericDescending, flexSortStringNoCaseDescending, flexSortStringDescending
            m_AuxTradeView.Sort Col, enSortDescending
    End Select
    geTrd.FinishSorting Col, Order
   
End Sub

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    Dim nIdx&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_TRADES_LIST
            With fgTrd
                m_GridLock(GT_TRADES_LIST).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                
                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= TLC_TYPE And nIdx <= TLC_LAST_COLUMN Then
                    m_gdTrd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If
                
                m_GridLock(GT_TRADES_LIST).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    Dim nIdx&, i&, nCount&
    
    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_TRADES_LIST
            With fgTrd
                m_GridLock(GT_TRADES_LIST).LockRedraw
                .AutoSize 0, .Cols - 1
                
                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= TLC_TYPE And nIdx <= TLC_LAST_COLUMN Then
                        m_gdTrd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next
                
                m_GridLock(GT_TRADES_LIST).UnlockRedraw
            End With
        
    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_TRADES_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_TRADES_LIST
            g_ClipMgr.CopyGridToClipboard fgTrd

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxDefTrdSort_Click()
    On Error Resume Next
    SortTradesByDefault
End Sub

Public Sub SortTradesByDefault()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim i&, nRow&, nCol&
    
    With fgTrd
        i = -1
        i = .ColIndex(TLC_DATE)
        If i >= 0 Then
            m_GridLock(GT_TRADES_LIST).LockRedraw
            Screen.MousePointer = vbHourglass
            
            nRow = .Row
            nCol = .Col
            .Col = i
            .Row = 1
            .Sort = flexSortGenericDescending
        
            .Row = nRow
            .Col = nCol
            
            geTrd.ShowSortImage i, -1
            
            m_GridLock(GT_TRADES_LIST).UnlockRedraw
            Screen.MousePointer = vbDefault
        End If
    
    End With
    
End Sub

Public Property Get CanSortTradesByDefault() As Boolean
    On Error Resume Next
    CanSortTradesByDefault = Not m_bInProc And fgTrd.ColIndex(TLC_DATE) >= 0
End Property

Private Sub mnuCtxTradeDelete_Click()
    On Error GoTo EH
    If m_bInProc Then Exit Sub
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, aTrdToDel As New EtsGeneralLib.MmTradeInfoColl
    Dim bCanDelete As Boolean, i&, nRow&
    
    Screen.MousePointer = vbHourglass
    m_bInProc = True
    
    If m_enMenuGrid = GT_TRADES_LIST Then
        With fgTrd
            If .SelectedRows > 1 Then
                For i = 0 To .SelectedRows - 1
                    nRow = .SelectedRow(i)
                    If Not .RowHidden(nRow) Then
                        Set aTrd = m_AuxTradeView.RowData(nRow)
                        
                        If Not aTrd Is Nothing Then
                            bCanDelete = g_Params.ActualTradesEditable Or (aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated)
                            Set aTrd = Nothing
                        Else
                            bCanDelete = False
                        End If
                        
                        If Not bCanDelete Then
                            Exit For
                        End If
                    End If
                Next
                
                If bCanDelete Then
                    m_bInProc = False
                    If gCmn.MyMsgBox(m_frmOwner, "Are you sure to delete selected trades or positions?", vbYesNo + vbQuestion) = vbYes Then
                        m_bInProc = True
                        For i = .SelectedRows - 1 To 0 Step -1
                            nRow = .SelectedRow(i)
                            If Not .RowHidden(nRow) Then
                                Set aTrd = m_AuxTradeView.RowData(nRow)
                                
                                If Not aTrd Is Nothing Then
                                    If g_Params.ActualTradesEditable Or aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated Then
                                        aTrdToDel.Add aTrd.TradeID, 0, aTrd
                                    End If
                                    Set aTrd = Nothing
                                End If
                            End If
                        Next
                        m_bInProc = False
                        
                        For Each aTrd In aTrdToDel
                            TradeChannel.DeleteTrade aTrd
                        Next
                        
                        aTrdToDel.Clear
                    End If
                End If
                
            ElseIf .SelectedRows = 1 Then
                nRow = .SelectedRow(0)
                If Not .RowHidden(nRow) Then
                    On Error Resume Next
                    m_bInProc = True
                    Set aTrd = m_AuxTradeView.RowData(nRow)
                    m_bInProc = False
                    On Error GoTo EH
                    If Not aTrd Is Nothing Then
                        If g_Params.ActualTradesEditable Or aTrd.Status = enTsManual Or aTrd.Status = enTsSimulated Then
                            m_bInProc = False
                            If gCmn.MyMsgBox(m_frmOwner, "Are you sure to delete selected " & IIf(aTrd.IsPosition, "position", "trade") & "?", vbYesNo + vbQuestion) = vbYes Then
                                TradeChannel.DeleteTrade aTrd
                            End If
                        End If
                    End If
                End If
            End If
        End With
    End If

    Screen.MousePointer = vbDefault
    m_bInProc = False
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to delete trade."
    On Error Resume Next
    ResetMenuData
    aTrdToDel.Clear
End Sub

Private Sub mnuCtxTradeEdit_Click()
    If m_bInProc Then Exit Sub
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nVisibleSelectedRows&, nFirstVisibleSelectedRow&, nRow&, i&
    
    On Error Resume Next
    m_bInProc = True
    
    nVisibleSelectedRows = 0
    nFirstVisibleSelectedRow = 0
    For i = 0 To fgTrd.SelectedRows - 1
        nRow = fgTrd.SelectedRow(i)
        If Not fgTrd.RowHidden(nRow) Then
            nVisibleSelectedRows = nVisibleSelectedRows + 1
            If nFirstVisibleSelectedRow = 0 Or nRow < nFirstVisibleSelectedRow Then _
                nFirstVisibleSelectedRow = nRow
        End If
    Next
        
    If nVisibleSelectedRows = 1 And m_enMenuGrid = GT_TRADES_LIST Then
        On Error Resume Next
        Set aTrd = m_AuxTradeView.RowData(nFirstVisibleSelectedRow)
        fgTrd.Row = nFirstVisibleSelectedRow
        
        On Error GoTo EH
        If Not aTrd Is Nothing Then
            m_bInProc = False
            frmTradeEditor.EditTrade aTrd, m_frmOwner
        End If
    End If

    m_bInProc = False
    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to edit trade."
    ResetMenuData
End Sub

Private Sub mnuCtxTradeExercise_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, aRoot As EtsGeneralLib.SynthRootAtom, aTrdExec As clsTradeExecAtom
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If fgTrd.SelectedRows = 1 And m_enMenuGrid = GT_TRADES_LIST Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aTrd = m_AuxTradeView.RowData(m_nMenuGridRow)
             If Not aTrd Is Nothing And Not aTrd.IsPosition Then
                If aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtFutOption Then
                    If aTrd.ContractType = enCtOption Then
                        Set aRoot = g_UnderlyingAll(aTrd.UndID).SyntheticRoots(aTrd.OptRootID)
'                        If Not aRoot Is Nothing Then
'                            Set aRoot = Nothing
'                            gCmn.MyMsgBox m_frmOwner, "Exercising trades for options with syntetic root is not supported.", vbExclamation
'                            Exit Sub
'                        End If
                    End If
                            
                    If aTrd.Price <> 0 Then
                        Set aTrdExec = collTrades.Add(CStr(aTrd.TradeID))
                        aTrdExec.IsExec = True
                        aTrdExec.QtyToExec = aTrd.Quantity
                        
                        If aTrd.ContractType = enCtOption Then
                            aTrdExec.UndTradePrice = aTrd.Opt.Strike
                        Else
                            aTrdExec.UndTradePrice = aTrd.FutOpt.Strike
                        End If
                        
                        aTrdExec.UndPrice = 0#
                        Set aTrdExec.Trd = aTrd
                        Set aTrdExec = Nothing
                            
                        Set frmExecTrades = New frmExerciseTrades
                        frmExecTrades.Execute collTrades, True, m_frmOwner
                        Set frmExecTrades = Nothing
                    End If
                End If
            End If
        End If
    End If
End Sub

Private Sub mnuCtxTradeExpiry_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, aTrdExec As clsTradeExecAtom
    Dim collTrades As New clsTradeExecColl, frmExecTrades As frmExerciseTrades
    
    If fgTrd.SelectedRows = 1 And m_enMenuGrid = GT_TRADES_LIST Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
            Set aTrd = m_AuxTradeView.RowData(m_nMenuGridRow)
            If Not aTrd Is Nothing And Not aTrd.IsPosition Then
                If aTrd.ContractType = enCtOption Or aTrd.ContractType = enCtFutOption Then
                    If aTrd.Price <> 0 Then
                        Set aTrdExec = collTrades.Add(CStr(aTrd.TradeID))
                        aTrdExec.IsExec = True
                        aTrdExec.QtyToExec = aTrd.Quantity
                        aTrdExec.UndTradePrice = 0#
                        aTrdExec.UndPrice = 0#
                        Set aTrdExec.Trd = aTrd
                        Set aTrdExec = Nothing
                            
                        Set frmExecTrades = New frmExerciseTrades
                        frmExecTrades.Execute collTrades, False, m_frmOwner
                        Set frmExecTrades = Nothing
                    End If
                End If
            End If
        End If
    End If
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_TRADES_FILTER
            CustomizeFltGridLayout

        Case GT_TRADES_LIST
            CustomizeTrdGridLayout
        
    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    
    Dim i&, nColIdx&, bMove As Boolean
    
    If m_enMenuGrid = GT_TRADES_LIST Then
        bMove = False
        
        If m_gdTrd.Idx(1) = TLC_NONE Then Exit Sub
        
        nColIdx = fgTrd.ColKey(m_nMenuGridCol)
        For i = 0 To TLC_LAST_COLUMN
            If m_gdTrd.Idx(i) = nColIdx Then
                m_gdTrd.Idx(i) = TLC_NONE
                m_gdTrd.Col(nColIdx).Visible = False
                bMove = True
            End If
            
            If bMove Then
                If i + 1 <= TLC_LAST_COLUMN Then
                    m_gdTrd.Idx(i) = m_gdTrd.Idx(i + 1)
                Else
                    m_gdTrd.Idx(i) = TLC_NONE
                End If
            End If
        Next
        
        If bMove Then
            FormatTrdGrid
            FormatTrdColumns
            'TradesUpdate True
            RaiseEvent OnStateChange
        End If
            
    End If
End Sub

'Private Sub TradesUpdate(Optional ByVal bAutosize As Boolean = False)
'    On Error Resume Next
'    Dim i&, nRows&
'
'    With fgTrd
'        m_GridLock(GT_TRADES_LIST).LockRedraw
'
'        nRows = .Rows - 1
'        For i = 1 To nRows
'            TradeUpdate i
'        Next
'
'        m_GridLock(GT_TRADES_LIST).UnlockRedraw
'    End With
'End Sub

Private Sub mnuCtxTradeNew_Click()
    On Error GoTo EH
    If m_bInProc Then Exit Sub
    Dim nUndID&, nID&, bBuy As Boolean, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nVisibleSelectedRows&, nFirstVisibleSelectedRow&, nRow&, i&
    
    m_bInProc = True
    nUndID = 0
    nID = 0
    bBuy = True
    
    nVisibleSelectedRows = 0
    nFirstVisibleSelectedRow = 0
    For i = 0 To fgTrd.SelectedRows - 1
        nRow = fgTrd.SelectedRow(i)
        If Not fgTrd.RowHidden(nRow) Then
            nVisibleSelectedRows = nVisibleSelectedRows + 1
            If nFirstVisibleSelectedRow = 0 Or nRow < nFirstVisibleSelectedRow Then _
                nFirstVisibleSelectedRow = nRow
        End If
    Next
        
    If nVisibleSelectedRows = 1 And m_enMenuGrid = GT_TRADES_LIST Then
        On Error Resume Next
        Set aTrd = m_AuxTradeView.RowData(nFirstVisibleSelectedRow)
        fgTrd.Row = nFirstVisibleSelectedRow
        
        On Error GoTo EH
        If Not aTrd Is Nothing And Not aTrd.IsPosition Then
            nUndID = aTrd.UndID
            nID = aTrd.ContractID
            bBuy = aTrd.IsBuy
            Set aTrd = Nothing
        Else
            nUndID = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING)
            nID = nUndID
            bBuy = True ' g_Params.QuoteOtherColBuy
        End If
    ElseIf m_enMenuGrid = GT_TRADES_FILTER Then
        If m_nMenuGridRow >= 0 And m_nMenuGridCol < m_nMenuGridCols - 1 Then
            nUndID = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING)
            nID = nUndID
            bBuy = (g_Params.QuoteOtherColBuy <> 0)
        End If
    End If

    m_bInProc = False
    If nUndID <> 0 Then frmTradeEditor.NewTrade nID, nUndID, bBuy, m_frmOwner

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to create new trade."
    ResetMenuData
End Sub

Private Sub mnuCtxPositionTransfer_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim nVisibleSelectedRows&, nFirstVisibleSelectedRow&, nRow&, i&
    
    m_bInProc = True
    
    nVisibleSelectedRows = 0
    nFirstVisibleSelectedRow = 0
    For i = 0 To fgTrd.SelectedRows - 1
        nRow = fgTrd.SelectedRow(i)
        If Not fgTrd.RowHidden(nRow) Then
            nVisibleSelectedRows = nVisibleSelectedRows + 1
            If nFirstVisibleSelectedRow = 0 Or nRow < nFirstVisibleSelectedRow Then _
                nFirstVisibleSelectedRow = nRow
        End If
    Next
        
    If nVisibleSelectedRows = 1 And m_enMenuGrid = GT_TRADES_LIST Then
        Set aTrd = m_AuxTradeView.RowData(nFirstVisibleSelectedRow)
        fgTrd.Row = nFirstVisibleSelectedRow
    End If

    m_bInProc = False
    If Not aTrd Is Nothing Then
        Dim sContractDesc$
        Select Case aTrd.ContractType
            Case enCtOption
                sContractDesc = aTrd.Symbol & " (" & aTrd.Und.Symbol & " " & Format$(aTrd.Opt.Expiry, "MMMM YYYY") & " " & _
                            CStr(aTrd.Opt.Strike) & IIf(aTrd.Opt.OptType = enOtCall, " Call)", " Put)")
                        
            Case enCtFuture
                sContractDesc = aTrd.Symbol & " (" & aTrd.Fut.ContractName & ")"
        
            Case enCtFutOption
                sContractDesc = aTrd.Symbol & " (" & aTrd.Fut.Symbol & " " & Format$(aTrd.FutOpt.Expiry, "MMMM YYYY") & " " & _
                            CStr(aTrd.FutOpt.Strike) & IIf(aTrd.FutOpt.OptType = enOtCall, " Call)", " Put)")
            Case Else
                sContractDesc = aTrd.Symbol & " (" & aTrd.Und.ContractName & ")"
        End Select
        
        frmPositionTransfer.TransferPosition aTrd.UndID, aTrd.ContractID, aTrd.Trader.ID, sContractDesc, m_frmOwner
        Set aTrd = Nothing
    End If
End Sub

Private Function GetFirstVisibleSelectedRow&()
    On Error Resume Next
    Dim i&, nRow&
    GetFirstVisibleSelectedRow = 0
    For i = 0 To fgTrd.SelectedRows - 1
        nRow = fgTrd.SelectedRow(i)
        If Not fgTrd.RowHidden(nRow) Then
            GetFirstVisibleSelectedRow = nRow
            Exit For
        End If
    Next
End Function

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
                                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, nRow&
    
    bBuy = True
    dPrice = 0#
    
    If m_enMenuGrid = GT_TRADES_LIST Then
        With fgTrd
            nRow = GetFirstVisibleSelectedRow()
            If nRow > 0 Then
                .Row = nRow
                
                Set aTrd = m_AuxTradeView.RowData(nRow)
                If Not aTrd Is Nothing Then
                    Set aUnd = g_Underlying(aTrd.UndID)
                    
                    If Not bIsStock And aTrd.ContractType = enCtOption Then
                        Set aOpt = New EtsGeneralLib.EtsOptAtom
                        aTrd.Opt.CopyTo aOpt
                    End If
                    
                    bBuy = aTrd.IsBuy
                    dPrice = IIf(bIsStock = (aTrd.ContractType = enCtStock Or aTrd.ContractType = enCtIndex), aTrd.Price, 0#)
                    
                    Set aTrd = Nothing
                End If
            End If
        End With
    End If

    If aUnd Is Nothing Then
        Set aUnd = g_Underlying(m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING))
        bBuy = True
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection bIsStock, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    If bIsStock Then
'        frmOrderEntry.NewStockOrder aUnd, True, 100, dPrice
'    Else
'        frmOrderEntry.NewOptionOrder aUnd, aOpt, True, 10, dPrice
'    End If
'
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Trade View: Fail to create new order."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxOrderNewOption_Click()
    On Error Resume Next
    OrderNew False
End Sub

Private Sub mnuCtxOrderNewStock_Click()
    On Error Resume Next
    OrderNew True
End Sub

Private Sub mnuCtxTntCardNew_Click()
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
'    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom
'
'    m_bInProc = True
'    FillDataForOrderFromCurrentSelection False, aUnd, aOpt, bBuy, dPrice, nQty
'    m_bInProc = False
'
'    On Error GoTo EH
'    frmTntCardEntry.NewCard m_frmOwner, aUnd, aOpt, bBuy, nQty, dPrice
'
'    Set aUnd = Nothing
'    Set aOpt = Nothing
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Trade View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxPrint_Click()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_TRADES_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Trades Filter", "Trades Filter", fgFlt
'
'        Case GT_TRADES_LIST
'            frmPrintPreview.Execute m_frmOwner, "Trades List", "Trades List", fgTrd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxRefresh_Click()
    On Error Resume Next
    Refresh
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
    TradesShow True
End Sub

'Private Sub AddTrade(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgTrd
'        m_GridLock(GT_TRADES_LIST).LockRedraw
'
'        .AddItem "", 1
'        nRow = 1 '.Rows - 1
'
'        m_AuxTradeView.RowData(nRow) = aNewTrdInfo
'        .TextMatrix(nRow, TLC_KEY) = aNewTrdInfo.TradeID
'
'        TradeUpdate nRow
'
'        If CheckTradeFilter(aNewTrdInfo) Then
'            .RowHidden(nRow) = False
'            m_nTrades = m_nTrades + 1
'            lblStatus.Caption = Format$(m_nTrades, "#,##0") & " trade(s)"
'        Else
'            .RowHidden(nRow) = True
'        End If
'        FormatTrdColumns
'
'        m_GridLock(GT_TRADES_LIST).UnlockRedraw
'    End With
'End Sub

'Private Sub UpdateTrade(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgTrd
'        m_GridLock(GT_TRADES_LIST).LockRedraw
'
'        nRow = .FindRow(aOldTrdInfo.TradeID, 1, TLC_KEY, , True)
'
'        If nRow > 0 Then
'            If nRow > 1 Then
'                .RemoveItem nRow
'                .AddItem "", 1
'                nRow = 1
'            Else
'                m_AuxTradeView.RowData(nRow) = Nothing
'            End If
'
'            m_AuxTradeView.RowData(nRow) = aNewTrdInfo
'            .TextMatrix(nRow, TLC_KEY) = aNewTrdInfo.TradeID
'            TradeUpdate nRow
'            If CheckTradeFilter(aNewTrdInfo) Then
'                If .RowHidden(nRow) Then
'                    .RowHidden(nRow) = False
'                    m_nTrades = m_nTrades + 1
'                    .ShowCell nRow, 1
'                End If
'            Else
'                If Not .RowHidden(nRow) Then
'                    .RowHidden(nRow) = True
'                    m_nTrades = m_nTrades - 1
'                    If nRow > 1 Then
'                        .ShowCell nRow - 1, 1
'                    Else
'                        .ShowCell 1, 1
'                    End If
'                End If
'            End If
'        End If
'
'        lblStatus.Caption = Format$(m_nTrades, "#,##0") & " trade(s)"
'        FormatTrdColumns
'
'        m_GridLock(GT_TRADES_LIST).UnlockRedraw
'    End With
'End Sub
'
'Private Sub DeleteTrade(aTrdInfo As EtsGeneralLib.MmTradeInfoAtom)
'    On Error Resume Next
'    Dim nRow&
'
'    With fgTrd
'        m_GridLock(GT_TRADES_LIST).LockRedraw
'
'        nRow = .FindRow(aTrdInfo.TradeID, 1, TLC_KEY, , True)
'
'        If nRow > 0 Then
'            If Not .RowHidden(nRow) Then
'                m_nTrades = m_nTrades - 1
'                lblStatus.Caption = Format$(m_nTrades, "#,##0") & " trade(s)"
'            End If
'
'            .RemoveItem nRow
'        End If
'
'        m_GridLock(GT_TRADES_LIST).UnlockRedraw
'    End With
'End Sub

Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    
'    Select Case enAction
'        Case enTaTradeNew
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            AddTrade aNewTrdInfo
'
'        Case enTaTradeUpdate
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            UpdateTrade aNewTrdInfo, aOldTrdInfo
'
'        Case enTaTradeDelete
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            DeleteTrade aOldTrdInfo
'
'    End Select
    RefreshView
End Sub

Private Sub TradeChannel_PositionTransfer(aTrdFrom As EtsGeneralLib.MmTradeInfoAtom, aTrdTo As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    If m_bShutDown Then Exit Sub

'    Debug.Assert Not aTrdFrom Is Nothing
'    Debug.Assert Not aTrdTo Is Nothing
'
'    AddTrade aTrdFrom
'    AddTrade aTrdTo
    RefreshView
End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
        
    With fgFlt
        .Top = 0
        .Left = 0
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
    End With

    With fgTrd
        .Left = 0
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Width = ScaleWidth
        .Height = ScaleHeight - .Top
        .DataRefresh
    End With
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub
Public Sub RefreshView()
 On Error Resume Next
   m_GridLock(GT_TRADES_LIST).LockRedraw
    
    fgTrd.FlexDataSource = Nothing
    m_AuxTradeView.TradeViewColumnsOrder = m_gdTrd.IdxCopy
    m_AuxTradeView.Refresh
    fgTrd.FlexDataSource = m_AuxTradeView
    fgTrd.Refresh
    fgTrd.DataRefresh
    FormatTrdColumns
    m_GridLock(GT_TRADES_LIST).UnlockRedraw
   
   Dim aDataSource As VSFlex7Ctl.IVSFlexDataSource
   Set aDataSource = m_AuxTradeView
   m_nTrades = aDataSource.GetRecordCount
   
   'For correct view of selectrd lines
   Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
   '
        For i = 1 To (m_nTrades)
   
           If Not fgTrd.RowHidden(i) Then
                Set aTrd = m_AuxTradeView.RowData(i)
   
                If Not aTrd Is Nothing Then
                    fgTrd.IsSelected(i) = m_AuxTradeView.IsSelectedTrade(aTrd.TradeID)
                    Set aTrd = Nothing
                End If
                
            End If
   
        Next
   
   lblStatus.Caption = Format$(m_nTrades, "#,##0") & " trade(s)"

End Sub


Public Sub Term()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_bShutDown = True
    
    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set geTrd = Nothing
    
    Set m_AuxTradeView.EtsMain = Nothing
    Set m_AuxTradeView.TradeChannel = Nothing
    Set m_AuxTradeView = Nothing
    
    Set frmLayout = Nothing
    Set TradeChannel = Nothing
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$
    
    sCaption = "Trades"
    
    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = "Trades"
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_TRADES_FILTER, TFC_UNDERLYING, TFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeTrdGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_TRADES_LIST, TLC_TYPE, TLC_LAST_COLUMN, m_gdTrd, m_frmOwner
    End If
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey

    ' common info
    For i = TFC_UNDERLYING To TFC_STRATEGY
        aStorage.SetLongValue "TradeFlt" & sKey, "Filter" & CStr(i), m_AuxTradeView.TradeViewFilter(i)
    Next
    
    m_gdFlt.WriteToStorage "TradeFltGrid" & sKey, aStorage, False
    m_gdTrd.WriteToStorage "TradeTrdGrid" & sKey, aStorage
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    ' common info
    For i = TFC_UNDERLYING To TFC_STRATEGY
        m_AuxTradeView.TradeViewFilter(i) = aStorage.GetLongValue("TradeFlt" & sKey, "Filter" & CStr(i), m_AuxTradeView.TradeViewFilter(i))
    Next
    
    If m_AuxTradeView.TradeViewFilter(TFC_TYPE) < 0 Or m_AuxTradeView.TradeViewFilter(TFC_TYPE) > 2 Then m_AuxTradeView.TradeViewFilter(TFC_TYPE) = 0
    
    m_gdFlt.ReadFromStorage "TradeFltGrid" & sKey, aStorage, False
    m_gdTrd.ReadFromStorage "TradeTrdGrid" & sKey, aStorage
    
    FilterUpdate
    FormatFltGrid
    FormatFltColumns
    fgTrd.Rows = 1
    FormatTrdGrid
    FormatTrdColumns
    
    tmrShow.Enabled = True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub Refresh()
    On Error Resume Next
    tmrShow.Enabled = True
End Sub

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom
    
    CurUnderlyingID = 0
    With fgTrd
        If .Row > 0 Then
            If Not .RowHidden(.Row) Then
                Set aTrd = m_AuxTradeView.RowData(.Row)
                If Not aTrd Is Nothing Then
                    CurUnderlyingID = aTrd.UndID
                    Set aTrd = Nothing
                End If
            End If
        End If
    End With
    
    If CurUnderlyingID = 0 Then
        If m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> 0 Then
            CurUnderlyingID = m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING)
        End If
    End If
End Function

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
    If m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) <> nNewUndID Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then
                With fgFlt
                    m_GridLock(GT_TRADES_FILTER).LockRedraw
                    
                    sValue = "0"
                    nValue = 0
                    
                    sComboList = g_Params.UnderlyingComboList
                    If Len(sComboList) > 0 Then
                        sComboList = "#0;<All>|" & sComboList
                    Else
                        sComboList = "#0;<All>"
                    End If
                    
                    Set aUnd = g_Underlying(m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING))
                    If Not aUnd Is Nothing Then
                        nValue = aUnd.ID
                        sValue = Trim$(Str$(aUnd.ID))
                        Set aUnd = Nothing
                    End If
                    .ColComboList(TFC_UNDERLYING) = sComboList
                    .TextMatrix(1, TFC_UNDERLYING) = sValue
                    m_AuxTradeView.TradeViewFilter(TFC_UNDERLYING) = nValue
                    
                    m_GridLock(GT_TRADES_FILTER).UnlockRedraw
                End With
            End If
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom, sComboList$, sValue$, nValue&
    If m_AuxTradeView.TradeViewFilter(TFC_GROUP) <> nNewUndGroupID Then
        Set aUndGroup = g_UnderlyingGroup(nNewUndGroupID)
        If Not aUndGroup Is Nothing Then
            With fgFlt
                m_GridLock(GT_TRADES_FILTER).LockRedraw
                
                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"
                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_AuxTradeView.TradeViewFilter(TFC_GROUP) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(aUndGroup.ID))
                    End If
                Next
                .ColComboList(TFC_GROUP) = sComboList
                .TextMatrix(1, TFC_GROUP) = sValue
                m_AuxTradeView.TradeViewFilter(TFC_GROUP) = nValue
                
                m_GridLock(GT_TRADES_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    Dim aStrategy As EtsGeneralLib.EtsStrategyAtom, sComboList$, sValue$, nValue&
    If m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) <> nStrategyID Then
        Set aStrategy = g_Strategy(nStrategyID)
        If Not aStrategy Is Nothing Then
            With fgFlt
                m_GridLock(GT_TRADES_FILTER).LockRedraw
                
                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"
                For Each aStrategy In g_Strategy
                    sComboList = sComboList & "|#" & Trim$(Str$(aStrategy.ID)) & ";" & aStrategy.Name
                    If aStrategy.ID = m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) Then
                        nValue = aStrategy.ID
                        sValue = Trim$(Str$(aStrategy.ID))
                    End If
                Next
                .ColComboList(TFC_STRATEGY) = sComboList
                .TextMatrix(1, TFC_STRATEGY) = sValue
                m_AuxTradeView.TradeViewFilter(TFC_STRATEGY) = nValue
                
                m_GridLock(GT_TRADES_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub PrintView()
'    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    frmPrintPreview.Execute m_frmOwner, "Trades", "Trades", fgFlt, fgTrd
'    Screen.MousePointer = vbDefault
End Sub

Private Sub HandleGridDblClick(ByVal bTradeNewAvailable As Boolean)
    On Error Resume Next
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols Then
        
        If IsAltPressed Then
            Select Case True
                Case IsCtrlPressed
                    mnuCtxOrderNewOption_Click
                    
                Case IsShiftPressed
                    mnuCtxTntCardNew_Click
                
                Case Else
                    mnuCtxOrderNewStock_Click
            End Select
            
            FixDblClickHandled
        
        ElseIf bTradeNewAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
            If m_enMenuGrid = GT_TRADES_LIST Then
                mnuCtxTradeEdit_Click
            Else
                mnuCtxTradeNew_Click
            End If
            FixDblClickHandled
        End If
    End If
End Sub

Public Function ExportScreen(ByVal sFileName As String, ByVal sFilePath As String, _
                             Optional ByVal sHeader As String = "", _
                             Optional ByVal FileSaveType As FileSaveTypeEnum = SE_ADDTOEND) As Boolean
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    ExportScreen = g_ScreenExport.Save(sFileName, sFilePath, fgTrd, , , sHeader, FileSaveType)
    Screen.MousePointer = vbNormal
End Function
