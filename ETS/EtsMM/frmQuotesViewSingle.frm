VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Begin VB.Form frmQuotesViewSingle 
   Caption         =   "Quotes"
   ClientHeight    =   5610
   ClientLeft      =   165
   ClientTop       =   735
   ClientWidth     =   9600
   Icon            =   "frmQuotesViewSingle.frx":0000
   KeyPreview      =   -1  'True
   LinkTopic       =   "Form1"
   ScaleHeight     =   5610
   ScaleWidth      =   9600
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox picBottom 
      BorderStyle     =   0  'None
      Height          =   300
      Left            =   240
      ScaleHeight     =   300
      ScaleWidth      =   7620
      TabIndex        =   1
      TabStop         =   0   'False
      Top             =   4560
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
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   2
         Left            =   0
         Picture         =   "frmQuotesViewSingle.frx":0E42
         ToolTipText     =   "Real Time (Disconnected)"
         Top             =   30
         Width           =   240
      End
      Begin VB.Image imgStopDis 
         Enabled         =   0   'False
         Height          =   240
         Left            =   2340
         Picture         =   "frmQuotesViewSingle.frx":13CC
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgStop 
         Height          =   240
         Left            =   2340
         MouseIcon       =   "frmQuotesViewSingle.frx":1516
         MousePointer    =   99  'Custom
         Picture         =   "frmQuotesViewSingle.frx":1668
         ToolTipText     =   "Cancel"
         Top             =   45
         Visible         =   0   'False
         Width           =   240
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   1
         Left            =   0
         Picture         =   "frmQuotesViewSingle.frx":17B2
         ToolTipText     =   "Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Image imgRealTime 
         Height          =   240
         Index           =   0
         Left            =   0
         Picture         =   "frmQuotesViewSingle.frx":1D3C
         ToolTipText     =   "Not Real Time"
         Top             =   30
         Width           =   240
      End
      Begin VB.Label lblStatus 
         AutoSize        =   -1  'True
         Height          =   195
         Left            =   300
         TabIndex        =   4
         Top             =   60
         Width           =   45
      End
      Begin VB.Label lblProcess 
         AutoSize        =   -1  'True
         Caption         =   "Process status..."
         Height          =   195
         Left            =   2655
         TabIndex        =   3
         Top             =   60
         Visible         =   0   'False
         Width           =   1170
      End
   End
   Begin MSComDlg.CommonDialog dlgCommon 
      Left            =   6000
      Top             =   3240
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin EtsMM.ctlQuotesViewSingle ctlView 
      Height          =   4335
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   5175
      _ExtentX        =   9128
      _ExtentY        =   7646
   End
   Begin MSComctlLib.StatusBar sbStatus 
      Align           =   2  'Align Bottom
      Height          =   300
      Left            =   0
      TabIndex        =   5
      Top             =   5310
      Width           =   9600
      _ExtentX        =   16933
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
      Begin VB.Menu mnuVSCalc 
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
      Begin VB.Menu mnuFileOTCOptionCalc 
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
      Begin VB.Menu mnuFileSeparator78 
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
         Enabled         =   0   'False
         Visible         =   0   'False
      End
      Begin VB.Menu mnuViewDividends 
         Caption         =   "&Dividends"
      End
      Begin VB.Menu mnuViewRates 
         Caption         =   "&Rates"
      End
      Begin VB.Menu mnuViewVola 
         Caption         =   "&Volatility"
      End
      Begin VB.Menu mnuViewFutures 
         Caption         =   "Futures"
      End
      Begin VB.Menu mnuSeparator6 
         Caption         =   "-"
      End
      Begin VB.Menu mnuViewUndGridLayout 
         Caption         =   "&Underlying Grid Layout..."
      End
      Begin VB.Menu mnuViewDivGridLayout 
         Caption         =   "Parameters &Grid Layout..."
      End
      Begin VB.Menu mnuViewVolGridLayout 
         Caption         =   "Volatility Grid &Layout..."
      End
      Begin VB.Menu mnuViewFutGridLayout 
         Caption         =   "Future Grid Layout..."
      End
      Begin VB.Menu mnuViewOptGridLayout 
         Caption         =   "&Option Grid Layout..."
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
   Begin VB.Menu mnuStrike 
      Caption         =   "&Strike"
      Begin VB.Menu mnuStrikeAll 
         Caption         =   "&Show All"
      End
      Begin VB.Menu mnuStrikeSeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuStrikeMore 
         Caption         =   "S&trikes..."
      End
   End
   Begin VB.Menu mnuRoot 
      Caption         =   "Option &Root"
      Begin VB.Menu mnuRootNext 
         Caption         =   "&Next"
      End
      Begin VB.Menu mnuRootPrev 
         Caption         =   "&Previous"
      End
      Begin VB.Menu mnuRootAll 
         Caption         =   "&Show All"
      End
      Begin VB.Menu mnuRootSeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 1>"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 2>"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 3>"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 4>"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 5>"
         Index           =   5
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 6>"
         Index           =   6
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 7>"
         Index           =   7
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 8>"
         Index           =   8
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootShow 
         Caption         =   "<root 9>"
         Index           =   9
         Visible         =   0   'False
      End
      Begin VB.Menu mnuRootMore 
         Caption         =   "&Option Roots..."
      End
   End
   Begin VB.Menu mnuExch 
      Caption         =   "E&xchange"
      Begin VB.Menu mnuExchNextOpt 
         Caption         =   "&Next Option"
         Shortcut        =   +{F8}
      End
      Begin VB.Menu mnuExchPrevOpt 
         Caption         =   "&Previous Option"
         Shortcut        =   +{F7}
      End
      Begin VB.Menu mnuExchAllOpt 
         Caption         =   "&Show All Option"
         Shortcut        =   +{F6}
      End
      Begin VB.Menu mnuExchSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 1>"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 2>"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 3>"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 4>"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 5>"
         Index           =   5
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 6>"
         Index           =   6
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 7>"
         Index           =   7
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 8>"
         Index           =   8
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchShowOpt 
         Caption         =   "<exch 9>"
         Index           =   9
         Visible         =   0   'False
      End
      Begin VB.Menu mnuExchMoreOpt 
         Caption         =   "&Option Exchanges..."
         Shortcut        =   +{F11}
      End
      Begin VB.Menu mnuExchSeparator2 
         Caption         =   "-"
      End
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
   Begin VB.Menu mnuFutures 
      Caption         =   "F&utures"
      Begin VB.Menu mnuFutNext 
         Caption         =   "&Next"
      End
      Begin VB.Menu mnuFutPrev 
         Caption         =   "&Previos"
      End
      Begin VB.Menu mnuFutAll 
         Caption         =   "&Show All"
      End
      Begin VB.Menu mnuFutSeparator 
         Caption         =   "-"
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 1>"
         Index           =   1
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 2>"
         Index           =   2
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 3>"
         Index           =   3
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 4>"
         Index           =   4
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 5>"
         Index           =   5
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 6>"
         Index           =   6
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 7>"
         Index           =   7
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 8>"
         Index           =   8
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutShow 
         Caption         =   "<fut 9>"
         Index           =   9
         Visible         =   0   'False
      End
      Begin VB.Menu mnuFutMore 
         Caption         =   "&Futures..."
      End
   End
   Begin VB.Menu mnuOrder 
      Caption         =   "Or&der"
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
      Begin VB.Menu mnuManageSeparator 
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
      Begin VB.Menu mnuManageSeparator1 
         Caption         =   "-"
      End
      Begin VB.Menu mnuManualPrices 
         Caption         =   "Manual Prices..."
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
Attribute VB_Name = "frmQuotesViewSingle"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit

Private WithEvents aParams As clsParams
Attribute aParams.VB_VarHelpID = -1
Private WithEvents frmItems As frmItemsList
Attribute frmItems.VB_VarHelpID = -1
Private WithEvents frmRoots As frmOptionRoot
Attribute frmRoots.VB_VarHelpID = -1
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ShowWindowInTaskbar Enter", Me.Caption
    
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ShowWindowInTaskbar Exit", Me.Caption

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

Public Function GetShortCaption() As String
    On Error Resume Next
    GetShortCaption = m_sShortCaption
End Function

Private Sub aParams_InternalVolatilityChanged(ByVal sSymbol As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_InternalVolatilityChanged Call : " & sSymbol, Me.Caption
    
    ctlView.InternalVolatilityChanged sSymbol
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_InternalVolatilityChanged Done : " & sSymbol, Me.Caption

End Sub

Private Sub aParams_NewUnderlyingAdded(ByVal nNewUndID As Long)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_NewUnderlyingAdded Call", Me.Caption
    
    ctlView.NewUnderlyingAdded nNewUndID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_NewUnderlyingAdded Done", Me.Caption

End Sub

Private Sub aParams_QuoteOptExpirySeparationChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_QuoteOptExpirySeparationChange Call", Me.Caption
    
    ctlView.OptExpirySeparationChange

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_QuoteOptExpirySeparationChange Done", Me.Caption

End Sub

Private Sub ctlView_OnActiveFutChange(ByVal aUndID As Long, ByVal iActiveFutID As Long)
    On Error Resume Next
    frmMain.ActiveFutChange aUndID, iActiveFutID
End Sub

Private Sub ctlView_OnFutParamsChange(ByVal aUndID As Long, ByVal iFutID As Long, ByVal dRatio As Double, ByVal dBasis As Double)
    On Error Resume Next
    frmMain.FutParamsChange aUndID, iFutID, dRatio, dBasis
End Sub

Private Sub ctlView_OnFutChange(ByVal bStateOnly As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnFutChange Enter", Me.Caption
    
    Dim aFutColl As EtsMmQuotesLib.MmQvFutCollection, aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim i&, nCount&
    
    Set aFutColl = ctlView.Underlying.Fut
    
    nCount = aFutColl.Count
    If ctlView.Underlying.UndType = enCtIndex Then
        If Not ctlView.Group.ShowIndexFutureOptions Then nCount = 0
    End If
    
    If bStateOnly Then
        If nCount > 0 Then
            i = 1
            For Each aFut In aFutColl
                If i <= nCount Then
                    mnuFutShow(i).Checked = aFut.Visible
                    mnuFutShow(i).Enabled = (nCount > 1)
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        End If
    Else
        If nCount > 0 Then
            mnuFutAll.Enabled = (nCount > 1)
            mnuFutNext.Enabled = mnuFutAll.Enabled
            mnuFutPrev.Enabled = mnuFutAll.Enabled
            mnuFutMore.Enabled = mnuFutAll.Enabled
            
            For i = 1 To 9
                mnuFutShow(i).Visible = False
            Next
            
            i = 1
            For Each aFut In aFutColl
                If i <= nCount Then
                    mnuFutShow(i).Caption = "&" & i & " - " & aFut.Symbol
                    mnuFutShow(i).Visible = True
                    mnuFutShow(i).Checked = aFut.Visible
                    mnuFutShow(i).Enabled = (nCount > 1)
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        Else
            mnuFutAll.Enabled = False
            mnuFutNext.Enabled = False
            mnuFutPrev.Enabled = False
            mnuFutMore.Enabled = False
            For i = 1 To 9
                mnuFutShow(i).Visible = False
            Next
        End If
    End If
    
    Set aFutColl = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnFutChange Exit", Me.Caption
    
End Sub

Private Sub ctlView_OnManualPriceChanged(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsGeneralLib.EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
On Error Resume Next
    frmMain.ManualPriceChange UndID, ID, price, CtType, Status
End Sub

Private Sub ctlView_OnOptExchangesChange(ByVal bStateOnly As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnOptExchangesChange Enter", Me.Caption
    
    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
    Dim i&, nCount&
    
    Set aEchanges = ctlView.OptExchanges
    
    nCount = aEchanges.Count
    If bStateOnly Then
        If nCount > 0 Then
            i = 1
            For Each aExch In aEchanges
                If i <= nCount Then
                    mnuExchShowOpt(i).Checked = aExch.Visible
                    mnuExchShowOpt(i).Enabled = (nCount > 1)
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        End If
    Else
        If nCount > 0 Then
            mnuExchAllOpt.Enabled = (nCount > 1)
            mnuExchNextOpt.Enabled = mnuExchAllOpt.Enabled
            mnuExchPrevOpt.Enabled = mnuExchAllOpt.Enabled
            mnuExchMoreOpt.Enabled = mnuExchAllOpt.Enabled
            
            For i = 1 To 9
                mnuExchShowOpt(i).Visible = False
                mnuExchShowOpt(i).Tag = 0
            Next
            
            i = 1
            For Each aExch In aEchanges
                If i <= nCount Then
                    If aExch.ID <> 0 Then
                        mnuExchShowOpt(i).Caption = "&" & i & " - " & aExch.Code & " - [" & aExch.Name & "]"
                    Else
                        mnuExchShowOpt(i).Caption = "&" & i & " - " & aExch.Name
                    End If
                    
                    mnuExchShowOpt(i).Visible = True
                    mnuExchShowOpt(i).Checked = aExch.Visible
                    mnuExchShowOpt(i).Enabled = (nCount > 1)
                    mnuExchShowOpt(i).Tag = aExch.ID
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        Else
            mnuExchAllOpt.Enabled = False
            mnuExchNextOpt.Enabled = False
            mnuExchPrevOpt.Enabled = False
            mnuExchMoreOpt.Enabled = False
            For i = 1 To 9
                mnuExchShowOpt(i).Visible = False
                mnuExchShowOpt(i).Tag = 0
            Next
        End If
    End If
    
    Set aEchanges = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnOptExchangesChange Exit", Me.Caption
    
End Sub

Private Sub ctlView_OnOptRootsChange(ByVal bStateOnly As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnOptRootsChange Enter", Me.Caption
    
    Dim aRoots As EtsMmQuotesLib.MmQvOptRootColl, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim i&, nCount&
    
    Set aRoots = ctlView.OptRoots
    
    nCount = aRoots.Count
    If bStateOnly Then
        If nCount > 0 Then
            i = 1
            For Each aRoot In aRoots
                If i <= nCount Then
                    mnuRootShow(i).Checked = aRoot.Visible
                    mnuRootShow(i).Enabled = mnuRootNext.Enabled
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        End If
    Else
        If nCount > 0 Then
            mnuRootAll.Enabled = (nCount > 1)
            mnuRootNext.Enabled = mnuRootAll.Enabled
            mnuRootPrev.Enabled = mnuRootNext.Enabled
            
            For i = 1 To 9
                mnuRootShow(i).Visible = False
            Next
            
            mnuRootMore.Enabled = True
            i = 1
            For Each aRoot In aRoots
                If i <= nCount Then
                    mnuRootShow(i).Caption = "&" & i & " - " & aRoot.Name
                    mnuRootShow(i).Visible = True
                    mnuRootShow(i).Checked = aRoot.Visible
                    mnuRootShow(i).Enabled = mnuRootNext.Enabled
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        Else
            mnuRootAll.Enabled = False
            mnuRootNext.Enabled = False
            mnuRootPrev.Enabled = False
            
            mnuRootMore.Enabled = False
            For i = 1 To 9
                mnuRootShow(i).Visible = False
            Next
        End If
    End If
    
    Set aRoots = Nothing

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnOptRootsChange Exit", Me.Caption

End Sub

Private Sub ctlView_OnStrikesChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnStrikesChange Enter", Me.Caption
    
    Dim nCount&
    nCount = ctlView.Strikes.Count
    mnuStrikeAll.Enabled = nCount > 1
    mnuStrikeMore.Enabled = nCount > 0

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnStrikesChange Exit", Me.Caption

End Sub

Private Sub ctlView_OnUndExchangesChange(ByVal bStateOnly As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnUndExchangesChange Enter", Me.Caption
    
    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
    Dim i&, nCount&
    
    Set aEchanges = ctlView.UndExchanges
    
    nCount = aEchanges.Count
    If bStateOnly Then
        If nCount > 0 Then
            i = 1
            For Each aExch In aEchanges
                If i <= nCount Then
                    mnuExchShowUnd(i).Checked = aExch.Visible
                    mnuExchShowUnd(i).Enabled = nCount > 0
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        End If
    Else
        If nCount > 0 Then
            mnuExchAllUnd.Enabled = (nCount > 0)
            mnuExchMoreUnd.Enabled = mnuExchAllUnd.Enabled
            
            For i = 1 To 9
                mnuExchShowUnd(i).Visible = False
                mnuExchShowUnd(i).Tag = 0
            Next
            
            i = 1
            For Each aExch In aEchanges
                If i <= nCount Then
                
                    If aExch.ID <> 0 Then
                        mnuExchShowUnd(i).Caption = "&" & i & " - " & aExch.Code & " - [" & aExch.Name & "]"
                    Else
                        mnuExchShowUnd(i).Caption = "&" & i & " - " & aExch.Name
                    End If
                    
                    If Not ctlView.Group.Und Is Nothing Then
                        If ctlView.Group.Und.PrimaryExchangeID = aExch.ID Then
                            mnuExchShowUnd(i).Caption = mnuExchShowUnd(i).Caption + " - <Primary>"
                        End If
                    End If
                    
                    mnuExchShowUnd(i).Visible = True
                    mnuExchShowUnd(i).Checked = aExch.Visible
                    mnuExchShowUnd(i).Enabled = nCount > 0
                    mnuExchShowUnd(i).Tag = aExch.ID
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        Else
            mnuExchAllUnd.Enabled = False
            mnuExchMoreUnd.Enabled = False
            For i = 1 To 9
                mnuExchShowUnd(i).Visible = False
                mnuExchShowUnd(i).Tag = 0
            Next
        End If
    End If
    
    Set aEchanges = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnUndExchangesChange Exit", Me.Caption
    
End Sub

Private Sub ctlView_OnExpiriesChange(ByVal bStateOnly As Boolean)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnExpiriesChange Enter", Me.Caption
    
    Dim aExpiries As EtsMmQuotesLib.MmQvExpColl, aExp As EtsMmQuotesLib.MmQvExpAtom
    Dim i&, nCount&
    
    Set aExpiries = ctlView.Expiries
    
    nCount = aExpiries.Count
    If bStateOnly Then
        If nCount > 0 Then
'            mnuExpiryAll.Enabled = (nCount > 1)
'            mnuExpiryNext.Enabled = (nCount > 1)
'            mnuExpiryPrev.Enabled = mnuExpiryNext.Enabled
            
            i = 1
            For Each aExp In aExpiries
                If i <= nCount Then
                    mnuExpiryShow(i).Checked = aExp.Visible
                    mnuExpiryShow(i).Enabled = mnuExpiryNext.Enabled
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        End If
    Else
        If nCount > 0 Then
            mnuExpiryAll.Enabled = (nCount > 1)
            mnuExpiryNext.Enabled = mnuExpiryAll.Enabled
            mnuExpiryPrev.Enabled = mnuExpiryNext.Enabled
            
            For i = 1 To 9
                mnuExpiryShow(i).Visible = False
            Next
            
            mnuExpiryMore.Enabled = True
            i = 1
            For Each aExp In aExpiries
                If i <= nCount Then
                    mnuExpiryShow(i).Caption = "&" & i & " - " & Format$(aExp.ExpiryMonth, "MMM D, YY") & IIf(Len(aExp.RootNames) > 0, " (" & aExp.RootNames & ")", "")
                    mnuExpiryShow(i).Visible = True
                    mnuExpiryShow(i).Checked = aExp.Visible
                    mnuExpiryShow(i).Enabled = mnuExpiryNext.Enabled
                End If
                i = i + 1
                If i > 9 Then Exit For
            Next
        Else
            mnuExpiryAll.Enabled = False
            mnuExpiryNext.Enabled = False
            mnuExpiryPrev.Enabled = False
            
            mnuExpiryMore.Enabled = False
            For i = 1 To 9
                mnuExpiryShow(i).Visible = False
            Next
        End If
    End If
    
    Set aExpiries = Nothing
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ctlView_OnExpiriesChange Exit", Me.Caption
    
End Sub

Private Sub ctlView_OnSetCaption()
    On Error Resume Next
   
    m_nCurImgIdx = 0
    
    If ctlView.RealTime Then m_nCurImgIdx = IIf(ctlView.RealTimeConnected, 1, 2)
    
    imgRealTime(0).Visible = (m_nCurImgIdx = 0)
    imgRealTime(1).Visible = (m_nCurImgIdx = 1)
    imgRealTime(2).Visible = (m_nCurImgIdx = 2)
    'lblStatus.Caption = imgRealTime(m_nCurImgIdx).ToolTipText
    
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
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogUserAction, "Quote ctlView_OnStateChange Enter", Me.Caption
    
    Dim nID&
    nID = ctlView.Group.ID
    mnuViewRealTime.Enabled = nID <> 0 And Not ctlView.InProc _
                            And Not ctlView.LastQuoteReqNow And Not ctlView.SubscribingNow
                            
    mnuViewRealTime.Checked = ctlView.RealTime
    mnuViewRefresh.Enabled = mnuViewRealTime.Enabled And Not ctlView.RealTime
    mnuViewRefreshPrices.Enabled = mnuViewRefresh.Enabled
     
    mnuViewDefOptSort.Enabled = nID <> 0 And Not ctlView.InProc
    
    mnuViewDividends.Enabled = (nID = 0 Or nID <> 0 And ctlView.Group.ContractType = enCtStock)
    mnuViewDividends.Checked = ctlView.DividendsVisible And ctlView.Group.ContractType = enCtStock
    mnuViewRates.Checked = ctlView.RatesVisible
    mnuViewCalcModel.Checked = ctlView.CalcModelVisible
    'mnuViewExpCalendar.Checked = ctlView.ExpCalendarVisible
    mnuViewVola.Checked = ctlView.VolaVisible
    mnuViewFutures.Checked = ctlView.FuturesVisible
    
    mnuRoot.Enabled = (nID = 0 Or nID <> 0 And ctlView.Group.ContractType <> enCtFuture)
    
    mnuExchAllUnd.Caption = IIf(nID = 0 Or nID <> 0 And ctlView.Group.ContractType <> enCtFuture, "Show &All Underlying", "Show &All Futures")
    mnuExchMoreUnd.Caption = IIf(nID = 0 Or nID <> 0 And ctlView.Group.ContractType <> enCtFuture, "&Underlying Exchanges...", "&Futures Exchanges...")

    ctlView_OnSetCaption
    
 '   If Not g_PerformanceLog Is Nothing Then _
 '       g_PerformanceLog.LogMmInfo enlogUserAction, "Quote ctlView_OnStateChange Exit", Me.Caption

End Sub

Public Sub Init(sKey As String)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quote ctlView_OnStateChange Init Enter", Me.Caption
    
    EnableActiveRealTime = True
    m_nWindowIndex = 0
    m_sFilePath = MakeCommonFilePath("", "", False)
    m_sCaption = ""
    m_sShortCaption = ""
    g_nQuotesUntitledIdx = g_nQuotesUntitledIdx + 1
    m_sFileName = "Untitled" & g_nQuotesUntitledIdx & ".qvw"
    m_bIsNew = True
    m_sKey = sKey
    mnuFileClose.Caption = "Close" & vbTab & "Alt+F4"
    
    mnuRootNext.Caption = "&Next" & vbTab & "Ctrl+Alt+F8"
    mnuRootPrev.Caption = "&Previous" & vbTab & "Ctrl+Alt+F7"
    mnuRootAll.Caption = "&Show All" & vbTab & "Ctrl+Alt+F6"
    mnuRootMore.Caption = "&Option Roots..." & vbTab & "Ctrl+Alt+F11"
    
    mnuStrikeAll.Caption = "&Show All" & vbTab & "Shift+Alt+F6"
    mnuStrikeMore.Caption = "S&trikes..." & vbTab & "Shift+Alt+F11"
    mnuOrderNewStock.Caption = "New &Stock Order..." & vbTab & "Alt+Ins"
    mnuOrderNewOption.Caption = "New &Option Order..." & vbTab & "Ctrl+Alt+Ins"
'    mnuTntCardNew.Caption = "&New Trade Message..." & vbTab & "Shift+Alt+Ins"
    
    mnuFileOrders.Visible = g_Params.OrdersVisible
    mnuOrderNewStock.Visible = g_Params.OrdersVisible
    mnuOrderNewOption.Visible = g_Params.OrdersVisible
    mnuOrder.Visible = g_Params.OrdersVisible
    
    mnuFileOrders.Enabled = g_Params.OrdersEnabled
    mnuOrderNewStock.Enabled = g_Params.OrdersEnabled
    mnuOrderNewOption.Enabled = g_Params.OrdersEnabled
    
    mnuVSCalc.Enabled = g_Params.IsVSCalcEnabled
    mnuVSCalc.Visible = g_Params.IsVSCalcEnabled
    
    Set ctlView.m_frmOwner = Me
    Set ctlView.pbProgress = pbProgress
    Set ctlView.lblProcess = lblProcess
    Set ctlView.lblStatus = lblStatus
    Set ctlView.imgStop = imgStop
    Set ctlView.imgStopDis = imgStopDis
    
    ctlView.Init
    Set aParams = g_Params
    mnuOptionsEventLog.Checked = aParams.EventLogVisible
'    mnuFileQuotation.Enabled = g_bIseEnable
    ctlView.ProcessRealTime = True

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quote ctlView_OnStateChange Init Exit", Me.Caption
    Set fMain = frmMain

End Sub

Public Sub ShowData(Optional ByVal nType As Long = TYPE_UNDERLYING, Optional ByVal nValue As Long = 0)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ShowData Call", Me.Caption
    
    ctlView.ShowData nType, nValue

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote ShowData Done", Me.Caption

End Sub

Private Sub fMain_OnActiveFuturesChange(ByVal aUndID As Long, ByVal iActiveFutID As Long)
Dim a As Integer
On Error Resume Next
    ctlView.ActiveFuturesChange aUndID, iActiveFutID
End Sub

Private Sub fMain_OnFuturesParamsChange(ByVal aUndID As Long, ByVal iFutID As Long, ByVal dRatio As Double, ByVal dBasis As Double)
On Error Resume Next
ctlView.FuturesParamsChange aUndID, iFutID, dRatio, dBasis
End Sub

Private Sub fMain_OnManualPriceChange(ByVal UndID As Long, ByVal ID As Long, ByVal price As Double, ByVal CtType As EtsGeneralLib.EtsContractTypeEnum, ByVal Status As ManualPriceUpdateEnum)
On Error Resume Next
    'manual price update for each ctrl
    ctlView.ManualPriceUpdate UndID, ID, price, CtType, Status
End Sub

Private Sub Form_KeyDown(KeyCode As Integer, Shift As Integer)
    On Error Resume Next
    If Shift = vbCtrlMask + vbAltMask Then
        Select Case KeyCode
            Case vbKeyF6
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Ctrl+Alt+F6 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                If mnuRootAll.Enabled Then mnuRootAll_Click
                
            Case vbKeyF7
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Ctrl+Alt+F7 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                If mnuRootPrev.Enabled Then mnuRootPrev_Click
                
            Case vbKeyF8
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Ctrl+Alt+F8 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                If mnuRootNext.Enabled Then mnuRootNext_Click
                
            Case vbKeyF11
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Ctrl+Alt+F11 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                If mnuRootMore.Enabled Then mnuRootMore_Click
        End Select
        
    ElseIf Shift = vbShiftMask + vbAltMask Then
            Select Case KeyCode
                Case vbKeyF6
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Shift+Alt+F6 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                    If mnuStrikeAll.Enabled Then mnuStrikeAll_Click
                    
                Case vbKeyF11
                    If Not g_PerformanceLog Is Nothing Then _
                        g_PerformanceLog.LogMmInfo enLogUserAction, "Hot key Shift+Alt+F11 pressed. " & ctlView.GetOptionInfo, Me.GetCaption
                    If mnuStrikeMore.Enabled Then mnuStrikeMore_Click
            End Select
    End If
End Sub

Private Sub Form_Load()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View FormLoad Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View FormLoad Exit - Not Done", Me.Caption
        Exit Sub
    End If
    m_bShowInTaskbar = True
    mnuWindowShowInTaskbar.Checked = True
    lblStatus.Caption = ""
    lblProcess.Caption = ""
    m_nCurImgIdx = 0
    
    Set frmItems = New frmItemsList
    Set frmRoots = New frmOptionRoot
    
    mnuToolsOptionScanner.Enabled = g_bIsScannersEnable
    mnuToolsStrategyScanner.Enabled = g_bIsScannersEnable
    
    g_Params.SetIcon WND_QUOTES, Me.hWnd, False
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View FormLoad Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Resize Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Resize Exit - Not Done", Me.Caption
        Exit Sub
    End If
    LockWindowUpdate Me.hWnd
    
    ctlView.Move 0, 0, ScaleWidth, ScaleHeight - sbStatus.Height
    picBottom.Move 0, ScaleHeight - picBottom.Height, ScaleWidth - 300, picBottom.Height

    LockWindowUpdate 0
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Resize Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Terminate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Terminate Enter", Me.Caption
    
    If gCmn Is Nothing Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Terminate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    Set aParams = Nothing
    Set frmItems = Nothing
    Set frmRoots = Nothing
    Set fMain = Nothing

    ctlView.Term

    g_ViewFrm.Remove m_sKey
    frmMain.ReloadWindowsList
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Terminate Exit - Done", Me.Caption
    
End Sub

Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogEnhDebug, "Quote View Unload", Me.Caption
        
    If Not ctlView.SaveSimulatedVolatility Then
        Cancel = True
        Exit Sub
    End If
    
    Form_Terminate
End Sub

Public Sub EnableRealTime(bEnable As Boolean)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then
        If bEnable Then
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View EnableRealTime Enter", Me.Caption
        Else
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View DisableRealTime Enter", Me.Caption
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
        If bEnable Then
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View EnableRealTime Exit", Me.Caption
        Else
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View DisableRealTime Exit", Me.Caption
        End If
    End If

End Sub

Private Sub Form_Activate()
    
    If Not g_PerformanceLog Is Nothing Then _
       g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Activation Enter", Me.Caption
    
    
    Dim nOperation&

    On Error Resume Next
    If gCmn Is Nothing Then Exit Sub
    If Not EnableActiveRealTime Or m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Activation Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        nOperation = g_PerformanceLog.BeginLogMmOperation
        
    If Not m_bShowInTaskbar Then
        ShowFormInTaskBar Me, False
    End If
    
    If (ctlView.RealTime) Then
        g_LastActiveWindow = m_nWindowIndex
    End If
    
    If aParams.ActiveRealTime Then
        ctlView.ProcessRealTime = True
        ctlView.RefreshView
    End If
    
    fMain.PassiveRealtimeControl

    If Not g_PerformanceLog Is Nothing Then
       g_PerformanceLog.FinishLogMmOperation nOperation, OPER_ACTIVATEWINDOW, GetCaption
       g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Activation Exit - Done", Me.Caption
    End If
    
        
End Sub

Private Sub Form_Deactivate()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Deactivate Enter", Me.Caption
    
    If gCmn Is Nothing Then
       If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Deactivate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    If m_bShowInTaskbarChanging Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Deactivate Exit - Not Done", Me.Caption
        Exit Sub
    End If
    
    If (ctlView.RealTime) Then
        g_LastActiveWindow = m_nWindowIndex
    End If
    
    If aParams.ActiveRealTime Then
        ctlView.ProcessRealTime = False
    End If
    
    fMain.PassiveRealtimeControl
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote View Deactivate Exit - Done", Me.Caption
    
End Sub

Private Sub aParams_ActiveRealTimeChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_ActiveRealTimeChange Enter", Me.Caption
    
    If Not EnableActiveRealTime Then
        If Not g_PerformanceLog Is Nothing Then _
            g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_ActiveRealTimeChange Exit - Not Done (Not EnableActiveRealTime)", Me.Caption
        Exit Sub
    End If
    
    If aParams.ActiveRealTime Then
        ctlView.ProcessRealTime = (Me.hWnd = GetForegroundWindow)
    Else
        ctlView.ProcessRealTime = True
        If ctlView.RealTime And Not ctlView.ProcessRealTime Then ctlView.RefreshView
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_ActiveRealTimeChange Exit - Done", Me.Caption

End Sub

Private Sub aParams_EventLogVisibleChange()
    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogUserAction, "Quote aParams_EventLogVisibleChange Call", Me.Caption
    
    mnuOptionsEventLog.Checked = aParams.EventLogVisible

 '   If Not g_PerformanceLog Is Nothing Then _
 '       g_PerformanceLog.LogMmInfo enlogUserAction, "Quote aParams_EventLogVisibleChange Done", Me.Caption

End Sub

Private Sub aParams_CalcModelChange()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_CalcModelChange Call", Me.Caption
    
    ctlView.ModelUpdate

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote aParams_CalcModelChange Done", Me.Caption

End Sub

Private Sub frmItems_OnOK(ByVal nKey As Long, aItems As EtsGeneralLib.EtsMmEntityAtomColl)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote frmItems_OnOK Enter", Me.Caption
    
    Dim aItem As EtsGeneralLib.EtsMmEntityAtom
    
    Select Case nKey
        Case 1 ' expiries
            For Each aItem In aItems
                ctlView.ShowExpiry aItem.ID, (aItem.Data <> 0), False
            Next
    
        Case 2 ' option exchanges
            For Each aItem In aItems
                ctlView.ShowOptExchange aItem.ID, (aItem.Data <> 0), False
            Next
            ctlView.RefreshOptsGrid
    
        Case 3 ' underlying exchanges
            For Each aItem In aItems
                ctlView.ShowUndExchange aItem.ID, (aItem.Data <> 0)
            Next
    
        Case 4 ' option roots
            For Each aItem In aItems
                ctlView.ShowOptRoot aItem.ID, (aItem.Data <> 0), False
            Next
    
        Case 5 ' strikes
            ctlView.ShowOptStrike aItems, False
            
        Case 6 ' futures
            For Each aItem In aItems
                ctlView.ShowFuture aItem.ID, (aItem.Data <> 0), False
            Next
            
    End Select
    ctlView.RefreshOptsGrid
    If nKey = 6 Then ctlView.RefreshFutsGrid
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote frmItems_OnOK Exit ", Me.Caption

End Sub

Private Sub frmRoots_OnOK(ByVal nKey As Long, aItems As EtsGeneralLib.EtsMmEntityAtomColl)
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote frmItems_OnOK Enter", Me.Caption
    Dim aRoots As EtsMmQuotesLib.MmQvOptRootColl, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim nCount As Long
    
    Select Case nKey
        Case 4 ' option roots
            For Each aItem In aItems
                ctlView.ShowOptRoot aItem.ID, (aItem.Data <> 0), False
            Next
            ctlView.RefreshOptsGrid
            
            Dim i As Long
            Set aRoots = ctlView.OptRoots
            nCount = aRoots.Count
            If nCount > 0 Then
                i = 1
                For Each aRoot In aRoots
                    If Not aItems(CStr(i)) Is Nothing Then
                        If aRoot.IsFit <> IIf(aItems(CStr(i)).Data3 <> 0, True, False) Then
                            aRoot.IsFit = IIf(aItems(CStr(i)).Data3 <> 0, True, False)
                            ctlView.Underlying.OptRoot(aRoot.ID).IsFit = aRoot.IsFit
                            gDBW.usp_OptionRoot_Save aRoot.ID, IIf(aRoot.IsFit, 1, 0)
                        End If
                    End If
                    i = i + 1
                Next
            End If
    End Select
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote frmRoots_OnOK Exit ", Me.Caption


End Sub

Private Sub lblStatus_DblClick()
    On Error Resume Next
'    If Not IsShiftPressed And IsAltPressed And Not IsCtrlPressed Then
'        ctlView.NewStockOrderAtNetDelta
'    End If
End Sub

Private Sub mnuExchMoreOpt_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Option Exchanges..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
    Dim i&, nCount&

    Set aEchanges = ctlView.OptExchanges

    nCount = aEchanges.Count
    If nCount > 0 Then
        i = 1
        For Each aExch In aEchanges
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = aExch.ID
                
                If aExch.ID <> 0 Then
                    aItem.Name = i & " - " & aExch.Code & " - [" & aExch.Name & "]"
                Else
                    aItem.Name = i & " - " & aExch.Name
                End If
                
                aItem.Data = IIf(aExch.Visible, 1, 0)
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit Option Exchanges """ & aItem.Name & IIf(aItem.Data = 1, """ Selected.", """ Not Selected."), Me.Caption
                
                Set aItem = Nothing
            End If
            i = i + 1
        Next

        frmItems.Execute "Option Exchanges", "Select Exchanges to Show:", aItems, 2, Me
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Option Exchanges..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
    
End Sub

Private Sub mnuExchMoreUnd_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Underlying Exchanges..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aEchanges As EtsGeneralLib.ExchColl, aExch As EtsGeneralLib.ExchAtom
    Dim i&, nCount&

    Set aEchanges = ctlView.UndExchanges

    nCount = aEchanges.Count - 1
    If nCount > 0 Then
        i = 1
        For Each aExch In aEchanges
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = aExch.ID
                
                If aExch.ID <> 0 Then
                    aItem.Name = i & " - " & aExch.Code & " - [" & aExch.Name & "]"
                Else
                    aItem.Name = i & " - " & aExch.Name
                End If
                
                If Not ctlView.Group.Und Is Nothing Then
                    If ctlView.Group.Und.PrimaryExchangeID = aExch.ID Then
                        aItem.Name = aItem.Name + " - <Primary>"
                    End If
                End If
                
                aItem.Data = IIf(aExch.Visible, 1, 0)
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit " & IIf(ctlView.Group.ID = 0 Or ctlView.Group.ID <> 0 And ctlView.Group.ContractType <> enCtFuture, "Underlying Exchanges", "Futures Exchanges") & " """ & aItem.Name & IIf(aItem.Data = 1, """ Selected.", """ Not Selected."), Me.Caption
                
                Set aItem = Nothing
            End If
            i = i + 1
        Next

        frmItems.Execute IIf(ctlView.Group.ID = 0 Or ctlView.Group.ID <> 0 And ctlView.Group.ContractType <> enCtFuture, _
                        "Underlying Exchanges", "Futures Exchanges"), "Select Exchanges to Show:", aItems, 3, Me, True
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Underlying Exchanges..."" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuExchNextOpt_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Next Option"" Enter. " & ctlView.GetOptionInfo, Me.Caption

    ctlView.ShowNextOptExchange

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Next Option"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuExchPrevOpt_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Previous Option"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowPrevOptExchange

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Previous Option"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuExchAllOpt_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show All Option"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowAllOptExchanges

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show All Option"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuExchAllUnd_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show All Underlying"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowAllUndExchanges

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show All Underlying"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuExchShowOpt_Click(Index As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show Option Exchange"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowOptExchange mnuExchShowOpt(Index).Tag, Not mnuExchShowOpt(Index).Checked

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show Option Exchange"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExchShowUnd_Click(Index As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show Underlying Exchange"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowUndExchange mnuExchShowUnd(Index).Tag, Not mnuExchShowUnd(Index).Checked

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Exchange->Show Underlying Exchange"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExpiryMore_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Expiries..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aExpiries As EtsMmQuotesLib.MmQvExpColl, aExp As EtsMmQuotesLib.MmQvExpAtom
    Dim i&, nCount&
    
    Set aExpiries = ctlView.Expiries
    
    nCount = aExpiries.Count
    If nCount > 0 Then
        i = 1
        For Each aExp In aExpiries
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = i
                aItem.Name = i & " - " & Format$(aExp.ExpiryMonth, "MMM D, YY") & IIf(Len(aExp.RootNames) > 0, " (" & aExp.RootNames & ")", "")
                aItem.Data = IIf(aExp.Visible, 1, 0)
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit Expiries """ & aItem.Name & IIf(aItem.Data = 1, """ Selected.", """ Not Selected."), Me.Caption
                    
                Set aItem = Nothing
            End If
            i = i + 1
        Next
        
        frmItems.Execute "Expiries", "Select Expiries to Show:", aItems, 1, Me
    End If

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Expiries..."" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileBatchReporting_Click()
    On Error Resume Next
    g_frmProjections.ShowData
End Sub

Private Sub mnuFIleGraphicalPnL_Click()
    On Error Resume Next
    frmMain.ShowGraphicalPnL
End Sub

Private Sub mnuFileOtcOptionCalc_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->OTC Option Calc..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.OTCOptionCalcCall

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->OTC Option Calc..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuManualPrices_Click()
    Dim frmMPrices As New frmManualPrices
    Dim i As Integer
    Dim ctrID() As Long, price() As Double, isManual() As Boolean
    
    frmMPrices.Show vbModal, Me
    
    If (frmMPrices.ChangedCount > 0) Then
        ctlView.UpdateManualPrices ctrID, price, isManual
    End If
        
End Sub

Private Sub mnuRootAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Show All"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowAllOptRoots

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Show All"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuRootNext_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Next"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowNextOptRoot

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Next"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuRootPrev_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Previous"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowPrevOptRoot
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Previous"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuRootShow_Click(Index As Integer)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Show"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowOptRoot Index, Not mnuRootShow(Index).Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Show"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuRootMore_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Option Roots..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aRoots As EtsMmQuotesLib.MmQvOptRootColl, aRoot As EtsMmQuotesLib.MmQvOptRootAtom
    Dim i&, nCount&
    
    Set aRoots = ctlView.OptRoots
    
    nCount = aRoots.Count
    If nCount > 0 Then
        i = 1
        For Each aRoot In aRoots
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = i
                aItem.Name = i & " - " & aRoot.Name
                aItem.Data = IIf(aRoot.Visible, 1, 0)
                aItem.Data3 = IIf(aRoot.IsFit, 1, 0)
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit Option Roots """ & aItem.Name _
                                                                & IIf(aItem.Data = 1, """ ""Display"" Selected.", """ ""Display"" Not Selected.") _
                                                                & IIf(aItem.Data3 = 1, " ""Use when Fitting to IV"" Selected.", " ""Use when Fitting to IV"" Not Selected."), Me.Caption
                
                Set aItem = Nothing
            End If
            i = i + 1
        Next
        
        frmRoots.Execute "Option Roots", "Select Option Roots to Show:", aItems, 4, Me
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Option Root->Option Roots..."" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFutAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Show All"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowAllFutures

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Show All"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFutNext_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Next"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowNextFut

    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Next"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFutPrev_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Previous"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowPrevFut
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Previous"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFutShow_Click(Index As Integer)
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Show"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowFuture Index, Not mnuFutShow(Index).Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Show"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFutMore_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Futures..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aFuts As EtsMmQuotesLib.MmQvFutCollection, aFut As EtsMmQuotesLib.MmQvFutAtom
    Dim i&, nCount&
    
    Set aFuts = ctlView.Underlying.Fut
    
    nCount = aFuts.Count
    If nCount > 0 Then
        i = 1
        For Each aFut In aFuts
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = i
                aItem.Name = i & " - " & aFut.Symbol
                aItem.Data = IIf(aFut.Visible, 1, 0)
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit Futures """ & aItem.Name _
                                                                & IIf(aItem.Data = 1, """ ""Display"" Selected.", """ ""Display"" Not Selected."), Me.Caption
                
                Set aItem = Nothing
            End If
            i = i + 1
        Next
        
        frmItems.Execute "Futures", "Select Futures to Show:", aItems, 6, Me
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Futures->Futures..."" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuFileClose_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Unload Me
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Close"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub Save(ByVal sFilePath As String, ByVal sFileName As String)
    Dim aStorage As New clsSettingsStorage
    Screen.MousePointer = vbHourglass
    aStorage.Init sFilePath & sFileName
    aStorage.SetStringValue "Type", "Value", "QuotesView"
    ctlView.SaveToFile aStorage, ""
    aStorage.SaveData
    m_sFilePath = sFilePath
    m_sFileName = sFileName
    m_bIsNew = False
    AdjustCaption
    Screen.MousePointer = vbDefault
End Sub

Public Sub SaveLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
    ctlView.SaveToFile aStorage, sKey
End Sub

Public Sub OpenLayout(aStorage As clsSettingsStorage, ByVal sKey As String)
    ctlView.OpenFromFile aStorage, sKey
End Sub

Private Sub mnuFileNew_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.ShowQuotes
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->New..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFilePrint_Click()
    On Error Resume Next
'    ctlView.PrintView
End Sub

Private Sub mnuFileQuotes_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowQuotes TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Quotes..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileReportsView_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Reports..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowReports TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Reports..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileRiskMatrix_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risk Matrix..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowRiskMatrix TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risk Matrix..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileRisks_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risks..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowRisks TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Risks..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileTrades_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Trades..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowTrades TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Trades..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileVola_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Monitor..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowVola TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Monitor..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileQuotation_Click()
    On Error Resume Next
'    frmMain.ShowQuotation TYPE_UNDERLYING, ctlView.Underlying.ID
End Sub

Private Sub mnuFileVolaAnalysis_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Analysis..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowVolaAnalysis TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Volatility Analysis..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileGreeks_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowGreeks TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Greeks..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileHedgeSummaryView_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowHedgeSummary TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Hedge Summary..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileOrders_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Orders..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowOrders TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Orders..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuFileTntCards_Click()
    On Error Resume Next
'    frmMain.ShowTntCards TYPE_UNDERLYING, ctlView.Underlying.ID
End Sub

Private Sub mnuFileStockHedge_Click()
    On Error Resume Next
'    frmMain.ShowStockHedge TYPE_UNDERLYING, ctlView.Underlying.ID
End Sub

Private Sub mnuFileIndex1Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex1Hedge TYPE_UNDERLYING, ctlView.Underlying.ID
End Sub

Private Sub mnuFileIndex2Hedge_Click()
    On Error Resume Next
'    frmMain.ShowIndex2Hedge TYPE_UNDERLYING, ctlView.Underlying.ID
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
            .DefaultExt = ".qvw"
            .DialogTitle = "Save Quotes"
            .InitDir = ""
            .FileName = m_sFilePath & m_sFileName
            .Filter = "ETS Quotes (*.qvw)|*.qvw|All Files|*.*"
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
    gCmn.ErrorMsgBox Me, "Fail to save quotes."
End Sub

Private Sub mnuFileSaveAs_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNNoReadOnlyReturn Or cdlOFNOverwritePrompt
        .CancelError = True
        .DefaultExt = ".qvw"
        .DialogTitle = "Save Quotes As"
        .InitDir = ""
        .FileName = m_sFilePath & m_sFileName
        .Filter = "ETS Quotes (*.qvw)|*.qvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowSave
        If Err.Number = 0 Then
            On Error GoTo EH
            Save ExtractFilePath(.FileName), ExtractFileName(.FileName)
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Save As..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to save quotes."
End Sub

Private Sub mnuFileOpen_Click()
    On Error GoTo EH
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aStorage As New clsSettingsStorage
    
    If Not ctlView.SaveSimulatedVolatility Then Exit Sub
    
    With dlgCommon
        .Flags = cdlOFNExplorer Or cdlOFNLongNames Or cdlOFNPathMustExist _
                Or cdlOFNHideReadOnly Or cdlOFNFileMustExist
        .CancelError = True
        .DefaultExt = ".qvw"
        .DialogTitle = "Open Quotes"
        .FileName = ""
        .InitDir = m_sFilePath
        .Filter = "ETS Quotes (*.qvw)|*.qvw|All Files|*.*"
        .FilterIndex = 1
        On Error Resume Next
        .ShowOpen
        If Err.Number = 0 Then
            On Error GoTo EH
            Screen.MousePointer = vbHourglass
            aStorage.Init .FileName
            aStorage.ReadData
            If aStorage.GetStringValue("Type", "Value") = "QuotesView" Then
                ctlView.OpenFromFile aStorage, ""
                m_sFilePath = ExtractFilePath(.FileName)
                m_sFileName = ExtractFileName(.FileName)
                m_bIsNew = False
                AdjustCaption
                Screen.MousePointer = vbDefault
            Else
                Screen.MousePointer = vbDefault
                gCmn.MyMsgBox Me, "Invalid quotes file."
            End If
        End If
    End With
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->Open..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
    
    Exit Sub
EH:
    Screen.MousePointer = vbDefault
    gCmn.ErrorMsgBox Me, "Fail to open quotes."
End Sub

Private Sub mnuStrikeAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Strike->Show All"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.ShowAllStrikes
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Strike->Show All"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuStrikeMore_Click()
    On Error Resume Next
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Strike->Strikes..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    Dim aItems As New EtsGeneralLib.EtsMmEntityAtomColl, aItem As EtsGeneralLib.EtsMmEntityAtom
    Dim aStrikes As EtsMmQuotesLib.MmQvStrikeColl, aStr As EtsMmQuotesLib.MmQvStrikeAtom
    Dim i&, nCount&
    
    Set aStrikes = ctlView.Strikes
    
    nCount = aStrikes.Count
    If nCount > 0 Then
        i = 1
        For Each aStr In aStrikes
            If aItems(CStr(i)) Is Nothing Then
                Set aItem = aItems.Add(CStr(i))
                aItem.ID = i
                aItem.Name = aStr.Strike
                aItem.Data = IIf(aStr.Visible, 1, 0)
                aItem.Data3 = aStr.Strike
                
                If Not g_PerformanceLog Is Nothing Then _
                    g_PerformanceLog.LogMmInfo enLogUserAction, "Before Edit Strikes """ & aItem.Name & IIf(aItem.Data = 1, """ Selected.", """ Not Selected."), Me.Caption
                
                Set aItem = Nothing
            End If
            i = i + 1
        Next
        
        frmItems.Execute "Strikes", "Select Strikes to Show:", aItems, 5, Me
    End If
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Strike->Strikes..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
        
End Sub

Private Sub mnuViewDefOptSort_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Default Options Sorting"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.SortOptionsByDefault
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Default Options Sorting"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

'Private Sub mnuViewExpCalendar_Click()
'    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Expiry Calendar"" Enter. " & ctlView.GetOptionInfo, Me.Caption
'
'    ctlView.ExpCalendarVisible = Not mnuViewExpCalendar.Checked
'
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Expiry Calendar"" Exit. Menu " & IIf(mnuViewExpCalendar.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
'End Sub

Private Sub mnuOptionsParameters_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    frmMain.ShowParameters
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Parameters..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewFutGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Future Grid Layout..."" Enter. " & ctlView.GetFutureInfo, Me.Caption
    
    ctlView.CustomizeFutureGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Future Grid Layout..."" Exit. " & ctlView.GetFutureInfo, Me.Caption
End Sub

Private Sub mnuViewFutures_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Futures"" Enter. " & ctlView.GetFutureInfo, Me.Caption
    
    ctlView.FuturesVisible = Not mnuViewFutures.Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Futures"" Exit. Menu " & IIf(mnuViewFutures.Checked, "Checked. ", "Unchecked. ") & ctlView.GetFutureInfo, Me.Caption
End Sub

Private Sub mnuViewRealTime_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Real Time Mode"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.ToggleRealtime
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Real Time Mode"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExpiryAll_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Show All"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.ShowAllExpiries
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Show All"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExpiryNext_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Next"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.ShowNextExpiry
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Next"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExpiryPrev_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Previous"" Enter. " & ctlView.GetOptionInfo, Me.Caption
        
    ctlView.ShowPrevExpiry
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Previous"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuExpiryShow_Click(Index As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Show"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.ShowExpiry Index, Not mnuExpiryShow(Index).Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Expiry->Show"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewRefresh_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.Refresh
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Refresh"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewDivGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Parameters Grid Layout..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.CustomizeDivGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Parameters Grid Layout..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewDividends_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Dividends"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.DividendsVisible = Not mnuViewDividends.Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Dividends"" Exit. Menu " & IIf(mnuViewDividends.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuOptionsEventLog_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ShowEventLog (Not mnuOptionsEventLog.Checked), , True
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Options->Event Log"" Exit. Menu " & IIf(mnuOptionsEventLog.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewOptGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Option Grid Layout..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.CustomizeOptionGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Option Grid Layout..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewRefreshPrices_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->RefreshPrices"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.RefreshPrices
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->RefreshPrices"" Exit. " & ctlView.GetOptionInfo, Me.Caption

End Sub

Private Sub mnuViewVolGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Volatility Grid Layout..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.CustomizeVolaGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Volatility Grid Layout..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewRates_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Rates"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.RatesVisible = Not mnuViewRates.Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Rates"" Exit. Menu " & IIf(mnuViewRates.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewCalcModel_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Calculation Model"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.CalcModelVisible = Not mnuViewCalcModel.Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Calculation Model"" Exit. Menu " & IIf(mnuViewCalcModel.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewVola_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Volatility"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.VolaVisible = Not mnuViewVola.Checked
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Volatility"" Exit. Menu " & IIf(mnuViewVola.Checked, "Checked. ", "Unchecked. ") & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuViewUndGridLayout_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Underlying Grid Layout..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ctlView.CustomizeUnderlyingGridLayout
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""View->Underlying Grid Layout..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuToolsBackOffice_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->ETS Back Office..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.RunBackOffice
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->ETS Back Office..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
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
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote mnuToolsHedger_Click", Me.Caption
    
'    frmMain.RunHedger
End Sub

Private Sub mnuToolsVolaManager_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager..."" Enter. " & ctlView.GetOptionInfo, Me.Caption

    frmMain.RunVolaManager
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Tools->Volatility Manager..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Public Sub AdjustWindowsListCaptions()
    On Error Resume Next
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogUserAction, "Quote AdjustWindowsListCaptions Enter", Me.Caption
    
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
    
'    If Not g_PerformanceLog Is Nothing Then _
'        g_PerformanceLog.LogMmInfo enlogUserAction, "Quote AdjustWindowsListCaptions Exit", Me.Caption

End Sub

Private Sub mnuVSCalc_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->VSCalc..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.ShowVSC TYPE_UNDERLYING, ctlView.Underlying.ID
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""File->VSCalc..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Windows..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.WindowsList
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Windows..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuWindowMain_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main ETS Window..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    ShowWindow frmMain.hWnd, SW_SHOWNORMAL
    frmMain.SetFocus
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Main ETS Window..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
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
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Tile Vertically"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuWindowCascade_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Cascade"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.WindowsCascade
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Cascade"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuWindowShowInTaskBar_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Show All In Taskbar"" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.WindowsShowInTaskBar Not g_Params.ShowWindowsInTaskbar
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Window->Show All In Taskbar"" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Private Sub mnuWindowGo_Click(Index As Integer)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote mnuWindowGo_Click: " & CStr(Index), Me.Caption
    
    frmMain.WindowsGo Index
End Sub

Public Sub SetGridLayout(ByVal enGridType As GridTypeEnum, gdGrid As clsGridDef)
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Quote SetGridLayout: " & CStr(enGridType), Me.Caption
    
    ctlView.SetGridLayout enGridType, gdGrid
End Sub

Private Sub mnuHelpContents_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""?->Contents..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.HelpContents
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""?->Contents..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
End Sub

Public Property Get WindowKey() As String
    On Error Resume Next
    WindowKey = m_sKey
End Property

Private Sub mnuManageUnderlyings_Click()
    On Error Resume Next
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage->Underlyings..."" Enter. " & ctlView.GetOptionInfo, Me.Caption
    
    frmMain.ShowManageUnderlyings
    
    If Not g_PerformanceLog Is Nothing Then _
        g_PerformanceLog.LogMmInfo enLogUserAction, "Menu ""Manage->Underlyings..."" Exit. " & ctlView.GetOptionInfo, Me.Caption
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
'    ctlView.MakeDataSnapshot DATA_SHAPSHOT_FILENAME & "_Quotes_" & m_sKey & ".txt"
End Sub

Public Sub StopRT()
    On Error Resume Next
    ctlView.ctlStopRT
End Sub

Private Sub mnuExerciseScreen_Click()
    On Error Resume Next
    frmMain.ShowExerciseScreen
End Sub
