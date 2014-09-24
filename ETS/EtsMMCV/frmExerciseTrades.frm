VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Begin VB.Form frmExerciseTrades 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Trades Exercise/Expiry"
   ClientHeight    =   5760
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9855
   Icon            =   "frmExerciseTrades.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   5760
   ScaleWidth      =   9855
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.CommandButton btnSelAll 
      Caption         =   "&Select All"
      Height          =   315
      Left            =   120
      TabIndex        =   1
      Top             =   5340
      Width           =   1395
   End
   Begin VB.CommandButton btnClrAll 
      Caption         =   "&Clear All"
      Height          =   315
      Left            =   1620
      TabIndex        =   2
      Top             =   5340
      Width           =   1395
   End
   Begin VB.CommandButton btnOK 
      Caption         =   "Exercise/Expiry"
      Height          =   315
      Left            =   6180
      TabIndex        =   3
      Top             =   5340
      Width           =   2055
   End
   Begin VB.CommandButton btnCancel 
      Cancel          =   -1  'True
      Caption         =   "Cancel"
      Height          =   315
      Left            =   8340
      TabIndex        =   4
      Top             =   5340
      Width           =   1395
   End
   Begin VSFlex7Ctl.VSFlexGrid fgTrd 
      Height          =   4575
      Left            =   120
      TabIndex        =   0
      Top             =   660
      Width           =   9615
      _cx             =   16960
      _cy             =   8070
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
   Begin VB.Label lblWarning 
      Alignment       =   1  'Right Justify
      Caption         =   "<warning>"
      BeginProperty Font 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   700
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      ForeColor       =   &H00000080&
      Height          =   255
      Left            =   2280
      TabIndex        =   7
      Top             =   120
      Visible         =   0   'False
      Width           =   7455
   End
   Begin VB.Label lblContractInfo 
      Caption         =   "Contract Info:"
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
      TabIndex        =   6
      Top             =   390
      Width           =   9615
   End
   Begin VB.Label lblPrompt 
      Caption         =   "Trades to Exercise/Expiry :"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   5
      Top             =   120
      Width           =   2115
   End
End
Attribute VB_Name = "frmExerciseTrades"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Public Enum ColumnEnum
    IDX_SELECT = 0
    IDX_TYPE
    IDX_QTY
    IDX_QTY_TO_EXEC
    IDX_UND_PRICE
    IDX_IS_POSITION
    IDX_BS
    IDX_PRICE
    IDX_LOTSIZE
    IDX_TOT_PRICE
    IDX_DATE
    IDX_TRADER
    IDX_STRATEGY
    IDX_BROKER
    IDX_CLR_BROKER
    IDX_COMMISSION
    IDX_MATCHED
    
    IDX_COLUMN_COUNT
    IDX_LAST_COLUMN = IDX_COLUMN_COUNT - 1
End Enum

Private m_mbResult As VbMsgBoxResult
Private m_hComplete As Long

Private geTrd As New clsGridEx
Private m_gdTrd As New clsGridDef
Private m_GridLock As New clsGridLock
Private m_bIsLoaded As Boolean
Private m_bExercise As Boolean
Private m_sCaption As String
Private m_bIsAmerican As Boolean
Private m_bIsFutures As Boolean

Private m_Trades As clsTradeExecColl
Private m_sCurrentOriginalText As String

Public Function Execute(ByRef collTrades As clsTradeExecColl, ByVal bExercise As Boolean, _
                        ByRef frmOwner As Form) As VbMsgBoxResult
    On Error GoTo EH
    Dim bRestoreEnabled As Boolean
    m_mbResult = vbCancel
    
    If collTrades Is Nothing Then Exit Function
    If collTrades.Count <= 0 Then Exit Function
    
    Screen.MousePointer = vbHourglass
    m_bExercise = bExercise
    Set m_Trades = collTrades
    
    Load Me
    
    UpdateContractInfo
    FormatGrid
    
    If Not frmOwner Is Nothing Then
        m_hComplete = CreateEvent(ByVal 0&, 1, 0, ByVal 0&)
        If m_hComplete <> 0 Then
            ResetEvent m_hComplete
            
            If frmOwner.Enabled Then
                frmOwner.Enabled = False
                bRestoreEnabled = True
            End If
            
            ShowTrades
            
            If m_bExercise Then CheckExerciseAbility
            
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
            
            If bRestoreEnabled Then
                On Error Resume Next
                frmOwner.Enabled = True
                frmOwner.SetFocus
            End If
        Else
            ShowTrades
            
            If m_bExercise Then CheckExerciseAbility
            
            Screen.MousePointer = vbDefault
            Me.Show vbModal
        End If
    Else
        ShowTrades
        
        If m_bExercise Then CheckExerciseAbility
            
        Screen.MousePointer = vbDefault
        Me.Show vbModal
    End If
    
    Set m_Trades = Nothing
    Unload Me
    Execute = m_mbResult
    Exit Function
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open trades exercise/expiry window."
    On Error Resume Next
    Set m_Trades = Nothing
    If bRestoreEnabled Then
        frmOwner.Enabled = True
        frmOwner.SetFocus
    End If
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

Private Sub btnClrAll_Click()
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom, nRow&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        For nRow = 1 To .Rows - 1
            Set aTrd = .RowData(nRow)
            If Not aTrd Is Nothing Then
                If aTrd.Trd.Price >= 0 Then
                    aTrd.IsExec = False
                    .Cell(flexcpChecked, nRow, IDX_SELECT) = flexUnchecked
                    .TextMatrix(nRow, IDX_QTY_TO_EXEC) = ""
                    .TextMatrix(nRow, IDX_UND_PRICE) = ""
                End If
                Set aTrd = Nothing
            End If
        Next
        
        m_GridLock.UnlockRedraw
    End With
    
    UpdateControls
End Sub

Private Sub btnOK_Click()
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom
    
    Screen.MousePointer = vbDefault
    
    For Each aTrd In m_Trades
        If aTrd.IsExec And aTrd.QtyToExec > 0 _
            And aTrd.Trd.Price >= 0 _
            And (Not m_bExercise Or m_bExercise And aTrd.UndTradePrice > 0#) Then
            
            g_TradeChannel.ProcessTradeExec aTrd, m_bExercise, 0, 0
            DoEvents
        End If
    Next
    
    Screen.MousePointer = vbDefault
    
    m_mbResult = vbOK
    If m_hComplete <> 0 Then
        SetEvent m_hComplete
    Else
        Me.Hide
    End If
End Sub

Private Sub btnSelAll_Click()
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom, nRow&
    
    With fgTrd
        m_GridLock.LockRedraw
        
        For nRow = 1 To .Rows - 1
            Set aTrd = .RowData(nRow)
            If Not aTrd Is Nothing Then
                If aTrd.Trd.Price >= 0 Then
                    aTrd.IsExec = True
                    .Cell(flexcpChecked, nRow, IDX_SELECT) = flexChecked
                    .TextMatrix(nRow, IDX_QTY_TO_EXEC) = aTrd.QtyToExec
                    .TextMatrix(nRow, IDX_UND_PRICE) = aTrd.UndTradePrice
                End If
                Set aTrd = Nothing
            End If
        Next
        
        m_GridLock.UnlockRedraw
    End With
    
    UpdateControls
End Sub

Private Sub fgTrd_AfterEdit(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom, sValue$, nValue&, bValue As Boolean, dValue#
    
    With fgTrd
        Set aTrd = .RowData(Row)
        
        If Not aTrd Is Nothing Then
            Select Case Col
                Case IDX_SELECT
                    bValue = (.Cell(flexcpChecked, Row, IDX_SELECT) = flexChecked)
                    If aTrd.IsExec <> bValue Then
                        aTrd.IsExec = bValue
                        If bValue Then
                            .TextMatrix(Row, IDX_QTY_TO_EXEC) = aTrd.QtyToExec
                            .TextMatrix(Row, IDX_UND_PRICE) = aTrd.UndTradePrice
                        Else
                            .TextMatrix(Row, IDX_QTY_TO_EXEC) = ""
                            .TextMatrix(Row, IDX_UND_PRICE) = ""
                        End If
                    End If
                    
                Case IDX_QTY_TO_EXEC, IDX_UND_PRICE
                    sValue = Trim$(.TextMatrix(Row, Col))
                    If m_sCurrentOriginalText <> sValue Then
                        
                        Select Case Col
                            Case IDX_QTY_TO_EXEC
                                nValue = Abs(ReadLng(sValue))
                                If nValue > 0 And aTrd.QtyToExec <> nValue Then
                                    aTrd.QtyToExec = nValue
                                End If
                                .TextMatrix(Row, Col) = aTrd.QtyToExec
                                
                            Case IDX_UND_PRICE
                                dValue = Abs(ReadDbl(sValue))
                                If dValue > 0 And aTrd.UndTradePrice <> dValue Then
                                    aTrd.UndTradePrice = dValue
                                End If
                                .TextMatrix(Row, Col) = aTrd.UndTradePrice
                            
                        End Select
                    End If
                    
            End Select
            
            Set aTrd = Nothing
            UpdateControls
        End If
    End With
End Sub

Private Sub fgTrd_StartEdit(ByVal Row As Long, ByVal Col As Long, Cancel As Boolean)
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom
    
    Cancel = (Col <> IDX_SELECT And Col <> IDX_QTY_TO_EXEC And (Col <> IDX_UND_PRICE Or Not m_bExercise And Col = IDX_UND_PRICE))
    If Not Cancel Then
        Set aTrd = fgTrd.RowData(Row)
        If Not aTrd Is Nothing Then
            If Col = IDX_QTY_TO_EXEC Or Col = IDX_UND_PRICE Then
                Cancel = Not aTrd.IsExec
            Else
                Cancel = (aTrd.Trd.Price < 0)
            End If
            Set aTrd = Nothing
        Else
            Cancel = True
        End If
        
        If Not Cancel Then
            m_sCurrentOriginalText = Trim$(fgTrd.TextMatrix(Row, Col))
        End If
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
'    gCmn.CustomizeForm Me
    
    If m_bExercise Then
        m_sCaption = "Trades Exercise"
        Me.Caption = m_sCaption
        lblPrompt.Caption = "Trades to Exercise :"
        btnOK.Caption = "Exercise"
    Else
        m_sCaption = "Trades Expiry"
        Me.Caption = m_sCaption
        lblPrompt.Caption = "Trades to Expiry :"
        btnOK.Caption = "Expiry"
    End If
    
    m_GridLock.Init fgTrd
    geTrd.Init fgTrd
    g_DefGrids(GT_TRADES_LIST).CopyTo m_gdTrd
    UpdateControls
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If m_hComplete <> 0 Then SetEvent m_hComplete
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    
    Set m_Trades = Nothing
    
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
        .ExplorerBar = flexExSort
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
    
        .Cols = IDX_LAST_COLUMN
        
        .ColDataType(IDX_SELECT) = flexDTString
        .TextMatrix(0, IDX_SELECT) = IIf(m_bExercise, "Exerc", "Expiry")
        .ColAlignment(IDX_SELECT) = flexAlignCenterCenter
        
        .ColDataType(IDX_TYPE) = m_gdTrd.Col(TLC_TYPE).DataType
        .ColFormat(IDX_TYPE) = m_gdTrd.Col(TLC_TYPE).Format
        .TextMatrix(0, IDX_TYPE) = m_gdTrd.Col(TLC_TYPE).Caption
        
        .ColDataType(IDX_QTY) = m_gdTrd.Col(TLC_QTY).DataType
        .ColFormat(IDX_QTY) = m_gdTrd.Col(TLC_QTY).Format
        .TextMatrix(0, IDX_QTY) = m_gdTrd.Col(TLC_QTY).Caption
    
        .ColDataType(IDX_QTY_TO_EXEC) = m_gdTrd.Col(TLC_QTY).DataType
        .ColFormat(IDX_QTY_TO_EXEC) = m_gdTrd.Col(TLC_QTY).Format
        .TextMatrix(0, IDX_QTY_TO_EXEC) = IIf(m_bExercise, "QtyToExc", "QtyToExp")
        
        .ColDataType(IDX_UND_PRICE) = m_gdTrd.Col(TLC_PRICE).DataType
        .ColFormat(IDX_UND_PRICE) = m_gdTrd.Col(TLC_PRICE).Format
        .TextMatrix(0, IDX_UND_PRICE) = IIf(m_bIsFutures, "FutTrdPrc", "UndTrdPrc")
        .ColHidden(IDX_UND_PRICE) = Not m_bExercise
        
        .ColDataType(IDX_IS_POSITION) = flexDTBoolean
        .ColFormat(IDX_IS_POSITION) = "Pos;Trd"
        .TextMatrix(0, IDX_IS_POSITION) = "Pos/Trd"
        .ColAlignment(IDX_IS_POSITION) = flexAlignCenterCenter
        
        .ColDataType(IDX_BS) = m_gdTrd.Col(TLC_BS).DataType
        .ColFormat(IDX_BS) = m_gdTrd.Col(TLC_BS).Format
        .TextMatrix(0, IDX_BS) = m_gdTrd.Col(TLC_BS).Caption
        .ColAlignment(IDX_BS) = flexAlignCenterCenter
    
        .ColDataType(IDX_PRICE) = m_gdTrd.Col(TLC_PRICE).DataType
        .ColFormat(IDX_PRICE) = m_gdTrd.Col(TLC_PRICE).Format
        .TextMatrix(0, IDX_PRICE) = m_gdTrd.Col(TLC_PRICE).Caption
    
        .ColDataType(IDX_LOTSIZE) = m_gdTrd.Col(TLC_LOTSIZE).DataType
        .ColFormat(IDX_LOTSIZE) = m_gdTrd.Col(TLC_LOTSIZE).Format
        .TextMatrix(0, IDX_LOTSIZE) = m_gdTrd.Col(TLC_LOTSIZE).Caption
    
        .ColDataType(IDX_TOT_PRICE) = m_gdTrd.Col(TLC_TOT_PRICE).DataType
        .ColFormat(IDX_TOT_PRICE) = m_gdTrd.Col(TLC_TOT_PRICE).Format
        .TextMatrix(0, IDX_TOT_PRICE) = m_gdTrd.Col(TLC_TOT_PRICE).Caption
    
        .ColDataType(IDX_DATE) = m_gdTrd.Col(TLC_DATE).DataType
        .ColFormat(IDX_DATE) = m_gdTrd.Col(TLC_DATE).Format
        .TextMatrix(0, IDX_DATE) = m_gdTrd.Col(TLC_DATE).Caption
    
        .ColDataType(IDX_TRADER) = m_gdTrd.Col(TLC_TRADER).DataType
        .ColFormat(IDX_TRADER) = m_gdTrd.Col(TLC_TRADER).Format
        .TextMatrix(0, IDX_TRADER) = m_gdTrd.Col(TLC_TRADER).Caption
    
        .ColDataType(IDX_STRATEGY) = m_gdTrd.Col(TLC_STRATEGY).DataType
        .ColFormat(IDX_STRATEGY) = m_gdTrd.Col(TLC_STRATEGY).Format
        .TextMatrix(0, IDX_STRATEGY) = m_gdTrd.Col(TLC_STRATEGY).Caption
    
        .ColDataType(IDX_BROKER) = m_gdTrd.Col(TLC_BROKER).DataType
        .ColFormat(IDX_BROKER) = m_gdTrd.Col(TLC_BROKER).Format
        .TextMatrix(0, IDX_BROKER) = m_gdTrd.Col(TLC_BROKER).Caption
    
        .ColDataType(IDX_CLR_BROKER) = m_gdTrd.Col(TLC_CLR_BROKER).DataType
        .ColFormat(IDX_CLR_BROKER) = m_gdTrd.Col(TLC_CLR_BROKER).Format
        .TextMatrix(0, IDX_CLR_BROKER) = m_gdTrd.Col(TLC_CLR_BROKER).Caption
    
        .ColDataType(IDX_COMMISSION) = m_gdTrd.Col(TLC_COMMISSION).DataType
        .ColFormat(IDX_COMMISSION) = m_gdTrd.Col(TLC_COMMISSION).Format
        .TextMatrix(0, IDX_COMMISSION) = m_gdTrd.Col(TLC_COMMISSION).Caption
    
        .ColDataType(IDX_MATCHED) = m_gdTrd.Col(TLC_MATCHED).DataType
        .ColFormat(IDX_MATCHED) = m_gdTrd.Col(TLC_MATCHED).Format
        .TextMatrix(0, IDX_MATCHED) = m_gdTrd.Col(TLC_MATCHED).Caption
        .ColAlignment(IDX_MATCHED) = flexAlignCenterCenter
        
        .Cell(flexcpAlignment, 0, 0, 0, .Cols - 1) = flexAlignCenterCenter
        geTrd.AdjustSortImages
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub ShowTrades()
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom, nRow&
    
    With fgTrd
        m_GridLock.LockRedraw
        .Rows = 1
        
        For Each aTrd In m_Trades
            
            nRow = .Rows
            .AddItem ""
            
            .RowData(nRow) = aTrd
            
            TradeUpdate nRow, aTrd
        Next
        
        .Cell(flexcpPictureAlignment, 1, IDX_SELECT, .Rows - 1, IDX_SELECT) = flexPicAlignCenterCenter
        
        .Col = IDX_IS_POSITION
        .Row = 1
        .Sort = flexSortGenericAscending
        geTrd.ShowSortImage IDX_IS_POSITION, 1
        
        .Row = 1
        .Col = 1
        
        .Cell(flexcpFontBold, 1, IDX_QTY_TO_EXEC, .Rows - 1, IDX_QTY_TO_EXEC) = True
        .Cell(flexcpFontBold, 1, IDX_UND_PRICE, .Rows - 1, IDX_UND_PRICE) = True
        .AutoSize 0, .Cols - 1, False, 0
        
        UpdateControls
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub TradeUpdate(ByVal nRow As Long, ByRef aTrd As clsTradeExecAtom)
    On Error Resume Next
    Dim sValue$
    
    With fgTrd
        m_GridLock.LockRedraw
        
        If aTrd.Trd.Price >= 0 Then
            .Cell(flexcpChecked, nRow, IDX_SELECT) = IIf(aTrd.IsExec, flexChecked, flexUnchecked)
        Else
            .TextMatrix(nRow, IDX_SELECT) = "Ex"
        End If
        
        .TextMatrix(nRow, IDX_TYPE) = aTrd.Trd.GetTradeTypeString
        .TextMatrix(nRow, IDX_QTY) = aTrd.Trd.Quantity
        
        If aTrd.IsExec And aTrd.Trd.Price >= 0 Then
            .TextMatrix(nRow, IDX_QTY_TO_EXEC) = aTrd.QtyToExec
            .TextMatrix(nRow, IDX_UND_PRICE) = aTrd.UndTradePrice
        Else
            .TextMatrix(nRow, IDX_QTY_TO_EXEC) = ""
            .TextMatrix(nRow, IDX_UND_PRICE) = ""
        End If
        
        .TextMatrix(nRow, IDX_IS_POSITION) = aTrd.Trd.IsPosition
        .TextMatrix(nRow, IDX_BS) = aTrd.Trd.IsBuy
        .TextMatrix(nRow, IDX_PRICE) = aTrd.Trd.Price
        .TextMatrix(nRow, IDX_LOTSIZE) = aTrd.Trd.LotSize
        .TextMatrix(nRow, IDX_TOT_PRICE) = aTrd.Trd.TotalPrice
        .TextMatrix(nRow, IDX_DATE) = aTrd.Trd.TradeDate
        
        sValue = ""
        sValue = aTrd.Trd.Trader.Name
        .TextMatrix(nRow, IDX_TRADER) = sValue
        
        sValue = ""
        If Not aTrd.Trd.Strategy Is Nothing Then sValue = aTrd.Trd.Strategy.Name
        .TextMatrix(nRow, IDX_STRATEGY) = sValue
        
        sValue = ""
        If Not aTrd.Trd.Broker Is Nothing Then sValue = aTrd.Trd.Broker.Name
        .TextMatrix(nRow, IDX_BROKER) = sValue
        
        sValue = ""
        If Not aTrd.Trd.ClearingBroker Is Nothing Then sValue = aTrd.Trd.ClearingBroker.Name
        .TextMatrix(nRow, IDX_CLR_BROKER) = sValue

        .TextMatrix(nRow, IDX_COMMISSION) = 0# 'aTrd.Trd.TotalCommission
        
        .TextMatrix(nRow, IDX_MATCHED) = aTrd.Trd.GetTradeMatchedString
        
        m_GridLock.UnlockRedraw
    End With
End Sub

Private Sub UpdateContractInfo()
    On Error Resume Next
    Dim sInfo$, sValue$, aUnd As EtsGeneralLib.UndAtom
    
    sInfo = "Contract Info: "
    
    Debug.Assert Not m_Trades Is Nothing
    Debug.Assert m_Trades.Count > 0
    
    m_bIsAmerican = True
    m_bIsFutures = False
    
    With m_Trades(1).Trd
        If Len(.Symbol) > 2 Then
            sInfo = sInfo & Left$(.Symbol, Len(.Symbol) - 2)
        Else
            sInfo = sInfo & .Symbol
        End If
        
        If .ContractType = enCtOption Then
            sInfo = sInfo & Format$(.Opt.Expiry, " MMMM YY") & IIf(.Opt.OptType <> enOtPut, " Call ", " Put ") & Int(.Opt.Strike)
            sInfo = sInfo & " (" & .Und.Symbol & ")"
            m_bIsFutures = False
        Else
            sInfo = sInfo & Format$(.FutOpt.Expiry, " MMMM YY") & IIf(.FutOpt.OptType <> enOtPut, " Call ", " Put ") & Int(.FutOpt.Strike)
            sInfo = sInfo & " (" & .Fut.Symbol & ")"
            m_bIsFutures = True
        End If
        
        m_bIsAmerican = .Und.IsAmerican
    End With
    
    lblContractInfo.Caption = sInfo
End Sub

Private Sub UpdateControls()
    On Error Resume Next
    Dim aTrd As clsTradeExecAtom, nTradesForExecCount&, nInvalidTradesCount&
    
    nTradesForExecCount = 0
    nInvalidTradesCount = 0
    For Each aTrd In m_Trades
        If aTrd.IsExec And aTrd.Trd.Price >= 0 Then
            nTradesForExecCount = nTradesForExecCount + 1
            
            If aTrd.QtyToExec <= 0 Or m_bExercise And aTrd.UndTradePrice < 0# Then
                nInvalidTradesCount = nInvalidTradesCount + 1
            End If
        End If
    Next
    
    btnOK.Enabled = (nTradesForExecCount > 0 And nInvalidTradesCount = 0)
End Sub

Private Sub CheckExerciseAbility()
    On Error Resume Next
    If Not m_bExercise Then Exit Sub
    
    lblWarning.Caption = ""
    lblWarning.Visible = False
    If Not m_bIsAmerican Then
        lblWarning.Caption = "European style"
        lblWarning.Visible = True
    End If
End Sub

