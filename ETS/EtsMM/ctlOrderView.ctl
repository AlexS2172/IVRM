VERSION 5.00
Object = "{D76D7128-4A96-11D3-BD95-D296DC2DD072}#1.0#0"; "vsflex7.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.UserControl ctlOrderView 
   ClientHeight    =   9435
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   12720
   KeyPreview      =   -1  'True
   ScaleHeight     =   9435
   ScaleWidth      =   12720
   Begin VB.PictureBox picLog 
      BorderStyle     =   0  'None
      Height          =   1395
      Left            =   120
      ScaleHeight     =   1395
      ScaleWidth      =   12075
      TabIndex        =   4
      TabStop         =   0   'False
      Top             =   7260
      Width           =   12075
      Begin VSFlex7Ctl.VSFlexGrid fgLog 
         Height          =   1095
         Left            =   0
         TabIndex        =   3
         Top             =   255
         Width           =   10995
         _cx             =   19394
         _cy             =   1931
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
      Begin VB.Label lblLogCaption 
         BackColor       =   &H80000002&
         Caption         =   " Orders Event Log"
         ForeColor       =   &H80000009&
         Height          =   195
         Left            =   0
         TabIndex        =   5
         Top             =   0
         Width           =   10995
      End
   End
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
   Begin VSFlex7Ctl.VSFlexGrid fgOrd 
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
   Begin MSComctlLib.ImageList ilEvent 
      Left            =   1260
      Top             =   3660
      _ExtentX        =   1005
      _ExtentY        =   1005
      BackColor       =   -2147483643
      ImageWidth      =   16
      ImageHeight     =   16
      MaskColor       =   12632256
      _Version        =   393216
      BeginProperty Images {2C247F25-8591-11D1-B16A-00C0F0283628} 
         NumListImages   =   3
         BeginProperty ListImage1 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlOrderView.ctx":0000
            Key             =   ""
         EndProperty
         BeginProperty ListImage2 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlOrderView.ctx":015A
            Key             =   ""
         EndProperty
         BeginProperty ListImage3 {2C247F27-8591-11D1-B16A-00C0F0283628} 
            Picture         =   "ctlOrderView.ctx":02B4
            Key             =   ""
         EndProperty
      EndProperty
   End
   Begin EtsMM.ctlSplitter ctlSplit 
      Height          =   75
      Left            =   0
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   6900
      Width           =   10755
      _ExtentX        =   18971
      _ExtentY        =   132
      SplitterOrientation=   2
   End
   Begin VB.Menu mnuCtx 
      Caption         =   "<Context>"
      Begin VB.Menu mnuCtxOrderNewStock 
         Caption         =   "New Stock Order..."
      End
      Begin VB.Menu mnuCtxOrderNewOption 
         Caption         =   "New Option Order..."
      End
      Begin VB.Menu mnuCtxOrderCancel 
         Caption         =   "Cancel Order..."
      End
      Begin VB.Menu mnuCtxOrderCancelReplace 
         Caption         =   "Cancel/Replace Order..."
      End
      Begin VB.Menu mnuCtxOrderStatus 
         Caption         =   "Request Order Status"
      End
      Begin VB.Menu mnuCtxSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxTntCardNew 
         Caption         =   "New Trade Message..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxDefOrdSort 
         Caption         =   "Default Orders Sorting"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuCtxSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxCopy 
         Caption         =   "Copy Grid"
      End
      Begin VB.Menu mnuCtxPrint 
         Caption         =   "Print Grid..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuCtxSeparator4 
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
      Begin VB.Menu mnuCtxSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxGridLayout 
         Caption         =   "Grid Layout..."
      End
      Begin VB.Menu mnuCtxSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuCtxClearLog 
         Caption         =   "Clear"
      End
   End
End
Attribute VB_Name = "ctlOrderView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Option Explicit

Private Const OOL_ORD As Long = 1
Private Const OOL_EXEC As Long = 2

Public Event OnSetCaption()
Public Event OnStateChange()

Private WithEvents OrdersProcessor As clsOrdersProcessor
Attribute OrdersProcessor.VB_VarHelpID = -1
Private WithEvents TradeChannel As clsTradeChannel
Attribute TradeChannel.VB_VarHelpID = -1
Private WithEvents frmLayout As frmGridLayout
Attribute frmLayout.VB_VarHelpID = -1

Public pbProgress As MSComctlLib.ProgressBar
Public lblProcess As vB.Label
Public lblStatus As vB.Label

Private m_gdFlt As New clsGridDef
Private m_gdOrd As New clsGridDef
Private m_gdLog As New clsGridDef

Private WithEvents geOrd As clsGridEx
Attribute geOrd.VB_VarHelpID = -1

Private m_nFilter(OFC_FIRST_COLUMN To OFC_LAST_COLUMN) As Long

Private m_bInProc As Boolean
Private m_bShutDown As Boolean
Private m_sCurrentOriginalText As String

Private m_ExpandedOrd As New EtsGeneralLib.EtsMmFilterAtomColl

Private m_enMenuGrid As GridTypeEnum
Private m_nMenuGridCol As Long
Private m_nMenuGridRow As Long
Private m_nMenuGridCols As Long
Private m_nMenuGridRows As Long

Private m_bKeyDown(GT_ORDERS_FILTER To GT_ORDERS_LOG) As Boolean
Private m_GridLock(GT_ORDERS_FILTER To GT_ORDERS_LOG) As New clsGridLock
Private m_nOrders As Long

Public m_frmOwner As Form
Private m_bLogVisible As Boolean
Private m_bLogLoaded As Boolean

Public Function Init() As Boolean
    On Error GoTo EH

    m_bLogVisible = True
    m_bLogLoaded = False

    ctlSplit.BackColor = &H8000000F
    ctlSplit.SplitterTop = ScaleHeight - picLog.Height

    picLog.Left = 0
    fgLog.Left = 0
    lblLogCaption.Left = 0
    ctlSplit.Left = 0
    fgFlt.Top = 0
    fgFlt.Left = 0
    fgOrd.Left = 0

    mnuCtxCopy.Caption = "Copy Grid" & vbTab & "Ctrl+C"
    mnuCtxAutosizeCol.Caption = "Autosize Column" & vbTab & "Ctrl+A"
    mnuCtxAutosizeGrid.Caption = "Autosize Grid" & vbTab & "Ctrl+G"
    mnuCtxOrderNewStock.Caption = "New Stock Order..." & vbTab & "Alt+Ins"
    mnuCtxOrderNewOption.Caption = "New Option Order..." & vbTab & "Ctrl+Alt+Ins"
    mnuCtxOrderCancel.Caption = "Cancel Order..." & vbTab & "Del"
    mnuCtxOrderStatus.Caption = "Request Order Status" & vbTab & "F5"
    mnuCtxOrderCancelReplace.Caption = "Cancel/Replace Order..." & vbTab & "Enter"
    mnuCtxTntCardNew.Caption = "New Trade Message..." & vbTab & "Shift+Alt+Ins"

    ResetMenuData
    m_sCurrentOriginalText = ""
    m_bKeyDown(GT_ORDERS_FILTER) = False
    m_bKeyDown(GT_ORDERS_LIST) = False
    m_bKeyDown(GT_ORDERS_LOG) = False
    m_nOrders = 0

    m_GridLock(GT_ORDERS_FILTER).Init fgFlt
    m_GridLock(GT_ORDERS_LIST).Init fgOrd
    m_GridLock(GT_ORDERS_LOG).Init fgLog

    InitColumns
    InitGrids

    Set geOrd = New clsGridEx
    geOrd.Init fgOrd

    FormatFltGrid
    FormatFltColumns

    FormatOrdGrid
    FormatOrdColumns

    FormatLogGrid
    FormatLogColumns

    InitFltData

    Set OrdersProcessor = g_OrdersProcessor
    Set TradeChannel = g_TradeChannel

    fgFlt.Col = OFC_FIRST_COLUMN
    fgFlt.Row = 1

    Set frmLayout = New frmGridLayout
    Init = True

    AdjustCaption
    Exit Function
EH:
    gCmn.ErrorMsgBox m_frmOwner, "Fail to init orders view."
End Function

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    Select Case nType
        Case TYPE_UNDERLYING
            m_nFilter(OFC_UNDERLYING) = nValue
        Case TYPE_GROUP
            m_nFilter(OFC_GROUP) = nValue
        Case TYPE_TRADER
            m_nFilter(OFC_TRADER) = nValue
    End Select

    FilterUpdate

    tmrShow.Enabled = True
End Sub

Private Sub FilterUpdate()
    On Error Resume Next
    Dim nValue&

    With fgFlt
        m_GridLock(GT_ORDERS_FILTER).LockRedraw

        nValue = m_nFilter(OFC_UNDERLYING)
        If g_Underlying(nValue) Is Nothing Then nValue = 0
        m_nFilter(OFC_UNDERLYING) = nValue
        .TextMatrix(1, OFC_UNDERLYING) = nValue

        .TextMatrix(1, OFC_CONTRACT_TYPE) = m_nFilter(OFC_CONTRACT_TYPE)

        .TextMatrix(1, OFC_ORDER_STATUS) = m_nFilter(OFC_ORDER_STATUS)

        nValue = m_nFilter(OFC_GROUP)
        If g_UnderlyingGroup(nValue) Is Nothing Then nValue = 0
        m_nFilter(OFC_GROUP) = nValue
        .TextMatrix(1, OFC_GROUP) = nValue

        nValue = m_nFilter(OFC_TRADER)
        If g_Trader(nValue) Is Nothing Then nValue = 0
        m_nFilter(OFC_TRADER) = nValue
        .TextMatrix(1, OFC_TRADER) = nValue

        .AutoSize 0, .Cols - 1, , 100

        m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub InitFltData()
    On Error Resume Next
    Dim i&, nCount&, sComboList$, sValue$, aUnd As EtsGeneralLib.UndAtom, nValue&
    Dim aTrader As EtsGeneralLib.EtsTraderAtom, aUndGroup As EtsGeneralLib.EtsUndGroupAtom

    With fgFlt
        m_GridLock(GT_ORDERS_FILTER).LockRedraw

        sValue = "0"
        nValue = 0
        'sComboList = g_Params.UnderlyingComboList
        sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
        If Len(sComboList) > 0 Then
            sComboList = "#0;<All>|" & sComboList
        Else
            sComboList = "#0;<All>"
        End If

        Set aUnd = g_Underlying(m_nFilter(OFC_UNDERLYING))
        If Not aUnd Is Nothing Then
            nValue = aUnd.ID
            sValue = Trim$(Str$(aUnd.ID))
            Set aUnd = Nothing
        End If
        .ColComboList(OFC_UNDERLYING) = sComboList
        .TextMatrix(1, OFC_UNDERLYING) = sValue
        m_nFilter(OFC_UNDERLYING) = nValue

        .ColComboList(OFC_CONTRACT_TYPE) = "#0;<All>|#1;Stocks|#2;Options"
        .TextMatrix(1, OFC_CONTRACT_TYPE) = "0"

        .ColComboList(OFC_ORDER_STATUS) = "#0;<All>|#1;Working|#2;Completed"
        .TextMatrix(1, OFC_ORDER_STATUS) = "0"

        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aUndGroup In g_UnderlyingGroup
            sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
            If aUndGroup.ID = m_nFilter(OFC_GROUP) Then
                nValue = aUndGroup.ID
                sValue = Trim$(Str$(aUndGroup.ID))
            End If
        Next
        .ColComboList(OFC_GROUP) = sComboList
        .TextMatrix(1, OFC_GROUP) = sValue
        m_nFilter(OFC_GROUP) = nValue

        sValue = "0"
        nValue = 0
        sComboList = "#0;<All>"
        For Each aTrader In g_Trader
            sComboList = sComboList & "|#" & Trim$(Str$(aTrader.ID)) & ";" & aTrader.Name
            If aTrader.ID = m_nFilter(OFC_TRADER) Then
                nValue = aTrader.ID
                sValue = Trim$(Str$(nValue))
            End If
        Next
        .ColComboList(OFC_TRADER) = sComboList
        .TextMatrix(1, OFC_TRADER) = sValue
        m_nFilter(OFC_TRADER) = nValue

        m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub OrdersLogShow()
    On Error Resume Next
    Dim aEvent As EtsGeneralLib.EtsEventAtom
    With fgLog
        m_GridLock(GT_ORDERS_LOG).LockRedraw
        .Rows = 1

        pbProgress.Max = OrdersProcessor.Events.Count

        For Each aEvent In OrdersProcessor.Events
            OrdersLogAddEvent aEvent, False

            DoEvents
            If m_bShutDown Then Exit Sub
            IncProgress pbProgress
        Next

        .AutoSize 0, 1
        .ColWidth(2) = .Width - .ColWidth(0) - .ColWidth(1) - ScaleX(GetSystemMetrics(SM_CXVSCROLL) + 10, vbPixels, vbTwips)

        m_GridLock(GT_ORDERS_LOG).UnlockRedraw

        If .Rows > 0 Then .Row = .Rows - 1
        SendMessage .hWnd, WM_VSCROLL, SB_BOTTOM, 0&
    End With
End Sub

Private Sub OrdersLogAddEvent(ByRef aEvent As EtsGeneralLib.EtsEventAtom, ByVal bShowNewRow As Boolean)
    On Error Resume Next
    If aEvent Is Nothing Then Exit Sub

    Dim nRow&, nMsgRows&

    With fgLog
        m_GridLock(GT_ORDERS_LOG).LockRedraw

        bShowNewRow = bShowNewRow And (.Row < 1 Or .Row = .Rows - 1)

        .AddItem aEvent.EvtID & vbTab & aEvent.EvtTime & vbTab & aEvent.EvtMessage
        nRow = .Rows - 1

        nMsgRows = InStrCount(aEvent.EvtMessage, vbCrLf)

        If nMsgRows > 0 Then
            .RowHeight(nRow) = .RowHeight(0) * (nMsgRows + 1)
        End If

        If aEvent.EvtType <> enEvtUnknown Then
            Set .Cell(flexcpPicture, nRow, OEC_TIME) = ilEvent.ListImages(aEvent.EvtType).Picture
            .Cell(flexcpPictureAlignment, nRow, OEC_TIME) = flexPicAlignLeftCenter
        End If

        m_GridLock(GT_ORDERS_LOG).UnlockRedraw

        If bShowNewRow Then
            If .Col < 0 Then .Col = OEC_MESSAGE
            .Row = nRow
            SendMessage .hWnd, WM_VSCROLL, SB_BOTTOM, 0&
        End If
    End With
End Sub

Private Sub OrdersShow(ByVal bReload As Boolean)
    On Error Resume Next
    Dim i&, nCount&, aOrd As EtsMmGeneralLib.MmOrderAtom, nRow&, nOldRow&, nOldCol&, nOrdRow&
    Dim aRowData As clsOvRowData, bHide As Boolean, aExec As EtsMmGeneralLib.MmOrderExecAtom
    Dim bCollapse As Boolean, aTrd As EtsGeneralLib.MmTradeInfoAtom

    m_bInProc = True
    AdjustState

    Screen.MousePointer = vbArrow
    DoEvents

    lblStatus.Visible = False
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Visible = True
    lblProcess.Caption = False
    lblProcess.Refresh
    lblProcess.Visible = True

    If Not m_bLogLoaded Then
        lblProcess.Caption = "Orders event log loading..."
        OrdersLogShow
        m_bLogLoaded = True
    End If

    lblProcess.Caption = IIf(bReload, "Data loading...", "Filter applying...")

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        nOldRow = .Row
        nOldCol = .Col

        m_nOrders = 0

        If bReload Then
            .Rows = 1

            nCount = OrdersProcessor.Orders.Count
            pbProgress.Max = nCount

            For Each aOrd In g_OrdersProcessor.Orders
                If Not aOrd Is Nothing Then
                    .AddItem ""
                    nOrdRow = .Rows - 1

                    Set aRowData = New clsOvRowData
                    Set aRowData.Ord = aOrd

                    .RowData(nOrdRow) = aRowData
                    .TextMatrix(nOrdRow, OLC_KEY) = CStr(aOrd.ID)

                    .IsSubtotal(nOrdRow) = True
                    .RowOutlineLevel(nOrdRow) = OOL_ORD

                    OrderUpdate nOrdRow, aOrd, True

                    bCollapse = m_ExpandedOrd(CStr(aOrd.ID)) Is Nothing

                    For Each aTrd In aOrd.Trades
                        .AddItem ""
                        nRow = .Rows - 1

                        Set aRowData = New clsOvRowData
                        Set aRowData.Ord = aOrd
                        Set aRowData.Trade = aTrd

                        .RowData(nRow) = aRowData
                        .TextMatrix(nRow, OLC_KEY) = CStr(aOrd.ID) & "_" & CStr(aTrd.TradeID)

                        .IsSubtotal(nRow) = False
                        .RowOutlineLevel(nRow) = OOL_EXEC

                        OrderTradeUpdate nRow, aTrd
                    Next

                    bHide = Not CheckOrderFilter(aOrd)
                    .RowHidden(nOrdRow) = bHide

                    If bHide Then
                        .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                        bCollapse = True
                    Else
                        m_nOrders = m_nOrders + 1
                    End If

                    If bCollapse Then .IsCollapsed(nOrdRow) = flexOutlineCollapsed

                    Set aOrd = Nothing
                    Set aRowData = Nothing
                End If

                DoEvents
                If m_bShutDown Then Exit Sub
                IncProgress pbProgress
            Next

            If m_bShutDown Then Exit Sub
            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh

            SortOrdersByDefault
            FormatOrdColumns
            OrdersUpdateTradeColors
        Else
            nCount = .Rows - 1
            pbProgress.Max = nCount

            bHide = False

            For nRow = 1 To nCount
                Set aRowData = .RowData(nRow)
                If Not aRowData Is Nothing Then
                    If aRowData.Trade Is Nothing Then
                        bHide = Not CheckOrderFilter(aRowData.Ord)

                        If bHide Then
                            .RowHidden(nRow) = True
                        Else
                            .RowHidden(nRow) = False
                            bHide = m_ExpandedOrd(CStr(aRowData.Ord.ID)) Is Nothing
                            If bHide Then .IsCollapsed(nRow) = flexOutlineCollapsed
                            m_nOrders = m_nOrders + 1
                        End If
                    Else
                        .RowHidden(nRow) = bHide
                    End If
                    Set aRowData = Nothing
                Else
                    Debug.Assert False
                End If

                DoEvents
                If m_bShutDown Then Exit Sub
                IncProgress pbProgress
            Next

            lblProcess.Caption = "Formatting..."
            lblProcess.Refresh
        End If

        If nOldRow > 0 And m_nOrders > 0 Then
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
            If m_nOrders > 0 Then
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

        UpdateOrdersCountLabel

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With

    DoEvents
    If m_bShutDown Then Exit Sub
    pbProgress.Visible = False
    lblProcess.Visible = False
    lblStatus.Visible = True

    m_bInProc = False
    AdjustState
    Screen.MousePointer = vbDefault
End Sub

Private Sub UpdateOrdersCountLabel()
    On Error Resume Next
    lblStatus.Caption = Format$(m_nOrders, "#,##0") & " order(s)"
End Sub

Private Sub OrdersUpdateTradeColors()
    On Error Resume Next
    Dim nCol&, nRow&, aTrd As EtsGeneralLib.MmTradeInfoAtom

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        For nRow = 1 To .Rows - 1
            Set aTrd = .RowData(nRow).Trade
            If Not aTrd Is Nothing Then
                Dim i&, nIdx&
                i = 0
                nIdx = m_gdOrd.Idx(0)
                While nIdx >= 0 And i <= OLC_LAST_COLUMN

                    nCol = i + 1
                    Select Case nIdx
                        Case OLC_UNDERLYING, OLC_SYMBOL, OLC_SERIES, OLC_ORDER_ID, OLC_BS, _
                            OLC_ORDER_QTY, OLC_ORDER_TYPE, OLC_ORDER_PRICE, OLC_ORDER_STATUS, _
                            OLC_ORDER_DATE, OLC_DEST, OLC_TRADER, OLC_OPT_TYPE, _
                            OLC_EXPIRY, OLC_STRIKE, OLC_LOTSIZE

                            .Cell(flexcpBackColor, nRow, nCol) = GetAlternateColor(.BackColor)

                    End Select

                    i = i + 1
                    nIdx = m_gdOrd.Idx(i)
                Wend

            End If
        Next

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With

    Set aTrd = Nothing
End Sub

Private Sub OrderTradeUpdate(ByVal nRow As Long, ByRef aRowTrd As EtsGeneralLib.MmTradeInfoAtom, _
                                Optional ByVal bUpdateColors As Boolean = False)
    On Error Resume Next
    Dim nCol&, aTrd As EtsGeneralLib.MmTradeInfoAtom
    Dim bColorAlreadyUpdated As Boolean

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        If Not aRowTrd Is Nothing Then
            Set aTrd = aRowTrd
        Else
            Set aTrd = .RowData(nRow).Trade
        End If

        If Not aTrd Is Nothing Then
            .MergeRow(nRow) = True

            Dim i&, nIdx&
            i = 0
            nIdx = m_gdOrd.Idx(0)
            While nIdx >= 0 And i <= OLC_LAST_COLUMN

                bColorAlreadyUpdated = False

                nCol = i + 1
                Select Case nIdx
                    Case OLC_UNDERLYING, OLC_SYMBOL, OLC_SERIES, OLC_ORDER_ID, OLC_BS, _
                        OLC_ORDER_QTY, OLC_ORDER_TYPE, OLC_ORDER_PRICE, OLC_ORDER_STATUS, _
                        OLC_ORDER_DATE, OLC_DEST, OLC_TRADER, OLC_OPT_TYPE, _
                        OLC_EXPIRY, OLC_STRIKE, OLC_LOTSIZE

                        .TextMatrix(nRow, nCol) = " "
                        .Cell(flexcpBackColor, nRow, nCol) = GetAlternateColor(.BackColor)

                        bColorAlreadyUpdated = True

                    Case OLC_EXEC_ID
                        .TextMatrix(nRow, nCol) = aTrd.TradeID 'aTrd.ExecID

                    Case OLC_EXEC_STATUS
                        .TextMatrix(nRow, nCol) = aTrd.ExecStatusString 'aExec.OrderExecStatusString

                    Case OLC_EXEC_PRICE
                        .TextMatrix(nRow, nCol) = aTrd.price 'aExec.ExecPrice

                    Case OLC_EXEC_QTY
                        .TextMatrix(nRow, nCol) = aTrd.Quantity 'aExec.ExecQty

                    Case OLC_EXEC_DATE
                        .TextMatrix(nRow, nCol) = aTrd.TradeDate 'aExec.ExecDate

                End Select

                If Not bColorAlreadyUpdated And bUpdateColors Then
                    .Cell(flexcpBackColor, nRow, nCol) = IIf(m_gdOrd.Col(nIdx).BackColor <> 0, m_gdOrd.Col(nIdx).BackColor, RGB(1, 1, 1))
                    .Cell(flexcpForeColor, nRow, nCol) = IIf(m_gdOrd.Col(nIdx).ForeColor <> 0, m_gdOrd.Col(nIdx).ForeColor, RGB(1, 1, 1))
                End If

                i = i + 1
                nIdx = m_gdOrd.Idx(i)
            Wend

        End If

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With

    Set aTrd = Nothing
End Sub

Private Sub OrderUpdate(ByVal nOrdRow As Long, ByRef aRowOrd As EtsMmGeneralLib.MmOrderAtom, _
                        ByVal bSymbol As Boolean, Optional ByVal bUpdateColors As Boolean = False)
    On Error Resume Next
    Dim nCol&
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        If Not aRowOrd Is Nothing Then
            Set aOrd = aRowOrd
        Else
            Set aOrd = .RowData(nOrdRow).Ord
        End If

        If Not aOrd Is Nothing Then
            .MergeRow(nOrdRow) = False

            Dim i&, nIdx&
            i = 0
            nIdx = m_gdOrd.Idx(0)
            While nIdx >= 0 And i <= OLC_LAST_COLUMN

                nCol = i + 1
                If bSymbol Then
                    Select Case nIdx
                        Case OLC_UNDERLYING
                            .TextMatrix(nOrdRow, nCol) = aOrd.Und.Symbol

                        Case OLC_SYMBOL
                            .TextMatrix(nOrdRow, nCol) = aOrd.Symbol

                        Case OLC_SERIES
                            If Not aOrd.Opt Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.Opt.Series
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_ORDER_ID
                            .TextMatrix(nOrdRow, nCol) = aOrd.OrderID

                        Case OLC_BS
                            .TextMatrix(nOrdRow, nCol) = IIf(aOrd.IsBuy, "Buy", "Sell")

                        Case OLC_ORDER_QTY
                            .TextMatrix(nOrdRow, nCol) = aOrd.OrderQty

                        Case OLC_ORDER_TYPE
                            .TextMatrix(nOrdRow, nCol) = aOrd.OrderTypeString

                        Case OLC_ORDER_DATE
                            .TextMatrix(nOrdRow, nCol) = aOrd.OrderDate

                        Case OLC_DEST
                            If Not aOrd.Dest Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.Dest.Code
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_TRADER
                            If Not aOrd.Trader Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.Trader.Name
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_OPT_TYPE
                            If Not aOrd.Opt Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = IIf(aOrd.Opt.OptType = enOtCall, "C", "P")
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_EXPIRY
                            If Not aOrd.Opt Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.Opt.Expiry
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_STRIKE
                            If Not aOrd.Opt Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.Opt.Strike
                            Else
                                .TextMatrix(nOrdRow, nCol) = ""
                            End If

                        Case OLC_LOTSIZE
                            If Not aOrd.OptRoot Is Nothing Then
                                .TextMatrix(nOrdRow, nCol) = aOrd.OptRoot.LotSize
                            Else
                                .TextMatrix(nOrdRow, nCol) = 1
                            End If

                        Case OLC_EXEC_ID
                            .TextMatrix(nOrdRow, nCol) = "" ' aExec.ExecID

                        Case OLC_EXEC_STATUS
                            .TextMatrix(nOrdRow, nCol) = "" ' aExec.GetOrderExecStatusString()

                        Case OLC_EXEC_PRICE
                            .TextMatrix(nOrdRow, nCol) = aOrd.AvgPrice

                        Case OLC_EXEC_QTY
                            .TextMatrix(nOrdRow, nCol) = aOrd.CumQty

                        Case OLC_EXEC_DATE
                            .TextMatrix(nOrdRow, nCol) = "" ' aExec.ExecDate

                    End Select
                End If

                Select Case nIdx
                    Case OLC_ORDER_PRICE
                        .TextMatrix(nOrdRow, nCol) = IIf(aOrd.OrderType = enOrtLimit, aOrd.OrderPrice, "")

                    Case OLC_ORDER_STATUS
                        .TextMatrix(nOrdRow, nCol) = aOrd.OrderStatusString

                End Select

                If bUpdateColors Then
                    .Cell(flexcpBackColor, nOrdRow, nCol) = IIf(m_gdOrd.Col(nIdx).BackColor <> 0, m_gdOrd.Col(nIdx).BackColor, RGB(1, 1, 1))
                    .Cell(flexcpForeColor, nOrdRow, nCol) = IIf(m_gdOrd.Col(nIdx).ForeColor <> 0, m_gdOrd.Col(nIdx).ForeColor, RGB(1, 1, 1))
                End If

                i = i + 1
                nIdx = m_gdOrd.Idx(i)
            Wend
        End If

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Function NoFilter() As Boolean
    On Error Resume Next
    NoFilter = Not (m_nFilter(OFC_UNDERLYING) <> 0 _
                    Or m_nFilter(OFC_CONTRACT_TYPE) <> 0 _
                    Or m_nFilter(OFC_ORDER_STATUS) <> 0 _
                    Or m_nFilter(OFC_GROUP) <> 0 _
                    Or m_nFilter(OFC_TRADER) <> 0)
End Function

Private Function CheckOrderFilter(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom) As Boolean
    On Error Resume Next
    Dim bMatched As Boolean, nValue&

    If Not NoFilter Then
        nValue = m_nFilter(OFC_UNDERLYING)
        bMatched = (nValue = 0 Or nValue = aOrd.UnderlyingID)

        If bMatched Then
            ' "#0;<All>|#1;Stocks|#2;Options"
            nValue = m_nFilter(OFC_CONTRACT_TYPE)
            bMatched = (nValue = 0)
            If Not bMatched Then
                Select Case nValue
                    Case 1 ' Stocks
                        bMatched = (aOrd.ContractType <> enCtOption)

                    Case 2 ' Options
                        bMatched = (aOrd.ContractType = enCtOption)

                End Select
            End If
        End If

        If bMatched Then
            ' "#0;<All>|#1;Working|#2;Completed"
            nValue = m_nFilter(OFC_ORDER_STATUS)
            bMatched = (nValue = 0)
            If Not bMatched Then
                Select Case nValue
                    Case 1 ' Working ???
                        bMatched = (aOrd.Status <> enOrsFilled)

                    Case 2 ' Completed ???
                        bMatched = (aOrd.Status = enOrsFilled)

                End Select
            End If
        End If

        If bMatched Then
            nValue = m_nFilter(OFC_GROUP)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = Not g_UnderlyingGroup(nValue).Und(aOrd.UnderlyingID) Is Nothing
            End If
        End If

        If bMatched Then
            nValue = m_nFilter(OFC_TRADER)
            bMatched = (nValue = 0)
            If Not bMatched Then
                bMatched = (aOrd.Trader.ID = nValue)
            End If
        End If

        CheckOrderFilter = bMatched
    Else
        CheckOrderFilter = True
    End If
End Function

Private Sub InitColumns()
    On Error Resume Next
    g_DefGrids(GT_ORDERS_FILTER).CopyTo m_gdFlt
    g_DefGrids(GT_ORDERS_LIST).CopyTo m_gdOrd
    g_DefGrids(GT_ORDERS_LOG).CopyTo m_gdLog
End Sub

Private Sub InitGrids()
    On Error Resume Next
    With fgFlt
        .Rows = 2
        .Cols = OFC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = False
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeNone
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight ' flexFlat
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

    With fgOrd
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
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExSortAndMove
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarSymbolsLeaf
        .OutlineCol = 1
        .ScrollBars = flexScrollBarBoth
        .ScrollTrack = True
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls

        .MergeCells = flexMergeFree
        .MergeCompare = flexMCExact
    End With

    With fgLog
        .Rows = 1
        .Cols = OEC_COLUMN_COUNT

        .AllowBigSelection = False
        .AllowSelection = True
        .AllowUserFreezing = flexFreezeNone
        .AllowUserResizing = flexResizeColumns
        .AutoSizeMode = flexAutoSizeColWidth
        .Appearance = flex3DLight ' flexFlat
        .AutoSearch = flexSearchNone
        .AutoSizeMouse = True
        .Editable = flexEDNone
        .Ellipsis = flexEllipsisEnd
        .ExplorerBar = flexExNone
        .ExtendLastCol = False
        .FixedCols = 0
        .FixedRows = 1
        .FocusRect = flexFocusSolid
        .FrozenCols = 0
        .FrozenRows = 0
        .OutlineBar = flexOutlineBarNone
        .ScrollBars = flexScrollBarBoth
        .SelectionMode = flexSelectionFree
        .TabBehavior = flexTabControls
        .HighLight = flexHighlightWithFocus
        '.ColWidthMin = 1350
    End With
End Sub

Private Sub FormatFltGrid()
    On Error Resume Next
    Dim i&, nCols&

    With fgFlt
        m_GridLock(GT_ORDERS_FILTER).LockRedraw

        nCols = OFC_COLUMN_COUNT
        .Cols = nCols

        For i = OFC_FIRST_COLUMN To OFC_LAST_COLUMN
            .TextMatrix(0, i) = m_gdFlt.Col(i).Caption
            .ColFormat(i) = m_gdFlt.Col(i).Format
            .ColDataType(i) = m_gdFlt.Col(i).DataType
        Next

        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter

        m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatFltColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, aFont As New StdFont

    With fgFlt
        m_GridLock(GT_ORDERS_FILTER).LockRedraw

        aFont.Name = m_gdFlt.Font.Name
        aFont.Size = m_gdFlt.Font.Size
        aFont.Bold = m_gdFlt.Font.Bold
        aFont.Italic = m_gdFlt.Font.Italic
        aFont.Strikethrough = m_gdFlt.Font.Strikethrough
        aFont.Underline = m_gdFlt.Font.Underline

        Set .Font = aFont

        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = OFC_FIRST_COLUMN To OFC_LAST_COLUMN
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

        m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
    End With
End Sub

Private Sub FormatOrdGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        nCols = 0
        For i = 0 To OLC_LAST_COLUMN
            If m_gdOrd.Idx(i) <> OLC_NONE Then
                nCols = nCols + 1
            Else
                Exit For
            End If
        Next

        If nCols = 0 Then
            m_gdOrd.Idx(0) = OLC_UNDERLYING
            nCols = 1
        End If

        nCols = nCols + 1 ' for key column
        .Cols = nCols

        .ColDataType(OLC_KEY) = flexDTString
        .ColHidden(OLC_KEY) = True
        .ColKey(OLC_KEY) = OLC_NONE

        For i = 0 To OLC_LAST_COLUMN
            nIdx = m_gdOrd.Idx(i)
            If nIdx <> OLC_NONE Then
                .ColDataType(i + 1) = m_gdOrd.Col(nIdx).DataType
                .ColFormat(i + 1) = m_gdOrd.Col(nIdx).Format
                .ColKey(i + 1) = nIdx
                .TextMatrix(0, i + 1) = m_gdOrd.Col(nIdx).Caption
                .ColWidth(i + 1) = IIf(m_gdOrd.Col(nIdx).Width > 0, m_gdOrd.Col(nIdx).Width, -1)
            Else
                Exit For
            End If
        Next

        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter
        geOrd.AdjustSortImages

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub FormatOrdColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont
    Dim aRowData As clsOvRowData

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        aFont.Name = m_gdOrd.Font.Name
        aFont.Size = m_gdOrd.Font.Size
        aFont.Bold = m_gdOrd.Font.Bold
        aFont.Italic = m_gdOrd.Font.Italic
        aFont.Strikethrough = m_gdOrd.Font.Strikethrough
        aFont.Underline = m_gdOrd.Font.Underline

        Set .Font = aFont

        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> OLC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdOrd.Col(nIdx).BackColor <> 0, m_gdOrd.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdOrd.Col(nIdx).ForeColor <> 0, m_gdOrd.Col(nIdx).ForeColor, RGB(1, 1, 1))

                .ColAlignment(i) = IIf(nIdx = OLC_BS Or nIdx = OLC_ORDER_STATUS _
                                        Or nIdx = OLC_ORDER_TYPE Or nIdx = OLC_EXEC_STATUS _
                                        Or nIdx = OLC_OPT_TYPE, _
                                        flexAlignCenterCenter, flexAlignGeneral)
            End If
        Next

        For i = 1 To nRows
            Set aRowData = .RowData(i)
            If aRowData.Trade Is Nothing Then
                .Cell(flexcpFontBold, i, 0, i, nCols) = True
            End If
        Next

        If m_gdOrd.HorizLinesVisible Then
            If m_gdOrd.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdOrd.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdOrd.GridLinesColor

        .BackColorSel = m_gdOrd.BackColorSel
        .ForeColorSel = m_gdOrd.ForeColorSel

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub FormatLogGrid()
    On Error Resume Next
    Dim i&, nCols&, nIdx&

    With fgLog
        m_GridLock(GT_ORDERS_LOG).LockRedraw

        nCols = OEC_COLUMN_COUNT
        .Cols = nCols

        For i = 0 To OEC_LAST_COLUMN
            nIdx = m_gdLog.Idx(i)
            If nIdx <> OEC_NONE Then
                .TextMatrix(0, i) = m_gdLog.Col(i).Caption
                .ColFormat(i) = m_gdLog.Col(i).Format
                .ColDataType(i) = m_gdLog.Col(i).DataType
                .ColKey(i) = nIdx

                If nIdx = OEC_NUM Then
                    .ColAlignment(i) = flexAlignCenterCenter
                End If
            Else
                Exit For
            End If
        Next

        .Cell(flexcpAlignment, 0, 0, 0, nCols - 1) = flexAlignCenterCenter

        m_GridLock(GT_ORDERS_LOG).UnlockRedraw
    End With
End Sub

Private Sub FormatLogColumns()
    On Error Resume Next
    Dim i&, nCols&, nRows&, nIdx&, aFont As New StdFont, nCol&, nBaseHeight&, nMsgRows&

    With fgLog
        m_GridLock(GT_ORDERS_LOG).LockRedraw

        aFont.Name = m_gdLog.Font.Name
        aFont.Size = m_gdLog.Font.Size
        aFont.Bold = m_gdLog.Font.Bold
        aFont.Italic = m_gdLog.Font.Italic
        aFont.Strikethrough = m_gdLog.Font.Strikethrough
        aFont.Underline = m_gdLog.Font.Underline

        Set .Font = aFont

        nCols = .Cols - 1
        nRows = .Rows - 1
        For i = 0 To nCols
            nIdx = .ColKey(i)
            If nIdx <> OEC_NONE Then
                .Cell(flexcpBackColor, 1, i, nRows, i) = IIf(m_gdLog.Col(nIdx).BackColor <> 0, m_gdLog.Col(nIdx).BackColor, RGB(1, 1, 1))
                .Cell(flexcpForeColor, 1, i, nRows, i) = IIf(m_gdLog.Col(nIdx).ForeColor <> 0, m_gdLog.Col(nIdx).ForeColor, RGB(1, 1, 1))
            End If
        Next

        If m_gdLog.HorizLinesVisible Then
            If m_gdLog.VertLinesVisible Then
                .GridLines = flexGridFlat
            Else
                .GridLines = flexGridFlatHorz
            End If
        ElseIf m_gdLog.VertLinesVisible Then
            .GridLines = flexGridFlatVert
        Else
            .GridLines = flexGridNone
        End If
        .GridColor = m_gdLog.GridLinesColor

        .BackColorSel = m_gdLog.BackColorSel
        .ForeColorSel = m_gdLog.ForeColorSel

        ' adjust heights for multiline rows
        nCol = .ColIndex(OEC_MESSAGE)
        If nCol >= 0 And .Rows > 1 Then
            nBaseHeight = .RowHeight(0)

            For i = 1 To .Rows - 1
                nMsgRows = InStrCount(.TextMatrix(i, nCol), vbCrLf)
                If nMsgRows > 0 Then
                    .RowHeight(i) = nBaseHeight * (nMsgRows + 1)
                End If
            Next
        End If

        m_GridLock(GT_ORDERS_LOG).UnlockRedraw
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
                m_nFilter(Col) = nValue

                AdjustCaption
                StoreExpandedRowsInfo
                OrdersShow False

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
        m_enMenuGrid = GT_ORDERS_FILTER
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
        m_bKeyDown(GT_ORDERS_FILTER) = True
    End If
End Sub

Private Sub fgFlt_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_ORDERS_FILTER) Then
        m_bKeyDown(GT_ORDERS_FILTER) = False
        With fgFlt
            m_enMenuGrid = GT_ORDERS_FILTER
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With

        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                'mnuCtxTradeNew_Click

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
            m_enMenuGrid = GT_ORDERS_FILTER
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
        If Not m_bInProc And Not m_bShutDown And Col >= OFC_FIRST_COLUMN And Col <= OFC_LAST_COLUMN Then
            Cancel = Not m_gdFlt.Col(Col).CanEdit
            If Not Cancel Then
                m_sCurrentOriginalText = Trim$(.TextMatrix(Row, Col))
            End If
        End If
    End With
End Sub

Private Sub fgLog_DblClick()
    On Error Resume Next

    With fgLog
        m_enMenuGrid = GT_ORDERS_LIST
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows

        HandleGridDblClick False
    End With
End Sub

Private Sub fgLog_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridCommonOrOrderCard(KeyCode, Shift) Then
        m_bKeyDown(GT_ORDERS_LOG) = True
    End If
End Sub

Private Sub fgLog_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_ORDERS_LOG) Then
        m_bKeyDown(GT_ORDERS_LOG) = False
        With fgLog
            m_enMenuGrid = GT_ORDERS_LOG
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With

        Select Case True
            Case KeyCode = vbKeyInsert And Shift = vbAltMask
                mnuCtxOrderNewStock_Click

            Case KeyCode = vbKeyInsert And Shift = vbCtrlMask + vbAltMask
                mnuCtxOrderNewOption_Click

            Case (KeyCode = vbKeyInsert Or KeyCode = vbKeyC) And Shift = vbCtrlMask
                mnuCtxCopy_Click

            Case KeyCode = vbKeyInsert And Shift = vbShiftMask + vbAltMask
                mnuCtxTntCardNew_Click

            Case KeyCode = vbKeyG And Shift = vbCtrlMask
                mnuCtxAutosizeGrid_Click

            Case KeyCode = vbKeyA And Shift = vbCtrlMask
                mnuCtxAutosizeCol_Click

        End Select
    End If
End Sub

Private Sub fgLog_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgLog
            m_enMenuGrid = GT_ORDERS_LOG
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
            ShowPopup
        End With
    End If
End Sub

Private Sub fgOrd_AfterMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nIdx&, i&, nOldPos&, nNewPos&
    If Col = Position Then Exit Sub

    nOldPos = Col - OLC_FIRST_COLUMN
    nNewPos = Position - OLC_FIRST_COLUMN
    nIdx = m_gdOrd.Idx(nOldPos)
    If nOldPos > nNewPos Then
        For i = nOldPos To nNewPos Step -1
            If i > nNewPos Then
                m_gdOrd.Idx(i) = m_gdOrd.Idx(i - 1)
            End If
        Next
        m_gdOrd.Idx(nNewPos) = nIdx
    Else
        For i = nOldPos To nNewPos
            If i < nNewPos Then
                m_gdOrd.Idx(i) = m_gdOrd.Idx(i + 1)
            End If
        Next
        m_gdOrd.Idx(nNewPos) = nIdx
    End If
End Sub

Private Sub fgOrd_AfterUserResize(ByVal Row As Long, ByVal Col As Long)
    On Error Resume Next
    Dim nIdx&

    If Col >= 0 And Row < 0 Then
        With fgOrd
            nIdx = .ColKey(Col)
            If nIdx >= OLC_FIRST_COLUMN And nIdx <= OLC_LAST_COLUMN Then
                m_gdOrd.Col(nIdx).Width = IIf(.ColWidth(Col) > 0, .ColWidth(Col), -1)
            End If
        End With
    End If
End Sub

Private Sub fgOrd_BeforeMoveColumn(ByVal Col As Long, Position As Long)
    On Error Resume Next
    Dim nOldIdx&, nNewIdx&

    nOldIdx = fgOrd.ColKey(Col)
    nNewIdx = fgOrd.ColKey(Position)

    If nOldIdx = OLC_NONE Or nNewIdx = OLC_NONE Then
        Position = Col
    ElseIf Not m_gdOrd.Col(nOldIdx).CanChangeOrder Or Not m_gdOrd.Col(nNewIdx).CanChangeOrder Then
        Position = Col
    End If
End Sub

Private Sub fgOrd_DblClick()
    On Error Resume Next
    With fgOrd
        m_enMenuGrid = GT_ORDERS_LIST
        m_nMenuGridCol = .MouseCol
        m_nMenuGridRow = .MouseRow
        m_nMenuGridCols = .Cols
        m_nMenuGridRows = .Rows

        HandleGridDblClick True
    End With
End Sub

Private Sub fgOrd_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If IsHotKeyGridCommonOrTradeOrderCard(KeyCode, Shift) _
        Or Shift = 0 And (KeyCode = vbKeyReturn Or KeyCode = vbKeyDelete Or KeyCode = vbKeyF5) Then
        m_bKeyDown(GT_ORDERS_LIST) = True
    End If
End Sub

Private Sub fgOrd_KeyPress(KeyAscii As Integer)
    On Error Resume Next
    With fgOrd
        'If .Col = 1 And .RowOutlineLevel(.Row) = OOL_ORD Then
        If .RowOutlineLevel(.Row) = OOL_ORD Then
            If KeyAscii = Asc("+") And .IsCollapsed(.Row) = flexOutlineCollapsed Then
                .IsCollapsed(.Row) = flexOutlineExpanded
                .SetFocus
            ElseIf KeyAscii = Asc("-") And .IsCollapsed(.Row) <> flexOutlineCollapsed Then
                .IsCollapsed(.Row) = flexOutlineCollapsed
                .SetFocus
            End If
        End If
    End With
End Sub

Private Sub fgOrd_KeyUp(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If m_bKeyDown(GT_ORDERS_LIST) Then
        m_bKeyDown(GT_ORDERS_LIST) = False
        With fgOrd
            m_enMenuGrid = GT_ORDERS_LIST
            m_nMenuGridCol = .Col
            m_nMenuGridRow = .Row
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows
        End With

        Select Case True
            Case KeyCode = vbKeyInsert And Shift = 0
                'mnuCtxTradeNew_Click

            Case KeyCode = vbKeyReturn And Shift = 0
                mnuCtxOrderCancelReplace_Click

            Case KeyCode = vbKeyF5 And Shift = 0
                mnuCtxOrderStatus_Click

            Case KeyCode = vbKeyDelete And Shift = 0
                mnuCtxOrderCancel_Click

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

Private Sub fgOrd_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)
    On Error Resume Next
    If Button = vbRightButton Then
        With fgOrd
            m_enMenuGrid = GT_ORDERS_LIST
            m_nMenuGridCol = .MouseCol
            m_nMenuGridRow = .MouseRow
            m_nMenuGridCols = .Cols
            m_nMenuGridRows = .Rows

            If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
                And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then
                .Row = m_nMenuGridRow
                .Col = m_nMenuGridCol
            End If
            ShowPopup
        End With
    End If
End Sub

Private Sub ShowPopup()
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom

    'mnuCtxOrderNewStock        "New Stock Order..."
    'mnuCtxOrderNewOption       "New Option Order..."
    'mnuCtxOrderCancel          "Cancel Order..."
    'mnuCtxOrderCancelReplace   "Cancel/Replace Order..."
    'mnuCtxOrderStatus          "Request Order Status"
    'mnuCtxDefOrdSort           "Default Orders Sorting"
    'mnuCtxCopy                 "Copy Grid"
    'mnuCtxPrint                "Print Grid..."
    'mnuCtxHideCol              "Hide Column"
    'mnuCtxAutosizeCol          "Autosize Column"
    'mnuCtxAutosizeGrid         "Autosize Grid"
    'mnuCtxGridLayout           "Grid Layout..."
    'mnuCtxClearLog             "Clear"


    mnuCtxOrderStatus.Enabled = Not m_bInProc
    mnuCtxCopy.Enabled = Not m_bInProc
    mnuCtxDefOrdSort.Enabled = CanSortOrdersByDefault()
    mnuCtxOrderNewStock.Enabled = Not m_bInProc
    mnuCtxOrderNewOption.Enabled = Not m_bInProc
    mnuCtxOrderCancelReplace.Enabled = Not m_bInProc
    mnuCtxTntCardNew.Enabled = Not m_bInProc

    Select Case m_enMenuGrid
        Case GT_ORDERS_FILTER
            mnuCtxOrderCancel.Enabled = False
            mnuCtxOrderCancelReplace.Enabled = False
            mnuCtxOrderStatus.Enabled = False

            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = False
            mnuCtxAutosizeGrid.Enabled = False
            mnuCtxClearLog.Enabled = False

            PopupMenu mnuCtx, , , , mnuCtxOrderCancelReplace

        Case GT_ORDERS_LIST
            With fgOrd
                If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then
                    Set aOrd = .RowData(m_nMenuGridRow).Ord
                End If

                If Not m_bInProc And (Not aOrd Is Nothing Or m_nFilter(OFC_UNDERLYING) <> 0) _
                                            And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols _
                                            And m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then

                    mnuCtxOrderNewStock.Enabled = True
                    mnuCtxOrderNewOption.Enabled = True
                    If .Rows > 1 Then
                        mnuCtxOrderCancel.Enabled = Not .RowHidden(m_nMenuGridRow)
                        mnuCtxOrderCancelReplace.Enabled = mnuCtxOrderCancel.Enabled
                    Else
                        mnuCtxOrderCancelReplace.Enabled = False
                        mnuCtxOrderCancel.Enabled = False
                    End If
                Else
                    mnuCtxOrderNewStock.Enabled = False
                    mnuCtxOrderNewOption.Enabled = False
                    mnuCtxOrderCancelReplace.Enabled = False
                    mnuCtxOrderCancel.Enabled = False
                End If

                mnuCtxOrderStatus.Enabled = Not aOrd Is Nothing

                mnuCtxHideCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols And m_gdOrd.Idx(1) <> OLC_NONE)
                mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols)
                mnuCtxAutosizeGrid.Enabled = True
                mnuCtxClearLog.Enabled = False

                Set aOrd = Nothing

                PopupMenu mnuCtx, , , , mnuCtxOrderCancelReplace
            End With

        Case GT_ORDERS_LOG
            mnuCtxOrderCancel.Enabled = False
            mnuCtxOrderCancelReplace.Enabled = False
            mnuCtxOrderStatus.Enabled = False

            mnuCtxHideCol.Enabled = False
            mnuCtxAutosizeCol.Enabled = (m_nMenuGridCol >= 0 And m_nMenuGridCol < m_nMenuGridCols)
            mnuCtxAutosizeGrid.Enabled = True
            mnuCtxClearLog.Enabled = True

            PopupMenu mnuCtx, , , , mnuCtxOrderCancelReplace

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
        Case GT_ORDERS_FILTER
            gdGrid.CopyTo m_gdFlt

            m_GridLock(GT_ORDERS_FILTER).LockRedraw
            FormatFltGrid
            FormatFltColumns
            m_GridLock(GT_ORDERS_FILTER).UnlockRedraw

        Case GT_ORDERS_LIST
            gdGrid.CopyTo m_gdOrd

            m_GridLock(GT_ORDERS_LIST).LockRedraw
            FormatOrdGrid
            OrdersUpdate True
            FormatOrdColumns
            OrdersUpdateTradeColors
            m_GridLock(GT_ORDERS_LIST).UnlockRedraw

            AdjustState

        Case GT_ORDERS_LOG
            gdGrid.CopyTo m_gdLog

            m_GridLock(GT_ORDERS_LOG).LockRedraw
            FormatLogGrid
            FormatLogColumns
            m_GridLock(GT_ORDERS_LOG).UnlockRedraw

    End Select

    UserControl_Resize
End Sub

Private Sub geOrd_AfterSort(ByVal Col As Long, Order As Integer)
    On Error Resume Next
    Dim bAsc As Boolean

    With fgOrd
        Select Case Order
            Case flexSortStringNoCaseAscending, flexSortStringAscending, flexSortNumericAscending, flexSortGenericAscending
                bAsc = True
            Case Else
                bAsc = False
        End Select

        m_GridLock(GT_ORDERS_LIST).LockRedraw
        SortOrdNodes Col, bAsc
        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub mnuCtxAutosizeCol_Click()
    On Error Resume Next
    Dim nIdx&

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_ORDERS_LIST
            With fgOrd
                m_GridLock(GT_ORDERS_LIST).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol

                nIdx = .ColKey(m_nMenuGridCol)
                If nIdx >= OLC_FIRST_COLUMN And nIdx <= OLC_LAST_COLUMN Then
                    m_gdOrd.Col(nIdx).Width = IIf(.ColWidth(m_nMenuGridCol) > 0, .ColWidth(m_nMenuGridCol), -1)
                End If

                m_GridLock(GT_ORDERS_LIST).UnlockRedraw
            End With

        Case GT_ORDERS_LOG
            With fgLog
                m_GridLock(GT_ORDERS_LOG).LockRedraw
                .AutoSize m_nMenuGridCol, m_nMenuGridCol
                m_GridLock(GT_ORDERS_LOG).UnlockRedraw
            End With

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxAutosizeGrid_Click()
    On Error Resume Next
    Dim nIdx&, i&, nCount&

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_ORDERS_LIST
            With fgOrd
                m_GridLock(GT_ORDERS_LIST).LockRedraw
                .AutoSize 0, .Cols - 1

                nCount = .Cols - 1
                For i = 0 To nCount
                    nIdx = .ColKey(i)
                    If nIdx >= OLC_FIRST_COLUMN And nIdx <= OLC_LAST_COLUMN Then
                        m_gdOrd.Col(nIdx).Width = IIf(.ColWidth(i) > 0, .ColWidth(i), -1)
                    End If
                Next

                m_GridLock(GT_ORDERS_LIST).UnlockRedraw
            End With

        Case GT_ORDERS_LOG
            With fgLog
                m_GridLock(GT_ORDERS_LOG).LockRedraw
                .AutoSize 0, .Cols - 1
                m_GridLock(GT_ORDERS_LOG).UnlockRedraw
            End With

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxClearLog_Click()
    On Error Resume Next
    Screen.MousePointer = vbHourglass

    Select Case m_enMenuGrid
        Case GT_ORDERS_LOG
            ClearLog

    End Select

    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxCopy_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub

    Screen.MousePointer = vbHourglass
    Select Case m_enMenuGrid
        Case GT_ORDERS_FILTER
            g_ClipMgr.CopyGridToClipboard fgFlt

        Case GT_ORDERS_LIST
            g_ClipMgr.CopyGridToClipboard fgOrd

        Case GT_ORDERS_LOG
            g_ClipMgr.CopyGridToClipboard fgLog

    End Select
    Screen.MousePointer = vbDefault
End Sub

Private Sub mnuCtxDefOrdSort_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    SortOrdersByDefault
End Sub

Public Sub SortOrdersByDefault()
    On Error Resume Next
    Dim i&, nRow&, nCol&

    With fgOrd
        i = -1
        i = .ColIndex(OLC_ORDER_DATE)
        If i >= 0 Then
            m_GridLock(GT_ORDERS_LIST).LockRedraw
            Screen.MousePointer = vbHourglass

            SortOrdNodes i, False
            geOrd.ShowSortImage i, -1

            m_GridLock(GT_ORDERS_LIST).UnlockRedraw
            Screen.MousePointer = vbDefault
        End If

    End With

End Sub

Public Property Get CanSortOrdersByDefault() As Boolean
    On Error Resume Next
    CanSortOrdersByDefault = Not m_bInProc And fgOrd.ColIndex(OLC_ORDER_DATE) >= 0
End Property

Private Sub mnuCtxOrderStatus_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, nRow&

    If m_nMenuGridRow < 0 Or m_nMenuGridRow >= m_nMenuGridRows _
        Or m_nMenuGridCol >= m_nMenuGridCols Then Exit Sub

    If m_enMenuGrid = GT_ORDERS_LIST Then

        m_bInProc = True
        Set aOrd = fgOrd.RowData(m_nMenuGridRow).Ord
        m_bInProc = False

        If Not aOrd Is Nothing Then
            On Error GoTo EH
            OrdersProcessor.OrderStatusRequest aOrd
            Set aOrd = Nothing
        End If
    End If

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Order View: Fail to request order status."
    ResetMenuData
    Set aOrd = Nothing
End Sub

Private Sub mnuCtxOrderCancel_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, nRow&

    If m_nMenuGridRow < 0 Or m_nMenuGridRow >= m_nMenuGridRows _
        Or m_nMenuGridCol >= m_nMenuGridCols Then Exit Sub

    If m_enMenuGrid = GT_ORDERS_LIST Then

        m_bInProc = True
        Set aOrd = fgOrd.RowData(m_nMenuGridRow).Ord
        m_bInProc = False

        If Not aOrd Is Nothing Then
            If gCmn.MyMsgBox(m_frmOwner, "Are you sure that you want to cancel " & _
                                IIf(aOrd.IsBuy, "Buy ", "Sell ") & aOrd.OrderQty & " " & aOrd.Symbol & " at " & _
                                IIf(aOrd.OrderType = enOrtMarket, "market price", "$" & aOrd.OrderPrice) & "?", _
                                vbYesNo + vbQuestion, "Confirm Cancel of " & aOrd.OrderID) = vbYes Then
                On Error GoTo EH
                OrdersProcessor.OrderCancel aOrd
                Set aOrd = Nothing
            End If
        End If
    End If

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Order View: Fail to cancel order."
    ResetMenuData
    Set aOrd = Nothing
End Sub

Private Sub mnuCtxOrderCancelReplace_Click()
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom

    If m_enMenuGrid = GT_ORDERS_LIST Then
        If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows Then

            m_bInProc = True
            Set aOrd = fgOrd.RowData(m_nMenuGridRow).Ord
            m_bInProc = False

            If Not aOrd Is Nothing Then
                On Error GoTo EH
                frmOrderEntry.AlterOrder aOrd
                Set aOrd = Nothing
            End If
        End If
    End If

    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Fail to cancel/replace order."
    ResetMenuData
    Set aOrd = Nothing
End Sub

Private Sub mnuCtxGridLayout_Click()
    On Error Resume Next
    Select Case m_enMenuGrid
        Case GT_ORDERS_FILTER
            CustomizeFltGridLayout

        Case GT_ORDERS_LIST
            CustomizeOrdGridLayout

        Case GT_ORDERS_LOG
            CustomizeLogGridLayout

    End Select
End Sub

Private Sub mnuCtxHideCol_Click()
    On Error Resume Next
    On Error Resume Next
    If m_nMenuGridCol < 0 Or m_nMenuGridRow < 0 Then Exit Sub

    Dim i&, nColIdx&, bMove As Boolean

    If m_enMenuGrid = GT_ORDERS_LIST Then
        bMove = False

        If m_gdOrd.Idx(1) = OLC_NONE Then Exit Sub

        nColIdx = fgOrd.ColKey(m_nMenuGridCol)
        For i = 0 To OLC_LAST_COLUMN
            If m_gdOrd.Idx(i) = nColIdx Then
                m_gdOrd.Idx(i) = OLC_NONE
                m_gdOrd.Col(nColIdx).Visible = False
                bMove = True
            End If

            If bMove Then
                If i + 1 <= OLC_LAST_COLUMN Then
                    m_gdOrd.Idx(i) = m_gdOrd.Idx(i + 1)
                Else
                    m_gdOrd.Idx(i) = OLC_NONE
                End If
            End If
        Next

        If bMove Then
            FormatOrdGrid
            OrdersUpdate True
            FormatOrdColumns
            OrdersUpdateTradeColors
            AdjustState
        End If

    End If
End Sub

Private Sub OrdersUpdate(ByVal bSymbol As Boolean)
    On Error Resume Next
    Dim i&, aRowData As clsOvRowData
    Dim aTrd As EtsGeneralLib.MmTradeInfoAtom, aOrd As EtsMmGeneralLib.MmOrderAtom

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        For i = 1 To .Rows - 1
            Set aRowData = .RowData(i)
            If Not aRowData Is Nothing Then

                Set aOrd = aRowData.Ord
                Set aTrd = aRowData.Trade

                If aTrd Is Nothing Then
                    OrderUpdate i, aOrd, bSymbol
                Else
                    OrderTradeUpdate i, aTrd
                End If

                Set aOrd = Nothing
                Set aTrd = Nothing
                Set aRowData = Nothing
            End If
        Next

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub FillDataForOrderFromCurrentSelection(ByVal bIsStock As Boolean, _
                                        ByRef aUnd As EtsGeneralLib.UndAtom, _
                                        ByRef aOpt As EtsGeneralLib.EtsOptAtom, _
                                        ByRef bBuy As Boolean, ByRef dPrice#, ByRef nQty&)
    On Error Resume Next
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom

    bBuy = True
    dPrice = 0#
    nQty = IIf(bIsStock, 100, 10)

    If m_enMenuGrid = GT_ORDERS_LIST Then

        With fgOrd
            Set aOrd = .RowData(m_nMenuGridRow).Ord
            If Not aOrd Is Nothing Then
                Set aUnd = aOrd.Und

                If Not bIsStock And Not aOrd.Opt Is Nothing Then
                    Set aOpt = New EtsGeneralLib.EtsOptAtom
                    aOpt.ID = aOrd.ContractID
                    aOpt.OptType = aOrd.Opt.OptType
                    aOpt.RootID = aOrd.Opt.RootID
                    aOpt.Symbol = aOrd.Opt.Symbol
                    aOpt.Expiry = aOrd.Opt.Expiry
                    aOpt.Strike = aOrd.Opt.Strike
                End If

                If bIsStock = (aOrd.Opt Is Nothing) Then
                    nQty = aOrd.OrderQty
                    bBuy = aOrd.IsBuy
                    dPrice = aOrd.OrderPrice
                End If

                Set aOrd = Nothing
            End If
        End With
    End If

    If aUnd Is Nothing Then
        Set aUnd = g_Underlying(m_nFilter(OFC_UNDERLYING))
        bBuy = True
    End If
End Sub

Private Sub OrderNew(ByVal bIsStock As Boolean)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim bBuy As Boolean, aUnd As EtsGeneralLib.UndAtom
    Dim dPrice#, nQty&, aOpt As EtsGeneralLib.EtsOptAtom

    m_bInProc = True
    FillDataForOrderFromCurrentSelection bIsStock, aUnd, aOpt, bBuy, dPrice, nQty
    m_bInProc = False

    On Error GoTo EH
    If bIsStock Then
        frmOrderEntry.NewStockOrder aUnd, bBuy, nQty, dPrice
    Else
        frmOrderEntry.NewOptionOrder aUnd, aOpt, bBuy, nQty, dPrice
    End If

    Set aUnd = Nothing
    Set aOpt = Nothing
    Exit Sub
EH:
    m_bInProc = False
    gCmn.ErrorMsgBox m_frmOwner, "Order View: Fail to create new order."
    ResetMenuData
    Set aUnd = Nothing
    Set aOpt = Nothing
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
    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim bBuy As Boolean, aUnd As etsgenerallib.UndAtom
'    Dim dPrice#, nQty&
'    Dim aOpt As etsgenerallib.EtsOptAtom
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
'
'    Exit Sub
'EH:
'    m_bInProc = False
'    gCmn.ErrorMsgBox m_frmOwner, "Orders View: Fail to create new trade message."
'    ResetMenuData
'    Set aUnd = Nothing
'    Set aOpt = Nothing
End Sub

Private Sub mnuCtxPrint_Click()
    On Error Resume Next
'    Screen.MousePointer = vbHourglass
'    Select Case m_enMenuGrid
'        Case GT_ORDERS_FILTER
'            frmPrintPreview.Execute m_frmOwner, "Orders Filter", "Orders Filter", fgFlt
'
'        Case GT_ORDERS_LIST
'            frmPrintPreview.Execute m_frmOwner, "Orders List", "Orders List", fgOrd
'
'    End Select
'    Screen.MousePointer = vbDefault
End Sub

Private Sub OrdersProcessor_OnNewTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, aTrdEx As EtsGeneralLib.MmTradeInfoAtom

    m_bInProc = True

    Set aOrd = OrdersProcessor.Orders(aTrd.OrderID)

    If Not aOrd Is Nothing Then ' current trader order
        With fgOrd
            m_GridLock(GT_ORDERS_LIST).LockRedraw
    
            nRow = .FindRow(CStr(aTrd.OrderID) & "_" & CStr(aTrd.TradeID), 1, OLC_KEY, , True)
            nOrdRow = .FindRow(CStr(aTrd.OrderID), 1, OLC_KEY, , True)
            If nRow < 0 And nOrdRow > 0 Then
    
                OrderUpdate nOrdRow, aOrd, True
    
                nRow = nOrdRow + 1
                .AddItem "", nRow
    
                Set aRowData = New clsOvRowData
                Set aRowData.Ord = aOrd
                Set aRowData.Trade = aTrd
    
                .RowData(nRow) = aRowData
                .TextMatrix(nRow, OLC_KEY) = CStr(aTrd.OrderID) & "_" & CStr(aTrd.TradeID)
    
                .IsSubtotal(nRow) = False
                .RowOutlineLevel(nRow) = OOL_EXEC
    
                OrderTradeUpdate nRow, aTrd, True
    
                bHide = Not CheckOrderFilter(aOrd)
                .RowHidden(nRow) = bHide Or .IsCollapsed(nOrdRow)
    
                If bHide Then
                    If Not .RowHidden(nOrdRow) Then
                        .RowHidden(nOrdRow) = True
                        m_nOrders = m_nOrders - 1
                        .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                    End If
                Else
                    If .RowHidden(nOrdRow) Then
                        .RowHidden(nOrdRow) = False
                        m_nOrders = m_nOrders + 1
                    End If
                End If
    
                UpdateOrdersCountLabel
            Else
                If nRow > 0 And nOrdRow > 0 Then
                    OrderUpdate nOrdRow, aOrd, True
                    OrderTradeUpdate nRow, aTrd
        
                    If CheckOrderFilter(aOrd) Then
                        If .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = False
                            m_nOrders = m_nOrders + 1
                        End If
                    Else
                        If Not .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = True
                            m_nOrders = m_nOrders - 1
                            .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                        End If
                    End If
        
                    UpdateOrdersCountLabel
                End If
            End If
    
            Set aRowData = Nothing
    
            m_GridLock(GT_ORDERS_LIST).UnlockRedraw
        End With
        Set aOrd = Nothing
    End If
    
    m_bInProc = False
End Sub

Private Sub tmrShow_Timer()
    On Error Resume Next
    tmrShow.Enabled = False
    StoreExpandedRowsInfo
    OrdersShow True
    m_ExpandedOrd.Clear
End Sub

Private Sub OrdersProcessor_OnNewOrder(ByRef aOrd As EtsMmGeneralLib.MmOrderAtom)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, aTrd As EtsGeneralLib.MmTradeInfoAtom, bHide As Boolean

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
        If nOrdRow < 0 Then
            nOrdRow = 1
            .AddItem "", nOrdRow

            Set aRowData = New clsOvRowData
            Set aRowData.Ord = aOrd

            .RowData(nOrdRow) = aRowData
            .TextMatrix(nOrdRow, OLC_KEY) = CStr(aOrd.ID)

            .IsSubtotal(nOrdRow) = True
            .RowOutlineLevel(nOrdRow) = OOL_ORD

            OrderUpdate nOrdRow, aOrd, True, True

            nRow = nOrdRow
            For Each aTrd In aOrd.Trades
                nRow = nRow + 1
                .AddItem ""

                Set aRowData = New clsOvRowData
                Set aRowData.Ord = aOrd
                Set aRowData.Trade = aTrd

                .RowData(nRow) = aRowData
                .TextMatrix(nRow, OLC_KEY) = CStr(aOrd.ID) & "_" & CStr(aTrd.TradeID) 'aExec.ID

                .IsSubtotal(nRow) = False
                .RowOutlineLevel(nRow) = OOL_EXEC

                OrderTradeUpdate nRow, aTrd, True
            Next

            bHide = Not CheckOrderFilter(aOrd)
            .RowHidden(nOrdRow) = bHide

            If bHide Then
                .IsCollapsed(nOrdRow) = flexOutlineCollapsed
            Else
                m_nOrders = m_nOrders + 1
                UpdateOrdersCountLabel
            End If

            .Cell(flexcpFontBold, nOrdRow, 0, nOrdRow, .Cols - 1) = True
        End If

        Set aRowData = Nothing

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub OrdersProcessor_OnUpdateOrder(aOrd As EtsMmGeneralLib.MmOrderAtom)
    On Error Resume Next
    If m_bInProc Then Exit Sub
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, aExec As EtsMmGeneralLib.MmOrderExecAtom

    With fgOrd
        m_GridLock(GT_ORDERS_LIST).LockRedraw

        nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
        If nOrdRow > 0 Then
            OrderUpdate nOrdRow, aOrd, True

            If CheckOrderFilter(aOrd) Then
                If .RowHidden(nOrdRow) Then
                    .RowHidden(nOrdRow) = False
                    m_nOrders = m_nOrders + 1
                End If
            Else
                If Not .RowHidden(nOrdRow) Then
                    .RowHidden(nOrdRow) = True
                    m_nOrders = m_nOrders - 1
                    .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                End If
            End If

            UpdateOrdersCountLabel
        End If

        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
    End With
End Sub

Private Sub OrdersProcessor_OnNewOrderExec(aOrd As EtsMmGeneralLib.MmOrderAtom, aOrdExec As EtsMmGeneralLib.MmOrderExecAtom)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
'
'    With fgOrd
'        m_GridLock(GT_ORDERS_LIST).LockRedraw
'
'        nRow = .FindRow(CStr(aOrd.ID) & "_" & CStr(aOrdExec.ID), 1, OLC_KEY, , True)
'        nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
'        If nRow < 0 And nOrdRow > 0 Then
'
'            OrderUpdate nOrdRow, aOrd, True
'
'            nRow = nOrdRow + 1
'            .AddItem "", nRow
'
'            Set aRowData = New clsOvRowData
'            Set aRowData.Ord = aOrd
'            Set aRowData.Exec = aOrdExec
'
'            .RowData(nRow) = aRowData
'            .TextMatrix(nRow, OLC_KEY) = CStr(aOrd.ID) & "_" & CStr(aOrdExec.ID)
'
'            .IsSubtotal(nRow) = False
'            .RowOutlineLevel(nRow) = OOL_EXEC
'
'            OrderExecUpdate nRow, aOrdExec, True
'
'            bHide = Not CheckOrderFilter(aOrd)
'            .RowHidden(nRow) = bHide Or .IsCollapsed(nOrdRow)
'
'            If bHide Then
'                If Not .RowHidden(nOrdRow) Then
'                    .RowHidden(nOrdRow) = True
'                    m_nOrders = m_nOrders - 1
'                    .IsCollapsed(nOrdRow) = flexOutlineCollapsed
'                End If
'            Else
'                If .RowHidden(nOrdRow) Then
'                    .RowHidden(nOrdRow) = False
'                    m_nOrders = m_nOrders + 1
'                End If
'            End If
'
'            UpdateOrdersCountLabel
'        End If
'
'        Set aRowData = Nothing
'
'        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
'    End With
End Sub

'Private Sub OrdersProcessor_OnUpdateOrderExec(aOrd As EtsMmGeneralLib.MmOrderAtom, aOrdExec As EtsMmGeneralLib.MmOrderExecAtom)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
'
'    With fgOrd
'        m_GridLock(GT_ORDERS_LIST).LockRedraw
'
'        nRow = .FindRow(CStr(aOrd.ID) & "_" & CStr(aOrdExec.ID), 1, OLC_KEY, , True)
'        nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
'        If nRow > 0 And nOrdRow > 0 Then
'            OrderUpdate nOrdRow, aOrd, True
'            OrderExecUpdate nRow, aOrdExec
'
'            If CheckOrderFilter(aOrd) Then
'                If .RowHidden(nOrdRow) Then
'                    .RowHidden(nOrdRow) = False
'                    m_nOrders = m_nOrders + 1
'                End If
'            Else
'                If Not .RowHidden(nOrdRow) Then
'                    .RowHidden(nOrdRow) = True
'                    m_nOrders = m_nOrders - 1
'                    .IsCollapsed(nOrdRow) = flexOutlineCollapsed
'                End If
'            End If
'
'            UpdateOrdersCountLabel
'        End If
'
'        m_GridLock(GT_ORDERS_LIST).UnlockRedraw
'    End With
'End Sub

Private Sub OrdersProcessor_OnUpdateOrderExec(aOrd As EtsMmGeneralLib.MmOrderAtom, aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error GoTo EH
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
    
    If Not aOrd Is Nothing Then
        With fgOrd
            m_GridLock(GT_ORDERS_LIST).LockRedraw
    
            nRow = .FindRow(CStr(aOrd.ID) & "_" & CStr(aTrd.TradeID), 1, OLC_KEY, , True)
            'nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
            If nRow > 0 Then
                .RemoveItem nRow
            End If
                'OrderUpdate nOrdRow, aOrd, True
    
            m_GridLock(GT_ORDERS_LIST).UnlockRedraw
        End With
    End If
    
    Exit Sub
EH:
    LogEvent EVENT_ERROR, "Orders: Fail to delete trade. " & Err.Description
End Sub

Private Sub OrdersProcessor_OnClearEvents()
    On Error Resume Next
    m_GridLock(GT_ORDERS_LOG).LockRedraw
    fgLog.Rows = 1
    m_GridLock(GT_ORDERS_LOG).UnlockRedraw
End Sub

Private Sub OrdersProcessor_OnNewEvent(aEvent As EtsGeneralLib.EtsEventAtom)
    On Error Resume Next
    OrdersLogAddEvent aEvent, True
End Sub

'Private Sub TradeChannel_TradeAction(aNewTrdInfo As EtsGeneralLib.MmTradeInfoAtom, aOldTrdInfo As EtsGeneralLib.MmTradeInfoAtom, enAction As TradeActionEnum)
'    On Error Resume Next
'    If m_bInProc Then Exit Sub
'
'    If Not aOldTrdInfo Is Nothing Then _
'        If aOldTrdInfo.OrderID = 0 Then Exit Sub
'
'    If Not aNewTrdInfo Is Nothing Then _
'        If aNewTrdInfo.OrderID = 0 Then Exit Sub
'
'    If tmrUndCalc.Enabled = True Then
'        m_bIsNewTrades = True
'    Else
'        tmrUndCalc.Enabled = True
'    End If
'
'    If m_bLastQuoteReqNow Then
'
'        Debug.Assert (TradeQueue.Add(aNewTrdInfo, aOldTrdInfo, enAction)) Is Nothing
'        Exit Sub
'    End If
'
'    'If m_bShutDown Or m_bLastQuoteReqNow Or m_bSubscribingNow Then Exit Sub
'
'    Select Case enAction
'        Case enTaTradeNew
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            AddTrade aNewTrdInfo
'
'        Case enTaTradeUpdate
'            Debug.Assert Not aNewTrdInfo Is Nothing
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            DeleteTrade aOldTrdInfo
'            AddTrade aNewTrdInfo
'
'        Case enTaTradeDelete
'            Debug.Assert Not aOldTrdInfo Is Nothing
'            DeleteTrade aOldTrdInfo
'    End Select
'End Sub

Private Sub UserControl_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    Dim nSplitterTop&

    With fgFlt
        .Height = .RowHeight(0) + (.Rows - 1) * .RowHeight(1) + ScaleY(.GridLineWidth * 2, vbPixels, vbTwips)
        .Width = ScaleWidth
    End With

    With fgOrd
        .Top = fgFlt.Height + GRID_VERT_SPACE
        .Width = ScaleWidth
    End With

    If m_bLogVisible Then
        picLog.Width = ScaleWidth
        fgLog.Width = ScaleWidth
        lblLogCaption.Width = ScaleWidth

        With ctlSplit
            .SplitterMinTop = fgFlt.Height
            .SplitterMaxHeight = ScaleHeight - .SplitterHeight
            .SplitterTop = ScaleHeight - picLog.Height - ctlSplit.Height
            .Width = ScaleWidth
        End With
    Else
        With fgOrd
            .Height = ScaleHeight - .Top
        End With
    End If
End Sub

Private Sub ctlSplit_SplitterMoved()
    On Error Resume Next
    If m_bLogVisible Then
        Dim bMoveDown As Boolean
        If picLog.Top < ctlSplit.Top + ctlSplit.Height Then ' move down
            picLog.Top = ctlSplit.Top + ctlSplit.Height
            fgOrd.Height = ctlSplit.Top - fgOrd.Top
            picLog.Height = ScaleHeight - picLog.Top
            fgLog.Height = ScaleHeight - picLog.Top - fgLog.Top
        Else
            picLog.Height = ScaleHeight - ctlSplit.Top - ctlSplit.Height
            fgLog.Height = ScaleHeight - ctlSplit.Top - ctlSplit.Height - fgLog.Top
            fgOrd.Height = ctlSplit.Top - fgOrd.Top
            picLog.Top = ctlSplit.Top + ctlSplit.Height
        End If
    End If
End Sub

Private Sub UserControl_Terminate()
    On Error Resume Next
    Term
End Sub

Public Sub Term()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_bShutDown = True

    Set pbProgress = Nothing
    Set lblProcess = Nothing
    Set lblStatus = Nothing
    Set geOrd = Nothing

    Set frmLayout = Nothing
    Set OrdersProcessor = Nothing
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    RaiseEvent OnSetCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    Dim sCaption$

    sCaption = "Orders - " & fgFlt.Cell(flexcpTextDisplay, 1, OFC_ORDER_STATUS)

    GetCaption = sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = fgFlt.Cell(flexcpTextDisplay, 1, OFC_ORDER_STATUS)
End Function

Public Property Get InProc() As Boolean
    InProc = m_bInProc
End Property

Public Sub CustomizeFltGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_ORDERS_FILTER, OFC_FIRST_COLUMN, OFC_LAST_COLUMN, m_gdFlt, m_frmOwner
    End If
End Sub

Public Sub CustomizeOrdGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_ORDERS_LIST, OLC_FIRST_COLUMN, OLC_LAST_COLUMN, m_gdOrd, m_frmOwner
    End If
End Sub

Public Sub CustomizeLogGridLayout()
    On Error Resume Next
    If Not frmLayout Is Nothing Then
        frmLayout.Execute GT_ORDERS_LOG, OEC_FIRST_COLUMN, OEC_LAST_COLUMN, m_gdLog, m_frmOwner
    End If
End Sub

Public Sub SaveToFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    aStorage.SetLongValue "Coordinates" & sKey, "Left", m_frmOwner.Left
    aStorage.SetLongValue "Coordinates" & sKey, "Top", m_frmOwner.Top
    aStorage.SetLongValue "Coordinates" & sKey, "Width", m_frmOwner.Width
    aStorage.SetLongValue "Coordinates" & sKey, "Height", m_frmOwner.Height
    
    ' common info
    For i = OFC_FIRST_COLUMN To OFC_LAST_COLUMN
        aStorage.SetLongValue "OrderFlt" & sKey, "Filter" & CStr(i), m_nFilter(i)
    Next

    m_gdFlt.WriteToStorage "OrderFltGrid" & sKey, aStorage, False
    m_gdOrd.WriteToStorage "OrderOrdGrid" & sKey, aStorage
    m_gdLog.WriteToStorage "OrderLogGrid" & sKey, aStorage

    aStorage.SetLongValue "Order" & sKey, "LogVisible", IIf(m_bLogVisible, 1, 0)
    aStorage.SetLongValue "Order" & sKey, "SplitterTop", ctlSplit.SplitterTop
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Sub OpenFromFile(aStorage As clsSettingsStorage, ByVal sKey As String)
    On Error GoTo EH
    Dim i&
    If Len(sKey) > 0 Then sKey = "." & sKey
    
    m_frmOwner.Left = aStorage.GetLongValue("Coordinates" & sKey, "Left", m_frmOwner.Left)
    m_frmOwner.Top = aStorage.GetLongValue("Coordinates" & sKey, "Top", m_frmOwner.Top)
    m_frmOwner.Width = aStorage.GetLongValue("Coordinates" & sKey, "Width", m_frmOwner.Width)
    m_frmOwner.Height = aStorage.GetLongValue("Coordinates" & sKey, "Height", m_frmOwner.Height)
    
    ' common info
    For i = OFC_FIRST_COLUMN To OFC_LAST_COLUMN
        m_nFilter(i) = aStorage.GetLongValue("OrderFlt" & sKey, "Filter" & CStr(i), m_nFilter(i))
    Next

    If m_nFilter(OFC_CONTRACT_TYPE) < 0 Or m_nFilter(OFC_CONTRACT_TYPE) > 2 Then m_nFilter(OFC_CONTRACT_TYPE) = 0

    m_gdFlt.ReadFromStorage "OrderFltGrid" & sKey, aStorage, False
    m_gdOrd.ReadFromStorage "OrderOrdGrid" & sKey, aStorage
    m_gdLog.ReadFromStorage "OrderLogGrid" & sKey, aStorage

    FilterUpdate
    FormatFltGrid
    FormatFltColumns

    fgOrd.Rows = 1
    FormatOrdGrid
    FormatOrdColumns

    FormatLogGrid
    FormatLogColumns

    ctlSplit.SplitterTop = aStorage.GetLongValue("Order" & sKey, "SplitterTop", ctlSplit.SplitterTop)

    m_bLogVisible = (aStorage.GetLongValue("Order" & sKey, "LogVisible", 1) <> 0)
    AdjustLogVisible
    AdjustState

    If Not m_bLogVisible Then UserControl_Resize

    tmrShow.Enabled = True
    Exit Sub
EH:
    gCmn.ErrorHandler ""
End Sub

Public Function CurUnderlyingID() As Long
    On Error Resume Next
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom

    CurUnderlyingID = 0
    With fgOrd
        If .Row > 0 Then
            If Not .RowHidden(.Row) Then
                Set aOrd = .RowData(.Row).Ord
                If Not aOrd Is Nothing Then
                    CurUnderlyingID = aOrd.UnderlyingID
                    Set aOrd = Nothing
                End If
            End If
        End If
    End With

    If CurUnderlyingID = 0 Then
        If m_nFilter(OFC_UNDERLYING) <> 0 Then
            CurUnderlyingID = m_nFilter(OFC_UNDERLYING)
        End If
    End If
End Function

Public Sub NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    Dim aUnd As EtsGeneralLib.UndAtom, sComboList$, sValue$, nValue&
    If m_nFilter(OFC_UNDERLYING) <> nNewUndID Then
        Set aUnd = g_Underlying(nNewUndID)
        If Not aUnd Is Nothing Then
            If aUnd.IsTraderContract Then
                With fgFlt
                    m_GridLock(GT_ORDERS_FILTER).LockRedraw

                    sValue = "0"
                    nValue = 0

                    'sComboList = g_Params.UnderlyingComboList
                    sComboList = g_Params.UnderlyingComboListAllWhtFutUnd
                    If Len(sComboList) > 0 Then
                        sComboList = "#0;<All>|" & sComboList
                    Else
                        sComboList = "#0;<All>"
                    End If

                    Set aUnd = g_Underlying(m_nFilter(OFC_UNDERLYING))
                    If Not aUnd Is Nothing Then
                        nValue = aUnd.ID
                        sValue = Trim$(Str$(aUnd.ID))
                        Set aUnd = Nothing
                    End If
                    .ColComboList(OFC_UNDERLYING) = sComboList
                    .TextMatrix(1, OFC_UNDERLYING) = sValue
                    m_nFilter(OFC_UNDERLYING) = nValue

                    m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
                End With
            End If
        End If
    End If
End Sub

Public Sub NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    Dim aUndGroup As EtsGeneralLib.EtsUndGroupAtom, sComboList$, sValue$, nValue&
    If m_nFilter(OFC_GROUP) <> nNewUndGroupID Then
        Set aUndGroup = g_UnderlyingGroup(nNewUndGroupID)
        If Not aUndGroup Is Nothing Then
            With fgFlt
                m_GridLock(GT_ORDERS_FILTER).LockRedraw

                sValue = "0"
                nValue = 0
                sComboList = "#0;<All>"

                For Each aUndGroup In g_UnderlyingGroup
                    sComboList = sComboList & "|#" & Trim$(Str$(aUndGroup.ID)) & ";" & aUndGroup.Name
                    If aUndGroup.ID = m_nFilter(OFC_GROUP) Then
                        nValue = aUndGroup.ID
                        sValue = Trim$(Str$(aUndGroup.ID))
                    End If
                Next
                .ColComboList(OFC_GROUP) = sComboList
                .TextMatrix(1, OFC_GROUP) = sValue
                m_nFilter(OFC_GROUP) = nValue

                m_GridLock(GT_ORDERS_FILTER).UnlockRedraw
            End With
        End If
    End If
End Sub

Public Sub PrintView()
    On Error Resume Next
    Screen.MousePointer = vbHourglass
    frmPrintPreview.Execute m_frmOwner, "Orders", "Orders", fgFlt, fgOrd
    Screen.MousePointer = vbDefault
End Sub

Public Sub ClearLog()
    On Error Resume Next
    OrdersProcessor.ClearEvents
End Sub

Public Property Get LogVisible() As Boolean
    On Error Resume Next
    LogVisible = m_bLogVisible
End Property

Public Property Let LogVisible(ByVal bVisible As Boolean)
    On Error Resume Next
    m_bLogVisible = bVisible
    AdjustLogVisible
    UserControl_Resize
    AdjustState
End Property

Private Sub AdjustLogVisible()
    On Error Resume Next
    ctlSplit.Visible = m_bLogVisible
    picLog.Visible = m_bLogVisible
End Sub

Private Sub AdjustState()
    On Error Resume Next
    RaiseEvent OnStateChange
End Sub

Private Sub HandleGridDblClick(ByVal bOrderCancelReplaceAvailable As Boolean)
    On Error Resume Next
    If m_nMenuGridRow > 0 And m_nMenuGridRow < m_nMenuGridRows _
        And m_nMenuGridCol > 0 And m_nMenuGridCol < m_nMenuGridCols Then

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

        ElseIf bOrderCancelReplaceAvailable And Not (IsCtrlPressed Or IsShiftPressed) Then
            mnuCtxOrderCancelReplace_Click
            FixDblClickHandled
        End If
    End If
End Sub

Private Sub StoreExpandedRowsInfo()
    On Error Resume Next
    Dim nRow&, aOrd As EtsMmGeneralLib.MmOrderAtom
    Dim aRowData As clsOvRowData

    m_ExpandedOrd.Clear

    With fgOrd
        nRow = .GetNodeRow(1, flexNTFirstSibling)
        While nRow > 0
            If .IsCollapsed(nRow) <> flexOutlineCollapsed Then
                Set aRowData = .RowData(nRow)
                Set aOrd = aRowData.Ord
                If Not aOrd Is Nothing Then
                    If m_ExpandedOrd(CStr(aOrd.ID)) Is Nothing Then m_ExpandedOrd.Add CStr(aOrd.ID)
                    Set aOrd = Nothing
                End If
            End If

            nRow = .GetNodeRow(nRow, flexNTNextSibling)
            Set aRowData = Nothing
        Wend
    End With
End Sub

Private Sub SortOrdNodes(ByVal nCol As Long, ByVal bAsc As Boolean)
    On Error Resume Next
    Dim nRow&, arrVals() As NodeSortItemType, nUBound&, i&, bByStrKey As Boolean
    Dim enColDataType As VSFlex7Ctl.DataTypeSettings

    With fgOrd
        If .Rows > 1 Then
            enColDataType = .ColDataType(nCol)

            Select Case enColDataType
                Case flexDTShort, flexDTLong, flexDTSingle, flexDTDouble, flexDTCurrency, flexDTDate, flexDTBoolean, flexDTDecimal
                    bByStrKey = False
                Case Else
                    bByStrKey = True
            End Select

            nUBound = 0
            nRow = .GetNodeRow(1, flexNTFirstSibling)
            While nRow > 0
                If nUBound = 0 Then ReDim arrVals(1 To 1) Else ReDim Preserve arrVals(1 To nUBound + 1)
                nUBound = nUBound + 1

                Set arrVals(nUBound).Node = .GetNode(nRow)

                Select Case enColDataType
                    Case flexDTShort, flexDTLong, flexDTSingle, flexDTDouble, flexDTCurrency, flexDTDecimal
                        arrVals(nUBound).NumKey = .TextMatrix(nRow, nCol)

                    Case flexDTDate
                        arrVals(nUBound).NumKey = CDate(.TextMatrix(nRow, nCol))

                    Case flexDTBoolean
                        arrVals(nUBound).NumKey = IIf(CBool(.TextMatrix(nRow, nCol)), 1, 0)

                    Case Else
                        arrVals(nUBound).StrKey = .TextMatrix(nRow, nCol)
                End Select

                nRow = .GetNodeRow(nRow, flexNTNextSibling)
            Wend

            If nUBound > 1 Then
                SortNodeArray arrVals, 1, nUBound, bByStrKey

                For i = 1 To nUBound
                    If bAsc Then
                        arrVals(i).Node.Move flexNMLast
                    Else
                        arrVals(i).Node.Move flexNMFirst
                    End If
                Next
            End If

            Erase arrVals
        End If
    End With
End Sub

Private Sub AddTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error Resume Next
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom, aTrdEx As EtsGeneralLib.MmTradeInfoAtom

    m_bInProc = True

    Set aOrd = OrdersProcessor.Orders(aTrd.OrderID)

    If Not aOrd Is Nothing Then ' current trader order
        Set aTrdEx = aOrd.Trades(aTrd.TradeID)
        If aTrdEx Is Nothing Then  ' new order exec
            aOrd.Trades.Add aTrd.TradeID, aTrd.TradeDate, aTrd
  
            With fgOrd
                m_GridLock(GT_ORDERS_LIST).LockRedraw
        
                nRow = .FindRow(CStr(aTrd.OrderID) & "_" & CStr(aTrd.TradeID), 1, OLC_KEY, , True)
                nOrdRow = .FindRow(CStr(aTrd.OrderID), 1, OLC_KEY, , True)
                If nRow < 0 And nOrdRow > 0 Then
        
                    OrderUpdate nOrdRow, aOrd, True
        
                    nRow = nOrdRow + 1
                    .AddItem "", nRow
        
                    Set aRowData = New clsOvRowData
                    Set aRowData.Ord = aOrd
                    Set aRowData.Trade = aTrd
        
                    .RowData(nRow) = aRowData
                    .TextMatrix(nRow, OLC_KEY) = CStr(aTrd.OrderID) & "_" & CStr(aTrd.TradeID)
        
                    .IsSubtotal(nRow) = False
                    .RowOutlineLevel(nRow) = OOL_EXEC
        
                    OrderTradeUpdate nRow, aTrd, True
        
                    bHide = Not CheckOrderFilter(aOrd)
                    .RowHidden(nRow) = bHide Or .IsCollapsed(nOrdRow)
        
                    If bHide Then
                        If Not .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = True
                            m_nOrders = m_nOrders - 1
                            .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                        End If
                    Else
                        If .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = False
                            m_nOrders = m_nOrders + 1
                        End If
                    End If
        
                    UpdateOrdersCountLabel
                End If
        
                Set aRowData = Nothing
        
                m_GridLock(GT_ORDERS_LIST).UnlockRedraw
            End With
        ' Update Existing Trade
        Else
            
            With fgOrd
                m_GridLock(GT_ORDERS_LIST).LockRedraw
        
                nRow = .FindRow(CStr(aOrd.ID) & "_" & CStr(aTrd.TradeID), 1, OLC_KEY, , True)
                nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
                If nRow > 0 And nOrdRow > 0 Then
                    OrderUpdate nOrdRow, aOrd, True
                    OrderTradeUpdate nRow, aTrd
        
                    If CheckOrderFilter(aOrd) Then
                        If .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = False
                            m_nOrders = m_nOrders + 1
                        End If
                    Else
                        If Not .RowHidden(nOrdRow) Then
                            .RowHidden(nOrdRow) = True
                            m_nOrders = m_nOrders - 1
                            .IsCollapsed(nOrdRow) = flexOutlineCollapsed
                        End If
                    End If
        
                    UpdateOrdersCountLabel
                End If
        
                m_GridLock(GT_ORDERS_LIST).UnlockRedraw
            End With

        End If

        Set aTrdEx = Nothing
        Set aOrd = Nothing
    End If
    
    m_bInProc = False
End Sub

Private Sub DeleteTrade(aTrd As EtsGeneralLib.MmTradeInfoAtom)
    On Error GoTo EH
    Dim nRow&, nOrdRow&, aRowData As clsOvRowData, bHide As Boolean
    Dim aOrd As EtsMmGeneralLib.MmOrderAtom
    
    m_bInProc = True
    
    Set aOrd = OrdersProcessor.Orders(aTrd.OrderID)

    If Not aOrd Is Nothing Then ' current trader order
        aOrd.Trades.Remove aTrd.TradeID
        
        With fgOrd
            m_GridLock(GT_ORDERS_LIST).LockRedraw
    
            nRow = .FindRow(CStr(aOrd.ID) & "_" & CStr(aTrd.TradeID), 1, OLC_KEY, , True)
            'nOrdRow = .FindRow(CStr(aOrd.ID), 1, OLC_KEY, , True)
            If nRow > 0 Then .RemoveItem nRow
                'OrderUpdate nOrdRow, aOrd, True
    
            m_GridLock(GT_ORDERS_LIST).UnlockRedraw
        End With
    End If
    
    Exit Sub
EH:
    m_bInProc = False
    LogEvent EVENT_ERROR, "Orders: Fail to delete trade. " & Err.Description
End Sub
