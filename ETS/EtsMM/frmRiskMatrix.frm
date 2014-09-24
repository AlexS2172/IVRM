VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmRiskMatrix 
   Caption         =   "Risk Matrix"
   ClientHeight    =   5970
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   8700
   Icon            =   "frmRiskMatrix.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   5970
   ScaleWidth      =   8700
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picBottom 
      BorderStyle     =   0  'None
      Height          =   300
      Left            =   240
      ScaleHeight     =   300
      ScaleWidth      =   6180
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   5040
      Width           =   6180
      Begin MSComctlLib.ProgressBar pbProgress 
         Height          =   240
         Left            =   30
         TabIndex        =   2
         Top             =   45
         Visible         =   0   'False
         Width           =   1935
         _ExtentX        =   3413
         _ExtentY        =   423
         _Version        =   393216
         Appearance      =   1
         Scrolling       =   1
      End
      Begin VB.Image imgStopDis 
         Enabled         =   0   'False
         Height          =   240
         Left            =   2070
         Picture         =   "frmRiskMatrix.frx":000C
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgStop 
         Height          =   240
         Left            =   2070
         MouseIcon       =   "frmRiskMatrix.frx":0156
         MousePointer    =   99  'Custom
         Picture         =   "frmRiskMatrix.frx":02A8
         ToolTipText     =   "Cancel"
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         Height          =   195
         Left            =   30
         TabIndex        =   4
         Top             =   60
         Width           =   45
      End
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2400
         TabIndex        =   3
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
      Begin VB.Label lblValue 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         Caption         =   "Value"
         Height          =   195
         Left            =   5760
         TabIndex        =   6
         Top             =   60
         UseMnemonic     =   0   'False
         Width           =   405
      End
   End
   Begin EtsMM.ctlRiskMatrix ctlView 
      Height          =   2175
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   2655
      _ExtentX        =   4683
      _ExtentY        =   3836
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   5
      Top             =   5670
      Width           =   8700
      _ExtentX        =   15346
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   5160
      Top             =   4080
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
      Begin VB.Menu mnuSeparator1 
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
      Begin VB.Menu mnuFIleGraphicalPnL 
         Caption         =   "PnL Projections"
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
      Begin VB.Menu mnuFileIndex2Hedge 
         Caption         =   "T&wo Indices Hedge..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSepBR1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileBatchReporting 
         Caption         =   "Batch Reporting"
      End
      Begin VB.Menu mnuSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "Print..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSeparator3 
         Caption         =   "-"
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
      Begin VB.Menu mnuSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnuViewRefresh 
         Caption         =   "&Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewWtdVega 
         Caption         =   "&Weighted Vega Settings..."
      End
      Begin VB.Menu mnuSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewFltGridLayout 
         Caption         =   "&Filter Grid Layout..."
      End
      Begin VB.Menu mnuViewScnGridLayout 
         Caption         =   "&Scenario Grid Layout..."
      End
      Begin VB.Menu mnuViewValGridLayout 
         Caption         =   "&Values Grid Layout..."
      End
   End
   Begin VB.Menu mnuScn 
      Caption         =   "&Scenario"
      Begin VB.Menu mnuScnNew 
         Caption         =   "&New"
      End
      Begin VB.Menu mnuScnSave 
         Caption         =   "&Save"
      End
      Begin VB.Menu mnuScnDelete 
         Caption         =   "&Delete..."
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
      Begin VB.Menu mnuToolsSeparator31 
         Caption         =   "-"
      End
      Begin VB.Menu mnuReloadGroupsSpectificRatio 
         Caption         =   "Reload Group Specific Ratio"
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
   End
End
Attribute VB_Name = "frmRiskMatrix"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Private m_sKey As String
Private m_sFilePath As String
Private m_sFileName As String
Private m_bIsNew As Boolean
Private m_nWindowIndex As Long
Private m_sCaption As String
Private m_sShortCaption As String

Public EnableActiveRealTime As Boolean

Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean

Public Sub ShowWindowInTaskbar(ByVal bShow As Boolean, Optional ByVal bExtended As Boolean = False)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix ShowWindowInTaskbar Enter", Me.Caption
    
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix ShowWindowInTaskbar Exit", Me.Caption

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

Public Sub Init(sKey As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "frmRiskMatrix Init Enter", Me.Caption
    
    EnableActiveRealTime = True
    m_sFilePath = MakeCommonFilePath("", "", False)
    m_sCaption = ""
    m_sShortCaption = ""
    g_nRiskMatrixUntitledIdx = g_nRiskMatrixUntitledIdx + 1
    m_sFileName = "Untitled" & g_nRiskMatrixUntitledIdx & ".mvw"
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
    Set ctlView.lblValue = lblValue
    Set ctlView.imgStop = imgStop
    Set ctlView.imgStopDis = imgStopDis
    
    mnuFileOrders.Visible = g_Params.OrdersVisible
    mnuOrderNewStock.Visible = g_Params.OrdersVisible
    mnuOrderNewOption.Visible = g_Params.OrdersVisible
    mnuOrder.Visible = g_Params.OrdersVisible
    
    mnuFileOrders.Enabled = g_Params.OrdersEnabled
    mnuOrderNewStock.Enabled = g_Params.OrdersEnabled
    mnuOrderNewOption.Enabled = g_Params.OrdersEnabled
        
    ctlView.Init
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'    mnuFileQuotation.Enabled = g_bIseEnable

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "frmRiskMatrix Init Exit", Me.Caption
        
End Sub

Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    ctlView.ShowDataByFilter aFilters
End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix ShowData Call", Me.Caption
        
    ctlView.ShowData nType, nValue
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix ShowData Done", Me.Caption
        
End Sub

Private Sub aParams_NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewUnderlyingAdded Call", Me.Caption
        
    ctlView.NewUnderlyingAdded nNewUndID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewUnderlyingAdded Done", Me.Caption
        
End Sub

Private Sub aParams_NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewUnderlyingGroupAdded Call", Me.Caption
    
    ctlView.NewUnderlyingGroupAdded nNewUndGroupID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewUnderlyingGroupAdded Done", Me.Caption

End Sub

Private Sub aParams_NewStrategyAdded(ByVal nStrategyID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewStrategyAdded Call", Me.Caption
    
    ctlView.NewStrategyAdded nStrategyID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix aParams_NewStrategyAdded Done", Me.Caption

End Sub

Private Sub ctlView_OnSetCaption()
    On Error Resume Next
    AdjustCaption
    frmMain.AdjustWindowsListCaptions
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    m_sCaption = ctlView.GetCaption & " - [" & m_sFileName & "]"
    m_sShortCaption = ctlView.GetShortCaption
    Me.Caption = IIf(m_nWindowIndex > 0 And m_nWindowIndex < 10, CStr(m_nWindowIndex) & ": ", "") & m_sCaption
End Sub

Private Sub ctlView_OnStateChange()
    On Error Resume Next
    mnuViewRefresh.Enabled = ctlView.Group.ID <> 0 And Not ctlView.InProc And Not ctlView.DataLoad And Not ctlView.LastQuoteReqNow
    mnuScnNew.Enabled = Not ctlView.InProc And Not ctlView.DataLoad And Not ctlView.LastQuoteReqNow
    mnuViewWtdVega.Enabled = Not ctlView.InProc And Not ctlView.WtdVegaIsOpened And Not ctlView.LastQuoteReqNow
    mnuScnSave.Enabled = mnuScnNew.Enabled
    mnuScnDelete.Enabled = mnuScnNew.Enabled
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Load Enter", Me.Caption
        
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Load Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    m_bShowInTaskbar = True
    mnuWindowShowInTaskbar.Checked = True
    lblStatus.Caption = ""
    lblProcess.Caption = ""
    lblValue.Caption = ""
    mnuToolsOptionScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    
    g_Params.SetIcon WND_RISKMATRIX, Me.hWnd, False
    
    
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Load Exit - Done", Me.Caption
        
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Resize Enter", Me.Caption
        
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogEnhDebug, "RiskMatrix View Resize Exit - Not Done", Me.Caption
        Exit Sub
    End If
    LockWindowUpdate Me.hWnd
    
    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height
    lblValue.Left = picBottom.ScaleWidth - lblValue.Width
    
    LockWindowUpdate 0
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Resize Exit - Done", Me.Caption
        
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Terminate Enter", Me.Caption
        
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Terminate Exit - Not Done", Me.Caption
       Exit Sub
    End If
    'ctlView.ScenarioSave False, False
    Set aParams = Nothing
    
    ctlView.StopNow
    ctlView.Term
    
    g_ViewFrm.Remove m_sKey
    frmMain.ReloadWindowsList
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Terminate Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "RiskMatrix View Unload", Me.Caption
    
    Form_Terminate
End Sub

Private Sub Form_Activate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Activation Enter", Me.Caption
    
    If gCmn Is Nothing Then Exit Sub
    If m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Activation Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Activation Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Deactivate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "RiskMatrix View Deactivate", Me.Caption
        
End Sub

Private Sub aParams_ActiveRealTimeChange()
    On Error Resume Next
End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub Save(ByVal sFilePath As String, ByVal sFileName As String)
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Save Enter", Me.Caption
    
    Dim aStorage As New clsSettingsStorage
    Screen.MousePointer = vbHourglass
    aStorage.Init sFilePath & sFileName
    aStorage.SetStringValue "Type", "Value", "RiskMatrix"
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

Private Sub mnuFileBatchReporting_Click()
    On Error Resume Next
    g_frmProjections.ShowData
End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Enter. ", Me.Caption
       
    Unload Me
       
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Exit. ", Me.Caption
  
End Sub

Private Sub mnuFIleGraphicalPnL_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
    g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->PnL Projections"" Enter. ", Me.Caption
    
    frmMain.ShowGraphicalPnLByFilter ctlView.Filter
    
    If Not g_PerformanceLog Is Nothing Then _
    g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->PnL Projections"" Exit. ", Me.Caption
End Sub

Private Sub mnuFileNew_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New"" Enter. ", Me.Caption
    
    frmMain.ShowRiskMatrix

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New"" Exit. ", Me.Caption

End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
'    ctlView.PrintView
End Sub

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Enter. ", Me.Caption
    
    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.Filter.Data(MFC_SYMBOL)

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes"" Exit. ", Me.Caption
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Reports"" Enter. ", Me.Caption
   
    frmMain.ShowReportsByFilter ctlView.Filter
    
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

    frmMain.ShowVolaMonitorByFilter ctlView.Filter

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

    frmMain.ShowVolaAnalisisByFilter ctlView.Filter

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Analysis"" Exit. ", Me.Caption

End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks"" Enter. ", Me.Caption
 
    frmMain.ShowGreeksByFilter ctlView.Filter

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks"" Exit. ", Me.Caption
        
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary View"" Enter. ", Me.Caption

    frmMain.ShowHedgeSummaryByFilter ctlView.Filter
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary View"" Exit. ", Me.Caption

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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Enter. ", Me.Caption
        
    If m_bIsNew Then
        With dlgCommon
            .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                    Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
            .CancelError = True
            .DefaultExt = ".mvw"
            .DialogTitle = "Save Risk Matrix"
            .InitDir = ""
            .FileName = m_sFilePath & m_sFileName
            .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Exit. ", Me.Caption
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save risk matrix."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save"" Exit with an error. ", Me.Caption

End Sub

Private Sub mnuFileSaveAs_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Enter. ", Me.Caption
        
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
        .CancelError = True
        .DefaultExt = ".mvw"
        .DialogTitle = "Save Risk Matrix As"
        .InitDir = ""
        .FileName = m_sFilePath & m_sFileName
        .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowSave
        If Err.Number = 0 Then
            On Error GoTo EH
            Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Exit. ", Me.Caption

    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save risk matrix."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As"" Exit with an errors. ", Me.Caption

End Sub

Private Sub mnuFileOpen_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Enter. ", Me.Caption

    Dim aStorage As New clsSettingsStorage
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
        .CancelError = True
        .DefaultExt = ".mvw"
        .DialogTitle = "Open Risk Matrix"
        .FileName = ""
        .InitDir = m_sFilePath
        .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowOpen
        If Err.Number = 0 Then
            On Error GoTo EH
            Screen.MousePointer = vbHourglass
            aStorage.Init .FileName
            aStorage.ReadData
            If aStorage.GetStringValue("Type", "Value") = "RiskMatrix" Then
                ctlView.OpenFromFile aStorage, ""
                m_sFilePath = ExtractFilePath(.FileName)
                m_sFileName = ExtractFileName(.FileName)
                m_bIsNew = False
                AdjustCaption
                Screen.MousePointer = vbDefault
            Else
                Screen.MousePointer = vbDefault
                gCmn.MyMsgBox Me, "Invalid risk matrix file."
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Exit. ", Me.Caption
        
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open risk matrix."
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open"" Exit with an errors. ", Me.Caption

End Sub

Private Sub mnuReloadGroupsSpectificRatio_Click()
On Error GoTo error_exception

    Dim sFileName As String
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage->Reload Group Specific Ratio"" Enter. ", Me.Caption
            
        ctlView.LoadGroupSpecificRatio
        
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage->Reload Group Specific Ratio"" Exit. ", Me.Caption
        
Exit Sub
error_exception:
    gCmn.ErrorMsgBox Me, "Fail to Reload Group Specific Ratio: " & Err.Description
End Sub

Private Sub mnuScnDelete_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->Delete"" Enter. ", Me.Caption
        
    ctlView.ScenarioDelete
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->Delete"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuScnNew_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->New"" Enter. ", Me.Caption
        
    ctlView.ScenarioNew
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->New"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuScnSave_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->Save"" Enter. ", Me.Caption
    
    ctlView.ScenarioSave True, True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Scenario->Save"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters"" Enter. ", Me.Caption
   
    frmMain.ShowParameters
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters"" Exit. ", Me.Caption
   
End Sub

Private Sub mnuViewRefresh_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Enter. ", Me.Caption
        
    ctlView.Refresh
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Exit. ", Me.Caption

End Sub

Private Sub mnuViewFltGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Filter Grid Layout"" Enter. ", Me.Caption

    ctlView.CustomizeFltGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Filter Grid Layout"" Exit. ", Me.Caption

End Sub

Private Sub mnuViewScnGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Scenario Grid Layout"" Enter. ", Me.Caption

    ctlView.CustomizeScnGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Scenario Grid Layout"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuViewValGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Values Grid Layout"" Enter. ", Me.Caption

    ctlView.CustomizeValGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Values Grid Layout"" Exit. ", Me.Caption
    
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Enter. ", Me.Caption

    ShowEventLog (Not mnuOptionsEventLog.Checked), , True

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Exit. ", Me.Caption

End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->BackOffice"" Enter. ", Me.Caption

    frmMain.RunBackOffice

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->BackOffice"" Exit. ", Me.Caption

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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager"" Enter. ", Me.Caption

    frmMain.RunVolaManager
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager"" Exit. ", Me.Caption
        
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Weighted Vega"" Enter. ", Me.Caption

    ctlView.ShowWeightedVegaSettings
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Weighted Vega"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowArrange_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Arrange"" Enter. ", Me.Caption
        
    frmMain.WindowsArrange
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Arrange"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowCloseAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Close All"" Enter. ", Me.Caption

    frmMain.WindowsCloseAll
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Close All"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowList_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->List"" Enter. ", Me.Caption

    frmMain.WindowsList
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->List"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowMain_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main"" Enter. ", Me.Caption

    ShowWindow frmMain.hWnd, SW_SHOWNORMAL
    frmMain.SetFocus
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowTileHorizontaly_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Horizontaly"" Enter. ", Me.Caption
        
    frmMain.WindowsTileHorizontally
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Horizontaly"" Exit. ", Me.Caption

End Sub

Private Sub mnuWindowTileVertically_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Vertically"" Enter. ", Me.Caption

    frmMain.WindowsTileVertically
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Verticaly"" Exit. ", Me.Caption

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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Show In Task Bar"" Exit. ", Me.Caption

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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Help->Contents"" Enter. ", Me.Caption

    frmMain.HelpContents
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Help->Contents"" Exit. ", Me.Caption

End Sub

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Underlyings"" Enter. ", Me.Caption
        
    frmMain.ShowManageUnderlyings
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Underlyings"" Enter. ", Me.Caption

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

Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Exercise Screen"" Enter. ", Me.Caption

    frmMain.ShowExerciseScreen
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage Exercise Screen"" Enter. ", Me.Caption

End Sub

Public Function GroupSpecificRatioFileDialog() As String
    On Error Resume Next

    GroupSpecificRatioFileDialog = STR_NA

    dlgCommon.Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                                     Or cdlOFNHideReadOnly Or cdlOFNFileMustExist

    dlgCommon.CancelError = True
    dlgCommon.DefaultExt = ".txt"
    dlgCommon.DialogTitle = "Load Group Ratio file"
    dlgCommon.FileName = ""
    dlgCommon.InitDir = g_Params.CurrentUserPrivateFolder
    dlgCommon.Filter = "Group Specific Ratio File (*.txt)|*.txt|All Files|*.*"
    dlgCommon.FilterIndex = 1

    dlgCommon.ShowOpen
    If Err.Number = 0 Then
        GroupSpecificRatioFileDialog = dlgCommon.FileName
    End If

End Function

