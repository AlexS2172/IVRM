VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmQuotationView 
   Caption         =   "ISE Market Maker"
   ClientHeight    =   6870
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   9690
   Icon            =   "frmQuotationView.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   6870
   ScaleWidth      =   9690
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picBottom 
      BorderStyle     =   0  'None
      Height          =   300
      Left            =   360
      ScaleHeight     =   300
      ScaleWidth      =   7620
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   5400
      Width           =   7620
      Begin MSComctlLib.ProgressBar pbProgress 
         Height          =   240
         Left            =   300
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
         Left            =   2340
         Picture         =   "frmQuotationView.frx":0E42
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   1
         Left            =   0
         Picture         =   "frmQuotationView.frx":0F8C
         ToolTipText     =   "Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Label lblValue 
         Alignment       =   1  'Right Justify
         AutoSize        =   -1  'True
         Caption         =   "Value"
         Height          =   195
         Left            =   4200
         TabIndex        =   6
         Top             =   60
         UseMnemonic     =   0   'False
         Width           =   405
      End
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2655
         TabIndex        =   4
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         Height          =   195
         Left            =   300
         TabIndex        =   3
         Top             =   60
         Width           =   45
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   0
         Left            =   0
         Picture         =   "frmQuotationView.frx":1516
         ToolTipText     =   "Not Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Image imgStop 
         Height          =   240
         Left            =   2340
         MouseIcon       =   "frmQuotationView.frx":1AA0
         MousePointer    =   99  'Custom
         Picture         =   "frmQuotationView.frx":1BF2
         ToolTipText     =   "Cancel"
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
   End
   Begin EtsMMCv.ctlQuotationView ctlView 
      Height          =   3735
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4575
      _ExtentX        =   8070
      _ExtentY        =   6588
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   6000
      Top             =   3960
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   5
      Top             =   6570
      Width           =   9690
      _ExtentX        =   17092
      _ExtentY        =   529
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
         Shortcut        =   ^K
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
         Shortcut        =   ^B
      End
      Begin VB.Menu mnuFileTntCards 
         Caption         =   "Trades Messa&ges..."
         Shortcut        =   ^U
      End
      Begin VB.Menu mnuFileStockHedge 
         Caption         =   "Sto&ck Hedge..."
         Shortcut        =   ^F
      End
      Begin VB.Menu mnuFileIndex1Hedge 
         Caption         =   "I&ndex Hedge..."
      End
      Begin VB.Menu mnuFileIndex2Hedge 
         Caption         =   "T&wo Indices Hedge..."
      End
      Begin VB.Menu mnuSeparator2 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFilePrint 
         Caption         =   "Print..."
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
      Begin VB.Menu mnuViewRealTime 
         Caption         =   "Real &Time Mode"
         Shortcut        =   {F9}
      End
      Begin VB.Menu mnuViewRefresh 
         Caption         =   "&Refresh"
         Shortcut        =   {F5}
      End
      Begin VB.Menu mnuSeparator4 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewDefOptSort 
         Caption         =   "Default Options &Sorting"
         Shortcut        =   ^D
      End
      Begin VB.Menu mnuSeparator5 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewCalcModel 
         Caption         =   "Calculation &Model"
      End
      Begin VB.Menu mnuViewExpCalendar 
         Caption         =   "Expiry &Calendar"
      End
      Begin VB.Menu mnuViewDividends 
         Caption         =   "&Dividends"
      End
      Begin VB.Menu mnuViewRates 
         Caption         =   "&Rates"
      End
      Begin VB.Menu mnuViewLog 
         Caption         =   "&Log"
      End
      Begin VB.Menu mnuSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewUndGridLayout 
         Caption         =   "&Underlying Grid Layout..."
      End
      Begin VB.Menu mnuViewOptGridLayout 
         Caption         =   "&Option Grid Layout..."
      End
      Begin VB.Menu mnuViewDivGridLayout 
         Caption         =   "Parameters &Grid Layout..."
      End
      Begin VB.Menu mnuViewLogGridLayout 
         Caption         =   "Log Grid La&yout..."
      End
      Begin VB.Menu mnuSeparator7 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewClearLog 
         Caption         =   "Cl&ear Log"
      End
   End
   Begin VB.Menu mnuExpiry 
      Caption         =   "&Expiry"
      Begin VB.Menu mnuExpiryNext 
         Caption         =   "&Next"
         Shortcut        =   {F8}
      End
      Begin VB.Menu mnuExpiryPrev 
         Caption         =   "&Previous"
         Shortcut        =   {F7}
      End
      Begin VB.Menu mnuExpiryAll 
         Caption         =   "&Show All"
         Shortcut        =   {F6}
      End
      Begin VB.Menu mnuExpirySeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 1>"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 2>"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 3>"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 4>"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 5>"
         Index           =   5
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 6>"
         Index           =   6
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 7>"
         Index           =   7
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 8>"
         Index           =   8
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryShow 
         Caption         =   "<exp 9>"
         Index           =   9
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExpiryMore 
         Caption         =   "&Expiries..."
         Shortcut        =   {F11}
      End
   End
   Begin VB.Menu mnuExch 
      Caption         =   "E&xchange"
      Begin VB.Menu mnuExchAllUnd 
         Caption         =   "Show &All Underlying"
         Shortcut        =   +^{F6}
      End
      Begin VB.Menu mnuExchSeparator3 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 1>"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 2>"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 3>"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 4>"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 5>"
         Index           =   5
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 6>"
         Index           =   6
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 7>"
         Index           =   7
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 8>"
         Index           =   8
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowUnd 
         Caption         =   "<exch 9>"
         Index           =   9
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchMoreUnd 
         Caption         =   "&Underlying Exchanges..."
         Shortcut        =   +^{F11}
      End
   End
   Begin VB.Menu mnuOptions 
      Caption         =   "&Options"
      Begin VB.Menu mnuOptionsParameters 
         Caption         =   "&Parameters..."
         Shortcut        =   ^P
      End
      Begin VB.Menu mnuSeparator8 
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
      Begin VB.Menu mnuWindowShowInTaskBar 
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
Attribute VB_Name = "frmQuotationView"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'Option Explicit
'
'Private WithEvents aParams As clsParams
'Private WithEvents frmItems As frmItemsList
'
'Private m_sKey As String
'Private m_sFilePath As String
'Private m_sFileName As String
'Private m_bIsNew As Boolean
'Private m_nWindowIndex As Long
'Private m_sCaption As String
'Private m_sShortCaption As String
'
'Public EnableActiveRealTime As Boolean
'
'Private m_bShowInTaskbar As Boolean
'Private m_bShowInTaskbarChanging As Boolean
'
'Public Sub ShowWindowInTaskbar(ByVal bShow As Boolean, Optional ByVal bExtended As Boolean = False)
'    On Error Resume Next
'    If m_bShowInTaskbar <> bShow Then
'        m_bShowInTaskbar = bShow
'        mnuWindowShowInTaskBar.Checked = bShow
'        m_bShowInTaskbarChanging = True
'
'        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_HIDEWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
'        ShowFormInTaskBar Me, m_bShowInTaskbar
'        If bExtended Then SetWindowPos Me.hWnd, 0, 0, 0, 0, 0, SWP_SHOWWINDOW Or SWP_NOSIZE Or SWP_NOMOVE Or SWP_NOZORDER Or SWP_NOACTIVATE
'
'        m_bShowInTaskbarChanging = False
'    End If
'End Sub
'
'Public Sub SetWinIndex(ByVal nIndex As Long)
'    On Error Resume Next
'    m_nWindowIndex = nIndex
'    AdjustCaption
'End Sub
'
'Public Function GetCaption() As String
'    On Error Resume Next
'    GetCaption = m_sCaption
'End Function
'
'Public Function GetShortCaption() As String
'    On Error Resume Next
'    GetShortCaption = m_sShortCaption
'End Function
'
'Private Sub aParams_InternalVolatilityChanged(ByVal sSymbol As String)
'    On Error Resume Next
'    ctlView.InternalVolatilityChanged sSymbol
'End Sub
'
'Private Sub ctlView_OnUndExchangesChange(ByVal bStateOnly As Boolean)
'    On Error Resume Next
'    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
'    Dim i&, nCount&, bSkipFirst As Boolean
'
'    Set aEchanges = ctlView.UndExchanges
'
'    nCount = aEchanges.Count - 1
'    If bStateOnly Then
'        If nCount > 0 Then
'            i = 1
'            bSkipFirst = True
'            For Each aExch In aEchanges
'                If Not bSkipFirst Then
'                    If i <= nCount Then
'                        mnuExchShowUnd(i).Checked = aExch.Visible
'                        mnuExchShowUnd(i).Enabled = nCount > 0
'                        i = i + 1
'                        If i > 9 Then Exit For
'                    End If
'                End If
'                bSkipFirst = False
'            Next
'
''            For i = 1 To 9
''                If i <= nCount Then
''                    Set aExch = aEchanges(i + 1)
''                    mnuExchShowUnd(i).Checked = aExch.Visible
''                    mnuExchShowUnd(i).Enabled = nCount > 0
''                    Set aExch = Nothing
''                End If
''            Next
'        End If
'    Else
'        If nCount > 0 Then
'            mnuExchAllUnd.Enabled = (nCount > 0)
'            mnuExchMoreUnd.Enabled = mnuExchAllUnd.Enabled
'
'            i = 1
'            bSkipFirst = True
'            For Each aExch In aEchanges
'                If Not bSkipFirst Then
'                    If i <= nCount Then
'                        mnuExchShowUnd(i).Caption = "&" & i & " - " & aExch.Code & " - [" & aExch.Name & "]"
'                        mnuExchShowUnd(i).Visible = True
'                        mnuExchShowUnd(i).Checked = aExch.Visible
'                        mnuExchShowUnd(i).Enabled = nCount > 0
'                    Else
'                        mnuExchShowUnd(i).Visible = False
'                    End If
'                    i = i + 1
'                    If i > 9 Then Exit For
'                End If
'                bSkipFirst = False
'            Next
'
''            For i = 1 To 9
''                If i <= nCount Then
''                    Set aExch = aEchanges(i + 1)
''                    mnuExchShowUnd(i).Caption = "&" & i & " - " & aExch.Code & " - [" & aExch.Name & "]"
''                    mnuExchShowUnd(i).Visible = True
''                    mnuExchShowUnd(i).Checked = aExch.Visible
''                    mnuExchShowUnd(i).Enabled = nCount > 0
''                    Set aExch = Nothing
''                Else
''                    mnuExchShowUnd(i).Visible = False
''                End If
''            Next
'        Else
'            mnuExchAllUnd.Enabled = False
'            mnuExchMoreUnd.Enabled = False
'            For i = 1 To 9
'                mnuExchShowUnd(i).Visible = False
'            Next
'        End If
'    End If
'
'    Set aEchanges = Nothing
'End Sub
'
'Private Sub ctlView_OnExpiriesChange(ByVal bStateOnly As Boolean)
'    On Error Resume Next
'    Dim aExpiries As clsQnExpColl, aExp As clsQnExpAtom
'    Dim i&, nCount&
'
'    Set aExpiries = ctlView.Expiries
'
'    nCount = aExpiries.Count
'    If bStateOnly Then
'        If nCount > 0 Then
''            mnuExpiryAll.Enabled = (nCount > 1)
''            mnuExpiryNext.Enabled = (nCount > 1)
''            mnuExpiryPrev.Enabled = mnuExpiryNext.Enabled
'
'            For i = 1 To 9
'                If i <= nCount Then
'                    Set aExp = aExpiries(i)
'                    mnuExpiryShow(i).Checked = aExp.Visible
'                    mnuExpiryShow(i).Enabled = mnuExpiryNext.Enabled
'                    Set aExp = Nothing
'                End If
'            Next
'        End If
'    Else
'        If nCount > 0 Then
'            mnuExpiryAll.Enabled = (nCount > 1)
'            mnuExpiryNext.Enabled = mnuExpiryAll.Enabled
'            mnuExpiryPrev.Enabled = mnuExpiryNext.Enabled
'
'            mnuExpiryMore.Enabled = True
'            For i = 1 To 9
'                If i <= nCount Then
'                    Set aExp = aExpiries(i)
'                    mnuExpiryShow(i).Caption = "&" & i & " - " & Format$(aExp.ExpiryMonth, "MMMM YYYY")
'                    mnuExpiryShow(i).Visible = True
'                    mnuExpiryShow(i).Checked = aExp.Visible
'                    mnuExpiryShow(i).Enabled = mnuExpiryNext.Enabled
'                    Set aExp = Nothing
'                Else
'                    mnuExpiryShow(i).Visible = False
'                End If
'            Next
'        Else
'            mnuExpiryAll.Enabled = False
'            mnuExpiryNext.Enabled = False
'            mnuExpiryPrev.Enabled = False
'
'            mnuExpiryMore.Enabled = False
'            For i = 1 To 9
'                mnuExpiryShow(i).Visible = False
'            Next
'        End If
'    End If
'
'    Set aExpiries = Nothing
'End Sub
'
'Private Sub ctlView_OnSetCaption()
'    On Error Resume Next
'    AdjustCaption
'    imgRealTime(0).Visible = Not ctlView.RealTime
'    imgRealTime(1).Visible = ctlView.RealTime
'    frmMain.AdjustWindowsListCaptions
'End Sub
'
'Private Sub AdjustCaption()
'    On Error Resume Next
'    m_sCaption = ctlView.GetCaption & " - [" & m_sFileName & "]"
'    m_sShortCaption = ctlView.GetShortCaption
'    Me.Caption = IIf(m_nWindowIndex > 0 And m_nWindowIndex < 10, CStr(m_nWindowIndex) & ": ", "") & m_sCaption
'End Sub
'
'Private Sub ctlView_OnStateChange()
'    On Error Resume Next
'    Dim nID&
'    nID = ctlView.Underlying.ID
'    mnuViewRealTime.Enabled = ctlView.IsConnected And nID <> 0 And Not ctlView.InProc And Not ctlView.LastQuoteReqNow
'    mnuViewRealTime.Checked = ctlView.RealTime
'    mnuViewRefresh.Enabled = ctlView.IsConnected And nID <> 0 And Not ctlView.RealTime And Not ctlView.InProc And Not ctlView.LastQuoteReqNow
'    mnuViewDefOptSort.Enabled = ctlView.IsConnected And nID <> 0 And Not ctlView.InProc
'
'    mnuViewDividends.Enabled = (nID = 0 Or nID <> 0 And ctlView.Underlying.UndType <> enCtIndex)
'    mnuViewDividends.Checked = ctlView.DividendsVisible And ctlView.Underlying.UndType <> enCtIndex
'    mnuViewRates.Checked = ctlView.RatesVisible
'    mnuViewCalcModel.Checked = ctlView.CalcModelVisible
'    mnuViewExpCalendar.Checked = ctlView.ExpCalendarVisible
'    mnuViewLog.Checked = ctlView.LogVisible
'End Sub
'
'Public Sub Init(sKey As String)
'    On Error Resume Next
'
'    EnableActiveRealTime = True
'    m_nWindowIndex = 0
'    m_sFilePath = ""
'    m_sCaption = ""
'    m_sShortCaption = ""
'    g_nQuotationUntitledIdx = g_nQuotationUntitledIdx + 1
'    m_sFileName = "Untitled" & g_nQuotationUntitledIdx & ".uvw"
'    m_bIsNew = True
'    m_sKey = sKey
'    mnuFileClose.Caption = "Close" & vbTab & "Alt+F4"
'
'    Set ctlView.m_frmOwner = Me
'    Set ctlView.pbProgress = pbProgress
'    Set ctlView.lblProcess = lblProcess
'    Set ctlView.lblStatus = lblStatus
'    Set ctlView.lblValue = lblValue
'    Set ctlView.imgStop = imgStop
'    Set ctlView.imgStopDis = imgStopDis
'
'    ctlView.Init
'    Set aParams = g_Params
'    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'    mnuFileQuotation.Enabled = g_bIseEnable
'    ctlView.ProcessRealTime = True
'End Sub
'
'Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
'    On Error Resume Next
'    ctlView.ShowData nType, nValue
'End Sub
'
'Private Sub Form_Load()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    m_bShowInTaskbar = True
'    mnuWindowShowInTaskBar.Checked = True
'    lblStatus.Caption = ""
'    lblProcess.Caption = ""
'    lblValue.Caption = ""
'
'    Set frmItems = New frmItemsList
'End Sub
'
'Private Sub Form_Resize()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    LockWindowUpdate Me.hWnd
'
'    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
'    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height
'    lblValue.Left = picBottom.ScaleWidth - lblValue.Width
'
'    LockWindowUpdate 0
'End Sub
'
'Private Sub Form_Terminate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    Set aParams = Nothing
'    g_ViewFrm.Remove m_sKey
'    Set frmItems = Nothing
'
'    ctlView.Term
'
'    frmMain.ReloadWindowsList
'End Sub
'
'Private Sub Form_Unload(Cancel As Integer)
'    On Error Resume Next
'    Form_Terminate
'End Sub
'
'Private Sub Form_Activate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    If Not EnableActiveRealTime Or m_bShowInTaskbarChanging Then Exit Sub
'    If aParams.ActiveRealTime Then
'        'ctlView.RefreshView
'        ctlView.ProcessRealTime = True
'    End If
'    If Not m_bShowInTaskbar Then
'        ShowFormInTaskBar Me, False
'    End If
'End Sub
'
'Private Sub Form_Deactivate()
'    On Error Resume Next
'    If gCmn Is Nothing Then Exit Sub
'    If m_bShowInTaskbarChanging Then Exit Sub
'    If aParams.ActiveRealTime Then
'        ctlView.ProcessRealTime = False
'    End If
'End Sub
'
'Private Sub aParams_ActiveRealTimeChange()
'    On Error Resume Next
'    If Not EnableActiveRealTime Then Exit Sub
'    If aParams.ActiveRealTime Then
'        ctlView.ProcessRealTime = (Me.hWnd = GetForegroundWindow)
'    Else
'        ctlView.ProcessRealTime = True
'    End If
'End Sub
'
'Private Sub aParams_EventLogVisibleChange()
'    On Error Resume Next
'    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'End Sub
'
'Private Sub aParams_CalcModelChange()
'    On Error Resume Next
'    ctlView.ModelUpdate
'End Sub
'
'Private Sub frmItems_OnOK(ByVal nKey As Long, aItems As clsEntityColl)
'    On Error Resume Next
'    Dim i&, nCount&, aItem As clsEntityAtom
'
'    nCount = aItems.Count
'    Select Case nKey
'        Case 1 ' expiries
'            For i = 1 To nCount
'                Set aItem = aItems(i)
'                ctlView.ShowExpiry i, (aItem.Data <> 0)
'                Set aItem = Nothing
'            Next
'
'        Case 3 ' underlying exchanges
'            For i = 1 To nCount
'                Set aItem = aItems(i)
'                ctlView.ShowUndExchange i + 1, (aItem.Data <> 0)
'                Set aItem = Nothing
'            Next
'
'    End Select
'End Sub
'
'Private Sub mnuExchMoreUnd_Click()
'    On Error Resume Next
'    Dim aItems As New clsEntityColl, aItem As clsEntityAtom
'    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
'    Dim i&, nCount&, bSkipFirst As Boolean
'
'    Set aEchanges = ctlView.UndExchanges
'
'    nCount = aEchanges.Count - 1
'    If nCount > 0 Then
'        bSkipFirst = True
'        i = 1
'        For Each aExch In aEchanges
'            If Not bSkipFirst Then
'                If aItems(CStr(i)) Is Nothing Then
'                    Set aItem = aItems.Add(CStr(i))
'                    aItem.ID = i
'                    aItem.Name = i & " - " & aExch.Code & " - [" & aExch.Name & "]"
'                    aItem.Data = IIf(aExch.Visible, 1, 0)
'                    Set aItem = Nothing
'                End If
'                i = i + 1
'            End If
'            bSkipFirst = False
'        Next
'
'        frmItems.Execute "Underlying Exchanges", "Select Exchanges to Show:", aItems, 3, Me
'    End If
'End Sub
'
'Private Sub mnuExchAllUnd_Click()
'    On Error Resume Next
'    ctlView.ShowAllUndExchanges
'End Sub
'
'Private Sub mnuExchShowUnd_Click(Index As Integer)
'    On Error Resume Next
'    ctlView.ShowUndExchange Index + 1, Not mnuExchShowUnd(Index).Checked
'End Sub
'
'Private Sub mnuExpiryMore_Click()
'    On Error Resume Next
'    Dim aItems As New clsEntityColl, aItem As clsEntityAtom
'    Dim aExpiries As clsQnExpColl, aExp As clsQnExpAtom
'    Dim i&, nCount&
'
'    Set aExpiries = ctlView.Expiries
'
'    nCount = aExpiries.Count
'    If nCount > 0 Then
'        For i = 1 To nCount
'            Set aExp = aExpiries(i)
'            If aItems(CStr(i)) Is Nothing Then
'                Set aItem = aItems.Add(CStr(i))
'                aItem.ID = i
'                aItem.Name = i & " - " & Format$(aExp.ExpiryMonth, "MMMM YYYY")
'                aItem.Data = IIf(aExp.Visible, 1, 0)
'                Set aItem = Nothing
'            End If
'            Set aExp = Nothing
'        Next
'
'        frmItems.Execute "Expiries", "Select Expiries to Show:", aItems, 1, Me
'    End If
'End Sub
'
'Private Sub mnuFileClose_Click()
'    On Error Resume Next
'    Unload Me
'End Sub
'
'Private Sub Save(ByVal sFilePath As String, ByVal sFileName As String)
'    Dim aStorage As New clsSettingsStorage
'    Screen.MousePointer = vbHourglass
'    aStorage.Init sFilePath & sFileName
'    aStorage.SetStringValue "Type", "Value", "QuotationView"
'    ctlView.SaveToFile aStorage, ""
'    aStorage.SaveData
'    m_sFilePath = sFilePath
'    m_sFileName = sFileName
'    m_bIsNew = False
'    AdjustCaption
'    Screen.MousePointer = vbDefault
'End Sub
'
'Public Sub SaveLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
'    ctlView.SaveToFile aStorage, sKey
'End Sub
'
'Public Sub OpenLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
'    ctlView.OpenFromFile aStorage, sKey
'End Sub
'
'Private Sub mnuFileNew_Click()
'    On Error Resume Next
'    frmMain.ShowQuotation
'End Sub
'
'Private Sub mnuFilePrint_Click()
'    On Error Resume Next
'    ctlView.PrintView
'End Sub
'
'Private Sub mnuFileQuotes_Click()
'    On Error Resume Next
'    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileReportsView_Click()
'    On Error Resume Next
'    frmMain.ShowReports TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileRiskMatrix_Click()
'    On Error Resume Next
'    frmMain.ShowRiskMatrix TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileRisks_Click()
'    On Error Resume Next
'    frmMain.ShowRisks TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileTrades_Click()
'    On Error Resume Next
'    frmMain.ShowTrades TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileVola_Click()
'    On Error Resume Next
'    frmMain.ShowVola TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileQuotation_Click()
'    On Error Resume Next
'    frmMain.ShowQuotation TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileVolaAnalysis_Click()
'    On Error Resume Next
'    frmMain.ShowVolaAnalysis TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileGreeks_Click()
'    On Error Resume Next
'    frmMain.ShowGreeks TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileHedgeSummaryView_Click()
'    On Error Resume Next
'    frmMain.ShowHedgeSummary TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileOrders_Click()
'    On Error Resume Next
'    frmMain.ShowOrders TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileTntCards_Click()
'    On Error Resume Next
'    frmMain.ShowTntCards TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileStockHedge_Click()
'    On Error Resume Next
'    frmMain.ShowStockHedge TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileIndex1Hedge_Click()
'    On Error Resume Next
'    frmMain.ShowIndex1Hedge TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileIndex2Hedge_Click()
'    On Error Resume Next
'    frmMain.ShowIndex2Hedge TYPE_UNDERLYING, ctlView.Underlying.ID
'End Sub
'
'Private Sub mnuFileSave_Click()
'    On Error GoTo EH
'
'    If m_bIsNew Then
'        With dlgCommon
'            .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                    Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
'            .CancelError = True
'            .DefaultExt = ".uvw"
'            .DialogTitle = "Save Quotation"
'            .InitDir = ""
'            .FileName = m_sFilePath & m_sFileName
'            .Filter = "ETS Quotation (*.uvw)|*.uvw|All Files|*.*"
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
'    gCmn.ErrorMsgBox Me, "Fail to save quotation."
'End Sub
'
'Private Sub mnuFileSaveAs_Click()
'    On Error GoTo EH
'
'    With dlgCommon
'        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
'        .CancelError = True
'        .DefaultExt = ".uvw"
'        .DialogTitle = "Save Quotation As"
'        .InitDir = ""
'        .FileName = m_sFilePath & m_sFileName
'        .Filter = "ETS Quotation (*.uvw)|*.uvw|All Files|*.*"
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
'    gCmn.ErrorMsgBox Me, "Fail to save quotation."
'End Sub
'
'Private Sub mnuFileOpen_Click()
'    On Error GoTo EH
'    Dim aStorage As New clsSettingsStorage
'
'    With dlgCommon
'        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
'                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    frmMain.RunBackOffice

End Sub

'        .CancelError = True
'        .DefaultExt = ".uvw"
'        .DialogTitle = "Open Quotation"
'        .FileName = ""
'        .InitDir = m_sFilePath
'        .Filter = "ETS Quotation (*.uvw)|*.uvw|All Files|*.*"
'        .FilterIndex = 1
'        On Error Resume Next
'        .ShowOpen
'        If Err.Number = 0 Then
'            On Error GoTo EH
'            Screen.MousePointer = vbHourglass
'            aStorage.Init .FileName
'            aStorage.ReadData
'            If aStorage.GetStringValue("Type", "Value") = "QuotationView" Then
'                ctlView.OpenFromFile aStorage, ""
'                m_sFilePath = ExtractFilePath(.FileName)
'                m_sFileName = ExtractFileName(.FileName)
'                m_bIsNew = False
'                AdjustCaption
'                Screen.MousePointer = vbDefault
'            Else
'                Screen.MousePointer = vbDefault
'                gCmn.MyMsgBox Me, "Invalid quotation file."
'            End If
'        End If
'    End With
'    Exit Sub
'EH:
'    Screen.MousePointer = vbDefault
'    gCmn.ErrorMsgBox Me, "Fail to open quotation."
'End Sub
'
'Private Sub mnuViewClearLog_Click()
'    On Error Resume Next
'    ctlView.ClearLog
'End Sub
'
'Private Sub mnuViewDefOptSort_Click()
'    On Error Resume Next
'    ctlView.SortOptionsByDefault
'End Sub
'
'Private Sub mnuViewExpCalendar_Click()
'    On Error Resume Next
'    ctlView.ExpCalendarVisible = Not mnuViewExpCalendar.Checked
'End Sub
'
'Private Sub mnuOptionsParameters_Click()
'    On Error Resume Next
'    frmMain.ShowParameters
'End Sub
'
'Private Sub mnuViewRealTime_Click()
'    On Error Resume Next
'    ctlView.StartRealTime (Not mnuViewRealTime.Checked)
'End Sub
'
'Private Sub mnuExpiryAll_Click()
'    On Error Resume Next
'    ctlView.ShowAllExpiries
'End Sub
'
'Private Sub mnuExpiryNext_Click()
'    On Error Resume Next
'    ctlView.ShowNextExpiry
'End Sub
'
'Private Sub mnuExpiryPrev_Click()
'    On Error Resume Next
'    ctlView.ShowPrevExpiry
'End Sub
'
'Private Sub mnuExpiryShow_Click(Index As Integer)
'    On Error Resume Next
'    ctlView.ShowExpiry Index, Not mnuExpiryShow(Index).Checked
'End Sub
'
'Private Sub mnuViewRefresh_Click()
'    On Error Resume Next
'    ctlView.Refresh
'End Sub
'
'Private Sub mnuViewDivGridLayout_Click()
'    On Error Resume Next
'    ctlView.CustomizeDivGridLayout
'End Sub
'
'Private Sub mnuViewLogGridLayout_Click()
'    On Error Resume Next
'    ctlView.CustomizeLogGridLayout
'End Sub
'
'Private Sub mnuViewDividends_Click()
'    On Error Resume Next
'    ctlView.DividendsVisible = Not mnuViewDividends.Checked
'End Sub
'
'Private Sub mnuOptionsEventLog_Click()
'    On Error Resume Next
'    ShowEventLog (Not mnuOptionsEventLog.Checked), , True
'End Sub
'
'Private Sub mnuViewOptGridLayout_Click()
'    On Error Resume Next
'    ctlView.CustomizeOptionGridLayout
'End Sub
'
'Private Sub mnuViewRates_Click()
'    On Error Resume Next
'    ctlView.RatesVisible = Not mnuViewRates.Checked
'End Sub
'
'Private Sub mnuViewCalcModel_Click()
'    On Error Resume Next
'    ctlView.CalcModelVisible = Not mnuViewCalcModel.Checked
'End Sub
'
'Private Sub mnuViewUndGridLayout_Click()
'    On Error Resume Next
'    ctlView.CustomizeUnderlyingGridLayout
'End Sub
'
'Private Sub mnuViewLog_Click()
'    On Error Resume Next
'    ctlView.LogVisible = Not mnuViewLog.Checked
'End Sub
'
'Private Sub mnuToolsBackOffice_Click()
'    On Error Resume Next
'    frmMain.RunBackOffice
'End Sub
'
'Private Sub mnuToolsHedger_Click()
'    On Error Resume Next
'    frmMain.RunHedger
'End Sub
'
'Private Sub mnuToolsVolaManager_Click()
'    On Error Resume Next
'    frmMain.RunVolaManager
'End Sub
'
'Public Sub AdjustWindowsListCaptions()
'    On Error Resume Next
'    Dim i&, nCount&
'
'    nCount = g_ViewFrm.Count
'    For i = 1 To 9
'        If i <= nCount Then
'            mnuWindowGo(i).Caption = "&" & CStr(i) & " " & g_ViewFrm(i).Frm.GetCaption
'            mnuWindowGo(i).Visible = True
'            mnuWindowGo(i).Enabled = True
'        Else
'            mnuWindowGo(i).Visible = False
'            mnuWindowGo(i).Enabled = False
'        End If
'    Next
'End Sub
'
'Private Sub mnuWindowArrange_Click()
'    On Error Resume Next
'    frmMain.WindowsArrange
'End Sub
'
'Private Sub mnuWindowCloseAll_Click()
'    On Error Resume Next
'    frmMain.WindowsCloseAll
'End Sub
'
'Private Sub mnuWindowList_Click()
'    On Error Resume Next
'    frmMain.WindowsList
'End Sub
'
'Private Sub mnuWindowMain_Click()
'    On Error Resume Next
'    ShowWindow frmMain.hWnd, SW_SHOWNORMAL
'    frmMain.SetFocus
'End Sub
'
'Private Sub mnuWindowTileHorizontaly_Click()
'    On Error Resume Next
'    frmMain.WindowsTileHorizontally
'End Sub
'
'Private Sub mnuWindowTileVertically_Click()
'    On Error Resume Next
'    frmMain.WindowsTileVertically
'End Sub
'
'Private Sub mnuWindowCascade_Click()
'    On Error Resume Next
'    frmMain.WindowsCascade
'End Sub
'
'Private Sub mnuWindowShowInTaskBar_Click()
'    On Error Resume Next
'    frmMain.WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar
'End Sub
'
'Private Sub mnuWindowGo_Click(Index As Integer)
'    On Error Resume Next
'    frmMain.WindowsGo Index
'End Sub
'
'Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
'    On Error Resume Next
'    ctlView.SetGridLayout enGridType, gdGrid
'End Sub
'
'Private Sub mnuHelpContents_Click()
'    On Error Resume Next
'    frmMain.HelpContents
'End Sub
'
'Public Property Get WindowKey() As String
'    On Error Resume Next
'    WindowKey = m_sKey
'End Property
'
'Private Sub mnuManageUnderlyings_Click()
'    On Error Resume Next
'    frmMain.ShowManageUnderlyings
'End Sub
'
Private Sub mnuToolsVolaManager_Click()
    On Error Resume Next
    frmMain.RunVolaManager

End Sub
