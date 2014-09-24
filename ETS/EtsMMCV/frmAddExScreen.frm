VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Begin VB.Form frmAddExScreen 
   Caption         =   "Exercise Screen"
   ClientHeight    =   4890
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   12555
   Icon            =   "frmAddExScreen.frx":0000
   LinkTopic       =   "Add Exercise Screen"
   ScaleHeight     =   4890
   ScaleWidth      =   12555
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton btnUnselectAll 
      Caption         =   "Unselect All"
      Height          =   375
      Left            =   8190
      TabIndex        =   7
      Top             =   3045
      Width           =   1335
   End
   Begin VB.CommandButton btnApply 
      Caption         =   "Apply"
      Height          =   375
      Left            =   6600
      TabIndex        =   6
      Top             =   3000
      Width           =   1335
   End
   Begin VB.CommandButton btnLeaveAsItIs 
      Caption         =   "Select All"
      Height          =   375
      Left            =   1320
      TabIndex        =   1
      Top             =   2880
      Width           =   1335
   End
   Begin MSComctlLib.ProgressBar ProgressBar1 
      Height          =   255
      Left            =   120
      TabIndex        =   4
      Top             =   2400
      Width           =   2055
      _ExtentX        =   3625
      _ExtentY        =   450
      _Version        =   393216
      Appearance      =   1
   End
   Begin EtsMMCv.cltAddExScreen cltAddExScreen1 
      Height          =   1815
      Left            =   120
      TabIndex        =   0
      Top             =   240
      Width           =   7095
      _ExtentX        =   7223
      _ExtentY        =   5106
   End
   Begin MSForms.CommandButton btnOK 
      Height          =   375
      Left            =   3120
      TabIndex        =   2
      Top             =   2880
      Width           =   1215
      Caption         =   "OK"
      Size            =   "2143;661"
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
      ParagraphAlign  =   3
   End
   Begin VB.Label stStatusBar 
      Caption         =   "Label1"
      Height          =   255
      Left            =   2040
      TabIndex        =   5
      Top             =   3480
      Width           =   4575
   End
   Begin MSForms.CommandButton btnCancel 
      Height          =   375
      Left            =   4920
      TabIndex        =   3
      Top             =   3000
      Width           =   1095
      Caption         =   "Cancel"
      Size            =   "1931;661"
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
      ParagraphAlign  =   3
   End
   Begin VB.Menu mnuFile 
      Caption         =   "&File"
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
      Begin VB.Menu mnuFileSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFileClose 
         Caption         =   "&Close"
      End
   End
   Begin VB.Menu mnuView 
      Caption         =   "&View"
      Begin VB.Menu mnRefresh 
         Caption         =   "Refresh"
         Shortcut        =   {F5}
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsParameters 
         Caption         =   "&Parameters..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuOptionsEventLog 
         Caption         =   "&Event Log"
         Shortcut        =   ^L
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
Attribute VB_Name = "frmAddExScreen"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Created by Artem Shiliyaev
Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1

Private m_bShowInTaskbar As Boolean
Private m_bShowInTaskbarChanging As Boolean

Private m_sKey As String

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

Public Sub Init(sKey As String)
    Set cltAddExScreen1.m_frmOwner = Me
    Set cltAddExScreen1.m_PrPrProgressBar = ProgressBar1
    Set cltAddExScreen1.m_SattusBar = stStatusBar
    cltAddExScreen1.Init True
    
    m_sKey = sKey
End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub btnApply_Click()
    cltAddExScreen1.ExecteTrade
    Reload
End Sub

Private Sub btnCancel_Click()
    Unload Me
End Sub

Private Sub btnLeaveAsItIs_Click()
    cltAddExScreen1.SelectAsItIsAll
End Sub

Private Sub btnOK_Click()
    cltAddExScreen1.ExecteTrade
    Unload Me
End Sub


Private Sub btnUnselectAll_Click()
    cltAddExScreen1.UnselectAll
End Sub

Private Sub Form_Activate()
    On Error Resume Next
    
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If
    
    frmMain.AdjustWindowsListCaptions
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    'If KeyCode = vbKeyF5 Then
    '    cltAddExScreen1.Init
    'End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    
    m_bShowInTaskbar = True
    mnuWindowShowInTaskbar.Checked = m_bShowInTaskbar
    
    mnuToolsOptionsScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable

    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
End Sub

Private Sub Form_Resize()
'    On Error Resume Next
    On Error GoTo EH
    If gCmn Is Nothing Then Exit Sub
    LockWindowUpdate Me.hWnd

    Dim lW As Integer, lH As Integer
    lW = btnOK.Width
    lH = btnOK.Height
    
    If ScaleHeight > 0 And ScaleWidth > 0 Then
        cltAddExScreen1.Move 20, 20, ScaleWidth - 40, ScaleHeight - lH - 90
    End If
    
    ProgressBar1.Move 100, ScaleHeight - lH - 80, ScaleWidth / 7, lH / 3
    stStatusBar.Move ScaleWidth / 6, ScaleHeight - lH - 80, ScaleWidth - ScaleWidth / 2 - 2 * lW - 100, lH
    
    btnUnselectAll.Move ScaleWidth - lW - 150, ScaleHeight - lH - 40, lW, lH
    btnLeaveAsItIs.Move ScaleWidth - 2 * lW - 200, ScaleHeight - lH - 40, lW, lH
    btnCancel.Move ScaleWidth - 3 * lW - 250, ScaleHeight - lH - 40, lW, lH
    btnOK.Move ScaleWidth - 4 * lW - 300, ScaleHeight - lH - 40, lW, lH
    btnApply.Move ScaleWidth - 5 * lW - 350, ScaleHeight - lH - 40, lW, lH
    
    cltAddExScreen1.AutoSizeGrig
    
    LockWindowUpdate 0
    Exit Sub
EH:
End Sub

Private Sub Form_Terminate()
    Set g_frmEarlyExer = Nothing
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    Set aParams = Nothing
    Set g_frmEarlyExer = Nothing
    g_ViewFrm.Remove m_sKey
    frmMain.ReloadWindowsList
End Sub


Private Sub mnRefresh_Click()
    Reload
End Sub

Private Sub Reload()
    Dim dtSelExp As Date
    dtSelExp = cltAddExScreen1.GetSelectDate
    cltAddExScreen1.Init False
    cltAddExScreen1.SetSelectedDate dtSelExp
    cltAddExScreen1.FilterUpdate
End Sub


Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    frmMain.ShowExerciseScreen
End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
    Unload Me
End Sub

Private Sub mnuFileGraphicalPnL_Click()
    On Error Resume Next
    'frmMain.ShowGraphicalPnL
End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
    frmMain.ShowGreeks
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    frmMain.ShowHedgeSummary
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
    frmMain.ShowIndex1Hedge
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
    frmMain.ShowIndex2Hedge
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

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
    frmMain.ShowQuotes
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    frmMain.ShowReports
End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
    frmMain.ShowRiskMatrix
End Sub

Private Sub mnuFileRisks_Click()
    On Error Resume Next
    frmMain.ShowRisks
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
    frmMain.ShowStockHedge
End Sub

Private Sub mnuFileTrades_Click()
    On Error Resume Next
    frmMain.ShowTrades
End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    frmMain.ShowVola
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    frmMain.ShowVolaAnalysis
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    frmMain.HelpContents
End Sub

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    frmMain.ShowManageUnderlyings
End Sub

Private Sub mnuManualVols_Click()
    On Error Resume Next
    'frmMain.ShowManualVols
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    ShowEventLog (Not mnuOptionsEventLog.Checked), , True
End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    frmMain.ShowParameters
End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    frmMain.RunBackOffice
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

Private Sub mnuWindowArrange_Click()
    On Error Resume Next
    frmMain.WindowsArrange
End Sub

Private Sub mnuWindowCascade_Click()
    On Error Resume Next
    frmMain.WindowsCascade
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

Private Sub mnuWindowShowInTaskBar_Click()
    On Error Resume Next
    frmMain.WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar
End Sub

Private Sub mnuWindowTileHorizontaly_Click()
    On Error Resume Next
    frmMain.WindowsTileHorizontally
End Sub

Private Sub mnuWindowTileVertically_Click()
    On Error Resume Next
    frmMain.WindowsTileVertically
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

Public Function GetCaption() As String
    On Error Resume Next
    GetCaption = Me.Caption
End Function

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = Me.Caption
End Function

Private Sub mnuWindowGo_Click(Index As Integer)
    On Error Resume Next
    frmMain.WindowsGo Index
End Sub

Private Sub TerminateForm()
    cltAddExScreen1.TerminateCtl
End Sub
