VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmRiskView 
   Caption         =   "Risks"
   ClientHeight    =   5595
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   7875
   Icon            =   "frmRiskView.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   5595
   ScaleWidth      =   7875
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picBottom 
      BorderStyle     =   0  'None
      Height          =   300
      Left            =   120
      ScaleHeight     =   300
      ScaleWidth      =   5820
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   4560
      Width           =   5820
      Begin MSComctlLib.ProgressBar pbProgress 
         Height          =   240
         Left            =   300
         TabIndex        =   3
         Top             =   45
         Visible         =   0   'False
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   423
         _Version        =   393216
         Appearance      =   1
         Scrolling       =   1
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   2
         Left            =   0
         Picture         =   "frmRiskView.frx":000C
         ToolTipText     =   "Real Time (Disconnected)"
         Top             =   30
         Width           =   240
      End
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2655
         TabIndex        =   5
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Height          =   195
         Left            =   300
         TabIndex        =   4
         Top             =   60
         Width           =   45
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   0
         Left            =   0
         Picture         =   "frmRiskView.frx":0596
         ToolTipText     =   "Not Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   1
         Left            =   0
         Picture         =   "frmRiskView.frx":0B20
         ToolTipText     =   "Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Image imgStop 
         Height          =   240
         Left            =   2340
         MouseIcon       =   "frmRiskView.frx":10AA
         MousePointer    =   99  'Custom
         Picture         =   "frmRiskView.frx":11FC
         ToolTipText     =   "Cancel"
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgStopDis 
         Enabled         =   0   'False
         Height          =   240
         Left            =   2340
         Picture         =   "frmRiskView.frx":1346
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
   End
   Begin EtsMM.ctlRiskView ctlView 
      Height          =   2655
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   3615
      _ExtentX        =   6376
      _ExtentY        =   4683
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   1
      Top             =   5295
      Width           =   7875
      _ExtentX        =   13891
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   5280
      Top             =   3720
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileNew 
         Caption         =   "&New..."
         Shortcut        =   ^N
      End
      Begin VB.Menu mnuFileOpen 
         Caption         =   "&Open..."
         Shortcut        =   ^O
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save &As..."
      End
      Begin VB.Menu mnuFileSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileTrades 
         Caption         =   "&Trades..."
         Shortcut        =   ^T
      End
      Begin VB.Menu mnuFileRisks 
         Caption         =   "&Risks..."
         Shortcut        =   ^R
      End
      Begin VB.Menu mnuFileQuotes 
         Caption         =   "&Quotes..."
         Shortcut        =   ^Q
      End
      Begin VB.Menu mnuFileRiskMatrix 
         Caption         =   "Risk &Matrix..."
         Shortcut        =   ^M
      End
      Begin VB.Menu mnuFileVola 
         Caption         =   "Volat&ility Monitor..."
         Shortcut        =   ^I
      End
      Begin VB.Menu mnuFileQuotation 
         Caption         =   "ISE M&arket Maker..."
         Enabled         =   0   'False
         Shortcut        =   ^K
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVolaAnalysis 
         Caption         =   "Volatilit&y Analysis..."
         Shortcut        =   ^Y
      End
      Begin VB.Menu mnuFileGreeks 
         Caption         =   "Gr&eeks..."
         Shortcut        =   ^E
      End
      Begin VB.Menu mnuFileGraphicalPnL 
         Caption         =   "PnL &Projections"
         Shortcut        =   ^A
      End
      Begin VB.Menu mnuFileHedgeSummaryView 
         Caption         =   "&Hedge Summary..."
         Shortcut        =   ^H
      End
      Begin VB.Menu mnuFileReportsView 
         Caption         =   "Re&ports..."
         Shortcut        =   ^J
      End
      Begin VB.Menu mnuFileOrders 
         Caption         =   "Or&ders..."
         Shortcut        =   ^B
      End
      Begin VB.Menu mnuFileTntCards 
         Caption         =   "Trades Messa&ges..."
         Enabled         =   0   'False
         Shortcut        =   ^U
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileStockHedge 
         Caption         =   "Sto&ck Hedge..."
         Enabled         =   0   'False
         Shortcut        =   ^F
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileIndex1Hedge 
         Caption         =   "I&ndex Hedge..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVSCalc 
         Caption         =   "Variance Swap Calculator"
         Shortcut        =   ^V
      End
      Begin VB.Menu mnuFileIndex2Hedge 
         Caption         =   "T&wo Indices Hedge..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSepBR1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileBatchReporting 
         Caption         =   "Batch Reporting ..."
      End
      Begin VB.Menu mnuFileSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "Print..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileSeparator79 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileOtcOptionCalc 
         Caption         =   "OTC Option Calc..."
         Shortcut        =   ^{F12}
      End
      Begin VB.Menu mnuToolsVolaManager 
         Caption         =   "&Volatility Manager..."
         Shortcut        =   ^{F11}
      End
      Begin VB.Menu mnuToolsStrategyScanner 
         Caption         =   "Strategy Scanner"
      End
      Begin VB.Menu mnuToolsOptionScanner 
         Caption         =   "Options Scanner"
      End
      Begin VB.Menu mnuToolsHedger 
         Caption         =   "&Hedger..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuToolsBackOffice 
         Caption         =   "&Back Office..."
      End
      Begin VB.Menu mnuFileSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewRealTime 
         Caption         =   "Real &Time Mode"
         Shortcut        =   {F9}
      End
      Begin VB.Menu mnuViewRefresh 
         Caption         =   "&Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuViewRefreshPrices 
         Caption         =   "Refresh &Prices"
         Shortcut        =   +{F5}
      End
      Begin VB.Menu mnuViewSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewWtdVega 
         Caption         =   "&Weighted Vega Settings..."
      End
      Begin VB.Menu mnuViewSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewFltGridLayout 
         Caption         =   "&Filter Grid Layout..."
      End
      Begin VB.Menu mnuViewTotGridLayout 
         Caption         =   "Totals &Grid Layout..."
      End
      Begin VB.Menu mnuViewPosGridLayout 
         Caption         =   "Positions Grid &Layout..."
      End
   End
   Begin VB.Menu mnuUnd 
      Caption         =   "&Underlying"
      Begin VB.Menu mnuUndNext 
         Caption         =   "&Next"
         Shortcut        =   {F8}
      End
      Begin VB.Menu mnuUndPrev 
         Caption         =   "&Previous"
         Shortcut        =   {F7}
      End
      Begin VB.Menu mnuUndShowAll 
         Caption         =   "&Show All"
         Shortcut        =   {F6}
      End
      Begin VB.Menu mnuUndHideAll 
         Caption         =   "&Hide All"
         Shortcut        =   {F4}
      End
   End
   Begin VB.Menu mnuOrder 
      Caption         =   "O&rder"
      Begin VB.Menu mnuOrderNewStock 
         Caption         =   "New &Stock Order..."
      End
      Begin VB.Menu mnuOrderNewOption 
         Caption         =   "New &Option Order..."
      End
   End
   Begin VB.Menu mnuTntCard 
      Caption         =   "Trade Messa&ge"
      Enabled         =   0   'False
      Visible         =   0   'False
      Begin VB.Menu mnuTntCardNew 
         Caption         =   "&New Trade Message..."
         Enabled         =   0   'False
      End
   End
   Begin VB.Menu mnuManage 
      Caption         =   "&Manage"
      Begin VB.Menu mnuManageUnderlyings 
         Caption         =   "&Underlyings..."
      End
      Begin VB.Menu mnuExerciseScreen 
         Caption         =   "Exercise Screen..."
      End
      Begin VB.Menu mnuToolsSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsParameters 
         Caption         =   "&Parameters..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuOptionsEventLog 
         Caption         =   "&Event Log"
         Shortcut        =   ^L
      End
      Begin VB.Menu mnuToolsSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuManualPrices 
         Caption         =   "Manual Prices..."
      End
   End
   Begin VB.Menu mnuSimulation 
      Caption         =   "&Simulation"
      Begin VB.Menu mnuSimulationUse 
         Caption         =   "Use simulation"
      End
      Begin VB.Menu mnuSimulationParameters 
         Caption         =   "Scenario"
      End
   End
   Begin VB.Menu mnuWindow 
      Caption         =   "&Window"
      Begin VB.Menu mnuWindowArrange 
         Caption         =   "&Arrange"
      End
      Begin VB.Menu mnuWindowCloseAll 
         Caption         =   "&Close All"
      End
      Begin VB.Menu mnuWindowSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowTileHorizontaly 
         Caption         =   "Tile &Horizontaly"
      End
      Begin VB.Menu mnuWindowTileVertically 
         Caption         =   "Tile &Vertically"
      End
      Begin VB.Menu mnuWindowCascade 
         Caption         =   "Ca&scade"
      End
      Begin VB.Menu mnuWindowSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowMain 
         Caption         =   "&Main ETS Window..."
         Shortcut        =   {F12}
      End
      Begin VB.Menu mnuWindowShowInTaskbar 
         Caption         =   "&Show All In Taskbar"
      End
      Begin VB.Menu mnuWindowSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 1>"
         Index           =   1
         Shortcut        =   ^{F1}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 2>"
         Index           =   2
         Shortcut        =   ^{F2}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 3>"
         Index           =   3
         Shortcut        =   ^{F3}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 4>"
         Index           =   4
         Shortcut        =   ^{F4}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 5>"
         Index           =   5
         Shortcut        =   ^{F5}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 6>"
         Index           =   6
         Shortcut        =   ^{F6}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 7>"
         Index           =   7
         Shortcut        =   ^{F7}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 8>"
         Index           =   8
         Shortcut        =   ^{F8}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowGo 
         Caption         =   "<win 9>"
         Index           =   9
         Shortcut        =   ^{F9}
         Visible         =   0   'False
      End
      Begin VB.Menu mnuWindowList 
         Caption         =   "&Windows..."
         Shortcut        =   ^W
      End
   End
   Begin VB.Menu mnuHelp 
      Caption         =   "&?"
      Begin VB.Menu mnuHelpContents 
         Caption         =   "&Contents..."
         Shortcut        =   {F1}
      End
      Begin VB.Menu mnuHelpSeparator1 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuHelpSupport 
         Caption         =   "&Support"
         Enabled         =   0   'False
         Visible         =   0   'False
         Begin VB.Menu mnuHelpSupportDataSnapshot 
            Caption         =   "&Make Data Snapshot..."
            Enabled         =   0   'False
         End
      End
   End
End
Attribute VB_Name = "frmRiskView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1
Private WithEvents fMain As frmMain
Attribute fMain.VB_VarHelpID = -1

Private m_sKey As String
Private m_sFilePath As String
Private m_sFileName As String
Private m_bIsNew As Boolean
Private m_nWindowIndex As Long
Private m_sCaption As String
Private m_sShortCaption As String
Private m_nCurImgIdx&

Public EnableActiveRealTime As Boolean

Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean
Public lRTNum As Long

Public Sub ShowWindowInTaskbar(ByVal bShow As Boolean, Optional ByVal bExtended As Boolean = False)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk ShowWindowInTaskbar Enter", Me.Caption
    
    If m_bShowInTaskbar <> bShow Then
        m_bShowInTaskbar = bShow
        mnuWindowShowInTaskbar.Checked = bShow
        m_bShowInTaskbarChanging = True
        
        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_HIDEWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
        ShowFormInTaskBar Me, m_bShowInTaskbar
        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_SHOWWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
        
        m_bShowInTaskbarChanging = False
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk ShowWindowInTaskbar Exit", Me.Caption

End Sub

Public Sub StopRT()
    On Error Resume Next
    ctlView.ctlStopRT
End Sub

Public Sub SetWinIndex(ByVal nIndex As Long)
    On Error Resume Next
    m_nWindowIndex = nIndex
    AdjustCaption
End Sub

Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = m_sCaption
End Function

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = m_sShortCaption
End Function

Public Function IsRealTime() As Boolean
    On Error Resume Next
    IsRealTime = ctlView.RealTime
End Function

Public Sub CheckPassiveRealtime()
    On Error Resume Next
    If (m_nWindowIndex = g_LastActiveWindow) Then
        ctlView.ProcessRealTime = True
    Else
        ctlView.ProcessRealTime = False
    End If
End Sub

Public Sub Init(sKey As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "frmRisk Init Enter", Me.Caption
    
    Set fMain = frmMain
    EnableActiveRealTime = True
    m_sFilePath = MakeCommonFilePath("", "", False)
    m_sCaption = ""
    m_sShortCaption = ""
    g_nRisksUntitledIdx = g_nRisksUntitledIdx + 1
    m_sFileName = "Untitled" & g_nRisksUntitledIdx & ".rvw"
    m_bIsNew = True
    m_sKey = sKey
    mnuFileClose.Caption = "Close" & vbTab & "Alt+F4"
    mnuOrderNewStock.Caption = "New &Stock Order..." & vbTab & "Alt+Ins"
    mnuOrderNewOption.Caption = "New &Option Order..." & vbTab & "Ctrl+Alt+Ins"
'    mnuTntCardNew.Caption = "&New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set ctlView.m_frmOwner = Me
    Set ctlView.pbProgress = pbProgress
    Set ctlView.lblProcess = lblProcess
    Set ctlView.lblStatus = lblStatus
    Set ctlView.imgStop = imgStop
    Set ctlView.imgStopDis = imgStopDis
    
   
    mnuOrderNewStock.Visible = g_Params.OrdersVisible
    mnuOrderNewOption.Visible = g_Params.OrdersVisible
    mnuFileOrders.Visible = g_Params.OrdersVisible
    mnuOrder.Visible = g_Params.OrdersVisible
    
    mnuOrderNewStock.Enabled = g_Params.OrdersEnabled
    mnuOrderNewOption.Enabled = g_Params.OrdersEnabled
    mnuFileOrders.Enabled = g_Params.OrdersEnabled
    
    mnuFileVSCalc.Enabled = g_Params.IsVSCalcEnabled
    mnuFileVSCalc.Visible = g_Params.IsVSCalcEnabled
    ctlView.Init
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
    ctlView.ProcessRealTime = True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "frmRisk Init Exit", Me.Caption
    
End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmRisk ShowData Call", Me.Caption
    
    ctlView.ShowData nType, nValue
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmRisk ShowData Done", Me.Caption

End Sub
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmRisk ShowData Call", Me.Caption
    
    ctlView.ShowDataByFilter aFilters
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmRisk ShowData Done", Me.Caption

End Sub



Private Sub aParams_InternalVolatilityChanged(ByVal sSymbol As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_InternalVolatilityChanged Call: " & sSymbol, Me.Caption
    
    ctlView.InternalVolatilityChanged sSymbol

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_InternalVolatilityChanged Done: " & sSymbol, Me.Caption

End Sub

Private Sub aParams_NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewUnderlyingAdded Call", Me.Caption
    
    ctlView.NewUnderlyingAdded nNewUndID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewUnderlyingAdded Done", Me.Caption

End Sub

Private Sub aParams_NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewUnderlyingGroupAdded Call", Me.Caption
    
    ctlView.NewUnderlyingGroupAdded nNewUndGroupID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewUnderlyingGroupAdded Done", Me.Caption

End Sub

Private Sub aParams_NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewStrategyAdded Call", Me.Caption
    
    ctlView.NewStrategyAdded nStrategyID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_NewStrategyAdded Done", Me.Caption

End Sub

Private Sub ctlView_OnManualPriceChanged(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsGeneralLib.EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
On Error Resume Next
    frmMain.ManualPriceChange UndID, ID, price, CtType, Status
End Sub

Private Sub ctlView_OnSetCaption()
    On Error Resume Next
    m_nCurImgIdx = 0
    
    If ctlView.RealTime Then m_nCurImgIdx = IIf(ctlView.RealTimeConnected, 1, 2)
    
    imgRealTime(0).Visible = (m_nCurImgIdx = 0)
    imgRealTime(1).Visible = (m_nCurImgIdx = 1)
    imgRealTime(2).Visible = (m_nCurImgIdx = 2)
    
    If (ctlView.TotalShownUndPositions + ctlView.TotalShownOptPositions) > 0 Then
        lblStatus.Caption = imgRealTime(m_nCurImgIdx).ToolTipText & " Pos :Und " & ctlView.TotalShownUndPositions & " /Opt " & ctlView.TotalShownOptPositions & "/Tot " & ctlView.TotalShownOptPositions + ctlView.TotalShownUndPositions
    Else
        lblStatus.Caption = imgRealTime(m_nCurImgIdx).ToolTipText
    End If
    
    lblStatus.Caption = lblStatus.Caption & " /Time Stamp " & Format(ctlView.getRecalculationTimeStemp, "HH:MM:SS")
    
    AdjustCaption
    
    frmMain.AdjustWindowsListCaptions
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    m_sCaption = ctlView.GetCaption & " - [" & m_sFileName & "]"
    m_sShortCaption = ctlView.GetShortCaption
    Me.Caption = IIf(m_nWindowIndex > 0 And m_nWindowIndex < 10, CStr(m_nWindowIndex) & ": ", "") & m_sCaption
End Sub

Private Sub ctlView_OnSetRefreshHint(ByVal bSet As Boolean, strHint As String)
    On Error Resume Next
    If bSet And Len(strHint) > 0 Then
        lblStatus.Caption = strHint
    Else
        lblStatus.Caption = imgRealTime(m_nCurImgIdx).ToolTipText
    End If
    lblStatus.Refresh
End Sub

Private Sub ctlView_OnStateChange()
    On Error Resume Next
    
    mnuViewRealTime.Enabled = ctlView.Group.ID <> 0 And Not ctlView.InProc
                            
    mnuViewRealTime.Checked = ctlView.RealTime
    
    mnuViewRefresh.Enabled = mnuViewRealTime.Enabled And Not ctlView.RealTime
    mnuViewRefreshPrices.Enabled = mnuViewRefresh.Enabled
    mnuViewWtdVega.Enabled = Not ctlView.InProc And Not ctlView.WtdVegaIsOpened
    
    Dim nCount&
    nCount = ctlView.UnderlyingsCount
    mnuUndHideAll.Enabled = (nCount > 0)
    mnuUndShowAll.Enabled = mnuUndHideAll.Enabled
    mnuUndNext.Enabled = (nCount > 1)
    mnuUndPrev.Enabled = mnuUndNext.Enabled
    
    ctlView_OnSetCaption

End Sub

Private Sub fMain_OnActiveFuturesChange(ByVal iUndID As Long, ByVal iActiveFutID As Long)
    On Error Resume Next
    ctlView.ActiveFuturesChange iUndID, iActiveFutID
End Sub

Private Sub fMain_OnFuturesParamsChange(ByVal iUndID As Long, ByVal iFutID As Long, ByVal dRatio As Double, ByVal dBasis As Double)
    On Error Resume Next
    ctlView.FuturesParamsChange iUndID, iFutID, dRatio, dBasis
End Sub

Private Sub fMain_OnManualPriceChange(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsGeneralLib.EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
On Error Resume Next
    'manual price update for each ctrl
    ctlView.ManualPriceUpdate UndID, ID, price, CtType, Status
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Load Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Load Exit - Not Done", Me.Caption
        Exit Sub
    End If
    m_bShowInTaskbar = True
    mnuWindowShowInTaskbar.Checked = True
    lblStatus.Caption = ""
    lblProcess.Caption = ""
    m_nCurImgIdx = 0
    mnuToolsOptionScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    g_Params.SetIcon WND_RISK, Me.hWnd, False

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Load Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Resize Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk View Resize Exit - Not Done", Me.Caption
        Exit Sub
    End If
    LockWindowUpdate Me.hWnd
    
    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height
    
    LockWindowUpdate 0
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Resize Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Terminate Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Terminate Exit - Not Done", Me.Caption
       Exit Sub
    End If
    Set aParams = Nothing
    
    ctlView.Term
    
    g_ViewFrm.Remove m_sKey
    frmMain.ReloadWindowsList
    Set fMain = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Terminate Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Risk View Unload", Me.Caption
    
    Form_Terminate
End Sub

Public Sub EnableRealTime(bEnable As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then
        If bEnable = True Then
            g_PerformanceLog.LogMmInfo enLogUserAction, "EnableRealTime Enter", Me.Caption
        Else
            g_PerformanceLog.LogMmInfo enLogUserAction, "DisableRealTime Enter", Me.Caption
        End If
    End If
    
    If aParams.ActiveRealTime Then
        If bEnable Then
            If Not EnableActiveRealTime Or (Me.hWnd <> GetForegroundWindow) Then Exit Sub
            ctlView.ProcessRealTime = True
            ctlView.RefreshView
        Else
            ctlView.ProcessRealTime = False
        End If
    End If
    
    If Not g_PerformanceLog Is Nothing Then
        If bEnable = True Then
            g_PerformanceLog.LogMmInfo enLogUserAction, "EnableRealTime Exit", Me.Caption
        Else
            g_PerformanceLog.LogMmInfo enLogUserAction, "DisableRealTime Exit", Me.Caption
        End If
    End If

End Sub

Private Sub Form_Activate()
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Activation Enter", Me.Caption
    
    Dim nOperation&
    
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not EnableActiveRealTime Or m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Activation Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
        
    If (ctlView.RealTime) Then
        g_LastActiveWindow = m_nWindowIndex
    End If
    
    If aParams.ActiveRealTime Then
        ctlView.ProcessRealTime = True
        ctlView.RefreshView
    End If
    
    fMain.PassiveRealtimeControl
    
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If

    If Not g_PerformanceLog Is Nothing Then
        g_PerformanceLog.FinishLogMmOperation nOperation, OPER_ACTIVATEWINDOW, GetCaption
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Activation Exit - Done", Me.Caption
    End If
End Sub

Private Sub Form_Deactivate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Deactivate Enter", Me.Caption
    
    If gCmn Is Nothing Or m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Deactivate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    If (ctlView.RealTime) Then
        g_LastActiveWindow = m_nWindowIndex
    End If
    
    If (aParams.ActiveRealTime) Then
        ctlView.ProcessRealTime = False
    End If
    
    fMain.PassiveRealtimeControl
        
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Risk View Deactivate Exit - Done", Me.Caption
    
End Sub

Private Sub aParams_ActiveRealTimeChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_ActiveRealTimeChange Enter", Me.Caption
    
    If Not EnableActiveRealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_ActiveRealTimeChange Exit - Not Done", Me.Caption
        Exit Sub
    End If
    If aParams.ActiveRealTime Then
        ctlView.ProcessRealTime = (Me.hWnd = GetForegroundWindow)
    Else
        If ctlView.RealTime And Not ctlView.ProcessRealTime Then ctlView.RefreshView
        ctlView.ProcessRealTime = True
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_ActiveRealTimeChange Exit - Done", Me.Caption

End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub aParams_CalcModelChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_CalcModelChange Call", Me.Caption
    
    ctlView.FilterUpdateModel True

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "aParams_CalcModelChange Done", Me.Caption

End Sub

Private Sub mnuFileBatchReporting_Click()
    On Error Resume Next
    g_frmProjections.ShowData
End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Enter. " & ctlView.GetOptionInfo, Me.Caption
       
       Unload Me
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Exit. " & ctlView.GetOptionInfo, Me.Caption
  
    
End Sub

Private Sub mnuFIleGraphicalPnL_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
    g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Enter. ", Me.Caption
    
    frmMain.ShowGraphicalPnL TYPE_UNDERLYING, ctlView.CurUnderlyingID

    If Not g_PerformanceLog Is Nothing Then _
    g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileOtcOptionCalc_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->OTC Option Calc"" Enter. " & ctlView.GetOptionInfo, Me.Caption
      
    ctlView.CallOtcOptionCalcRV
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->OTC Option Calc"" Exit. " & ctlView.GetOptionInfo, Me.Caption
  
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
'    ctlView.PrintView
End Sub

Private Sub mnuFileVSCalc_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->VSCalc..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.ShowVSC TYPE_UNDERLYING, ctlView.CurUnderlyingID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->VSCalc..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuManualPrices_Click()
    Dim frmMPrices As New frmManualPrices
   
    frmMPrices.Show vbModal, Me
    
    If (frmMPrices.ChangedCount > 0) Then
        ctlView.Refresh
    End If
End Sub

Private Sub mnuSimulationParameters_Click()
    On Error Resume Next
    '-------------------'

    ctlView.ShowSimulationScenario
    
End Sub

Private Sub mnuSimulationUse_Click()
    On Error Resume Next
    '-------------------'
    
    mnuSimulationUse.Checked = Not mnuSimulationUse.Checked
    
    ctlView.UseSimulation mnuSimulationUse.Checked
    
End Sub

Private Sub mnuUndHideAll_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Hide All""Enter. ", Me.Caption
    
    ctlView.ShowAllUnd False

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Hide All"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuUndNext_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Next""Enter. ", Me.Caption
    
    
    ctlView.ShowNextUnd
   
  If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Next"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuUndPrev_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Previous"" Enter. ", Me.Caption
    
    
    ctlView.ShowPrevUnd
   
  If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Previous "" Exit. ", Me.Caption
    

End Sub

Private Sub mnuUndShowAll_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Show All"" Enter. ", Me.Caption
    
    ctlView.ShowAllUnd True

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Underlying->Show All"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters"" Enter. ", Me.Caption
    
    
    frmMain.ShowParameters

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuViewRealTime_Click()
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Real Time"" Enter. ", Me.Caption
    
    
    ctlView.ToggleRealtime

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Real Time"" Exit. ", Me.Caption

End Sub

Private Sub Save(ByVal sFilePath As String, ByVal sFileName As String)
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Save Enter", Me.Caption
    
    Dim aStorage As New clsSettingsStorage
    Screen.MousePointer = vbHourglass
    aStorage.Init sFilePath & sFileName
    aStorage.SetStringValue "Type", "Value", "RisksView"
    ctlView.SaveToFile aStorage, ""
    aStorage.SaveData
    m_sFilePath = sFilePath
    m_sFileName = sFileName
    m_bIsNew = False
    AdjustCaption
    Screen.MousePointer = vbDefault

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Save Exit", Me.Caption

End Sub

Public Sub SaveLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "SaveLayout Call", Me.Caption
    
    ctlView.SaveToFile aStorage, sKey

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "SaveLayout Done", Me.Caption

End Sub

Public Sub OpenLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "OpenLayout Call", Me.Caption
    
    ctlView.OpenFromFile aStorage, sKey
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "OpenLayout Done", Me.Caption

End Sub

Private Sub mnuFileNew_Click()
    On Error Resume Next
        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    
    frmMain.ShowRisks

        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Enter. ", Me.Caption
    
    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.CurUnderlyingID

        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Reports"" Enter. ", Me.Caption
    
    
    frmMain.ShowReports ctlView.Group.GroupType, ctlView.Group.ID

        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Reports"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risk Matrix"" Enter. ", Me.Caption
    
    frmMain.ShowRiskMatrixByFilters ctlView.Filter

        If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risk Matrix"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileRisks_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risks"" Enter. ", Me.Caption

    frmMain.ShowRisksByFilters ctlView.Filter

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risks"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileTrades_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Trades"" Enter. ", Me.Caption

    frmMain.ShowTradesByFilter ctlView.Filter

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Trades"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Monitor"" Enter. ", Me.Caption

    frmMain.ShowVola ctlView.Group.GroupType, ctlView.Group.ID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Monitor"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileQuotation_Click()
    On Error Resume Next
'    frmMain.ShowQuotation TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Analysis"" Enter. ", Me.Caption

    frmMain.ShowVolaAnalysis ctlView.Group.GroupType, ctlView.Group.ID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Analysis"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks"" Enter. " & ctlView.GetOptionInfo, Me.Caption
 
   frmMain.ShowGreeks ctlView.Group.GroupType, ctlView.Group.ID

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary View"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.ShowHedgeSummary ctlView.Group.GroupType, ctlView.Group.ID

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary View"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileOrders_Click()
    On Error Resume Next
    frmMain.ShowOrders ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileTntCards_Click()
    On Error Resume Next
'    frmMain.ShowTntCards ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
'    frmMain.ShowStockHedge ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex1Hedge ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex2Hedge ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileSave_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    If m_bIsNew Then
        With dlgCommon
            .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                    Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
            .CancelError = True
            .DefaultExt = ".rvw"
            .DialogTitle = "Save Risks"
            .InitDir = ""
            .FileName = m_sFilePath & m_sFileName
            .Filter = "ETS Risks (*.rvw)|*.rvw|All Files|*.*"
            .FilterIndex = 1
            On Error Resume Next
            .ShowSave
            If Err.Number = 0 Then
                On Error GoTo EH
                Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
            End If
        End With
    Else
        Save m_sFilePath, m_sFileName
    End If
    
   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Exit. " & ctlView.GetOptionInfo, Me.Caption

    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save risks."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Exit with an error. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileSaveAs_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
        .CancelError = True
        .DefaultExt = ".rvw"
        .DialogTitle = "Save Risks As"
        .InitDir = ""
        .FileName = m_sFilePath & m_sFileName
        .Filter = "ETS Risks (*.rvw)|*.rvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowSave
        If Err.Number = 0 Then
            On Error GoTo EH
            Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
        End If
    End With
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Exit. " & ctlView.GetOptionInfo, Me.Caption

    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save risks."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Exit with an errors. ", Me.Caption

End Sub

Private Sub mnuFileOpen_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    Dim aStorage As New clsSettingsStorage
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
        .CancelError = True
        .DefaultExt = ".rvw"
        .DialogTitle = "Open Risks"
        .FileName = ""
        .InitDir = m_sFilePath
        .Filter = "ETS Risks (*.rvw)|*.rvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowOpen
        If Err.Number = 0 Then
            On Error GoTo EH
            Screen.MousePointer = vbHourglass
            aStorage.Init .FileName
            aStorage.ReadData
            If aStorage.GetStringValue("Type", "Value") = "RisksView" Then
                ctlView.OpenFromFile aStorage, ""
                m_sFilePath = ExtractFilePath(.FileName)
                m_sFileName = ExtractFileName(.FileName)
                m_bIsNew = False
                AdjustCaption
                Screen.MousePointer = vbDefault
            Else
                Screen.MousePointer = vbDefault
                gCmn.MyMsgBox Me, "Invalid risks file."
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Exit. " & ctlView.GetOptionInfo, Me.Caption

    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open risks."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Exit with an errors. ", Me.Caption

End Sub

Private Sub mnuViewRefresh_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.Refresh
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuViewFltGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Filter Grid Layout"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.CustomizeFltGridLayout

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Filter Grid Layout"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuViewRefreshPrices_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->RefreshPrices"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.RefreshPrices
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->RefreshPrices"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuViewTotGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Total Grid Layout"" Enter. ", Me.Caption

    ctlView.CustomizeTotGridLayout

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Total Grid Layout"" Exit. ", Me.Caption

End Sub

Private Sub mnuViewPosGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Positions Grid Layout"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.CustomizePosGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Positions Grid Layout"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ShowEventLog (Not mnuOptionsEventLog.Checked), , True

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->BackOffice"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.RunBackOffice

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->BackOffice"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub
Private Sub mnuToolsOptionScanner_Click()
    On Error Resume Next
    frmMain.RunOptionScanner
End Sub

Private Sub mnuToolsStrategyScanner_Click()
    On Error Resume Next
    frmMain.RunStrategyScanner
End Sub

Private Sub mnuToolsHedger_Click()
    On Error Resume Next
'    frmMain.RunHedger
End Sub

Private Sub mnuToolsVolaManager_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.RunVolaManager
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Public Sub AdjustWindowsListCaptions()
    On Error Resume Next
    Dim i&, nCount&
    
    nCount = g_ViewFrm.Count
    For i = 1 To 9
        If i <= nCount Then
            mnuWindowGo(i).Caption = "&" & CStr(i) & " " & g_ViewFrm(i).Frm.GetCaption
            mnuWindowGo(i).Visible = True
            mnuWindowGo(i).Enabled = True
        Else
            mnuWindowGo(i).Visible = False
            mnuWindowGo(i).Enabled = False
        End If
    Next
End Sub

Private Sub mnuViewWtdVega_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Weighted Vega"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.ShowWeightedVegaSettings
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Weighted Vega"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowArrange_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Arrange"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.WindowsArrange

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Arrange"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowCloseAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Close All"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.WindowsCloseAll

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Close All"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowList_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->List"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.WindowsList

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->List"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowMain_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ShowWindow frmMain.hWnd, SW_SHOWNORMAL
    frmMain.SetFocus

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowTileHorizontaly_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Horizontaly"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.WindowsTileHorizontally

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Horizontaly"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowTileVertically_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Vertically"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.WindowsTileVertically

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Verticaly"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuWindowCascade_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Cascade"" Enter. ", Me.Caption

    frmMain.WindowsCascade
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Cascade"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowShowInTaskBar_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Show In Task Bar"" Enter. ", Me.Caption

    frmMain.WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Show In Task Bar"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowGo_Click(Index As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Go"" Enter. " & CStr(Index), Me.Caption
    
    frmMain.WindowsGo Index
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Go"" Exit. " & CStr(Index), Me.Caption

End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "SetGridLayout Call", Me.Caption
    
    ctlView.SetGridLayout enGridType, gdGrid

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "SetGridLayout Done", Me.Caption

End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Help->Contents"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.HelpContents

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Help->Contents"" Exit. " & ctlView.GetOptionInfo, Me.Caption
    
End Sub

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Underlyings"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.ShowManageUnderlyings

   If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Underlyings"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
End Sub

Private Sub mnuOrderNewOption_Click()
    On Error Resume Next
    frmOrderEntry.NewOptionOrder Nothing, Nothing, True, 10, 0#
End Sub

Private Sub mnuOrderNewStock_Click()
    On Error Resume Next
    frmOrderEntry.NewStockOrder Nothing, True, 100, 0#
End Sub

Private Sub mnuTntCardNew_Click()
    On Error Resume Next
'    frmTntCardEntry.NewCard Me, Nothing, Nothing, True, 10, 0#
End Sub

Private Sub mnuHelpSupportDataSnapshot_Click()
    On Error Resume Next
'    ctlView.MakeDataSnapshot DATA_SHAPSHOT_FILENAME & "_Risks_" & m_sKey & ".txt"
End Sub

Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    frmMain.ShowExerciseScreen
End Sub
