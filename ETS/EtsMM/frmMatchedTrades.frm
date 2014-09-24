VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.Form frmMatchedTrades 
   BorderStyle     =   1  'Fixed Single
   Caption         =   "ETS Trades Matching"
   ClientHeight    =   4305
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   8220
   Icon            =   "frmMatchedTrades.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   4305
   ScaleWidth      =   8220
   StartUpPosition =   3  'Windows Default
   Begin VB.Timer tmrCheckMatched 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   1080
      Top             =   3780
   End
   Begin VB.Timer tmrFlash 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   480
      Top             =   3720
   End
   Begin VB.CommandButton btnLeave 
      Caption         =   "Leave Manual Trade(s)"
      Height          =   315
      Left            =   6060
      TabIndex        =   2
      Top             =   3900
      Width           =   2055
   End
   Begin VB.CommandButton btnDel 
      Caption         =   "Delete Selected Manual Trade"
      Height          =   315
      Left            =   3420
      TabIndex        =   1
      Top             =   3900
      Width           =   2535
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTrd 
      Height          =   3135
      Left            =   120
      TabIndex        =   0
      Top             =   660
      Width           =   7995
      _cx             =   14102
      _cy             =   5530
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
   Begin VB.Label lblTradeInfo 
      Caption         =   "Trade Info:"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   390
      Width           =   7995
   End
   Begin VB.Label lblPrompt 
      Caption         =   "Real-time Trade has found a matching Manual Trade(s) :"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   3
      Top             =   120
      Width           =   7995
   End
End
Attribute VB_Name = "frmMatchedTrades"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private m_nTop As Long
Private m_nLeft As Long
Private m_nState As Long

Private m_nFlash As Long
Private geTrd As New clsGridEx
Private m_gdTrd As New clsGridDef
Private m_GridLock As New clsGridLock
Private m_bIsLoaded As Boolean

Private m_CurTrdMatch As MmTradeMatchAtom
Private m_TrdMatchColl As MmTradeMatchColl

Public Sub StartMonitoring()
    On Error Resume Next
    If m_bIsLoaded Then Exit Sub
    
    m_bIsLoaded = True
    
    Load Me
    tmrCheckMatched.Enabled = True
End Sub

Private Sub ProcessTradeMatch()
    On Error GoTo EH
    If Not m_CurTrdMatch Is Nothing Then
        tmrCheckMatched.Enabled = False
        
        g_TradeChannel.ProccessTradesMatch m_CurTrdMatch
        
        Set m_CurTrdMatch = Nothing
        tmrCheckMatched_Timer
        tmrCheckMatched.Enabled = True
    End If
    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to process trade matching."
    Set m_CurTrdMatch = Nothing
    tmrCheckMatched_Timer
    tmrCheckMatched.Enabled = True
End Sub

Private Sub btnDel_Click()
    On Error Resume Next
    ProcessTradeMatch
End Sub

Private Sub btnLeave_Click()
    On Error Resume Next
    Set m_CurTrdMatch.TrdMatch = Nothing
    ProcessTradeMatch
End Sub

Private Sub fgTrd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim sKey$
    
    With fgTrd
        sKey = .RowData(Row)
        If Len(sKey) > 0 And Not m_CurTrdMatch Is Nothing Then
            .Cell(flexcpChecked, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexUnchecked
            .Cell(flexcpPictureAlignment, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexPicAlignCenterCenter
            
            Set m_CurTrdMatch.TrdMatch = Nothing
            Set m_CurTrdMatch.TrdMatch = m_CurTrdMatch.TrdManual(CLng(sKey))
            
            If Not m_CurTrdMatch.TrdMatch Is Nothing Then
                .Cell(flexcpChecked, Row, TLC_KEY) = flexChecked
            End If
            
            UpdateButtons
        End If
    End With
End Sub

Private Sub fgTrd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim sKey$
    sKey = fgTrd.RowData(Row)
    Cancel = (m_CurTrdMatch Is Nothing Or Len(sKey) <= 0)
End Sub

Private Sub tmrCheckMatched_Timer()
    On Error Resume Next
    If Not m_CurTrdMatch Is Nothing Then
    
        If Not m_TrdMatchColl(m_CurTrdMatch.TrdActual.TradeID) Is Nothing Then
            If m_CurTrdMatch.Updated And m_CurTrdMatch.TrdManual.Count > 0 Then
                UpdateTradeInfo
                UpdateCurrentTradeMatchInfo
                m_CurTrdMatch.Updated = False
                UpdateButtons
                
                If Not Me.Visible Then ShowWindow Me.hWnd, SW_SHOWNOACTIVATE
                m_nFlash = 0
                tmrFlash.Enabled = True
            End If
            
            If m_CurTrdMatch.TrdManual.Count > 0 Then
                Exit Sub
            Else
                Set m_CurTrdMatch = Nothing
            End If
        Else
            Set m_CurTrdMatch = Nothing
        End If
        
    End If
    
    If m_TrdMatchColl.Count > 0 Then
    
        For Each m_CurTrdMatch In m_TrdMatchColl
            If Not m_CurTrdMatch Is Nothing Then GoTo Found
        Next
Found:
        UpdateTradeInfo
        ShowCurrentTradeMatchInfo
        m_CurTrdMatch.Updated = False
        
        If Not Me.Visible Then ShowWindow Me.hWnd, SW_SHOWNOACTIVATE
        m_nFlash = 0
        tmrFlash.Enabled = True
        
    ElseIf Me.Visible Then
    
        UpdateTradeInfo
        m_nTop = Me.Top
        m_nLeft = Me.Left
        m_nState = Me.WindowState
        Me.Hide
        ClearTradeMatchInfo
        
    End If
    UpdateButtons
End Sub

Private Sub UpdateTradeInfo()
    On Error Resume Next
    Dim sInfo$, sValue$, aEnt As EtsGeneralLib.EtsMmEntityAtom
    
    sInfo = "Trade Info: "
    If Not m_CurTrdMatch Is Nothing Then
        Debug.Assert Not m_CurTrdMatch.TrdActual Is Nothing
        
        With m_CurTrdMatch.TrdActual
            sValue = "<none>"
            sValue = .Trader.Name
            sInfo = sInfo & sValue
            
            sInfo = sInfo & IIf(.IsBuy, ", Buy ", ", Sell ")
            
            If .ContractType = enCtOption Then
                If Len(.Symbol) > 2 Then
                    sInfo = sInfo & Left$(.Symbol, Len(.Symbol) - 2)
                Else
                    sInfo = sInfo & .Symbol
                End If
                
                sInfo = sInfo & Format$(.Opt.Expiry, " MMMM YY") & IIf(.Opt.OptType <> enOtPut, " Call ", " Put ") & Int(.Opt.Strike)
                sInfo = sInfo & " (" & .Und.Symbol & ")"
            Else
                sInfo = sInfo & ", " & .Und.Symbol
            End If
        End With
        
        lblTradeInfo.Caption = sInfo
        
    Else
        sInfo = sInfo & " N/A"
    End If
End Sub

Private Sub UpdateCurrentTradeMatchInfo()
    On Error Resume Next
    Dim nRow&, aTrd As EtsGeneralLib.MmTradeInfoAtom, sKey$
    
    With fgTrd
        m_GridLock.LockRedraw
        
        If Not m_CurTrdMatch Is Nothing Then
            Debug.Assert Not m_CurTrdMatch.TrdActual Is Nothing
            
            TradeUpdate 1, m_CurTrdMatch.TrdActual
            .TextMatrix(nRow, TLC_KEY) = "Actual"
            
            For Each aTrd In m_CurTrdMatch.TrdManual
                nRow = .FindRow(CStr(aTrd.TradeID), 2, , True)
                If nRow < 0 Then
                    .AddItem ""
                    nRow = .Rows - 1
                    .RowData(nRow) = CStr(aTrd.TradeID)
                    TradeUpdate nRow, aTrd
                Else
                    TradeUpdate nRow, aTrd
                End If
            Next
            
            For nRow = .Rows - 1 To 2 Step -1
                sKey = .RowData(nRow)
                If m_CurTrdMatch.TrdManual(CLng(sKey)) Is Nothing Then
                    .RemoveItem nRow
                End If
            Next
            
            .Cell(flexcpChecked, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexUnchecked
            .Cell(flexcpPictureAlignment, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexPicAlignCenterCenter
            
            If Not m_CurTrdMatch.TrdMatch Is Nothing Then
                nRow = .FindRow(CStr(m_CurTrdMatch.TrdMatch.TradeID), 2, , True)
                If nRow > 1 Then
                    .Cell(flexcpChecked, nRow, TLC_KEY) = flexChecked
                Else
                    Set m_CurTrdMatch.TrdMatch = Nothing
                End If
            End If
            
            .AutoSize 0, .Cols - 1, False
        End If
        
        m_GridLock.UnlockRedraw
    End With
End Sub
       
Private Sub ShowCurrentTradeMatchInfo()
    On Error Resume Next
    Dim nRow&, aTrd As EtsGeneralLib.MmTradeInfoAtom, nCol&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        .Rows = 2
        .Cell(flexcpText, 1, 0, 1, .Cols - 1) = ""
        
        If Not m_CurTrdMatch Is Nothing Then
            Debug.Assert Not m_CurTrdMatch.TrdActual Is Nothing
            
            TradeUpdate 1, m_CurTrdMatch.TrdActual
            .TextMatrix(nRow, TLC_KEY) = "Actual"
            
            For Each aTrd In m_CurTrdMatch.TrdManual
                .AddItem ""
                nRow = .Rows - 1
                .RowData(nRow) = CStr(aTrd.TradeID)
                TradeUpdate nRow, aTrd
            Next
            
            .Cell(flexcpChecked, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexUnchecked
            .Cell(flexcpPictureAlignment, 2, TLC_KEY, .Rows - 1, TLC_KEY) = flexPicAlignCenterCenter
        
            If Not m_CurTrdMatch.TrdMatch Is Nothing Then
                nRow = .FindRow(CStr(m_CurTrdMatch.TrdMatch.TradeID), 2, , True)
                If nRow > 1 Then
                    .Cell(flexcpChecked, nRow, TLC_KEY) = flexChecked
                Else
                    Set m_CurTrdMatch.TrdMatch = Nothing
                End If
            End If
            
            .AutoSize 0, .Cols - 1, False
        End If
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub TradeUpdate(ByVal nRow As Long, aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nCol&, sValue$
    
    With fgTrd
        m_GridLock.LockRedraw
        
        nCol = .ColIndex(TLC_PRICE)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.price
        
        nCol = .ColIndex(TLC_QTY)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.Quantity
        
        nCol = .ColIndex(TLC_LOTSIZE)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.LotSize
        
        nCol = .ColIndex(TLC_TOT_PRICE)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.TotalPrice
        
        nCol = .ColIndex(TLC_DATE)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.TradeDate
        
        nCol = .ColIndex(TLC_STRATEGY)
        If nCol >= 0 Then
            sValue = ""
            If Not aTrd.Strategy Is Nothing Then sValue = aTrd.Strategy.Name
            .TextMatrix(nRow, nCol) = sValue
        End If
        
        nCol = .ColIndex(TLC_BROKER)
        If nCol >= 0 Then
            sValue = ""
            If Not aTrd.Broker Is Nothing Then sValue = aTrd.Broker.Name
            .TextMatrix(nRow, nCol) = sValue
        End If
        
        nCol = .ColIndex(TLC_CLR_BROKER)
        If nCol >= 0 Then
            sValue = ""
            If Not aTrd.ClearingBroker Is Nothing Then sValue = aTrd.ClearingBroker.Name
            .TextMatrix(nRow, nCol) = sValue
        End If
    
        nCol = .ColIndex(TLC_COMMISSION)
        If nCol >= 0 Then .TextMatrix(nRow, nCol) = aTrd.TotalCommission
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub ClearTradeMatchInfo()
    On Error Resume Next
    With fgTrd
        m_GridLock.LockRedraw
        
        .Rows = 2
        .Cell(flexcpText, 1, 0, 1, .Cols - 1) = ""
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub UpdateButtons()
    On Error Resume Next
    If Not m_CurTrdMatch Is Nothing Then
        btnDel.Enabled = Not m_CurTrdMatch.TrdMatch Is Nothing
        btnLeave.Enabled = m_CurTrdMatch.TrdManual.Count > 0
    Else
        btnDel.Enabled = False
        btnLeave.Enabled = False
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
    LoadSettings
    Me.Top = m_nTop
    Me.Left = m_nLeft
    Me.WindowState = m_nState
    Me.Caption = AppTitle() & " Trades Matching"
    
    m_GridLock.Init fgTrd
    geTrd.Init fgTrd
    g_DefGrids(GT_TRADES_LIST).CopyTo m_gdTrd
    FormatGrid
    Set m_TrdMatchColl = g_TradeChannel.TradesForMatch
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    Cancel = (UnloadMode = vbFormControlMenu)
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    m_bIsLoaded = False
    tmrCheckMatched.Enabled = False
    Set m_CurTrdMatch = Nothing
    Set m_TrdMatchColl = Nothing
    SaveSettings
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Form_Terminate
End Sub

Private Sub tmrFlash_Timer()
    On Error Resume Next
    If m_nFlash < 4 Then
        FlashWindow Me.hWnd, 1
        m_nFlash = m_nFlash + 1
    Else
        FlashWindow Me.hWnd, 0
        tmrFlash.Enabled = False
    End If
End Sub

Private Sub FormatGrid()
    On Error Resume Next
    Dim i&, nIdx&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        .Rows = 2
        .Cols = 1
        
        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .Appearance = flex3DLight ' flexFlat
        .AutoSizeMode = flexAutoSizeColWidth
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = False
        .Editable = flexEDKbdMouse
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSort
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 1
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
    
        .Cols = TLC_LAST_COLUMN
        
        .ColKey(0) = TLC_KEY
        .ColDataType(0) = flexDTString
        .TextMatrix(0, 0) = "Match"
        
        nIdx = 1
        For i = TLC_PRICE To TLC_COMMISSION
            If i <> TLC_TRADER Then
                .ColKey(nIdx) = i
                .ColDataType(nIdx) = m_gdTrd.Col(i).DataType
                .ColFormat(nIdx) = m_gdTrd.Col(i).Format
                .TextMatrix(0, nIdx) = m_gdTrd.Col(i).Caption
                nIdx = nIdx + 1
            End If
        Next
        .Cols = nIdx
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        '.Cell(flexcpFontBold, 1, 0, 1, .Cols - 1) = True
        .Cell(flexcpBackColor, 1, 0, 1, .Cols - 1) = &HC0FFC0
        geTrd.AdjustSortImages
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub LoadSettings()
    On Error Resume Next
    Dim nTmp&
    
    m_nTop = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MathedTradesTop", Me.Top)
    m_nLeft = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MathedTradesLeft", Me.Left)
    m_nState = g_aUserXMLParams.GetLongValue(APP_XML_KEY & "Settings", "MathedTradesState", vbNormal)
    
    If m_nLeft < 0 Then m_nLeft = 0
    nTmp = GetSystemMetrics(SM_CXFULLSCREEN) * 15
    If m_nLeft > nTmp Then m_nLeft = nTmp - Me.Width
    
    If m_nTop < 0 Then m_nTop = 0
    nTmp = GetSystemMetrics(SM_CYFULLSCREEN) * 15
    If m_nTop > nTmp Then m_nTop = nTmp - Me.Height
    
    If m_nState = vbMaximized Then m_nState = vbNormal
End Sub

Private Sub SaveSettings()
    On Error Resume Next
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MathedTradesTop", m_nTop
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MathedTradesLeft", m_nLeft
    g_aUserXMLParams.SetLongValue APP_XML_KEY & "Settings", "MathedTradesState", m_nState
End Sub
