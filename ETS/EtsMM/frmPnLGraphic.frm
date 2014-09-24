VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmPnLGraphic 
   Caption         =   "PnL Projections"
   ClientHeight    =   8310
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   11880
   Icon            =   "frmPnLGraphic.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   8310
   ScaleWidth      =   11880
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picBottom 
      BorderStyle     =   0  'None
      Height          =   321
      Left            =   105
      ScaleHeight     =   315
      ScaleWidth      =   6210
      TabIndex        =   2
      TabStop         =   0   'False
      Top             =   9240
      Width           =   6210
      Begin MSComctlLib.ProgressBar pbProgress 
         Height          =   240
         Left            =   30
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
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2400
         TabIndex        =   5
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         Height          =   195
         Left            =   30
         TabIndex        =   4
         Top             =   60
         Width           =   45
      End
      Begin VB.Image imgStop 
         Height          =   240
         Left            =   2070
         MouseIcon       =   "frmPnLGraphic.frx":000C
         MousePointer    =   99  'Custom
         Picture         =   "frmPnLGraphic.frx":015E
         ToolTipText     =   "Cancel"
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgStopDis 
         Enabled         =   0   'False
         Height          =   240
         Left            =   2070
         Picture         =   "frmPnLGraphic.frx":02A8
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
   End
   Begin EtsMM.ctlPnLGraphic ctlView 
      Height          =   9150
      Left            =   105
      TabIndex        =   0
      Top             =   105
      Width           =   10620
      _extentx        =   18733
      _extenty        =   16140
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   225
      Left            =   0
      TabIndex        =   1
      Top             =   8085
      Width           =   11880
      _ExtentX        =   20955
      _ExtentY        =   397
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
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
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save"
         Shortcut        =   ^S
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileSaveAs 
         Caption         =   "Save &As..."
         Visible         =   0   'False
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
      Begin VB.Menu mnuFileGraphicalPnL 
         Caption         =   "PnL Projections..."
         Shortcut        =   ^A
      End
      Begin VB.Menu mnuFileHedgeSummaryView 
         Caption         =   "&Hedge Summary..."
         Shortcut        =   ^H
      End
      Begin VB.Menu mnuSep3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileBatchReporting 
         Caption         =   "Batch Reporting ..."
      End
      Begin VB.Menu mnSep1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileReportsView 
         Caption         =   "Re&ports..."
         Shortcut        =   ^J
      End
      Begin VB.Menu mnuFileOrders 
         Caption         =   "Or&ders..."
         Enabled         =   0   'False
         Shortcut        =   ^B
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileTntCards 
         Caption         =   "Trades Messa&ges..."
         Enabled         =   0   'False
         Shortcut        =   ^U
         Visible         =   0   'False
      End
      Begin VB.Menu mnuSeparator2 
         Caption         =   "-"
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "Print..."
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
      Begin VB.Menu mnuToolsBackOffice 
         Caption         =   "&Back Office..."
      End
      Begin VB.Menu mnuToolsHedger 
         Caption         =   "&Hedger..."
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuToolsStrategyScanner 
         Caption         =   "&Strategy Scanner..."
      End
      Begin VB.Menu mnuToolsOptionsScanner 
         Caption         =   "&Options Scanner..."
      End
      Begin VB.Menu mnuSeparator3 
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
   End
   Begin VB.Menu mnuManage 
      Caption         =   "&Manage"
      Begin VB.Menu mnuManageUnderlyings 
         Caption         =   "&Underlyings..."
      End
      Begin VB.Menu mnExerciseScreen 
         Caption         =   "Exercise Screen"
      End
      Begin VB.Menu mnuSeparator11 
         Caption         =   "--"
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
   Begin VB.Menu mnuOrder 
      Caption         =   "O&rder"
      Enabled         =   0   'False
      Visible         =   0   'False
      Begin VB.Menu mnuOrderNewStock 
         Caption         =   "New &Stock Order..."
         Enabled         =   0   'False
      End
      Begin VB.Menu mnuOrderNewOption 
         Caption         =   "New &Option Order..."
         Enabled         =   0   'False
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
Attribute VB_Name = "frmPnLGraphic"
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

Private Const m_nFormMinHeight As Long = 9655
Private Const m_nFormMinWidth As Long = 11565

Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean
Public Sub ShowDataByFilter(ByRef aFilters As EtsGeneralLib.EtsFilterData)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmPnLGraphics ShowData Call", Me.Caption

    ctlView.ShowDataByFilter aFilters

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "frmPnLGraphics ShowData Done", Me.Caption

End Sub
Public Sub ShowWindowInTaskbar(ByVal bShow As Boolean, Optional ByVal bExtended As Boolean = False)
    On Error Resume Next
    If m_bShowInTaskbar <> bShow Then
        m_bShowInTaskbar = bShow
        mnuWindowShowInTaskbar.Checked = bShow
        m_bShowInTaskbarChanging = True
        
        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_HIDEWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
        ShowFormInTaskBar Me, m_bShowInTaskbar
        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_SHOWWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
        
        m_bShowInTaskbarChanging = False
    End If
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
    
    EnableActiveRealTime = False
    m_sFilePath = MakeCommonFilePath("", "", False)
    m_sCaption = ""
    m_sShortCaption = ""
    g_nGraphicalPnLUntitledIdx = g_nGraphicalPnLUntitledIdx + 1
    m_sFileName = "Untitled" & g_nGraphicalPnLUntitledIdx & ".gvw"
    m_bIsNew = True
    m_sKey = sKey
    mnuFileClose.Caption = "Close" & vbTab & "Alt+F4"
'    mnuOrderNewStock.Caption = "New &Stock Order..." & vbTab & "Alt+Ins"
'    mnuOrderNewOption.Caption = "New &Option Order..." & vbTab & "Ctrl+Alt+Ins"
'    mnuTntCardNew.Caption = "&New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set ctlView.m_frmOwner = Me
    Set ctlView.pbProgress = pbProgress
    Set ctlView.lblProcess = lblProcess
    Set ctlView.lblStatus = lblStatus
    Set ctlView.lblValue = lblValue
    Set ctlView.imgStop = imgStop
    Set ctlView.imgStopDis = imgStopDis
    
    ctlView.Init
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'    mnuFileQuotation.Enabled = g_bIseEnable
End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    ctlView.ShowData nType, nValue
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
'    mnuScnNew.Enabled = Not ctlView.InProc And Not ctlView.DataLoad And Not ctlView.LastQuoteReqNow
'    mnuViewWtdVega.Enabled = Not ctlView.InProc And Not ctlView.WtdVegaIsOpened And Not ctlView.LastQuoteReqNow
'    mnuScnSave.Enabled = mnuScnNew.Enabled
'    mnuScnDelete.Enabled = mnuScnNew.Enabled
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    m_bShowInTaskbar = True
    mnuWindowShowInTaskbar.Checked = True
    lblStatus.Caption = ""
    lblProcess.Caption = ""
    lblValue.Caption = ""
    mnuToolsOptionsScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    
    g_Params.SetIcon WND_PNLPROJECTIONS, Me.hWnd, False

End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    LockWindowUpdate Me.hWnd
    

    If Height < m_nFormMinHeight Then Height = m_nFormMinHeight
    If Width < m_nFormMinWidth Then Width = m_nFormMinWidth

    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height
    lblValue.Left = picBottom.ScaleWidth - lblValue.Width

    LockWindowUpdate 0

End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    ctlView.Refresh
'    ctlView.ScenarioSave False, False
    Set aParams = Nothing

    ctlView.StopNow
    ctlView.Term

    g_ViewFrm.Remove m_sKey
    frmMain.ReloadWindowsList
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Form_Terminate
End Sub

Private Sub Form_Activate()
    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If m_bShowInTaskbarChanging Then Exit Sub
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If
End Sub

Private Sub Form_Deactivate()
    On Error Resume Next
End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub Save(ByVal sFilePath As String, ByVal sFileName As String)
'    Dim aStorage As New clsSettingsStorage
'    Screen.MousePointer = vbHourglass
'    aStorage.Init sFilePath & sFileName
'    aStorage.SetStringValue "Type", "Value", "RiskMatrix"
'    ctlView.SaveToFile aStorage, ""
'    aStorage.SaveData
'    m_sFilePath = sFilePath
'    m_sFileName = sFileName
'    m_bIsNew = False
'    AdjustCaption
'    Screen.MousePointer = vbDefault
End Sub

Public Sub SaveLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
'    ctlView.SaveToFile aStorage, sKey
End Sub

Public Sub OpenLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
'    ctlView.OpenFromFile aStorage, sKey
End Sub


Private Sub mnExerciseScreen_Click()
    frmMain.ShowExerciseScreen
End Sub



Private Sub mnuFileBatchReporting_Click()
    On Error Resume Next
    g_frmProjections.ShowData
End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub mnuFIleGraphicalPnL_Click()
    On Error Resume Next
    frmMain.ShowGraphicalPnL
End Sub

Private Sub mnuFileNew_Click()
    On Error Resume Next
    frmMain.ShowGraphicalPnL
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
    ctlView.PrintView
End Sub

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    frmMain.ShowReports TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
    frmMain.ShowRiskMatrix TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileRisks_Click()
    On Error Resume Next
    frmMain.ShowRisks TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileTrades_Click()
    On Error Resume Next
    frmMain.ShowTrades TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    frmMain.ShowVola TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileQuotation_Click()
    On Error Resume Next
    frmMain.ShowQuotation TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    frmMain.ShowVolaAnalysis TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
    frmMain.ShowGreeks TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    frmMain.ShowHedgeSummary TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileOrders_Click()
    On Error Resume Next
    frmMain.ShowOrders TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileTntCards_Click()
    On Error Resume Next
'    frmMain.ShowTntCards ctlView.Group.GroupType, ctlView.Group.ID
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
    frmMain.ShowStockHedge TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
    frmMain.ShowIndex1Hedge TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
    frmMain.ShowIndex2Hedge TYPE_UNDERLYING, ctlView.Group.ID
End Sub

Private Sub mnuFileOtcOptionCalc_Click()
    On Error GoTo EH
    Dim sPath As String
    Dim sParams As String
    Dim lCalcModel As Long
    
    lCalcModel = CLng(g_Params.CalcModel)
    sPath = App.Path & "\OTCCalc\OTCOptionCalc.exe"
    sParams = "0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0 " & CStr(lCalcModel)

    If ShellExecute(0&, "Open", sPath, sParams, "", SW_SHOWNORMAL) <= 32 Then
         gCmn.MyMsgBox Me, "Fail to open OTC OptionCalc at '" & sPath & "'.", vbCritical
    End If

    Exit Sub
EH:
    gCmn.ErrorMsgBox Me, "Fail to open OTC Option Calc."
End Sub

Private Sub mnuFileSave_Click()
'    On Error GoTo EH
'
'    If m_bIsNew Then
'        With dlgCommon
'            .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                    Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
'            .CancelError = True
'            .DefaultExt = ".mvw"
'            .DialogTitle = "Save Risk Matrix"
'            .InitDir = ""
'            .FileName = m_sFilePath & m_sFileName
'            .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
'            .FilterIndex = 1
'            On Error Resume Next
'            .ShowSave
'            If Err.Number = 0 Then
'                On Error GoTo EH
'                Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
'            End If
'        End With
'    Else
'        Save m_sFilePath, m_sFileName
'    End If
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to save risk matrix."
End Sub

Private Sub mnuFileSaveAs_Click()
'    On Error GoTo EH
'
'    With dlgCommon
'        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
'        .CancelError = True
'        .DefaultExt = ".mvw"
'        .DialogTitle = "Save Risk Matrix As"
'        .InitDir = ""
'        .FileName = m_sFilePath & m_sFileName
'        .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
'        .FilterIndex = 1
'        On Error Resume Next
'        .ShowSave
'        If Err.Number = 0 Then
'            On Error GoTo EH
'            Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
'        End If
'    End With
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to save risk matrix."
End Sub

Private Sub mnuFileOpen_Click()
'    On Error GoTo EH
'    Dim aStorage As New clsSettingsStorage
'
'    With dlgCommon
'        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
'        .CancelError = True
'        .DefaultExt = ".mvw"
'        .DialogTitle = "Open Risk Matrix"
'        .FileName = ""
'        .InitDir = m_sFilePath
'        .Filter = "ETS Risk Matrix (*.mvw)|*.mvw|All Files|*.*"
'        .FilterIndex = 1
'        On Error Resume Next
'        .ShowOpen
'        If Err.Number = 0 Then
'            On Error GoTo EH
'            Screen.MousePointer = vbHourglass
'            aStorage.Init .FileName
'            aStorage.ReadData
'            If aStorage.GetStringValue("Type", "Value") = "RiskMatrix" Then
'                ctlView.OpenFromFile aStorage, ""
'                m_sFilePath = ExtractFilePath(.FileName)
'                m_sFileName = ExtractFileName(.FileName)
'                m_bIsNew = False
'                AdjustCaption
'                Screen.MousePointer = vbDefault
'            Else
'                Screen.MousePointer = vbDefault
'                gCmn.MyMsgBox Me, "Invalid risk matrix file."
'            End If
'        End If
'    End With
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to open risk matrix."
End Sub

'Private Sub mnuScnDelete_Click()
'    On Error Resume Next
'    ctlView.ScenarioDelete
'End Sub
'
'Private Sub mnuScnNew_Click()
'    On Error Resume Next
'    ctlView.ScenarioNew
'End Sub
'
'Private Sub mnuScnSave_Click()
'    On Error Resume Next
'    ctlView.ScenarioSave True, True
'End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    frmMain.ShowParameters
End Sub

Private Sub mnuViewRefresh_Click()
    On Error Resume Next
    ctlView.Refresh
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    ShowEventLog (Not mnuOptionsEventLog.Checked), , True
End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    frmMain.RunBackOffice
End Sub

Private Sub mnuToolsHedger_Click()
    On Error Resume Next
'    frmMain.RunHedger
End Sub

Private Sub mnuToolsVolaManager_Click()
    On Error Resume Next
    frmMain.RunVolaManager
End Sub

Private Sub mnuToolsOptionsScanner_Click()
    On Error Resume Next
    frmMain.RunOptionScanner
End Sub

Private Sub mnuToolsStrategyScanner_Click()
    On Error Resume Next
    frmMain.RunStrategyScanner
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

Private Sub mnuWindowArrange_Click()
    On Error Resume Next
    frmMain.WindowsArrange
End Sub

Private Sub mnuWindowCloseAll_Click()
    On Error Resume Next
    frmMain.WindowsCloseAll
End Sub

Private Sub mnuWindowList_Click()
    On Error Resume Next
    frmMain.WindowsList
End Sub

Private Sub mnuWindowMain_Click()
    On Error Resume Next
    ShowWindow frmMain.hWnd, SW_SHOWNORMAL
    frmMain.SetFocus
End Sub

Private Sub mnuWindowTileHorizontaly_Click()
    On Error Resume Next
    frmMain.WindowsTileHorizontally
End Sub

Private Sub mnuWindowTileVertically_Click()
    On Error Resume Next
    frmMain.WindowsTileVertically
End Sub

Private Sub mnuWindowCascade_Click()
    On Error Resume Next
    frmMain.WindowsCascade
End Sub

Private Sub mnuWindowShowInTaskBar_Click()
    On Error Resume Next
    frmMain.WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar
End Sub

Private Sub mnuWindowGo_Click(Index As Integer)
    On Error Resume Next
    frmMain.WindowsGo Index
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    'ctlView.SetGridLayout enGridType, gdGrid
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    frmMain.HelpContents
End Sub

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    frmMain.ShowManageUnderlyings
End Sub

Private Sub mnuOrderNewOption_Click()
    On Error Resume Next
'    frmOrderEntry.NewOptionOrder Nothing, Nothing, True, 10, 0#
End Sub

Private Sub mnuOrderNewStock_Click()
    On Error Resume Next
'    frmOrderEntry.NewStockOrder Nothing, True, 100, 0#
End Sub

Private Sub mnuTntCardNew_Click()
    On Error Resume Next
'    frmTntCardEntry.NewCard Me, Nothing, Nothing, True, 10, 0#
End Sub

'Private Sub mnVMESettings_Click()
'    frmMain.ShowVMESettings
'End Sub
