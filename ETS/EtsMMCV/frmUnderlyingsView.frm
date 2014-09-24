VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmUnderlyingsView 
   Caption         =   "Underlyings"
   ClientHeight    =   6735
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   8805
   Icon            =   "frmUnderlyingsView.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   6735
   ScaleWidth      =   8805
   StartUpPosition =   3  'Windows Default
   Begin EtsMMCv.ctlUnderlyingsView ctlView 
      Height          =   3495
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   6735
      _extentx        =   11880
      _extenty        =   6165
   End
   Begin VB.PictureBox picBottom 
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   300
      Left            =   60
      ScaleHeight     =   300
      ScaleWidth      =   3735
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   5940
      Width           =   3735
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
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2040
         TabIndex        =   5
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         Caption         =   "Status"
         Height          =   195
         Left            =   30
         TabIndex        =   4
         Top             =   60
         Visible         =   0   'False
         Width           =   450
      End
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   2
      Top             =   6435
      Width           =   8805
      _ExtentX        =   15531
      _ExtentY        =   529
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
      EndProperty
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   5640
      Top             =   5760
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
      Begin VB.Menu mnuFileSave 
         Caption         =   "&Save Changes"
         Shortcut        =   ^S
      End
      Begin VB.Menu mnuExportScreen 
         Caption         =   "Export Screen"
      End
      Begin VB.Menu mnuSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileTrades 
         Caption         =   "&Trades..."
         Enabled         =   0   'False
         Shortcut        =   ^T
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileRisks 
         Caption         =   "&Risks..."
         Shortcut        =   ^R
      End
      Begin VB.Menu mnuFileQuotes 
         Caption         =   "&Quotes..."
         Enabled         =   0   'False
         Shortcut        =   ^Q
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileRiskMatrix 
         Caption         =   "Risk &Matrix..."
         Enabled         =   0   'False
         Shortcut        =   ^M
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVola 
         Caption         =   "Volat&ility Monitor..."
         Enabled         =   0   'False
         Shortcut        =   ^I
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileQuotation 
         Caption         =   "ISE M&arket Maker..."
         Enabled         =   0   'False
         Shortcut        =   ^K
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileVolaAnalysis 
         Caption         =   "Volatilit&y Analysis..."
         Enabled         =   0   'False
         Shortcut        =   ^Y
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileGreeks 
         Caption         =   "Gr&eeks..."
         Shortcut        =   ^E
      End
      Begin VB.Menu mnuFileHedgeSummaryView 
         Caption         =   "&Hedge Summary..."
         Enabled         =   0   'False
         Shortcut        =   ^H
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFileReportsView 
         Caption         =   "Re&ports..."
         Enabled         =   0   'False
         Shortcut        =   ^J
         Visible         =   0   'False
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
      Begin VB.Menu mnuViewFltGridLayout 
         Caption         =   "&Filter Grid Layout..."
      End
      Begin VB.Menu mnuViewUndGridLayout 
         Caption         =   "&Underlyings Grid Layout..."
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsParameters 
         Caption         =   "&Parameters..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuSeparator6 
         Caption         =   "-"
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
   Begin VB.Menu mnuTools 
      Caption         =   "&Tools"
      Begin VB.Menu mnuToolsVolaManager 
         Caption         =   "&Volatility Manager..."
      End
      Begin VB.Menu mnuToolsBackOffice 
         Caption         =   "ETS &Back Office..."
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
Attribute VB_Name = "frmUnderlyingsView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Private m_sKey As String
Private m_nWindowIndex As Long
Private m_sCaption As String
Private m_sShortCaption As String

Private m_sExportFileName As String

Public EnableActiveRealTime As Boolean

Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean

Public Sub ShowWindowInTaskbar(ByVal bShow As Boolean, Optional ByVal bExtended As Boolean = False)
    On Error Resume Next
    If m_bShowInTaskbar <> bShow Then
        m_bShowInTaskbar = bShow
        mnuWindowShowInTaskBar.Checked = bShow
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
    
    EnableActiveRealTime = True
    m_sCaption = ""
    m_sShortCaption = ""
    m_sKey = sKey
    mnuFileClose.Caption = "Close" & vbTab & "Alt+F4"
'    mnuOrderNewStock.Caption = "New &Stock Order..." & vbTab & "Alt+Ins"
'    mnuOrderNewOption.Caption = "New &Option Order..." & vbTab & "Ctrl+Alt+Ins"
'    mnuTntCardNew.Caption = "&New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    Set ctlView.m_frmOwner = Me
    Set ctlView.pbProgress = pbProgress
    Set ctlView.lblProcess = lblProcess
    Set ctlView.lblStatus = lblStatus
    
    ctlView.Init
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'    mnuFileQuotation.Enabled = g_bIseEnable
End Sub

Public Sub ShowData()
    On Error Resume Next
    ctlView.ShowData
End Sub

Private Sub aParams_NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    ctlView.NewUnderlyingAdded nNewUndID
End Sub

Private Sub aParams_NewUnderlyingGroupAdded(ByVal nNewUndGroupID As Long)
    On Error Resume Next
    ctlView.NewUnderlyingGroupAdded nNewUndGroupID
End Sub

Private Sub ctlView_OnSetCaption()
    On Error Resume Next
    AdjustCaption
    frmMain.AdjustWindowsListCaptions
End Sub

Private Sub AdjustCaption()
    On Error Resume Next
    m_sCaption = ctlView.GetCaption
    If ctlView.IsDirty Then m_sCaption = m_sCaption & "*"
    
    m_sShortCaption = ctlView.GetShortCaption
    Me.Caption = IIf(m_nWindowIndex > 0 And m_nWindowIndex < 10, CStr(m_nWindowIndex) & ": ", "") & m_sCaption
End Sub

Private Sub ctlView_OnStateChange()
    On Error Resume Next
    mnuViewRefresh.Enabled = Not ctlView.InProc
    mnuFileSave.Enabled = Not ctlView.InProc And ctlView.IsDirty
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Load Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Load Exit - Not Done", Me.Caption
        Exit Sub
    End If
    m_bShowInTaskbar = True
    mnuWindowShowInTaskBar.Checked = True
    lblStatus.Caption = ""
    lblProcess.Caption = ""

    mnuToolsOptionsScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Load Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Resize Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Resize Exit - Not Done", Me.Caption
        Exit Sub
    End If
    LockWindowUpdate Me.hWnd
    
    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height

    LockWindowUpdate 0
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Resize Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Terminate Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Terminate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    Set aParams = Nothing
    
    ctlView.Term
    
    If Not g_ViewFrm(m_sKey) Is Nothing Then
        g_ViewFrm.Remove m_sKey
        frmMain.ReloadWindowsList
    End If

    Set g_frmUnderlyings = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Terminate Exit - Done", Me.Caption

End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Underlyings View Terminate Unload", Me.Caption
    
    Form_Terminate
End Sub

Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Underlyings View QueryUnload Enter", Me.Caption
    
    If UnloadMode = vbFormControlMenu And ctlView.IsDirty Then
        Dim vbResult As VbMsgBoxResult
        vbResult = gCmn.MyMsgBox(Me, "Some underlyings' parameters was changed. Save changes?", vbQuestion + vbYesNoCancel)
        If vbResult = vbYes Then ctlView.SaveUnderlyings
        Cancel = (vbResult = vbCancel)
    Else
        Cancel = False
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Underlyings View QueryUnload Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Activate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Activate Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Activate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    If m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Activate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Activate Exit - Done", Me.Caption

End Sub

Private Sub Form_Deactivate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Underlyings View Deactivate", Me.Caption
    
End Sub

Private Sub aParams_ActiveRealTimeChange()
    On Error Resume Next
End Sub

Private Sub mnuExportScreen_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Export Screen"" Enter. ", Me.Caption
    
    Dim bRes As Boolean
    Dim vbRes As VbMsgBoxResult
        
    GenerateExportFileName True
    
    vbRes = gCmn.MyMsgBox(Me, "Export this screen to the file """ & g_Params.ScreenExportPath & m_sExportFileName & """?", vbQuestion + vbYesNo)
    If vbRes <> vbYes Then Exit Sub
    
    bRes = ctlView.ExportScreen(m_sExportFileName, g_Params.ScreenExportPath, , SE_REWRITE)
    
    If bRes Then
        vbRes = gCmn.MyMsgBox(Me, "Information has been saved in the file """ & g_Params.ScreenExportPath & m_sExportFileName & """", vbInformation + vbOKCancel, , , "Open")
        If vbRes = vbCancel Then
            ShellExecute 0&, "Open", g_Params.ScreenExportPath & m_sExportFileName, "", "", SW_SHOWNORMAL
        End If
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Export Screen"" Exit. ", Me.Caption
End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
'    ctlView.PrintView
End Sub

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    frmMain.ShowReports TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
    frmMain.ShowRiskMatrix TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileRisks_Click()
    On Error Resume Next
    frmMain.ShowRisks TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileSave_Click()
    On Error Resume Next
    ctlView.SaveUnderlyings
End Sub

Private Sub mnuFileTrades_Click()
    On Error Resume Next
    frmMain.ShowTrades TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    frmMain.ShowVola TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileQuotation_Click()
    On Error Resume Next
'    frmMain.ShowQuotation TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    frmMain.ShowVolaAnalysis TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
    frmMain.ShowGreeks TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    frmMain.ShowHedgeSummary TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileOrders_Click()
    On Error Resume Next
'    frmMain.ShowOrders TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileTntCards_Click()
    On Error Resume Next
'    frmMain.ShowTntCards TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
'    frmMain.ShowStockHedge TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex1Hedge TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex2Hedge TYPE_UNDERLYING, ctlView.CurUnderlyingID
End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    frmMain.ShowParameters
End Sub

Private Sub mnuViewRefresh_Click()
    On Error Resume Next
    ctlView.Refresh
End Sub

Private Sub mnuViewFltGridLayout_Click()
    On Error Resume Next
    ctlView.CustomizeFltGridLayout
End Sub

Private Sub mnuViewUndGridLayout_Click()
    On Error Resume Next
    ctlView.CustomizeUndGridLayout
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
    frmMain.RunOptionsScanner
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
    ctlView.SetGridLayout enGridType, gdGrid
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    frmMain.HelpContents
End Sub

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Public Sub Term()
    On Error Resume Next
    Form_Terminate
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

Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    frmMain.ShowExerciseScreen
End Sub

Private Sub GenerateExportFileName(Optional ByVal bUseTime As Boolean)
    On Error Resume Next
    
    m_sExportFileName = CurrentNtUserName & "_" & "underlyings"
    
    If bUseTime Then m_sExportFileName = m_sExportFileName & Format(Now, "_hhmmss")
    m_sExportFileName = m_sExportFileName & ".csv"
    
End Sub
