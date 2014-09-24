VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "comdlg32.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEndOfDay 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "End of Day"
   ClientHeight    =   8535
   ClientLeft      =   45
   ClientTop       =   330
   ClientWidth     =   9870
   Icon            =   "frmEndOfDay.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   8535
   ScaleWidth      =   9870
   ShowInTaskbar   =   0   'False
   StartUpPosition =   2  'CenterScreen
   Begin VB.Timer tmrExecute 
      Enabled         =   0   'False
      Interval        =   500
      Left            =   2040
      Top             =   120
   End
   Begin ElladaFlatControls.FlatButton ActiveLabel1 
      Height          =   255
      Left            =   600
      TabIndex        =   29
      Top             =   8160
      Width           =   1935
      _ExtentX        =   3413
      _ExtentY        =   450
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "Copy log to clipboard"
   End
   Begin MSComctlLib.TreeView TreeView1 
      Height          =   7095
      Left            =   3240
      TabIndex        =   13
      Top             =   1320
      Width           =   6495
      _ExtentX        =   11456
      _ExtentY        =   12515
      _Version        =   393217
      Indentation     =   106
      Style           =   2
      BorderStyle     =   1
      Appearance      =   0
   End
   Begin VB.Timer tmrProgressEmulate 
      Enabled         =   0   'False
      Interval        =   20
      Left            =   3720
      Top             =   240
   End
   Begin VB.PictureBox picSettings 
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      FillColor       =   &H0000FFFF&
      ForeColor       =   &H80000004&
      Height          =   1395
      Left            =   4920
      ScaleHeight     =   1365
      ScaleWidth      =   2565
      TabIndex        =   27
      Top             =   3480
      Visible         =   0   'False
      Width           =   2595
      Begin MSForms.CheckBox chkDividents 
         Height          =   315
         Left            =   120
         TabIndex        =   14
         Top             =   60
         Width           =   1515
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "2672;556"
         Value           =   "0"
         Caption         =   "Pay Dividends"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkOptions 
         Height          =   315
         Left            =   120
         TabIndex        =   15
         Top             =   360
         Width           =   1575
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "2778;556"
         Value           =   "0"
         Caption         =   "Exercise options"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkExpiredContr 
         Height          =   315
         Left            =   120
         TabIndex        =   16
         Top             =   720
         Width           =   2235
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "3942;556"
         Value           =   "0"
         Caption         =   "Remove Expired Contracts"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkOffset 
         Height          =   315
         Left            =   120
         TabIndex        =   17
         Top             =   960
         Width           =   2175
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "3836;556"
         Value           =   "0"
         Caption         =   "Offsetting"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
   End
   Begin VB.PictureBox picSplitMerge 
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      FillColor       =   &H0000FFFF&
      ForeColor       =   &H80000004&
      Height          =   800
      Left            =   4680
      ScaleHeight     =   765
      ScaleWidth      =   2565
      TabIndex        =   26
      Top             =   5280
      Visible         =   0   'False
      Width           =   2595
      Begin MSForms.CheckBox chkSplit 
         Height          =   315
         Left            =   120
         TabIndex        =   18
         Top             =   60
         Width           =   2355
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4154;556"
         Value           =   "0"
         Caption         =   "Split Trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkMerge 
         Height          =   315
         Left            =   120
         TabIndex        =   19
         Top             =   360
         Width           =   2355
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4154;556"
         Value           =   "0"
         Caption         =   "Merge Trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
   End
   Begin MSComctlLib.ProgressBar pbProgress 
      Height          =   315
      Left            =   120
      TabIndex        =   24
      Top             =   480
      Width           =   8175
      _ExtentX        =   14420
      _ExtentY        =   556
      _Version        =   393216
      Appearance      =   0
      Max             =   1000
   End
   Begin ElladaFlatControls.FlatButton btnClose 
      Height          =   300
      Left            =   8400
      TabIndex        =   1
      Top             =   480
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Close"
   End
   Begin ElladaFlatControls.FlatButton btnExecute 
      Height          =   300
      Left            =   8400
      TabIndex        =   0
      Top             =   120
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "Arial"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Execute"
   End
   Begin MSComDlg.CommonDialog dlgFileSelect 
      Left            =   4560
      Top             =   120
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      FillColor       =   &H0000FFFF&
      ForeColor       =   &H80000004&
      Height          =   6735
      Left            =   120
      ScaleHeight     =   6705
      ScaleWidth      =   2985
      TabIndex        =   28
      Top             =   1320
      Width           =   3015
      Begin VB.OptionButton optBasketIndex 
         Appearance      =   0  'Flat
         BackColor       =   &H00F7F3F7&
         Caption         =   "from file"
         Enabled         =   0   'False
         Height          =   255
         Index           =   1
         Left            =   360
         MaskColor       =   &H80000005&
         TabIndex        =   33
         Tag             =   "noauto"
         Top             =   2160
         Value           =   -1  'True
         Visible         =   0   'False
         Width           =   1335
      End
      Begin VB.OptionButton optBasketIndex 
         Appearance      =   0  'Flat
         BackColor       =   &H00F7F3F7&
         Caption         =   "from IVolatility"
         Enabled         =   0   'False
         Height          =   255
         Index           =   0
         Left            =   360
         MaskColor       =   &H80000005&
         TabIndex        =   32
         Tag             =   "noauto"
         Top             =   1920
         Visible         =   0   'False
         Width           =   1335
      End
      Begin MSForms.CheckBox chkLoadClosePrice 
         Height          =   315
         Left            =   120
         TabIndex        =   34
         Tag             =   "1"
         Top             =   4440
         Width           =   2655
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4683;556"
         Value           =   "0"
         Caption         =   "Load Stock close price"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkDivsImport 
         Height          =   315
         Left            =   120
         TabIndex        =   35
         Tag             =   "1"
         Top             =   3720
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Dividends Import"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkImportGroups 
         Height          =   315
         Left            =   120
         TabIndex        =   31
         Tag             =   "1"
         Top             =   3360
         Width           =   2535
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4471;556"
         Value           =   "0"
         Caption         =   "Import Groups"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkImportCurve 
         Height          =   315
         Left            =   120
         TabIndex        =   30
         Tag             =   "1"
         Top             =   1920
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "IR Curve Import"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkPositionNetting 
         Height          =   315
         Left            =   120
         TabIndex        =   12
         Tag             =   "1"
         Top             =   3000
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Position Netting"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkVolatilityImport 
         Height          =   315
         Left            =   120
         TabIndex        =   11
         Tag             =   "1"
         Top             =   2640
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Default Volatility Import"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkTradesImport 
         Height          =   315
         Left            =   120
         TabIndex        =   10
         Tag             =   "1"
         Top             =   2280
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Trades Import"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkLoadBasketWeights 
         Height          =   315
         Left            =   120
         TabIndex        =   8
         Tag             =   "1"
         Top             =   1560
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Load Index Basket Weights"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkExportSecurities 
         Height          =   315
         Left            =   120
         TabIndex        =   7
         Tag             =   "1"
         Top             =   5160
         Visible         =   0   'False
         Width           =   2655
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4683;556"
         Value           =   "0"
         Caption         =   "Export new traded securities"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkExportTrades 
         Height          =   315
         Left            =   120
         TabIndex        =   6
         Tag             =   "1"
         Top             =   5520
         Visible         =   0   'False
         Width           =   2535
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4471;556"
         Value           =   "0"
         Caption         =   "Export new non RTTF trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkImportMktStr 
         Height          =   315
         Left            =   120
         TabIndex        =   9
         Tag             =   "1"
         Top             =   4080
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Finish Import market structure"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkEraseDeletedTrades 
         Height          =   315
         Left            =   120
         TabIndex        =   5
         Tag             =   "1"
         Top             =   1200
         Width           =   2775
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4895;556"
         Value           =   "0"
         Caption         =   "Erase Deleted and Expired Trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkLoadOptExchanges 
         Height          =   315
         Left            =   120
         TabIndex        =   4
         Tag             =   "1"
         Top             =   840
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Load Option Exchanges"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkReconcileTrades 
         Height          =   315
         Left            =   120
         TabIndex        =   2
         Tag             =   "1"
         Top             =   120
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Reconcile Trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkReconcilePositions 
         Height          =   315
         Left            =   120
         TabIndex        =   3
         Tag             =   "1"
         Top             =   480
         Width           =   2415
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4260;556"
         Value           =   "0"
         Caption         =   "Reconcile Positions"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.CheckBox chkExportAllTrades 
         Height          =   315
         Left            =   120
         TabIndex        =   36
         Tag             =   "1"
         Top             =   4800
         Width           =   2655
         VariousPropertyBits=   746588179
         BackColor       =   -2147483633
         ForeColor       =   -2147483630
         DisplayStyle    =   4
         Size            =   "4683;556"
         Value           =   "0"
         Caption         =   "Export Trades"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
   End
   Begin VB.Line Line1 
      X1              =   120
      X2              =   9720
      Y1              =   960
      Y2              =   960
   End
   Begin VB.Label lblStatus 
      BackStyle       =   0  'Transparent
      Caption         =   "Process Status"
      Height          =   255
      Left            =   120
      TabIndex        =   25
      Top             =   120
      Width           =   6675
   End
   Begin MSForms.CheckBox chkSysDate 
      Height          =   315
      Left            =   4560
      TabIndex        =   20
      Top             =   5880
      Visible         =   0   'False
      Width           =   2115
      VariousPropertyBits=   746588179
      BackColor       =   -2147483633
      ForeColor       =   -2147483630
      DisplayStyle    =   4
      Size            =   "3731;556"
      Value           =   "0"
      Caption         =   "Change System Date"
      SpecialEffect   =   0
      FontHeight      =   165
      FontCharSet     =   204
      FontPitchAndFamily=   2
   End
   Begin VB.Label Label3 
      BackStyle       =   0  'Transparent
      Caption         =   "Show Reports"
      Height          =   195
      Left            =   3240
      TabIndex        =   23
      Top             =   1080
      Visible         =   0   'False
      Width           =   1335
   End
   Begin VB.Label Label2 
      BackStyle       =   0  'Transparent
      Caption         =   "Split / Merge"
      Height          =   195
      Left            =   4680
      TabIndex        =   22
      Top             =   4920
      Visible         =   0   'False
      Width           =   1155
   End
   Begin VB.Label Label1 
      BackStyle       =   0  'Transparent
      Caption         =   "Steps"
      Height          =   195
      Left            =   120
      TabIndex        =   21
      Top             =   1080
      Width           =   2475
   End
End
Attribute VB_Name = "frmEndOfDay"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Const xFilter = "" ' "vcSymbol = 'DJX'"
Private Const RT_TIMEOUT = 6000
Private Const RT_DELAY = 0

Private fBusy As Boolean
Private fCancel As Boolean

Private fCanSave As Boolean

Private tAlarm As Date

Private tStart As Date
Private tStop As Date
Private Duration As Double
Private Performance As Double
Private m_Parent As String

Private LastErrorNumber As Long
Private LastErrorDescription  As String
Private LastErrorSource As String

Private m_aTradeLog As New clsTradeLogColl

Const EXECUTEIT = "&Execute"
Const STOPIT = "&Stop"
Const STEP_VALUE = 200

Const YEAR_FORMAT = "YYYY"
Const DAY_FORMAT = "DD"
Const MONTH_FORMAT = "MM"

Const INVALID_HANDLE_VALUE = -1
Enum enmTradeLogError
    errInvalidOption = 1
    errInvalidUser
    errInvalidTradePrice
    errInvalidTradeDate
    errInvalidStockSymbol
    errInvalidAccountAcronym
    errInvalidOptionType
    
    errEmptyFile
    errInvalidFileFormat
    errParserError
    errDBError
End Enum
Enum enmStatus
    st00_Initial
    st01_ReconcilePositions
    st02_ReconcileTrades
    st03_LoadOptExhanges
    st04_EraseDeletedTrades
    st05_ImportMarketStructure
    st06_ExportTodayTrades
    st07_ExportNewSecurities
    st08_LoadBasketIndex
    st09_TradesImport
    st10_VolatilityImport
    st11_PositionNetting
    st12_Completed
    st13_ImportCurve
    st14_ImportGroups
    st15_LoadStockClosePrice
    st16_ExportAllTrades
End Enum

Private Type tAction
    StatusEnm As enmStatus
    TickValue As Long
End Type

Private udtDemoProcess() As tAction
Private lngCurrentTicker As Long
Private lngCounter As Long

Private fNeedRefreshMarket As Boolean
Private fNeedRefreshTraders As Boolean
Private fNeedRefreshCurrent As Boolean

Private SaveOptionExchBuffer As String
Private SaveOptionExchCount As Long

Private p_Symbol As String

Private m_hLog As Long

Private m_MsUndColl As clsMsUndColl

Private m_clsPosRecProcessor As clsPosReconcileProcessor

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Execute()
    
    If Not CheckMarketStructureUpdates Then Exit Sub
    
    If CheckMarketStructureInCache Then chkImportMktStr.Visible = True
    
    ValidateState
    pbProgress.Value = pbProgress.Min
    
    gCmn.CustomizeForm Me
    SetStatus st00_Initial
    Busy = False
    m_Parent = ""
    With TreeView1
        .Nodes.Clear
        .Nodes.Add , , "r0", ""
    End With
    
    fNeedRefreshMarket = False
    fNeedRefreshTraders = False
    fNeedRefreshCurrent = False
    
Me.Show vbModal
    
    If fNeedRefreshMarket Then
        frmMain.RefreshMarket
        fNeedRefreshMarket = False
    End If
    If fNeedRefreshTraders Then
        frmMain.RefreshTraders
        fNeedRefreshTraders = False
    End If
    If fNeedRefreshCurrent Then
        frmMain.RefreshCurrent
        fNeedRefreshCurrent = False
    End If
    
End Sub

Private Sub chkDivsImport_Click()
    ValidateState
End Sub

Private Sub chkExpiredContr_Click()
    ValidateState
End Sub

Private Sub chkExportAllTrades_Click()
    ValidateState
End Sub

Private Sub chkExportSecurities_Click()
    ValidateState
End Sub

Private Sub chkExportTrades_Click()
    ValidateState
End Sub

Private Sub chkImportCurve_Click()
    ValidateState
End Sub

Private Sub chkImportGroups_Click()
    ValidateState
End Sub

Private Sub chkLoadBasketWeights_Click()
    ValidateState
    optBasketIndex(0).Enabled = chkLoadBasketWeights.Value
    optBasketIndex(1).Enabled = chkLoadBasketWeights.Value
End Sub

Private Sub chkLoadClosePrice_Click()
    ValidateState
End Sub

Private Sub chkPositionNetting_Click()
    ValidateState
End Sub

Private Sub chkTradesImport_Click()
    ValidateState
End Sub

Private Sub chkVolatilityImport_Click()
    ValidateState
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
    On Error Resume Next
    fCancel = False
    OpenLogFile
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
    On Error Resume Next
    If UnloadMode = vbFormControlMenu Then
        Cancel = Busy
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    fCancel = True
    Set m_MsUndColl = Nothing
    Set m_clsPosRecProcessor = Nothing
    CloseLogFile
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnClose_Click()
    On Error Resume Next
    Unload Me
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub chkReconcilePositions_Click()
    ValidateState
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub chkReconcileTrades_Click()
    ValidateState
End Sub

Private Sub chkEraseDeletedTrades_Click()
    ValidateState
End Sub

Private Sub chkImportMktStr_Click()
    ValidateState
End Sub

Private Sub chkLoadOptExchanges_Click()
    ValidateState
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetStatus(udtStatus As enmStatus)
    
    Select Case udtStatus
    Case st00_Initial
    Case st01_ReconcilePositions
        lblStatus.Caption = "Reconciling Positions..."
    Case st02_ReconcileTrades
        lblStatus.Caption = "Reconciling Trades..."
    Case st03_LoadOptExhanges
        lblStatus.Caption = "Loading Options Exchanges..."
    Case st04_EraseDeletedTrades
        lblStatus.Caption = "Erasing deleted and expired trades..."
    Case st05_ImportMarketStructure
        lblStatus.Caption = "Finish importing market structure..."
    Case st06_ExportTodayTrades
        lblStatus.Caption = "Export today non RTTF trades..."
    Case st07_ExportNewSecurities
        lblStatus.Caption = "Export new securities traded today..."
    Case st08_LoadBasketIndex
        lblStatus.Caption = "Loading index basket weights..."
    Case st09_TradesImport
        lblStatus.Caption = "Trades import..."
    Case st10_VolatilityImport
        lblStatus.Caption = "Default volatility import..."
    Case st11_PositionNetting
        lblStatus.Caption = "Position netting..."
    Case st12_Completed
        lblStatus.Caption = "Financial day closed."
    Case st14_ImportGroups
        lblStatus.Caption = "Importing Groups..."
    Case st15_LoadStockClosePrice
        lblStatus.Caption = "Load stock close price..."
    End Select
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Let EnableControls(ByVal blnEnabled As Boolean)
Dim ctlBox As Variant
    For Each ctlBox In Me.Controls
        If TypeOf ctlBox Is MSForms.CheckBox Then
            If ctlBox.Tag = 1 Then
                ctlBox.Enabled = blnEnabled
            End If
        End If
    Next ctlBox
    
    optBasketIndex(0).Enabled = chkLoadBasketWeights.Value And blnEnabled
    optBasketIndex(1).Enabled = chkLoadBasketWeights.Value And blnEnabled
    
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ValidateState()
Dim blnEnabled As Boolean
Dim ctlBox As Variant
    blnEnabled = False
    For Each ctlBox In Me.Controls
        If TypeOf ctlBox Is MSForms.CheckBox Then
            If ctlBox.Tag = 1 Then
                If ctlBox.Value = True Then
                    blnEnabled = True
                End If
            End If
        End If
    Next ctlBox
    btnExecute.Enabled = blnEnabled
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetValidXMLText(sStr As String) As String
    On Error Resume Next
    Dim sOut As String
    
    sOut = sStr
    sOut = Replace(sOut, "&", "&amp;")
    sOut = Replace(sOut, "<", "&lt;")
    sOut = Replace(sOut, ">", "&gt;")
    sOut = Replace(sOut, "'", "&apos;")
    sOut = Replace(sOut, """", "&quot;")
    
    GetValidXMLText = sOut
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Get Busy() As Boolean
    Busy = fBusy
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Property Let Busy(ByVal vNewValue As Boolean)
    fBusy = vNewValue
    If fBusy Then
        btnExecute.Caption = STOPIT
        btnClose.Enabled = False
    Else
        btnExecute.Caption = EXECUTEIT
        btnClose.Enabled = True
    End If
    DoEvents
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnExecute_Click()
    On Error Resume Next
    If Busy Then
        fCancel = True
    Else
        tmrExecute.Enabled = True
    End If
End Sub

Private Sub tmrExecute_Timer()
    On Error GoTo EH
    tmrExecute.Enabled = False
    '//////////////
    ExecuteEOD  '/
    '////////////
    Exit Sub
EH:
    ShowError
End Sub

Private Function CheckMarketStructureUpdates() As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset ', sMsg$
    
    CheckMarketStructureUpdates = True
    
    'Set rs = gDBW.usp_TradeLog_Get
    'If Not rs.EOF Then
    '    sMsg = "You need to update market structure for some symbols. Would you like to run end of day procedure anyway? (Symbol(s): "
    '
    '    While Not rs.EOF
    '        sMsg = sMsg & " " & DBRead(rs!vcSymbol, "") & ","
    '        rs.MoveNext
    '    Wend
    '
    '    sMsg = Left$(sMsg, Len(sMsg) - 1) & ")"
    '
    '    CheckMarketStructureUpdates = True '(gCmn.MyMsgBox(sMsg, vbYesNo Or vbExclamation) <> vbNo)
    'Else
    '    CheckMarketStructureUpdates = True
    'End If
    
    Set rs = Nothing
    Exit Function
EH:
    ShowError
    Set rs = Nothing
End Function

Private Function CheckMarketStructureInCache() As Boolean
    On Error GoTo EH
    Dim rs As ADODB.Recordset, aUnd As clsMsUndAtom
    
    CheckMarketStructureInCache = False
    
    Set rs = gDBW.usp_EodMsUnderlyingCache_Get
    If Not rs.EOF Then
        CheckMarketStructureInCache = True
        Set m_MsUndColl = New clsMsUndColl

'        sMsg = "You need to finish update market structure for some underlyings. Do it after reconcilation position or deleting erasing trades (Symbol(s): "

        While Not rs.EOF
            Set aUnd = m_MsUndColl.Add(Str$(rs!iUnderlyingID))

'            If Len(sMsg) < 1000 Then _
'                sMsg = sMsg & " " & DBRead(rs!vcUndSymbol, "") & ","
            aUnd.ID = gCmn.ReadLng(rs!iUnderlyingID)
            aUnd.Symbol = gCmn.ReadStr(rs!vcUndSymbol)
            aUnd.UpdateDate = gCmn.ReadDate(rs!dtUpdateDate)
            rs.MoveNext
        Wend

'        If Len(sMsg) < 1000 Then
'            sMsg = Left$(sMsg, Len(sMsg) - 1) & ")"
'        Else
'            sMsg = Left$(sMsg, Len(sMsg) - 1) & "...)"
'        End If
'        gCmn.MyMsgBox sMsg
    End If
    
    Set rs = Nothing
    Exit Function
EH:
    ShowError
    Set rs = Nothing
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteEOD() As Boolean
    On Error GoTo EH
    DoEvents
    SetAlarm 0
    Busy = True
    EnableControls = False
    fCancel = False
    LastErrorNumber = 0
    LastErrorDescription = ""
    LastErrorSource = ""
          
    SetAlarm 0
    SetStatus st02_ReconcileTrades
    Idle
    If chkReconcileTrades.Value <> 0 Then
        ExecuteReconcileTrades
    End If
    
    SetAlarm 0
    SetStatus st01_ReconcilePositions
    Idle
    If chkReconcilePositions.Value <> 0 Then
        ExecuteReconcilePositions
    End If
    
    SetAlarm 0
    SetStatus st03_LoadOptExhanges
    Idle
    If chkLoadOptExchanges.Value <> 0 Then
        ExecuteLoadOptExchanges
    End If
    
    SetAlarm 0
    SetStatus st04_EraseDeletedTrades
    Idle
    If chkEraseDeletedTrades.Value <> 0 Then
        ExecuteEraseDeletedTrades
    End If

    SetAlarm 0
    SetStatus st05_ImportMarketStructure
    Idle
    If chkImportMktStr.Value <> 0 Then
        ExecuteFinishImportMarketStructure
    End If
    
    SetAlarm 0
    SetStatus st06_ExportTodayTrades
    Idle
    If chkExportTrades.Value <> 0 Then
        ExecuteExportTodayTrades
    End If
    
    SetAlarm 0
    SetStatus st07_ExportNewSecurities
    Idle
    If chkExportSecurities.Value <> 0 Then
        ExecuteExportNewSecurities
    End If
    
    SetAlarm 0
    SetStatus st08_LoadBasketIndex
    Idle
    If chkLoadBasketWeights.Value <> 0 Then
        ExecuteLoadBasketWeights
    End If
    
    SetAlarm 0
    SetStatus st09_TradesImport
    Idle
    If chkTradesImport.Value <> 0 Then
        ExecuteTradesImport
    End If
    
    If chkDivsImport.Value <> 0 Then
        ExecuteDividentImport
    End If
    
    SetAlarm 0
    SetStatus st10_VolatilityImport
    Idle
    If chkVolatilityImport.Value <> 0 Then
        ExecuteVolatilityImport
    End If
    
    SetAlarm 0
    SetStatus st11_PositionNetting
    Idle
    If chkPositionNetting.Value <> 0 Then
        ExecutePositionNetting
    End If
    
    SetAlarm 0
    SetStatus st13_ImportCurve
    Idle
    If chkImportCurve.Value <> 0 Then
        ExecuteImportCurve
    End If
    
    SetAlarm 0
    SetStatus st14_ImportGroups
    Idle
    If chkImportGroups.Value <> 0 Then
        ExecuteGroupSectorImport
    End If
    
    SetAlarm 0
    SetStatus st15_LoadStockClosePrice
    Idle
    If chkLoadClosePrice.Value <> 0 Then
        Dim frmSCP As New frmStockClosePrice
        frmSCP.Execute
    End If
    
    SetAlarm 0
    SetStatus st16_ExportAllTrades
    Idle
    If chkExportAllTrades.Value <> 0 Then
        ExportAllTrades
    End If
    
    SetAlarm 0
    SetStatus st12_Completed
    Idle
    ExecuteEOD = True
Done:
    On Error Resume Next
    fCancel = True
    Busy = False
    ExecuteCompleted
    Screen.MousePointer = vbDefault
    Exit Function
EH:
    If fCancel Then
        ShowMessage "Task was canceled by user"
    Else
        ShowError
    End If
    On Error Resume Next
    SetStatus st00_Initial
    Busy = False
    CancelExecute
    Screen.MousePointer = vbDefault
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Idle()
    DoEvents
    If LastErrorNumber <> 0 Then
        Err.Raise LastErrorNumber, LastErrorSource, LastErrorDescription
    End If
    If fCancel Then
        Err.Raise vbObjectError, "Task manager", "Task was canceled by user"
    End If
    If tAlarm > 0 Then
        If Now > tAlarm Then
            tAlarm = 0
            Err.Raise vbObjectError, "Task manager", "Timeout expired"
        End If
    End If
    Sleep RT_DELAY
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetAlarm(ByVal AlarmInterval As Long)
    On Error Resume Next
    If AlarmInterval = 0 Then
        tAlarm = 0
    Else
        tAlarm = Now + TimeSerial(0, 0, AlarmInterval)
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartExecute()
    On Error Resume Next
    btnClose.Enabled = False
    btnExecute.Caption = STOPIT
    EnableControls = False
    Me.MousePointer = 13
    Me.Refresh
    
    LastErrorNumber = 0
    LastErrorDescription = ""
    LastErrorSource = ""
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CancelExecute()
    On Error Resume Next
    pbProgress.Value = pbProgress.Min
    btnExecute.Enabled = True
    
    EnableControls = True
    btnClose.Enabled = True
    Me.MousePointer = vbDefault
    Me.Refresh
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ExecuteCompleted()
    On Error Resume Next
    pbProgress.Value = pbProgress.Max
    btnExecute.Enabled = True
    
    EnableControls = True
    btnClose.Enabled = True
    Me.MousePointer = vbDefault
    Me.Refresh
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UpdateCheck(ByRef ctlCheck As MSForms.CheckBox, enmState As enmStatus)
    If ctlCheck.Value = True Then
        lngCounter = lngCounter + 1
        lngCurrentTicker = lngCurrentTicker + STEP_VALUE
        ReDim Preserve udtDemoProcess(lngCounter)
        With udtDemoProcess(lngCounter)
            .StatusEnm = enmState
            .TickValue = lngCurrentTicker
        End With
    End If
End Sub


Private Function ExecuteEraseDeletedTrades() As Boolean
Dim rsSymbol As ADODB.Recordset
Dim Symbol As String
Dim Count As Long
    On Error GoTo EH
    m_Parent = LogMsg("Process 'Erase Deleted Trades'")
    LogTaskStarted
    
    gDBW.usp_Trade_ClearDeleted
             
    ExecuteEraseDeletedTrades = True
Done:
    On Error Resume Next
    LogTaskFinished , , ""
    m_Parent = ""
    Set rsSymbol = Nothing
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    ExecuteEraseDeletedTrades = False
    GoTo Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteFinishImportMarketStructure() As Boolean
    Dim s_Parent As String, aUnd As clsMsUndAtom, aOpt As clsMsOptAtom
    Dim rs As ADODB.Recordset, nUndCount&
    
    On Error GoTo EH
    
    m_Parent = LogMsg("Process 'Finish Import Market Structure'")
    LogTaskStarted
                   
    If Not m_MsUndColl Is Nothing Then
        
        pbProgress.Min = 0
        pbProgress.Max = IIf(m_MsUndColl.Count, m_MsUndColl.Count, 100)
        pbProgress.Value = 0
        
        For Each aUnd In m_MsUndColl
            
            Set rs = gDBW.usp_EodMsOptionCache_Get(aUnd.ID)
            
            If Not rs.EOF Then
                gDBW.usp_EodMsUnderlyingCache_Save aUnd.ID, aUnd.UpdateDate
    
                While Not rs.EOF
                    Set aOpt = aUnd.Opt.Add(Str$(rs!iOptionID))
                    
                    aOpt.ID = gCmn.ReadLng(rs!iOptionID)
                    aOpt.ExpiryDate = gCmn.ReadDate(rs!dtExpiryDate)
                    aOpt.ExpiryOV = gCmn.ReadDate(rs!dtExpiryOV)
                    aOpt.TradingClose = gCmn.ReadDate(rs!dtTradingClose)
                    aOpt.IsCall = (gCmn.ReadByte(rs!tiIsCall) <> 0)
                    aOpt.Strike = gCmn.ReadDbl(rs!fStrike)
                    aOpt.Symbol = gCmn.ReadStr(rs!vcSymbol)
                    aOpt.LotSize = gCmn.ReadLng(rs!iLotSize)
                    aOpt.IsUpload = False
                    rs.MoveNext
                    
                    Idle
                Wend
            End If
            
            For Each aOpt In aUnd.Opt
                ImportOption aUnd.ID, aOpt
                Idle
            Next
            
            gDBW.usp_EodMsUnderlyingCache_Check aUnd.ID
            
            pbProgress.Value = pbProgress.Value + 1
            
            Idle

        Next
    End If
    
    nUndCount = CountUnUploadedUnd
    
    If nUndCount Then
    
        Dim bEraseCache As Boolean, sMsg$
        
        LogMsg nUndCount & " underlying was not completely uploaded"
        
        sMsg = "Not all underlyings options were uploaded. " & vbCrLf
        sMsg = sMsg & "Your can cancel further processing and lose this options or try to upload it later after reconcilation, etc." & vbCrLf
        sMsg = sMsg & "Cancel further processing?"
        
        bEraseCache = (gCmn.MyMsgBox(sMsg, vbYesNo Or vbExclamation) <> vbNo)
        
        If bEraseCache Then
            gDBW.usp_EodMsLoad_Init
            chkImportMktStr.Visible = False
        End If
    Else
        chkImportMktStr.Visible = False
    End If
    
    Set rs = Nothing
    On Error Resume Next
    LogTaskFinished , m_MsUndColl.Count
    m_Parent = ""
    Exit Function
EH:
    LogMsg Err.Description
    m_Parent = s_Parent
    Set rs = Nothing
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteGroupSectorImport() As Boolean
    On Error GoTo EH
    m_Parent = LogMsg("Process Group Sector Import")
    LogTaskStarted
    ExecuteGroupSectorImport = True
    Dim FileName As String
    FileName = ExecuteOpenFileDialog("Open Group Sector Import File", False, "Comma separated values files (*.csv)|*.csv|All files (*.*)|*.*")
    If Len(FileName) = 0 Then
        Exit Function
    End If
    
    Dim lCount As Long
    lCount = LoadSGroupSectorImport(FileName)
Done:
    On Error Resume Next
    LogTaskFinished , lCount, "Group Sector Import"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadSGroupSectorImport(FileName As String) As Long
    Dim Items As Variant
    lblStatus.Caption = "Load Group/Sector Import From file"
    Items = Split(LoadFirstStringFromFile(FileName), ",")
    If UBound(Items) <> 1 Then
        Err.Raise vbObjectError, "Load Group/Sector", "Wrong format of input file in line " & 1
    End If
    Dim strBuff As String
    strBuff = LoadStringFromFile(FileName)
    Dim strXML As String
    Dim lCount As Long
    strXML = LoadGroupSectorImportFromText(strBuff, lCount)
    If strXML <> "" Then
        lblStatus.Caption = "Saving To Data Base"
        Dim rsNotExist As ADODB.Recordset
        Set rsNotExist = gDBW.usp_ContractInGroup_Import(strXML)
        Dim iNotExCount As Integer
        iNotExCount = 0
        If Not rsNotExist Is Nothing Then
            With rsNotExist
                If .RecordCount > 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        Dim strStock As String
                        strStock = !SymbolsNotExist
                        LogMsg ("New stock " + strStock + " was added, please update market structure")
                        Dim lID As Integer
                        On Error Resume Next
                        lID = gDBW.usp_Stock_Save(, , strStock, , , , , , , , , 0, 1, , , 1)
                        
                     iNotExCount = iNotExCount + 1
                     .MoveNext
                    Loop
                LogMsg (CStr(iNotExCount) + " Unknown stocks")
                End If
            End With
            gDBW.usp_ContractInGroup_Import (strXML)
        End If
    
    End If
    LoadSGroupSectorImport = lCount
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadGroupSectorImportFromText(ByVal Buffer As String, ByRef vCount As Long) As String
    Dim Lines As Variant
    Dim Items As Variant
    Dim l As Variant
    Dim Col As String
    Dim Cell As String
    Dim s_Parent As String
    Dim Count As Long, bUpdate As Boolean
    
    Dim strXML As String
    
    LoadGroupSectorImportFromText = ""
    strXML = "<Groups>"

    s_Parent = m_Parent
    lblStatus.Caption = "Reading Group/Sector Import..."
    m_Parent = LogMsg("Read Group/Sector Import")
    pbProgress.Min = 0
    pbProgress.Max = 1000
    pbProgress.Value = 0
    Count = 0
    On Error GoTo EH
    
    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) < 0 Then
            GoTo Done
        End If
    End If
    Count = 0
    pbProgress.Max = UBound(Lines) + 1

    For Each l In Lines
        On Error GoTo EH
        Count = Count + 1
        If Len(l) = 0 Then GoTo Skip
        Items = MySplit(l)
        If UBound(Items) <> 1 Then
            Err.Raise vbObjectError, "Load Stock Positions", "Wrong format of input file in line " & Count
            GoTo Skip
        End If
        
        strXML = strXML + "<Group GroupId =""" + GetValidXMLText(Trim(Items(0))) + """ " + "Symbol=""" + GetValidXMLText(Trim(Items(1))) + """></Group>"
        vCount = vCount + 1
        On Error GoTo ParseError
        pbProgress.Value = pbProgress.Value + 1
Skip:
        On Error Resume Next
        pbProgress.Value = Count
        Idle
    Next
Done:
    On Error GoTo 0
    m_Parent = s_Parent
    If strXML = "<Groups>" Then 'Empty
        Err.Raise vbObjectError, , "No exersised Stock Position found."
        Exit Function
    End If
    strXML = strXML + "</Groups>"
    LoadGroupSectorImportFromText = strXML
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
FailedRead:
    Dim Msg As String
    Msg = Err.Description
    Resume CancelUpdate
    Exit Function
CancelUpdate:
    On Error Resume Next
    LoadGroupSectorImportFromText = ""
ParseError:
    LogMsg "Trade skipped. Error parsing trades data in line " & Count & " '" & Col & "'='" & Cell & "'. " & Msg
    Col = ""
    Cell = ""
    GoTo Skip
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ImportOption(ByVal UndId As Long, aOpt As clsMsOptAtom)
    On Error GoTo EH

    gDBW.usp_Option_Import UndId, aOpt.Symbol, aOpt.IsCall, aOpt.Strike, aOpt.ExpiryDate, aOpt.ExpiryOV, aOpt.TradingClose, aOpt.LotSize, aOpt.ID
    aOpt.IsUpload = True
    Exit Sub
EH:
    LogMsg Err.Description
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function CountUnUploadedUnd() As Long
    Dim aUnd As clsMsUndAtom, aOpt As clsMsOptAtom, nCount&
    On Error GoTo EH
    CountUnUploadedUnd = 0
    nCount = 0
    
    For Each aUnd In m_MsUndColl
        For Each aOpt In aUnd.Opt
            If Not aOpt.IsUpload Then
                nCount = nCount + 1
                Exit For
            End If
            Idle
        Next
    Next
    CountUnUploadedUnd = nCount
    Exit Function
EH:
    LogMsg Err.Description
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogMsg(ByVal Msg As String) As String
Static ID As Long
Dim Key As String
    On Error Resume Next
    With TreeView1
        ID = ID + 1
        Key = "s" & Format$(ID)
        LogMsg = Key
        If Len(m_Parent) = 0 Then
            With .Nodes.Add("r0", tvwChild, Key, Msg)
                .EnsureVisible
            End With
        Else
            With .Nodes.Add(m_Parent, tvwChild, Key, Msg)
                .EnsureVisible
            End With
        End If
        DoEvents
    End With
    
    Print #m_hLog, Now & " " & Msg
    
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogTaskStarted(Optional ByVal TaskCaption As String)
Dim Msg As String
    On Error Resume Next
    tStart = Now
    If Len(TaskCaption) > 0 Then
        Msg = "Task '" & TaskCaption & "' started "
    Else
        Msg = "Task started "
    End If
    LogMsg Msg & tStart
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LogTaskFinished(Optional ByVal TaskCaption As String = "", Optional ByVal ItemsProcessed As Double = 0, Optional ItemName As String = "Stocks")
Dim Msg As String
    On Error Resume Next
    tStop = Now
    Duration = DateDiff("s", tStart, tStop)
    If Duration > 0 And ItemsProcessed > 0 Then
        Performance = ItemsProcessed / Duration
    Else
        Performance = 0
    End If
    If Len(TaskCaption) > 0 Then
        LogSave TaskCaption
    End If
    If Len(TaskCaption) > 0 Then
        Msg = "Task '" & TaskCaption & "' terminated "
    Else
        Msg = "Task terminated "
    End If
    LogMsg Msg & tStop
    LogMsg "Task Duration: " & FormatDuration(Duration)
    If ItemsProcessed > 0 Then
        LogMsg ItemName & " processed: " & Format$(ItemsProcessed)
    End If
    If Performance > 0 Then
        LogMsg "Performance, " & ItemName & "/sec " & Format$(Performance, "0.0")
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load stock symbols list
'
'   iContractTypeID'
'   iContractID
'   vcSymbol
'   vcExchangeCode
'
'    @iFormat int = 1    -- 1 - list only
'                        -- 2 - list with prices by exchange
'
'
Private Function GetAllStocks(Optional ByVal iFormat As Long = 1) As ADODB.Recordset
'    Set GetAllStocks = gDBW.usp_StockWithPrice_Get(Null, Null, 1)
    Set GetAllStocks = gDBW.Execute(True, "usp_allStockEOD_Get", iFormat)
    GetAllStocks.Sort = "vcSymbol"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'  1. Run usp_ContractInExchangeEOD_Init_Save
'  2. send/receive messages. call usp_ContractInExchangeEOD_Save as usual
'  3. End/Cancel run usp_ContractInExchangeEOD_Complete_Save
'
Private Function SaveOptionExchInit()
    SaveOptionExchInit = 0 = gDBW.Execute(False, "usp_ContractInExchangeEOD_Init_Save")
    SaveOptionExchInit = 0
    SaveOptionExchBuffer = ""
    fCanSave = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveOptionExchDone()
    If fCanSave Then
        If SaveOptionExchCount > 0 Then
            If Len(SaveOptionExchBuffer) > 0 Then
                gDBW.ExecuteSQL False, SaveOptionExchBuffer
            End If
        End If
    End If
    SaveOptionExchBuffer = ""
    SaveOptionExchCount = 0
    fCanSave = False
    SaveOptionExchDone = 0 = gDBW.Execute(False, "usp_ContractInExchangeEOD_Complete_Save")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveOptionExch(ByVal iExchangeId As Variant, _
                                ByVal iContractID As Long)
            
    Dim strExchID As String
    
    If fCanSave Then
        SaveOptionExchCount = SaveOptionExchCount + 1
        
        Select Case VarType(iExchangeId)
            Case vbNull, vbEmpty
                strExchID = "Null"
            Case Else
                strExchID = Trim$(Str$(iExchangeId))
        End Select
        
        SaveOptionExchBuffer = SaveOptionExchBuffer & "exec usp_ContractInExchangeEOD_Save " & strExchID & "," _
            & Trim$(Str$(iContractID)) & ",Null,1;"
        
        If SaveOptionExchCount > 200 Then
            If Len(SaveOptionExchBuffer) > 0 Then
                Debug.Print Len(SaveOptionExchBuffer)
                SaveOptionExch = 0 = gDBW.ExecuteSQL(False, SaveOptionExchBuffer)
                SaveOptionExchBuffer = ""
                SaveOptionExchCount = 0
            End If
        End If
    End If
    
End Function


Public Function MySplit(ByVal InputText As String) As Variant
    Dim intIndex&
    Dim Items As Variant, strReplacedText$
    
    On Error Resume Next
    
    Items = Split(InputText, """")
    
    For intIndex = LBound(Items) To UBound(Items)
        If intIndex Mod 2 = 1 Then
            Items(intIndex) = Trim(Replace(Items(intIndex), ",", " "))
        End If
    Next
    
    strReplacedText = Join(Items)
    
    MySplit = Split(strReplacedText, ",")
    
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadTradesFromText(ByVal Buffer As String, rsTradesData As ADODB.Recordset) As Boolean
    Dim Lines As Variant
    Dim Items As Variant
    Dim l As Variant
    Dim Col As String
    Dim Cell As String
    Dim s_Parent As String
    Dim Count As Long, bUpdate As Boolean

    s_Parent = m_Parent
    lblStatus.Caption = "Reading Trades..."
    m_Parent = LogMsg("Read Trades")
    pbProgress.Min = 0
    pbProgress.Max = 1000
    pbProgress.Value = 0
    Count = 0
    On Error GoTo EH
    
    Set rsTradesData = New ADODB.Recordset
    With rsTradesData
        .Fields.Append "ExecID", adVarChar, 20
        .Fields.Append "TradeDate", adDate
        .Fields.Append "SettlementDate", adDate
        .Fields.Append "AccountAcronym", adVarChar, 20
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "OptionSymbol", adVarChar, 20
        .Fields.Append "OptionRoot", adVarChar, 20
        .Fields.Append "ExpirationMonth", adVarChar, 20
        .Fields.Append "Strike", adDouble
        .Fields.Append "OptionType", adVarChar, 4
        .Fields.Append "NetTrade", adInteger
        .Fields.Append "TradePrice", adDouble
        .Fields.Append "Multiplier", adInteger
        .Fields.Append "TradeCode", adVarChar, 20
        .Fields.Append "ExchangeCode", adVarChar, 20
        .Fields.Append "GrossProceeds", adDouble
        
        .Fields.Append "Commission", adDouble
        .Fields.Append "ExchangeFee", adDouble
        .Fields.Append "SECFees", adDouble
        .Fields.Append "OCCFees", adDouble
        .Fields.Append "NetProceeds", adDouble
        
        .Fields.Append "ExecutionTime", adDate
        .Fields.Append "ExpirationDate", adDate
        .Fields.Append "SecurityName", adVarChar, 255
        .Open
    End With
    If Buffer = "" Then
        m_aTradeLog.Add.Init 0, errEmptyFile
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) <= 0 Then
            GoTo Done
        End If
    End If
    Count = 0
    pbProgress.Max = UBound(Lines) + 1
    For Each l In Lines
        On Error GoTo EH
        If Len(l) = 0 Then GoTo Skip
        Count = Count + 1
        Items = MySplit(l)
        If UBound(Items) < 23 Then
            m_aTradeLog.Add.Init 0, errInvalidFileFormat
            Err.Raise vbObjectError, "Load Trades", "Wrong format of input file in line " & Count
            GoTo Skip
        End If
        
        On Error GoTo ParseError
        With rsTradesData
            .AddNew
            
            Col = "TradeID":              Cell = Items(0):  If Len(Cell) > 0 Then !ExecID = Cell
            Col = "Trade Date":           Cell = Items(1):  If Len(Cell) > 4 Then !TradeDate = gCmn.ReadDate(Cell)
            Col = "Settlement Date":      Cell = Items(2):  If Len(Cell) > 0 Then !SettlementDate = gCmn.ReadDate(Cell)
            Col = "Account Acronym":      Cell = Items(3):  If Len(Cell) > 0 Then !AccountAcronym = Trim$(Cell)
            Col = "Base Symbol":          Cell = Items(4):  If Len(Cell) > 0 Then !StockSymbol = Trim$(Cell)
            Col = "Option Symbol":        Cell = Items(5):  If Len(Cell) > 0 Then !OptionSymbol = Trim$(Cell)
            Col = "Option Root":          Cell = Items(6):  If Len(Cell) > 0 Then !OptionRoot = Trim$(Cell)
            Col = "Expiration Month":     Cell = Items(7):  If Len(Cell) > 0 Then !ExpirationMonth = Trim$(Cell)
            Col = "Strike":               Cell = Items(8):  If Len(Cell) > 0 Then !Strike = gCmn.ReadDbl(Cell)
            Col = "Option Type":          Cell = Items(9):  If Len(Cell) > 0 Then !OptionType = Trim$(Cell)
            Col = "Net Trade":            Cell = Items(10):  If Len(Cell) > 0 Then !NetTrade = gCmn.ReadLng(Cell)
            Col = "Trade Price":          Cell = Items(11):  If Len(Cell) > 0 Then !TradePrice = gCmn.ReadDbl(Cell)
            Col = "Multiplier":           Cell = Items(12):  If Len(Cell) > 0 Then !Multiplier = gCmn.ReadLng(Cell)
            Col = "Trade Code":           Cell = Items(13):  If Len(Cell) > 0 Then !TradeCode = Trim$(Cell)
            Col = "Exchange Code":        Cell = Items(14):  If Len(Cell) > 0 Then !ExchangeCode = Trim$(Cell)
            Col = "Gross Proceeds":       Cell = Items(15):  If Len(Cell) > 0 Then !GrossProceeds = gCmn.ReadDbl(Cell)
            Col = "Commission":           Cell = Items(16):  If Len(Cell) > 0 Then !Commission = gCmn.ReadDbl(Cell)
            Col = "Exchange Fee":         Cell = Items(17):  If Len(Cell) > 0 Then !ExchangeFee = gCmn.ReadDbl(Cell)
            Col = "SEC Fees":             Cell = Items(18):  If Len(Cell) > 0 Then !SECFees = gCmn.ReadDbl(Cell)
            Col = "OCC Fees":             Cell = Items(19):  If Len(Cell) > 0 Then !OCCFees = gCmn.ReadDbl(Cell)
            Col = "Net Proceeds":         Cell = Items(20):  If Len(Cell) > 0 Then !NetProceeds = gCmn.ReadDbl(Cell)
            Col = "Execution Time":       Cell = Items(21):  If Len(Cell) > 4 Then !ExecutionTime = gCmn.ReadTime(Cell)
            Col = "Expiration Date":      Cell = Items(22):  If Len(Cell) > 4 Then !ExpirationDate = gCmn.ReadDate(Cell)
            Col = "Security Name":        Cell = Items(23):  If Len(Cell) > 0 Then !SecurityName = Trim$(Cell)
                      
            Col = "":                     Cell = ""
            
            If DBRead(!TradeCode, "") = "ZEA" Then
                bUpdate = True
                
                If DBRead(!AccountAcronym, "") = "" Then
                    LogMsg "Trade skipped. Invalid Account Acronym in line " & Count
                    m_aTradeLog.Add.Init !ExecID, errInvalidAccountAcronym
                    bUpdate = False
                ElseIf DBRead(!StockSymbol, "") = "" Then
                    LogMsg "Trade skipped. Invalid Stock Symbol in line " & Count
                    m_aTradeLog.Add.Init !ExecID, errInvalidStockSymbol
                    bUpdate = False
                ElseIf DBRead(!TradeDate, 0) = 0 Then
                    LogMsg "Trade skipped. Invalid Trade Date in line " & Count
                    m_aTradeLog.Add.Init !ExecID, errInvalidTradeDate
                    bUpdate = False
                ElseIf DBRead(!OptionType, "") = "C" Or DBRead(!OptionType, "") = "P" Then
                    If !OptionRoot = "" Then
                        LogMsg "Trade skipped. Invalid Option Root in line " & Count
                        m_aTradeLog.Add.Init !ExecID, errInvalidOption
                        bUpdate = False
                    ElseIf DBRead(!ExpirationDate, 0) = 0 Then
                        LogMsg "Trade skipped. Invalid Expiration Date in line " & Count
                        m_aTradeLog.Add.Init !ExecID, errInvalidOption
                        bUpdate = False
                    ElseIf DBRead(!Strike, 0) = 0 Then
                        LogMsg "Trade skipped. Invalid Strike in line " & Count
                        m_aTradeLog.Add.Init !ExecID, errInvalidOption
                        bUpdate = False
                    End If
                ElseIf DBRead(!OptionType, "") = "X" Then
                    LogMsg "Trade skipped. Invalid Option Type in line " & Count
                    m_aTradeLog.Add.Init !ExecID, errInvalidOptionType
                    bUpdate = False
                End If
            
                If bUpdate Then
                    .Update
                Else
                    .Delete
                End If
                
            End If
        End With
Skip:
        On Error Resume Next
        pbProgress.Value = Count
        Idle
    Next
    m_aTradeLog.m_nTradesCount = Count
    LoadTradesFromText = True
Done:
    On Error GoTo 0
    m_Parent = s_Parent
    If rsTradesData.RecordCount = 0 Then
        Err.Raise vbObjectError, , "No exersised trades found."
        Exit Function
    End If
    m_aTradeLog.m_nTradesCount = Count
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    m_aTradeLog.m_nTradesCount = Count
    Exit Function
FailedRead:
    Dim Msg As String
    Msg = Err.Description
    Resume CancelUpdate
    m_aTradeLog.m_nTradesCount = Count
    Exit Function
CancelUpdate:
    On Error Resume Next
    rsTradesData.CancelUpdate
ParseError:
    LogMsg "Trade skipped. Error parsing trades data in line " & Count & " '" & Col & "'='" & Cell & "'. " & Msg
    Col = ""
    Cell = ""
    m_aTradeLog.Add.Init 0, errParserError
    GoTo Skip
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Insert Positions ad Trades
'
Private Function SaveTradesToDatabase(rsInTrades As ADODB.Recordset, dtUTCTradeDate As Date) As Long
    Dim Count&
    Dim vOptType As Variant, rs As ADODB.Recordset, nResult&, btContractTypeID As Byte
    Dim s_Parent$, sContractType$, nContractTypeID&
    
    s_Parent = m_Parent
    lblStatus.Caption = "Saving Trades..."
    m_Parent = LogMsg("Save trades")
    
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100
    
    On Error GoTo Failed
    LogMsg ("Close expired options' trades")
    gDBW.usp_TradeByExpiredOptions_Del dtUTCTradeDate
    
    Count = rsInTrades.RecordCount
    
    If Count > 0 Then
        pbProgress.Max = Count
        Count = 0
        
        LogMsg "Save trades"
        
        With rsInTrades
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    Count = Count + 1
                    On Error GoTo Failed
                    If DBRead(!TradeCode, "") = "ZEA" Then
                        
                        sContractType = DBRead(!OptionType, "")
                        If sContractType <> "" Then
                            btContractTypeID = IIf(sContractType = "S", TYPE_STOCK, TYPE_OPTION)
                            vOptType = IIf(sContractType = "C", 1, IIf(sContractType = "P", 0, Null))
                            
                            
                            If DBRead(!StockSymbol, "") <> "" Then
                                If btContractTypeID = TYPE_OPTION Then
                                    If DBRead(!OptionSymbol, "") <> "" And Not IsNull(vOptType) Then
                                        If DateDiff("d", dtUTCTradeDate, !ExpirationDate) > 0 Then

                                            Set rs = gDBW.usp_TradeFeed_1_New( _
                                                !AccountAcronym, _
                                                !ExecID, _
                                                CByte(IIf(DBRead(!NetTrade, 0) < 0, 0, 1)), _
                                                Abs(DBRead(!NetTrade, 0)), _
                                                !TradePrice, _
                                                CDate(DBRead(!TradeDate, 0) + DBRead(!ExecutionTime, 0)), _
                                                !OptionSymbol, _
                                                TYPE_OPTION, _
                                                !StockSymbol, _
                                                vOptType, _
                                                !Strike, _
                                                !ExpirationDate, _
                                                !Multiplier, _
                                                !OptionRoot, _
                                                Null _
                                                )
                                        Else
                                            LogMsg "Trade skipped. Option contract already expired: Option symbol: " & !OptionSymbol & ", Expiration Date: " & !ExpirationDate
                                            m_aTradeLog.Add.Init !ExecID, errInvalidOption
                                        End If
                                    Else
                                        LogMsg "Trade skipped. Invalid option for symbol: " & !StockSymbol
                                        m_aTradeLog.Add.Init !ExecID, errInvalidOption
                                    End If
                                Else
                                    Set rs = gDBW.usp_TradeFeed_1_New( _
                                            !AccountAcronym, _
                                            !ExecID, _
                                            CByte(IIf(DBRead(!NetTrade, 0) < 0, 0, 1)), _
                                            Abs(DBRead(!NetTrade, 0)), _
                                            !TradePrice, _
                                            CDate(DBRead(!TradeDate, 0) + DBRead(!ExecutionTime, 0)), _
                                            !StockSymbol, _
                                            btContractTypeID, _
                                            !StockSymbol, _
                                            Null, _
                                            Null, _
                                            Null, _
                                            !Multiplier, _
                                            Null, _
                                            Null _
                                            )
        
                                    If DBRead(rs!iMaxSeqNum) <> 0 Then
                                        On Error Resume Next
                                        nResult = -1
                                        nResult = DBRead(rs!iStructureMissed)
                                        On Error GoTo Failed
                                        
                                        If nResult <> 0 Then
                                            If nResult = 1 Then
                                                LogMsg "New stock was added to database: " & !StockSymbol & ". Please update market structure."
                                            End If
                                        End If
                                    End If
                                    Set rs = Nothing
                                End If
                            Else
                                LogMsg "Trade skipped. Invalid stock symbol"
                                m_aTradeLog.Add.Init !ExecID, errInvalidStockSymbol
                            End If
                            
                            
                        End If
                    End If
Skip:
                    On Error GoTo EH
                    Idle
                    pbProgress.Value = pbProgress.Value + 1
                    .MoveNext
                Loop
            End If
        End With
    End If
EH:
    Set rs = Nothing
    SaveTradesToDatabase = Count
    m_Parent = s_Parent
    Exit Function
Failed:
    Set rs = Nothing
    LogMsg "Error saving trade: " & Err.Description
    m_aTradeLog.Add.Init 0, errDBError
    Resume Skip
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadTradesFromFile(ByVal f As String) As Long
    Dim rsInTrades As ADODB.Recordset
    Dim s As String
    Dim Msg As String
    Dim Items As Variant, dtUTCTradeDate As Date
    
    On Error GoTo EH
        
    Items = Split(LoadFirstStringFromFile(f), ",")
    If UBound(Items) < 22 Then
        m_aTradeLog.Add.Init 0, errInvalidFileFormat
        Err.Raise vbObjectError, "Load Trades", "Wrong format of input file in line " & 1
    End If
    dtUTCTradeDate = CDate(Items(1))
    
    If Not frmReconcileParams.Execute("Confirm trades reconciliation date", "Reconcilation date :", dtUTCTradeDate) Then
        LogMsg "Trades Reconciliation canceled."
    Else
        Msg = "Reading from file failed."
        s = LoadStringFromFile(f)
        If LoadTradesFromText(s, rsInTrades) Then
            Msg = "Saving trades failed."
            LoadTradesFromFile = SaveTradesToDatabase(rsInTrades, dtUTCTradeDate)
        End If
    End If
    
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteReconcileTrades() As Boolean
Dim Count As Long
    On Error GoTo EH
    
    m_aTradeLog.Clear
    
    Dim f As String
    f = ExecuteOpenFileDialog("Open Trades Report File", False, "Comma separated values files (*.csv)|*.csv|All files (*.*)|*.*")
    If Len(f) = 0 Then
        Exit Function
    End If
    
    m_Parent = LogMsg("Process 'Reconcile Trades'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    LogMsg ("Trades File: " & f)
    
    Count = LoadTradesFromFile(f)
    
    m_aTradeLog.CreateTradeLog Count
    
    ExecuteReconcileTrades = True
Done:
    On Error Resume Next
    LogTaskFinished , Count, "Trades"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteReconcilePositions() As Boolean
Dim Count As Long
    On Error GoTo EH
    Dim f As String
    'Dim f1 As String
    
    Dim sFilePath As String
    Dim sFileMask As String
    
    Dim bIsOpenFileDialog As Boolean
    
    sFilePath = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PositionReconciliationFilePath", "")
    If Len(sFilePath) < 3 Then
        bIsOpenFileDialog = True
    Else
        If Mid(sFilePath, Len(sFilePath), 1) <> "\" Then sFilePath = sFilePath & "\"
        
        sFileMask = g_aUserXMLParams.GetStringValue(APP_XML_KEY & "Settings", "PositionReconciliationFileMask", "")
        If Len(sFileMask) < (Len(YEAR_FORMAT) + Len(MONTH_FORMAT) + Len(DAY_FORMAT)) Then
            bIsOpenFileDialog = True
        End If
    End If
    
    If bIsOpenFileDialog = False Then
        Dim sFileName As String
        Dim dtFileDate As Date
    
        Dim sLastFileName As String
        Dim dtLastFileDate As Date
    
        Dim sFileMaskForStorage As String
        sFileMaskForStorage = GetStorageMaskFromFileMask(sFileMask)
    
        sFileName = Dir(sFilePath & sFileMaskForStorage)
        If Len(sFileName) = 0 Then
            bIsOpenFileDialog = True
        Else
            While Len(sFileName) > 0
                dtFileDate = GetDateFromString(sFileName, sFileMask)
                
                If dtFileDate > dtLastFileDate Then
                    sLastFileName = sFileName
                    dtLastFileDate = dtFileDate
                End If
                
                sFileName = Dir()
            Wend
            
            f = sFilePath & sLastFileName
        End If
    End If
    
    If bIsOpenFileDialog = True Then
        f = ExecuteOpenFileDialog("Open Positions Report File", False, "Positions files(*.csv;*.dat;*.txt)|*.csv;*.dat;*.txt|All files (*.*)|*.*")
        If Len(f) = 0 Then
            Exit Function
        End If
    End If
    
    
    m_Parent = LogMsg("Process 'Reconcile Postitions'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
        
    LogMsg ("Position File: " & f)
    
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
    
    Dim sDecryptFileName$
    If Not DecryptFile(f, sDecryptFileName) Then
        Err.Raise vbObjectError, "Load Positions", "Fail to decrypt file"
        Exit Function
    End If
    If sDecryptFileName <> "" Then f = sDecryptFileName
    
    Count = LoadPositionsFromFile(f, dtLastFileDate)
    
    ExecuteReconcilePositions = True
Done:
    On Error Resume Next
    If sDecryptFileName <> "" Then Kill (sDecryptFileName)
    LogTaskFinished , Count, "Positions"
    m_Parent = ""
    fNeedRefreshCurrent = True
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function ExecuteLoadBasketWeights() As Boolean
    On Error GoTo EH
    Dim nCount&, sFileName$
    Dim collIndices As New clsBasketIndexColl
    Dim collNewIndices As New clsBasketIndexColl
    
    If optBasketIndex(0).Value Then
        m_Parent = LogMsg("Process 'Load Index Basket Weights from IVolatility'")
        LogTaskStarted
        pbProgress.Min = 0
        pbProgress.Max = 100
        pbProgress.Value = 0
    
        If LoadBasketIndicesFromDB(collIndices) Then
            nCount = LoadBasketIndicesFromIVolatility(collIndices)
            If nCount > 0 Then _
                nCount = SaveBasketIndicesToDB_EX(collIndices, nCount)
        End If
    Else
    
    sFileName = ExecuteOpenFileDialog("Open Index Basket Weights File", False, "Text files (*.txt;*.csv)|*.txt;*.csv|All files (*.*)|*.*")
    If Len(sFileName) = 0 Then Exit Function
    
    
    m_Parent = LogMsg("Process 'Load Index Basket Weights'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    If LoadBasketIndicesFromDB(collIndices) Then
        nCount = LoadBasketIndicesFromFile(sFileName, collIndices)
        If nCount > 0 Then _
            nCount = SaveBasketIndicesToDB(collIndices, nCount)
    End If
    End If
    
    ExecuteLoadBasketWeights = True
Done:
    On Error Resume Next
    LogTaskFinished , nCount, "Component weights"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function LoadBasketIndicesFromDB(collIndices As clsBasketIndexColl) As Boolean
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom, sKey$
    Dim rs As ADODB.Recordset
    
    LoadBasketIndicesFromDB = False
    
    collIndices.Clear
    
    Set rs = gDBW.usp_BoIndexWithBasketInfo_Get
    While Not rs.EOF
        sKey = Trim$(UCase$(gCmn.ReadStr(rs!vcSymbol)))
        If collIndices(sKey) Is Nothing Then
            Set aIdx = collIndices.Add(sKey)
            aIdx.ID = gCmn.ReadLng(rs!iIndexID)
            aIdx.Symbol = gCmn.ReadStr(rs!vcSymbol)
            aIdx.IsBasket = gCmn.ReadByte(rs!tiIsBasket)
            aIdx.HasComponents = gCmn.ReadByte(rs!tiHasComponents)
            Set aIdx = Nothing
        End If
        
        rs.MoveNext
    Wend
    Set rs = Nothing
    
    LoadBasketIndicesFromDB = True
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to load index basket weights info from database."
End Function

Private Function LoadBasketIndicesFromIVolatility(collIndices As clsBasketIndexColl) As Long
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom
    Dim aIdxComp As clsBasketIndexCompAtom
    Dim sIndex$, sStock$, dWeight#, nCount&, dPriceIndex#, dPriceStock#
    
    LogMsg ("Load Index Basket Weights from IVolatility")
    
    Dim rs As ADODB.Recordset
    Set rs = gDBW.usp_IndexDefinition_LoadFromIV
    While Not rs.EOF
        sIndex = DBRead(rs!index_symbol, "")
        Set aIdx = collIndices(sIndex)

        If Not aIdx Is Nothing Then
            dPriceIndex = DBRead(rs!Index_close_price, 0)
            dPriceStock = DBRead(rs!Component_close_price, 0)
            dWeight = DBRead(rs!Component_weight, 0)
            sStock = DBRead(rs!Component_symbol, "")
            
            Set aIdxComp = aIdx.NewComponents(sStock)
            If aIdxComp Is Nothing Then
                Set aIdxComp = aIdx.NewComponents.Add(sStock)
            End If
            
            aIdx.ClosePrice = dPriceIndex
            aIdxComp.Symbol = sStock
            aIdxComp.Weight = dWeight
            aIdxComp.ClosePrice = dPriceStock
            
            nCount = nCount + 1
            
            Set aIdx = Nothing
            Set aIdxComp = Nothing
        End If
        
        rs.MoveNext
    Wend
    
    For Each aIdx In collIndices
        If aIdx.NewComponents.Count = 0 And aIdx.IsBasket Then LogMsg "No data available for basket index """ & aIdx.Symbol & """."
    Next
    
    LoadBasketIndicesFromIVolatility = nCount
    
    Exit Function
EH:
    LoadBasketIndicesFromIVolatility = nCount
    gCmn.ErrorHandler "Fail to load index basket weights from IVolatility."
End Function

Private Function LoadBasketIndicesFromFile&(ByVal sFileName$, collIndices As clsBasketIndexColl)
    On Error GoTo FailedFile
    Dim aIdx As clsBasketIndexAtom, aIdxComp As clsBasketIndexCompAtom, nCount&, sWeights$
    Dim arrWeight() As String, nLBnd&, nUBnd&
    Dim arrItems() As String, nItLBnd&, nItUBnd&, i&
    Dim sIndex$, sStock$, dWeight#, sParent$, nPos&

    sParent = m_Parent
    lblStatus.Caption = "Reading Index Basket Weights..."
    m_Parent = LogMsg("Read Index Basket Weights")
    pbProgress.Min = 0
    pbProgress.Max = 1000
    pbProgress.Value = 0
    nCount = 0
    
    LogMsg ("File: " & sFileName)
    sWeights = LoadStringFromFile(sFileName)
    On Error Resume Next
    
    On Error Resume Next
    arrWeight = Split(sWeights, vbLf)
    nLBnd = 0
    nUBnd = -1
    nLBnd = LBound(arrWeight)
    nUBnd = UBound(arrWeight)
    
    If nUBnd >= nLBnd Then
        pbProgress.Max = nUBnd - nLBnd + 1
    
        For i = nLBnd To nUBnd
            On Error Resume Next
            If Len(Trim$(arrWeight(i))) > 0 Then
                arrItems = Split(arrWeight(i), ",")
                nItLBnd = 0
                nItUBnd = -1
                nItLBnd = LBound(arrItems)
                nItUBnd = UBound(arrItems)
                
                On Error GoTo FailedFile
                If nItUBnd - nItLBnd >= 2 Then
                    sIndex = Trim$(UCase$(arrItems(nItLBnd)))
                    nPos = InStr(1, sIndex, " INDEX")
                    If nPos > 0 Then sIndex = Trim$(Left$(sIndex, nPos - 1))
                    
                    If IsValidTicker(sIndex) Then
                        sStock = Trim$(UCase$(arrItems(nItLBnd + 1)))
                        
                        If IsValidTicker(sStock) Then
                            If Len(sIndex) > 0 And Len(sStock) > 0 Then
                                dWeight = gCmn.ReadDbl(arrItems(nItLBnd + 2)) / 100#
                                
                                Set aIdx = collIndices(sIndex)
                                If aIdx Is Nothing Then
                                    Set aIdx = collIndices.Add(sIndex)
                                    aIdx.ID = 0
                                    aIdx.Symbol = sIndex
                                End If
                                
                                Set aIdxComp = aIdx.NewComponents(sStock)
                                If aIdxComp Is Nothing Then
                                    Set aIdxComp = aIdx.NewComponents.Add(sStock)
                                    aIdxComp.ID = 0
                                    aIdxComp.Symbol = sStock
                                    nCount = nCount + 1
                                End If
                                aIdxComp.Weight = dWeight
                                
                                Set aIdxComp = Nothing
                                Set aIdx = Nothing
                            Else
                                LogMsg "Line " & i + 1 & " skipped: No index or stock info."
                            End If
                        Else
                            LogMsg "Line " & i + 1 & " skipped: Invalid stock ticker '" & sStock & "'"
                        End If
                    Else
                        LogMsg "Line " & i + 1 & " skipped: Invalid index ticker '" & sIndex & "'"
                    End If
                Else
                    LogMsg "Line " & i + 1 & " skipped: Invalid format."
                End If
                
                Erase arrItems
            End If
            
            On Error GoTo EH
            Idle
            pbProgress.Value = pbProgress.Value + 1
        Next
    End If
    Erase arrWeight
    
    If nCount = 0 Then
        Err.Raise vbObjectError, , "No index basket weights found."
        Exit Function
    End If
    
    LogMsg nCount & " index basket weights loaded."
    
    LoadBasketIndicesFromFile = nCount
    m_Parent = sParent
    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    Erase arrWeight
    Erase arrItems
    LoadBasketIndicesFromFile = nCount
    LogMsg Msg
    m_Parent = sParent
End Function


Private Function SaveBasketIndicesToDB&(collIndices As clsBasketIndexColl, ByVal nWeightCount&)
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom, aIdxComp As clsBasketIndexCompAtom, nCount&
    Dim i&, sParent$, nContractID&, nContractTypeID&
    Dim rs As ADODB.Recordset, sMsg$, nIndexId&, nStockID&, nMsHint&
    
    sParent = m_Parent
    lblStatus.Caption = "Saving Index Basket Weights..."
    m_Parent = LogMsg("Save Index Basket Weights")
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = nWeightCount
    nCount = 0
    
    If collIndices.Count > 0 Then
        ' clear all old weights
        'gDBW.usp_BoIndexWithBasketInfo_ClearWeights 0
        
        For Each aIdx In collIndices
            On Error GoTo FailedSave
            
            If aIdx.ID <> 0 Then
                gDBW.usp_BoIndexWithBasketInfo_ClearWeights aIdx.ID
            End If
            
            If aIdx.NewComponents.Count > 0 Then
                'gDBW.usp_BoIndexWithBasketInfo_ClearWeights aIdx.ID
                
                For Each aIdxComp In aIdx.NewComponents
                    nIndexId = aIdx.ID
                    nStockID = aIdxComp.ID
                    nMsHint = gDBW.usp_BoIndexBasketWeight_Save(nIndexId, aIdx.Symbol, nStockID, aIdxComp.Symbol, aIdxComp.Weight, 1, 1, 0, sMsg)
                    
                    If nMsHint > 0 Then ' new contracts
                        If (nMsHint And 1) = 1 Then _
                            LogMsg "New index was added to database: " & aIdx.Symbol & ". Please update market structure."
                        
                        If (nMsHint And 2) = 2 Then _
                            LogMsg "New stock was added to database: " & aIdxComp.Symbol & ". Please update market structure."
                    
                    ElseIf nMsHint < 0 Then ' error
                        LogMsg "Fail to save basket index weight: " & sMsg
                        If nMsHint >= -4 Then Exit For ' index errors
                    End If
                    
                    nCount = nCount + 1
                    On Error GoTo EH
                    Idle
                    pbProgress.Value = pbProgress.Value + 1
                Next
            End If
        Next
    End If
    
    SaveBasketIndicesToDB = nCount
    'If nWeightCount > 0 Then SaveOptionExchDone
    m_Parent = sParent
    Exit Function
FailedSave:
    Dim Msg As String
    Msg = "Fail to save index basket weights."
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    
    'If nWeightCount > 0 Then SaveOptionExchDone
    
    SaveBasketIndicesToDB = nCount
    LogMsg Msg
    m_Parent = sParent
End Function

Private Function SaveBasketIndicesToDB_EX(collIndices As clsBasketIndexColl, ByVal nWeightCount&)
    On Error GoTo EH
    Dim aIdx As clsBasketIndexAtom, aIdxComp As clsBasketIndexCompAtom, nCount&
    Dim i&, sParent$, nContractID&, nContractTypeID&
    Dim rs As ADODB.Recordset, sMsg$, nIndexId&, nStockID&, nMsHint&
    Dim bIsError As Boolean
    
    sParent = m_Parent
    lblStatus.Caption = "Saving Index Basket Weights..."
    m_Parent = LogMsg("Save Index Basket Weights")
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = nWeightCount
    nCount = 0
        
    For Each aIdx In collIndices
        On Error GoTo FailedSave
        bIsError = True
            
        If aIdx.ID <> 0 And aIdx.NewComponents.Count > 0 Then
            gDBW.usp_BoIndexWithBasketInfo_ClearWeights aIdx.ID
'            gDBW.usp_ContractPrice_Save Null, aIdx.ID, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, aIdx.ClosePrice
        End If
            
        For Each aIdxComp In aIdx.NewComponents
            nIndexId = aIdx.ID
            nStockID = aIdxComp.ID
            nMsHint = gDBW.usp_BoIndexBasketWeight_Save(nIndexId, aIdx.Symbol, nStockID, aIdxComp.Symbol, aIdxComp.Weight, 1, 1, 0, sMsg)
'            gDBW.usp_ContractPrice_Save Null, nStockID, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, Null, aIdxComp.ClosePrice
                    
            If nMsHint > 0 Then ' new contracts
                If (nMsHint And 1) = 1 Then _
                    LogMsg "New index was added to database: " & aIdx.Symbol & ". Please update market structure."
                        
                If (nMsHint And 2) = 2 Then _
                    LogMsg "New stock was added to database: " & aIdxComp.Symbol & ". Please update market structure."
                    
            ElseIf nMsHint < 0 Then ' error
                LogMsg "Fail to save basket index weight: " & sMsg
                If nMsHint >= -4 Then Exit For ' index errors
            ElseIf nMsHint = 0 Then
                bIsError = False
            End If
            
            nCount = nCount + 1
            On Error GoTo EH
            Idle
            pbProgress.Value = pbProgress.Value + 1
        Next
        
        If Not bIsError Then LogMsg "Basket index """ & aIdx.Symbol & """ was successfully updated."
    Next
    
    SaveBasketIndicesToDB_EX = nCount
    m_Parent = sParent
    Exit Function
FailedSave:
    Dim Msg As String
    Msg = "Fail to save index basket weights."
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    
    SaveBasketIndicesToDB_EX = nCount
    LogMsg Msg
    m_Parent = sParent
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteLoadOptExchanges() As Boolean
    Dim Count As Long
    On Error GoTo EH
    
    Dim f As String, arrFiles() As String, nLBound&, nUBound&, i&
    Dim collExch As New clsExchColl, collSymExch As New clsSymbolExchColl
    
    f = ExecuteOpenFileDialog("Open Option Exchanges Files", True, "Text and Perl Script files (*.txt; *.pl)|*.txt;*.pl|All files (*.*)|*.*")
    If Len(f) = 0 Then
        Exit Function
    End If
    
    On Error Resume Next
    arrFiles = Split(f, Chr(0))
    nLBound = 0
    nUBound = -1
    nLBound = LBound(arrFiles)
    nUBound = UBound(arrFiles)
    On Error GoTo EH
    
    If nLBound > nUBound Then Exit Function
    
    m_Parent = LogMsg("Process 'Load Option Exchanges'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    If LoadOptExchangesFromDB(collExch, collSymExch) Then
        If nUBound > nLBound Then
            For i = nLBound + 1 To nUBound
                arrFiles(i) = arrFiles(nLBound) & "\" & arrFiles(i)
            Next
            Count = LoadOptExchangesFromFiles(arrFiles, nLBound + 1, nUBound, collExch, collSymExch)
        Else
            Count = LoadOptExchangesFromFiles(arrFiles, nLBound, nUBound, collExch, collSymExch)
        End If
        
        If Count > 0 Then
            Count = SaveOptExchangesToDB(collSymExch)
        End If
    End If
    
    ExecuteLoadOptExchanges = True
Done:
    Erase arrFiles
    On Error Resume Next
    LogTaskFinished , Count, "Symbols"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function SaveOptExchangesToDB(ByRef collSymExch As clsSymbolExchColl) As Long
    On Error GoTo FailedSave
    Dim aExch As clsExchAtom, aSymExch As clsSymbolExchAtom, nSymCount&, i&, nExchCount&, e&, sParent$, nCount&
    
    nSymCount = collSymExch.Count
    
    sParent = m_Parent
    lblStatus.Caption = "Saving Option Exchanges..."
    m_Parent = LogMsg("Save Option Exhanges")
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = nSymCount
    nCount = 0
    
    If nSymCount > 0 Then
        
        SaveOptionExchInit
        
        For i = 1 To nSymCount
            On Error GoTo FailedSave
            Set aSymExch = collSymExch(i)
            
            nExchCount = aSymExch.Exch.Count
            For e = 1 To nExchCount
                Set aExch = aSymExch.Exch(e)
                SaveOptionExch aExch.ID, aSymExch.ID
                Set aExch = Nothing
            Next
            
            If nExchCount > 0 Then nCount = nCount + 1
            Set aSymExch = Nothing
        
            On Error GoTo EH
            Idle
            pbProgress.Value = pbProgress.Value + 1
        Next
    End If
    
    SaveOptExchangesToDB = nCount
    If nSymCount > 0 Then SaveOptionExchDone
    m_Parent = sParent
    Exit Function
FailedSave:
    Dim Msg As String
    Msg = "Fail to save option exchanges."
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    
    If nSymCount > 0 Then SaveOptionExchDone
    
    SaveOptExchangesToDB = nCount
    LogMsg Msg
    m_Parent = sParent
End Function

Private Function LoadOptExchangesFromDB(ByRef collExch As clsExchColl, ByRef collSymExch As clsSymbolExchColl) As Boolean
    On Error GoTo EH
    Dim aExch As clsExchAtom, aSymExch As clsSymbolExchAtom, sKey$
    Dim rs As ADODB.Recordset
    
    LoadOptExchangesFromDB = False
    
    collExch.Clear
    collSymExch.Clear
    
    Set rs = gDBW.usp_Exchange_Get(Null)
    While Not rs.EOF
        sKey = UCase$(Trim$(gCmn.ReadStr(rs!vcOccCode)))
        
        If collExch(sKey) Is Nothing Then
            Set aExch = collExch.Add(sKey)
            aExch.ID = gCmn.ReadLng(rs!iExchangeId)
            aExch.Code = sKey
            aExch.Symbol = gCmn.ReadStr(rs!vcExchangeName)
            Set aExch = Nothing
        End If
        
        rs.MoveNext
    Wend
    Set rs = Nothing
    
    If collExch.Count > 0 Then
        Set rs = gDBW.usp_BoUnderlyings_Get()
        While Not rs.EOF
            sKey = UCase$(Trim$(gCmn.ReadStr(rs!vcSymbol)))
            
            If collSymExch(sKey) Is Nothing Then
                Set aSymExch = collSymExch.Add(sKey)
                aSymExch.ID = gCmn.ReadLng(rs!iContractID)
                aSymExch.Symbol = sKey
                Set aSymExch = Nothing
            End If
            
            rs.MoveNext
        Wend
        Set rs = Nothing
        
        If collSymExch.Count = 0 Then
            LogMsg "Error: No symbols in database."
        End If
    Else
        LogMsg "Error: No exchanges with OCC code defined."
    End If
    
    LoadOptExchangesFromDB = (collExch.Count > 0 And collSymExch.Count > 0)
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to load symbols info from database."
End Function

Private Function LoadOptExchangesFromFiles(ByRef arrFiles() As String, ByVal nLBound As Long, _
                                            ByVal nUBound As Long, ByRef collExch As clsExchColl, _
                                            ByRef collSymExch As clsSymbolExchColl) As Long
    Dim i&, sExchanges$, arrExch() As String, nLBnd&, nUBnd&, arrItems() As String, nItLBnd&, nItUBnd&, n&
    Dim sParent$, nCount&, aExch As clsExchAtom, aSymExch As clsSymbolExchAtom, c&, nExchLen&, sCode$, sSymbol$, sExchCodes$
    On Error GoTo FailedFile
    
    sParent = m_Parent
    lblStatus.Caption = "Reading Option Exchanges..."
    m_Parent = LogMsg("Read Option Exhanges")
    pbProgress.Min = 0
    pbProgress.Max = 1000
    pbProgress.Value = 0
    nCount = 0
    
    For i = nLBound To nUBound
        sExchanges = sExchanges & LoadStringFromFile(arrFiles(i))
    Next
    
    On Error Resume Next
    arrExch = Split(sExchanges, vbLf)
    nLBnd = 0
    nUBnd = -1
    nLBnd = LBound(arrExch)
    nUBnd = UBound(arrExch)
    
    If nUBnd >= nLBnd Then
        pbProgress.Max = nUBnd - nLBnd + 1
        
        For i = nLBnd To nUBnd
            On Error Resume Next
            If Len(Trim$(arrExch(i))) > 0 Then
                arrItems = Split(arrExch(i), " ")
                nItLBnd = 0
                nItUBnd = -1
                nItLBnd = LBound(arrItems)
                nItUBnd = UBound(arrItems)
                On Error GoTo FailedFile
                
                If nItUBnd - nItLBnd > 0 Then
                    
                    sSymbol = ""
                    sExchCodes = ""
                    n = 0
                    For c = nItLBnd To nItUBnd
                        sCode = UCase$(Trim$(arrItems(c)))
                        If sCode <> "" Then
                            Select Case n
                                Case 1
                                    sSymbol = sCode
                                    
                                Case 2
                                    sExchCodes = sCode
                                    
                            End Select
                            
                            n = n + 1
                            If n > 2 Then Exit For
                        End If
                    Next
                
                    nExchLen = Len(sExchCodes)
                    If Len(sSymbol) > 0 And nExchLen > 0 Then
                        Set aSymExch = collSymExch(sSymbol)
                        If Not aSymExch Is Nothing Then
                            
                            For c = 1 To nExchLen
                                sCode = Mid$(sExchCodes, c, 1)
                                Set aExch = collExch(sCode)
                                If Not aExch Is Nothing Then
                                    If aSymExch.Exch(sCode) Is Nothing Then aSymExch.Exch.Add sCode, aExch
                                End If
                            Next
                            
                            nCount = nCount + 1
                            
                            Set aSymExch = Nothing
                        End If
                    End If
                End If
                
                Erase arrItems
            End If
            
            On Error GoTo EH
            Idle
            pbProgress.Value = pbProgress.Value + 1
        Next
    End If
    
    Erase arrExch
    
    If nCount = 0 Then
        Err.Raise vbObjectError, , "No option exchanges found."
        Exit Function
    End If
    
    LoadOptExchangesFromFiles = nCount
    m_Parent = sParent
    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    Erase arrExch
    Erase arrItems
    LoadOptExchangesFromFiles = nCount
    LogMsg Msg
    m_Parent = sParent
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteOpenFileDialog(sTitle As String, ByVal bMultiSelect As Boolean, sFilter As String) As String
    With Me.dlgFileSelect
        .Flags = cdlOFNFileMustExist Or cdlOFNLongNames Or cdlOFNPathMustExist Or cdlOFNExplorer
        If bMultiSelect Then
            .Flags = .Flags Or cdlOFNAllowMultiselect
        End If
        .CancelError = True
        .DialogTitle = sTitle
        .Filter = sFilter
        
'        If Len(g_FolderForPositionsFile) > 0 Then
'            .InitDir = g_FolderForPositionsFile
'        Else
            .InitDir = MakeCommonFilePath("", "", True)
'        End If
        .FileName = ""
        On Error GoTo CatchCancel
        .ShowOpen
        ExecuteOpenFileDialog = .FileName
        g_FolderForPositionsFile = ExecuteOpenFileDialog
    End With
    Exit Function
CatchCancel:
    If Err.Number = 32755 Then
        ' Cancel pressed. Exit
        Exit Function
    End If
    ShowError
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteSaveFileDialog(sTitle As String, sDefName As String, sFilter As String) As String
    With dlgFileSelect
        .Flags = cdlOFNLongNames Or cdlOFNExplorer Or cdlOFNOverwritePrompt
        .CancelError = True
        .DialogTitle = sTitle
        .Filter = sFilter
        
'        If Len(g_FolderForExportFile) > 0 Then
'            .InitDir = g_FolderForExportFile
'        Else
            .InitDir = MakeCommonFilePath("", "", True)
'        End If
        .FileName = sDefName
        On Error GoTo CatchCancel
        .ShowSave
        ExecuteSaveFileDialog = .FileName
        g_FolderForExportFile = ExecuteSaveFileDialog
    End With
    Exit Function
CatchCancel:
    If Err.Number = 32755 Then
        ' Cancel pressed. Exit
        Exit Function
    End If
    ShowError
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadPositionsFromFile(ByVal sFileName As String, ByVal dtFileDate As Date) As Long
    Dim collPos As New clsRecPosColl, collTrader As New clsRecPosTraderColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, dtUTCTradeDate As Date, bIsLoadPrice As Boolean, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&, bClearSim As Boolean, bClearMan As Boolean, bIsSaveHistory As Boolean
    On Error GoTo FailedFile
    
    sFirstString = LoadFirstStringFromFile(sFileName)
    
    bFormatType = m_clsPosRecProcessor.ParseFileFormatType(sFirstString)
    If bFormatType < 1 Or bFormatType > 6 Then
        Err.Raise vbObjectError, "Load Positions", "Wrong format of input file"
    End If
    If dtUTCTradeDate = dtFileDate Then
        dtUTCTradeDate = m_clsPosRecProcessor.TradeDate
    Else
        dtUTCTradeDate = dtFileDate
    End If
                        
    If (bFormatType = 3 Or bFormatType = 6) Then
        bIsLoadPrice = False
    Else
        bIsLoadPrice = True
    End If
    bClearSim = True
    bClearMan = True
    bIsSaveHistory = True
    
    bRet = frmReconcileParams.Execute("Confirm position reconciliation params", "Reconcilation date :", dtUTCTradeDate, bIsLoadPrice, bClearSim, bClearMan, bIsSaveHistory)
    LogMsg "Position Reconciliation with file format #" & CStr(bFormatType) & ". Positions date : " & dtUTCTradeDate
    
    If Not bRet Then
        LogMsg "Position Reconciliation canceled."
    Else
        If Not PositionsClearSimulatedAndManualTrades(bClearSim, bClearMan) Then Exit Function
        
        m_clsPosRecProcessor.TradeDate = dtUTCTradeDate
                
        sParent = m_Parent
        lblStatus.Caption = "Loading Positions from database..."
        m_Parent = LogMsg("Load Positions from database")
        pbProgress.Min = 0
        pbProgress.Max = 100
        pbProgress.Value = 0
        bRet = m_clsPosRecProcessor.LoadPositionsFromDatabase(False)
        m_Parent = sParent
        
        If bRet Then
            sParent = m_Parent
            lblStatus.Caption = "Reading Positions from File..."
            m_Parent = LogMsg("Read Positions from File")
            pbProgress.Min = 0
            pbProgress.Value = 0
            pbProgress.Max = 100
        
            On Error GoTo FailedFile
            bRet = m_clsPosRecProcessor.LoadPositionsFromText(LoadStringFromFile(sFileName))
            m_Parent = sParent
            If bRet Then
                On Error GoTo EH
                
                sLogParent = m_Parent
                lblStatus.Caption = "Update option root lot size from file..."
                m_Parent = LogMsg("Update option root lot size")
                bRet = m_clsPosRecProcessor.SaveNewLotSizeToDatabase
                m_Parent = sLogParent
    
                If bIsLoadPrice Then
                    sLogParent = m_Parent
                    m_Parent = LogMsg("Upload close prices from positions file to DB")
                    lblStatus.Caption = "Uploading contract close prices from file..."
                    bRet = m_clsPosRecProcessor.SaveClosePriceToDatabase
                    m_Parent = sLogParent
                End If
                
                sParent = m_Parent
                lblStatus.Caption = "Checking Positions..."
                m_Parent = LogMsg("Check Postitions")
                bRet = m_clsPosRecProcessor.ShowPositionsReport
                m_Parent = sParent
                If bRet Then
                    On Error GoTo FailedSave
                    sParent = m_Parent
                    lblStatus.Caption = "Saving Positions..."
                    m_Parent = LogMsg("Save Positions")
                    nRet = m_clsPosRecProcessor.SavePositionsToDatabase(bIsSaveHistory)
                    LoadPositionsFromFile = nRet
                    m_Parent = sParent
                Else
                    LogMsg "Position Reconciliation canceled."
                End If
            End If
        End If
    End If
    
    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
    GoTo EH
FailedSave:
    Msg = "Saving positions failed."
    GoTo EH
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadStringFromFile(ByVal FileName As String) As String
    Dim s$, n&, f&
    n = FileLen(FileName)
    If n > 0 Then
        s = Space(n)
        f = FreeFile
        Open FileName For Binary Access Read As #f
        Get #f, , s
        Close #f
        LoadStringFromFile = s
    End If
End Function

Private Function LoadFirstStringFromFile(ByVal FileName As String) As String
    Dim s$, f&
    Const ForReading = 1
    Const TristateUseDefault = -2
    f = FreeFile
    s = ""
    Open FileName For Input As #f
    If Not EOF(f) Then
        Line Input #f, s   ' Read line into variable.
    End If
    Close #f
    LoadFirstStringFromFile = s
End Function

Private Function PositionsClearSimulatedAndManualTrades(bClearSim As Boolean, bClearMan As Boolean) As Boolean
    On Error GoTo EH
    Dim sParent$
    PositionsClearSimulatedAndManualTrades = False
    
    sParent = m_Parent
    lblStatus.Caption = "Clear Simulated/Manual trades..."
    m_Parent = LogMsg("Clear Simulated/Manual trades")
    
   
    If bClearSim Or bClearMan <> 0 Then
    
        gDBW.usp_TradeSim_Clear IIf(bClearSim, 1, 0), IIf(bClearMan, 1, 0)
        
        If bClearSim And bClearMan Then
            LogMsg "Simulated/Manual Trades are deleted."
        ElseIf bClearSim Then
            LogMsg "Simulated Trades are deleted."
            LogMsg "Manual Trades are not deleted."
        Else
            LogMsg "Simulated Trades are not deleted."
            LogMsg "Manual Trades are deleted."
        End If
    Else
        LogMsg "Simulated/Manual Trades are not deleted."
    End If
    
    PositionsClearSimulatedAndManualTrades = True
    
Done:
    On Error Resume Next
    m_Parent = sParent
    Exit Function
EH:
    LogMsg "Fail to clear Simulated/Manual trades: " & Err.Description
    Resume Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteExportTodayTrades() As Boolean
    Dim s_Parent$, hFile&, sLine$, nContractType&, nContractID&, dStrike#, bIsCall As Boolean, sExportSymbol$
    Dim rs As ADODB.Recordset, dtLastTrade As Date, f$, nUndId&, nUndTypeId&, nRootID&, sUndSymbol$, sRootSymbol$, sSymbol$
    Dim sPortiaId$, dtFirstDayInExpiryMonth As Date, sTemp$, nCount&, dtTradeDate As Date, nQuantity&, dPrice#, bIsBuy As Boolean
    Dim nLotSize&, sTraderName$, sBrokerName$, dBrokerCommission#, bTradeMark As Boolean, sStrategyName$, sTraderGroup$, sFileName$
    Dim sFuturePortiaId$, sFutureRoot$, sFutureRootExpSymbol$
    Dim dtExpiry As Date
    Dim dtFutMaturity As Date
    
    On Error GoTo EH
    m_Parent = LogMsg("Process 'Export new non RTTF trades'")
    nCount = 0
    LogTaskStarted
    
    dtLastTrade = Now
    If Not frmReconcileParams.Execute("Confirm last trade date", "Last trade date :", dtLastTrade) Then
        LogMsg "Export new non RTTF trades canceled."
    Else
        dtLastTrade = dtLastTrade
        
        sFileName = "TRADE"
        sFileName = sFileName & Format$(dtLastTrade, "yymmdd")
        f = ExecuteSaveFileDialog("Select New Non RTTF trades Export File", sFileName, "Comma separated values files (*.csv)|*.csv")
        If Len(f) = 0 Then
            Exit Function
        End If
        hFile = FreeFile
        Open f For Output As #hFile
        Set rs = gDBW.usp_BoTradeNew_Get(LocalToGmt(dtLastTrade))
        If Not rs Is Nothing Then
            With rs
                If .RecordCount > 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        dtTradeDate = GmtToLocal(gCmn.ReadDate(!dtTradeDate))
                        nQuantity = gCmn.ReadLng(!iQuantity)
                        dPrice = gCmn.ReadDbl(!fPrice)
                        bIsBuy = gCmn.ReadByte(!tiIsBuy) <> 0
                        
                        nContractID = gCmn.ReadLng(!iContractID)
                        nContractType = gCmn.ReadLng(!iContractTypeID)
                        sSymbol = gCmn.ReadStr(!vcSymbol)
                        sExportSymbol = gCmn.ReadStr(!vcExportSymbol)
                        
                        dtExpiry = gCmn.ReadDate(!dtExpiryDate)
                        dStrike = gCmn.ReadDbl(!fStrike)
                        bIsCall = gCmn.ReadByte(!tiIsCall) <> 0
                        nUndId = gCmn.ReadLng(!iUnderlyingContractID)
                        nUndTypeId = gCmn.ReadLng(!iUnderlyingContractTypeID)
                        sUndSymbol = gCmn.ReadStr(!vcUnderlyingSymbol)
                        nRootID = gCmn.ReadLng(!iOptionRootID)
                        sRootSymbol = gCmn.ReadStr(!vcOptionRootSymbol)
                        nLotSize = gCmn.ReadLng(!iLotSize)
                        sTraderName = gCmn.ReadStr(!vcTraderName)
                        sBrokerName = gCmn.ReadStr(!vcBrokerName)
                        
                        dBrokerCommission = gCmn.ReadDbl(!fBrokerCommission)
                        
                        If nQuantity * nLotSize > DBL_EPSILON Then
                            dBrokerCommission = dBrokerCommission / (nQuantity * nLotSize)
                        Else
                            dBrokerCommission = 0
                        End If
                        
                        bTradeMark = gCmn.ReadLng(!iTradeMark) <> 0
                        sStrategyName = gCmn.ReadStr(!vcStrategyName)
                        sTraderGroup = gCmn.ReadStr(!vcTraderGroupName)
                        
                        sFutureRoot = gCmn.ReadStr(!vcFutureRootSymbol)
                        sFutureRootExpSymbol = Trim$(gCmn.ReadStr(!vcFutureRootExportSymbol))
                        dtFutMaturity = gCmn.ReadDate(!FutureMaturityDate)
                        
                        sPortiaId = ""
                        sFuturePortiaId = ""
                        If nContractType = 1 Then
                            sPortiaId = sSymbol & " INDEX"
                        ElseIf nContractType = 2 Then
                            sPortiaId = sSymbol & " US"
                        ElseIf nContractType = 3 Then
                            sPortiaId = GetCIBCOptionPortiaId(sExportSymbol, nContractID, sSymbol, dtExpiry, dStrike, bIsCall, nUndId, sUndSymbol, dtFirstDayInExpiryMonth)
                        ElseIf nContractType = 4 Then
                            sPortiaId = GetCIBCFuturePortiaId(IIf(Len(sFutureRootExpSymbol) = 0, sFutureRoot, sFutureRootExpSymbol), _
                                                            dtFutMaturity, dtFirstDayInExpiryMonth)
                        ElseIf nContractType = 5 Then
                            sFuturePortiaId = GetCIBCFuturePortiaId(IIf(Len(sFutureRootExpSymbol) = 0, sFutureRoot, sFutureRootExpSymbol), _
                                                            dtFutMaturity, dtFirstDayInExpiryMonth)
                            sPortiaId = GetCIBCFutureOptionPortiaId(dtExpiry, dStrike, bIsCall, sFuturePortiaId, dtFirstDayInExpiryMonth)
                        End If
                        
                        If Len(sPortiaId) > 0 Then
                            sLine = sTraderName & ","
                            sLine = sLine & IIf(bIsBuy, "B", "S") & ","
                            sLine = sLine & Trim$(Str$(nQuantity)) & ","
                            sLine = sLine & Trim$(Str$(dPrice)) & ","
                            sLine = sLine & sBrokerName & ","
                            sLine = sLine & Trim$(Str$(dBrokerCommission)) & ","
                            sLine = sLine & sStrategyName & ","
                            sLine = sLine & Trim$(Format$(dtTradeDate, "mm/dd/yyyy")) & ","
                            sLine = sLine & Trim$(Format$(dtTradeDate, "hh:nn:ss")) & ","
                            
                            'settlement day
                            sLine = sLine & Trim$(Format$(GetNextBussinessDay(dtTradeDate, IIf(nContractType = 3 Or nContractType = 5, 1, 3)), "mm/dd/yyyy")) & ","
                            sLine = sLine & sTraderGroup & ","
                            sLine = sLine & IIf(nContractType > 3, "CIBC", "OPCO") & ","
                            sLine = sLine & IIf(bTradeMark, "True", "False") & ","
                            sLine = sLine & Trim$(Format$(dtLastTrade, "mm/dd/yyyy")) & ","
                            sLine = sLine & sPortiaId
                            
                            '*******************************************************
                            '*File Format:
                            ' Trader, Buy/Sell, Quantity, Price, Broker,
                            ' Commision, Strategy, Trade date, Trade time, Settlement date, Trader Group,
                            ' Clearing broker, ADP Upload, Last trade date(by default file create date),
                            ' Portia ID
                            '*******************************************************
                            Print #hFile, sLine
                            nCount = nCount + 1
                        End If
                        .MoveNext
                    Loop
                End If
            End With
        End If
        If hFile Then Close #hFile
    End If
    LogTaskFinished , nCount, "Trades"
    m_Parent = ""
    Exit Function
EH:
    If hFile Then Close #hFile
    LogMsg Err.Description
    m_Parent = s_Parent
    Set rs = Nothing
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteExportNewSecurities() As Boolean
    Dim s_Parent$, hFile&, sLine$, nContractType&, nContractID&, dStrike#, bIsCall As Boolean, sExportSymbol$
    Dim rs As ADODB.Recordset, dtLastTrade As Date, f$, nUndId&, nUndTypeId&, nRootID&, sUndSymbol$, sRootSymbol$, sSymbol$
    Dim sPortiaId$, dtFirstDayInExpiryMonth As Date, sTemp$, nCount&, sFileName$, sFuturePortiaId$, sFutureRoot$
    Dim sFutureRootExpSymbol$
    Dim dtExpiry As Date
    Dim dtMaturity As Date
    
    On Error GoTo EH
    m_Parent = LogMsg("Process 'Export new securities traded from last reconcile'")
    LogTaskStarted
    nCount = 0
    dtLastTrade = Now
    If Not frmReconcileParams.Execute("Confirm last trade date", "Last trade date :", dtLastTrade) Then
        LogMsg "Export new securities traded from last reconcile canceled."
    Else
        dtLastTrade = dtLastTrade
    
        sFileName = "SECUR"
        sFileName = sFileName & Format$(dtLastTrade, "yymmdd")
        f = ExecuteSaveFileDialog("Select New Securities Export File", sFileName, "Comma separated values files (*.csv)|*.csv")
        If Len(f) = 0 Then
            Exit Function
        End If
        hFile = FreeFile
        Open f For Output As #hFile
        Set rs = gDBW.usp_BoTradeNewContract_Get(dtLastTrade)
        If Not rs Is Nothing Then
            With rs
                If .RecordCount > 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        nContractID = gCmn.ReadLng(!iContractID)
                        nContractType = gCmn.ReadLng(!iContractTypeID)
                        sSymbol = gCmn.ReadStr(!vcSymbol)
                        sExportSymbol = gCmn.ReadStr(!vcExportSymbol)
                        dtExpiry = gCmn.ReadDate(!dtExpiry)
                        dStrike = gCmn.ReadDbl(!fStrike)
                        bIsCall = gCmn.ReadByte(!tiIsCall) <> 0
                        nUndId = gCmn.ReadLng(!iUnderlyingContractID)
                        nUndTypeId = gCmn.ReadLng(!iUnderlyingContractTypeID)
                        sUndSymbol = gCmn.ReadStr(!vcUnderlyingSymbol)
                        nRootID = gCmn.ReadLng(!iOptionRootID)
                        sRootSymbol = gCmn.ReadStr(!vcOptionRootSymbol)
                        
                        sFutureRoot = gCmn.ReadStr(!vcFutureRootSymbol)
                        sFutureRootExpSymbol = Trim$(gCmn.ReadStr(!vcFutureRootExportSymbol))
                        dtMaturity = gCmn.ReadDate(!dtFutureMaturity)
                        
                        sPortiaId = ""
                        sFuturePortiaId = ""
                        If nContractType = 1 Then
                            sPortiaId = sSymbol & " INDEX"
                        ElseIf nContractType = 2 Then
                            sPortiaId = sSymbol & " US"
                        ElseIf nContractType = 3 Then
                            sPortiaId = GetCIBCOptionPortiaId(sExportSymbol, nContractID, sSymbol, dtExpiry, dStrike, bIsCall, nUndId, sUndSymbol, dtFirstDayInExpiryMonth)
                        ElseIf nContractType = 4 Then
                            sPortiaId = GetCIBCFuturePortiaId(IIf(Len(sFutureRootExpSymbol) = 0, sFutureRoot, sFutureRootExpSymbol), _
                                                        dtMaturity, dtFirstDayInExpiryMonth)
                        ElseIf nContractType = 5 Then
                            sFuturePortiaId = GetCIBCFuturePortiaId(IIf(Len(sFutureRootExpSymbol) = 0, sFutureRoot, sFutureRootExpSymbol), _
                                                dtMaturity, dtFirstDayInExpiryMonth)
                            sPortiaId = GetCIBCFutureOptionPortiaId(dtExpiry, dStrike, bIsCall, sFuturePortiaId, dtFirstDayInExpiryMonth)
                        End If
                        
                        If Len(sPortiaId) > 0 Then
                            
                            If nContractType = 1 Then
                                sLine = "IND,"
                                sLine = sLine & sSymbol
                                sLine = sLine & ",US,0,0,,,,,,,,,,,,"
                            ElseIf nContractType = 2 Then
                                sLine = "ST,"
                                sLine = sLine & sSymbol
                                sLine = sLine & ",US,0,0,,,,,,,,,,,,"
                            ElseIf nContractType = 3 Then
                                sLine = "OP,"
                                sLine = sLine & ",US,,,"
                                sLine = sLine & IIf(bIsCall, "C", "P") & ","
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "mmm")) & ","
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "yyyy")) & ","
                                sLine = sLine & Trim$(Str$(dStrike)) & ","
                                
                                If nUndTypeId = 1 Then
                                    sTemp = sUndSymbol & " Index,Index,0,"
                                ElseIf nUndTypeId = 2 Then
                                    sTemp = sUndSymbol & " US,Equity,0,"
                                End If
                                sLine = sLine & sTemp
                                sLine = sLine & Trim$(sRootSymbol) & ","
                                sLine = sLine & ",,,"
                            ElseIf nContractType = 4 Then
                                sLine = "FUT,"
                                sLine = sLine & ",,,,,"
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "mmm")) & ","
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "yyyy")) & ",,"
                                sLine = sLine & sUndSymbol & " Index,,,,"
                                sLine = sLine & sPortiaId & ",,,"
                            ElseIf nContractType = 5 Then
                                sLine = "Devon Option,"
                                sLine = sLine & ",US,,,"
                                sLine = sLine & IIf(bIsCall, "C", "P") & ","
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "mmm")) & ","
                                sLine = sLine & Trim$(Format$(dtFirstDayInExpiryMonth, "yyyy")) & ","
                                sLine = sLine & Trim$(Str$(dStrike)) & ","
                                sLine = sLine & sFuturePortiaId & ",Future,1,,,,,"
                            End If
                            sLine = sLine & sPortiaId
                            
                            '*******************************************************
                            '*File Format:
                            ' Security Type, Ticker, Country/Exchange code, CDF indicator, Mutual Fund,
                            ' Call/Put indicator, Expiration month, Expiration year, Strike, Underlying Ticker,
                            ' Equity/Index, Option On Future Indicator, Root, Future Ticker, Bloomberg Name,
                            ' YellowKey, Portia ID
                            '*******************************************************
                            Print #hFile, sLine
                            nCount = nCount + 1
                        End If
                        .MoveNext
                    Loop
                End If
            End With
        End If
        If hFile Then Close #hFile
    End If
    LogTaskFinished , nCount, "Securities"
    m_Parent = ""
    Exit Function
EH:
    If hFile Then Close #hFile
    LogMsg Err.Description
    m_Parent = s_Parent
    Set rs = Nothing
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub DebugSave(ByVal message As String)
    Debug.Print message
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ActiveLabel1_Click()
Dim Buffer As String
Dim NodX As Node
Dim s As String
Dim i As Long
    On Error Resume Next
    Buffer = ""
    With TreeView1
        For Each NodX In .Nodes
            s = NodX.FullPath
            i = UBound(Split(s, "\"))
            If i > 0 Then
                Buffer = Buffer & String(i - 1, vbTab)
            End If
            s = NodX.Text
            If Len(s) > 0 Then
                Buffer = Buffer & s & vbCrLf
            End If
        Next
    End With
    With Clipboard
        .Clear
        .SetText Buffer
    End With
End Sub

Private Sub OpenLogFile()
    Dim sPath$, nLen#
    On Error Resume Next
    Dim sStringFileName As String
    sStringFileName = "ivrm_bo_eod_" + Format$(Now, "YYYYMM_DD_HHMM") + ".log"
    sPath = MakeCommonFilePath("back_office", sStringFileName, True, True)
    
    m_hLog = FreeFile
    nLen = FileLen(sPath)
    If nLen > 1000000 Then
        Open sPath For Output As #m_hLog
    Else
        Open sPath For Append As #m_hLog
    End If
    
    PrintLogFile ("Started by " & CurrentNtUserName$())
End Sub

Private Sub CloseLogFile()
    On Error Resume Next
    Close #m_hLog
End Sub

Public Sub PrintLogFile(ByVal Msg As String)
    On Error Resume Next
    Print #m_hLog, Now & " " & Msg
End Sub

Private Function GetCIBCOptionPortiaId( _
                ByVal sExportSymbol As String, _
                ByVal nContractID As Long, _
                ByVal sSymbol As String, _
                ByVal dtExpiryDate As Date, _
                ByVal dStrike As Double, _
                ByVal bIsCall As Boolean, _
                ByVal nUndId As Long, _
                ByVal sUndSymbol As String, _
                ByRef dtFirstDayInExpiryMonth As Date _
) As String
    Dim nMonth&, sPortiaId$, nYear&, rs As ADODB.Recordset, sKey$
    Dim collOpt As clsRecPosOptColl, aOpt As clsRecPosOptAtom
    On Error GoTo ErrHand
    nYear = Year(dtExpiryDate)
    nMonth = Month(dtExpiryDate)
    If nMonth = 0& Then nMonth = 12&
    sPortiaId = Format$(nYear - 2000, "#0")
    
    dtFirstDayInExpiryMonth = DateSerial(nYear, nMonth, 1)
    
    If Len(sExportSymbol) Then
        GetCIBCOptionPortiaId = sExportSymbol
        Exit Function
    End If
    
    If bIsCall Then
        sPortiaId = sPortiaId & Left(Chr(Asc("A") + nMonth - 1), 1)
    Else
        sPortiaId = sPortiaId & Left(Chr(Asc("M") + nMonth - 1), 1)
    End If
    
    sPortiaId = sPortiaId & " " & Trim$(Str$(dStrike))
    sPortiaId = sUndSymbol & " " & sPortiaId
    sPortiaId = Replace(sPortiaId, ",", "")

    Set rs = gDBW.usp_BoOptionByParams_Get(nUndId, dtExpiryDate, dStrike, bIsCall)
    If Not rs Is Nothing Then
        With rs
            If .RecordCount > 1 Then
                Set collOpt = New clsRecPosOptColl
                collOpt.Clear
                .MoveFirst
                Do While Not .EOF
                    sKey = gCmn.ReadStr(!iContractID)
                    Set aOpt = collOpt.Add(sKey)
                    aOpt.Expiry = dtFirstDayInExpiryMonth
                    aOpt.ID = gCmn.ReadLng(!iContractID)
                    aOpt.IsCall = bIsCall
                    aOpt.ExportSymbol = gCmn.ReadStr(!vcExportSymbol)
                    aOpt.OptionRootID = gCmn.ReadLng(!iOptionRootID)
                    aOpt.OptionRootSymbol = gCmn.ReadStr(!vcOptionRootSymbol)
                    aOpt.Strike = dStrike
                    aOpt.Symbol = gCmn.ReadStr(!vcSymbol)
                    If aOpt.ID = nContractID And Len(aOpt.ExportSymbol) = 0 Then
                        aOpt.ExportSymbol = sPortiaId ' set default
                    End If
                    Set aOpt = Nothing
                    .MoveNext
                Loop
            ElseIf .RecordCount = 1 Then
                gDBW.usp_BoContractExportSymbol_Save nContractID, sPortiaId
                GetCIBCOptionPortiaId = sPortiaId
                Exit Function
            End If
        End With
    End If
    
    Set aOpt = frmOptionSelect.Execute(True, collOpt, sSymbol, nContractID)
    If Not aOpt Is Nothing Then
        sPortiaId = aOpt.ExportSymbol
        gDBW.usp_BoContractExportSymbol_Save nContractID, sPortiaId
        GetCIBCOptionPortiaId = sPortiaId
        Set aOpt = Nothing
    Else
        GetCIBCOptionPortiaId = ""
    End If
    collOpt.Clear
    Set collOpt = Nothing
Exit Function
ErrHand:
    Set collOpt = Nothing
End Function

Private Function GetCIBCFutureOptionPortiaId( _
                ByVal dtExpiry As Date, _
                ByVal dStrike As Double, _
                ByVal bIsCall As Boolean, _
                ByVal sFutSymbol As String, _
                ByRef dtFirstDayInExpiryMonth As Date _
) As String
    Dim nMonth&, sPortiaId$, nYear&
    On Error Resume Next
    nYear = Year(dtExpiry)
    nMonth = Month(dtExpiry)
    If nMonth = 0& Then nMonth = 12&
    sPortiaId = Format$(nYear - 2000, "#0")
        
    If bIsCall Then
        sPortiaId = sPortiaId & Left(Chr(Asc("A") + nMonth - 1), 1)
    Else
        sPortiaId = sPortiaId & Left(Chr(Asc("M") + nMonth - 1), 1)
    End If
    
    sPortiaId = sPortiaId & " " & Trim$(Str$(dStrike))
        
    sPortiaId = sFutSymbol & " " & sPortiaId
    
    dtFirstDayInExpiryMonth = DateSerial(nYear, nMonth, 1)
    
    GetCIBCFutureOptionPortiaId = sPortiaId
End Function

Private Function GetCIBCFuturePortiaId( _
                ByVal sFutRootSymbol As String, _
                ByVal dtMaturity As Date, _
                ByRef dtFirstDayInExpiryMonth As Date _
) As String
    Dim nMonth&, sPortiaId$, nYear&
    On Error Resume Next
    nYear = Year(dtMaturity)
    nMonth = Month(dtMaturity)
    If nMonth = 0& Then nMonth = 12&
        
    sPortiaId = Format$(nYear - 2000, "#0")
    Select Case nMonth
        Case 1:
            sPortiaId = "F" & sPortiaId
        Case 2:
            sPortiaId = "G" & sPortiaId
        Case 3:
            sPortiaId = "H" & sPortiaId
        Case 4:
            sPortiaId = "J" & sPortiaId
        Case 5:
            sPortiaId = "K" & sPortiaId
        Case 6:
            sPortiaId = "M" & sPortiaId
        Case 7:
            sPortiaId = "N" & sPortiaId
        Case 8:
            sPortiaId = "Q" & sPortiaId
        Case 9:
            sPortiaId = "U" & sPortiaId
        Case 10:
            sPortiaId = "V" & sPortiaId
        Case 11:
            sPortiaId = "X" & sPortiaId
        Case 12:
            sPortiaId = "Z" & sPortiaId
    End Select
            
    sPortiaId = sFutRootSymbol & sPortiaId
        
    dtFirstDayInExpiryMonth = DateSerial(nYear, nMonth, 1)
    
    GetCIBCFuturePortiaId = sPortiaId
End Function

Private Function ExecuteImportCurve() As Boolean
    On Error GoTo EH
    Dim nCount&, sFileName$
    Dim collIndices As New clsBasketIndexColl
    
    sFileName = ExecuteOpenFileDialog("Open Curves File", False, "Text files (*.txt;*.csv)|*.txt;*.csv|All files (*.*)|*.*")
    If Len(sFileName) = 0 Then Exit Function
    
    
    m_Parent = LogMsg("IR Curve import is started from")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    LogMsg (sFileName)
    
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
    
    nCount = LoadImportedCurvesFromFile(sFileName)
    
    ExecuteImportCurve = True
Done:
    On Error Resume Next
    LogTaskFinished , nCount, "Curves import"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done

End Function

Private Function ExecuteTradesImport() As Boolean
    On Error GoTo EH
    Dim nCount&, sFileName$
    Dim collIndices As New clsBasketIndexColl
    
    sFileName = ExecuteOpenFileDialog("Open Trades File", False, "Text files (*.txt;*.csv)|*.txt;*.csv|All files (*.*)|*.*")
    If Len(sFileName) = 0 Then Exit Function
    
    
    m_Parent = LogMsg("Process 'Trades Import'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    LogMsg ("Trades File: " & sFileName)
    
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
    
    nCount = LoadImportedTradesFromFile(sFileName)
    
    ExecuteTradesImport = True
Done:
    On Error Resume Next
    LogTaskFinished , nCount, "Trades import"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function LoadImportedTradesFromFile&(ByVal sFileName$)
    Dim collPos As New clsRecPosColl, collTrader As New clsRecPosTraderColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&
    On Error GoTo FailedFile
    
    sFirstString = LoadFirstStringFromFile(sFileName)
    
    bFormatType = m_clsPosRecProcessor.ParseFileFormatTypeForTradesImport(sFirstString)
    If bFormatType <> 1 Then
        Err.Raise vbObjectError, "Trades Import", "Wrong format of input file"
    End If
                        
    sParent = m_Parent
    lblStatus.Caption = "Loading data from database..."
    m_Parent = LogMsg("Load data from database")
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    bRet = m_clsPosRecProcessor.LoadDataForTradesImportFromDatabase
    m_Parent = sParent
    
    If bRet Then
        sParent = m_Parent
        lblStatus.Caption = "Reading trades from file..."
        m_Parent = LogMsg("Read trades from file")
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = 100
    
        On Error GoTo FailedFile
        bRet = m_clsPosRecProcessor.LoadTradesFromText(LoadStringFromFile(sFileName))
        m_Parent = sParent
        If bRet Then
            On Error GoTo FailedSave
            sParent = m_Parent
            lblStatus.Caption = "Saving trades..."
            m_Parent = LogMsg("Save trades")
            nRet = m_clsPosRecProcessor.SaveTradesToDatabase()
            LoadImportedTradesFromFile = nRet
            m_Parent = sParent
        End If
    End If
    
    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
    GoTo EH
FailedSave:
    Msg = "Saving trades failed."
    GoTo EH
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function

Private Function LoadImportedCurvesFromFile&(ByVal sFileName$)
    Dim collPos As New clsRecPosColl, collTrader As New clsRecPosTraderColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&
    On Error GoTo FailedFile
    
    sFirstString = LoadFirstStringFromFile(sFileName)
    
    bFormatType = m_clsPosRecProcessor.ParseFileFormatTypeForCurvesImport(sFirstString)
    If bFormatType <> 1 Then
        Err.Raise vbObjectError, "Curves Import", "Wrong format of input file"
    End If
                        
    lblStatus.Caption = "Reading curves from file..."
    m_Parent = LogMsg("Read trades from file")
    pbProgress.Min = 0
    pbProgress.Value = 0
    pbProgress.Max = 100

    On Error GoTo FailedFile
    Dim sXML$
    bRet = m_clsPosRecProcessor.LoadCurvesFromText(LoadStringFromFile(sFileName), sXML)
    m_Parent = sParent
    If bRet Then
        On Error GoTo FailedSave
        sParent = m_Parent
        lblStatus.Caption = "Saving curves..."
        m_Parent = LogMsg("Save curves")
        nRet = m_clsPosRecProcessor.SaveCurvesToDatabase(sXML)
        LoadImportedCurvesFromFile = nRet
        m_Parent = sParent
    End If

    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
    GoTo EH
FailedSave:
    Msg = "Saving curves failed."
    GoTo EH
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function
Private Function LoadDividentImport(ByVal Buffer As String, ByRef vCount As Long) As String
    vCount = 0
    Dim Lines As Variant
    Dim Items As Variant
    Dim DateParts As Variant
    Dim l As Variant
    Dim Col As String
    Dim Cell As String
    Dim s_Parent As String
    Dim nContractID As Long
    Dim sKey As String
    Dim Msg As String
    Dim nDay As Long, nMonth As Long, nYear As Long
    
    Dim Count As Long, bUpdate As Boolean
    Dim j As Long

    Dim strXML As String
    Dim mIdxSymArr() As String
    
    Dim rsIndexList As ADODB.Recordset
    Dim dtExpDate As Date
    Dim dDivAmount As Double
    
    
'    j = 0
'    Set rsIndexList = gDBW.usp_BoContractsSymbol_Get(GINT_INDEXES)
'    ReDim mIdxSymArr(rsIndexList.RecordCount)
'    With rsIndexList
'        If .RecordCount > 0 Then
'            .MoveFirst
'            Do While Not .EOF
'                sKey = gCmn.ReadStr(.Fields("vcSymbol"))
'                mIdxSymArr(j) = sKey
'                j = j + 1
'                .MoveNext
'            Loop
'        End If
'    End With

    LoadDividentImport = ""
    strXML = "<Dividents>"

    s_Parent = m_Parent
    lblStatus.Caption = "Reading Dividents Import..."
    m_Parent = LogMsg("Read Dividents Import")
    pbProgress.Min = 0
    pbProgress.Max = 1000
    pbProgress.Value = 0
    Count = 0
    On Error GoTo EH

    If Buffer = "" Then
        Err.Raise vbObjectError, , "File is empty"
        Exit Function
    End If
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) <= 0 Then
        Lines = Split(Buffer, vbLf)
        If UBound(Lines) < 0 Then
            GoTo Done
        End If
    End If
    Count = 0
    pbProgress.Max = UBound(Lines) + 1

    For Each l In Lines
        On Error GoTo EH
        Count = Count + 1
        If Len(l) = 0 Then GoTo Skip
        Items = MySplit(l)
        If UBound(Items) <> 2 Then
            Err.Raise vbObjectError, "Load Stock Positions", "Wrong format of input file in line " & Count
            GoTo Skip
        End If
    
'        For j = 0 To rsIndexList.RecordCount
'            If mIdxSymArr(j) = UCase(Trim(Items(0))) Then
'                LogMsg "Symbol " & Trim(Items(0)) & " set as Index in ETS! It was Skipped!"
'            GoTo Skip
'            End If
'        Next
        
        On Error GoTo ParseError
        
        Col = "Expiry Date"
        Cell = Trim(Items(1))
        
        'Correct div date parsing
        DateParts = Split(Cell, "/")
        If UBound(DateParts) <> 2 Then
            GoTo Skip
        Else
            nDay = DateParts(1)
            nMonth = DateParts(0)
            nYear = DateParts(2)

            If nMonth >= 1 And nMonth <= 12 And _
                nDay >= 1 And nDay <= 31 And _
                nYear >= 2000 Then
                    dtExpDate = DateSerial(nYear, nMonth, nDay)
            Else
                GoTo ParseError
            End If
        End If
        
        dtExpDate = CDate(Trim(Items(1)))
        
        
        
        Col = "Div Amount"
        Cell = Trim(Items(2))
        dDivAmount = gCmn.ReadDbl(Trim(Items(2)))
        
        If dDivAmount <= 0# Then
            LogMsg "Dividend in line " & Count & " with 0 of amount! It was skipped!"
            GoTo Skip
        End If
        
        strXML = strXML + "<Divs Symbol=""" + GetValidXMLText(Trim(Items(0))) + """ " + "DivDate=""" + GetValidXMLText(Format(dtExpDate)) + """ "
        strXML = strXML + "DivAmt=""" + GetValidXMLText(Trim(Items(2))) + """ " + "></Divs>"
        vCount = vCount + 1
        pbProgress.Value = pbProgress.Value + 1
Skip:
        On Error Resume Next
        pbProgress.Value = Count
        Idle
    Next
Done:
    On Error GoTo 0
    m_Parent = s_Parent
    If strXML = "<Dividents>" Then 'Empty
        Err.Raise vbObjectError, , "No Dividents found."
        Exit Function
    End If
    strXML = strXML + "</Dividents>"
    LoadDividentImport = strXML
    Exit Function
EH:
    LogMsg "Error: " & Err.Description
    Resume Done
    Exit Function
FailedRead:
    Msg = Err.Description
    Resume CancelUpdate
    Exit Function
CancelUpdate:
    On Error Resume Next
    LoadDividentImport = ""
ParseError:
    LogMsg "Data skipped. Error parsing data in line " & Count & " '" & Col & "'='" & Cell & "'. " & Msg
    Col = ""
    Cell = ""
    GoTo Skip
End Function

Private Function ExecuteDividentImport() As Boolean
    On Error GoTo EH
    
    m_Parent = LogMsg("Process Dividents Import")
    LogTaskStarted
    ExecuteDividentImport = True
    Dim FileName As String
    FileName = ExecuteOpenFileDialog("Open Divident Import File", False, "Comma separated values files (*.csv)|*.csv|All files (*.*)|*.*")
    If Len(FileName) = 0 Then
        Exit Function
    End If

    Dim strBuff As String, lCount As Long, strXML As String
    strBuff = LoadStringFromFile(FileName)

    strXML = LoadDividentImport(strBuff, lCount)

    Dim rsNotExist As ADODB.Recordset
    Set rsNotExist = gDBW.usp_BO_DividentsImport(strXML)
    Dim iNotExCount As Integer
    iNotExCount = 0
    If Not rsNotExist Is Nothing Then
        With rsNotExist
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    Dim strStock As String
                    strStock = !SymbolsNotExist
                    LogMsg ("In file finded stock" + strStock + ". Please, add it.")
                    Dim lID As Integer
                    On Error Resume Next
                    'lID = gDBW.usp_Stock_Save(, , strStock, , , , , , , , , 0, 1, , , 1)

                    iNotExCount = iNotExCount + 1
                    .MoveNext
                Loop
            LogMsg (CStr(iNotExCount) + " Unknown stocks")
            End If
        End With
    End If

    'Set rsNotExist = gDBW.usp_BO_DividentsImport(strXML)

Done:
    On Error Resume Next
    LogTaskFinished , lCount, "Divident Import"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function


Private Function ExecuteVolatilityImport() As Boolean
    On Error GoTo EH
    Dim nCount&, sFileName$
    Dim collIndices As New clsBasketIndexColl
    
    sFileName = ExecuteOpenFileDialog("Open Volatility File", False, "Text files (*.txt;*.csv)|*.txt;*.csv|All files (*.*)|*.*")
    If Len(sFileName) = 0 Then Exit Function
    
    
    m_Parent = LogMsg("Process 'Volatility Import'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    
    LogMsg ("Volatility File: " & sFileName)
    
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
    
    nCount = LoadImportedVolatilityFromFile(sFileName)
    
    ExecuteVolatilityImport = True
Done:
    On Error Resume Next
    LogTaskFinished , nCount, "Volatility import"
    m_Parent = ""
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function LoadImportedVolatilityFromFile&(ByVal sFileName$)
    Dim collPos As New clsRecPosColl, collTrader As New clsRecPosTraderColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&
    On Error GoTo FailedFile
    
    sFirstString = LoadFirstStringFromFile(sFileName)
    
    bFormatType = m_clsPosRecProcessor.ParseFileFormatTypeForVolatilityImport(sFirstString)
    If bFormatType <> 1 Then
        Err.Raise vbObjectError, "Volatility Import", "Wrong format of input file"
    End If
                        
    sParent = m_Parent
    lblStatus.Caption = "Loading data from database..."
    m_Parent = LogMsg("Load data from database")
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    bRet = m_clsPosRecProcessor.LoadDataForVolatilityImportFromDatabase
    m_Parent = sParent
    
    If bRet Then
        sParent = m_Parent
        lblStatus.Caption = "Reading volatilities from file..."
        m_Parent = LogMsg("Read volatilities from file")
        pbProgress.Min = 0
        pbProgress.Value = 0
        pbProgress.Max = 100
    
        On Error GoTo FailedFile
        bRet = m_clsPosRecProcessor.LoadVolatilityFromText(LoadStringFromFile(sFileName))
        m_Parent = sParent
        If bRet Then
            On Error GoTo FailedSave
            
'            sParent = m_Parent
'            lblStatus.Caption = "Cleaning old volatilities..."
'            m_Parent = LogMsg("Clean old volatilities")
'            nRet = m_clsPosRecProcessor.ClearOldVolatilityFromDatabase()
            
            m_Parent = sParent
            lblStatus.Caption = "Saving volatilities..."
            m_Parent = LogMsg("Save volatilities")
            nRet = m_clsPosRecProcessor.SaveVolatilityToDatabase()
            LoadImportedVolatilityFromFile = nRet
            m_Parent = sParent
        End If
    End If
    
    Exit Function
FailedFile:
    Dim Msg As String
    Msg = "Reading from file failed."
    GoTo EH
FailedSave:
    Msg = "Saving volatilities failed."
    GoTo EH
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function

Private Function ExportAllTrades() As Boolean
On Error GoTo ErrHandler
    Dim sFileName As String
    
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
            
    sFileName = ExecuteSaveFileDialog("Export Trades", "TRADES" & Format(Date, "YYYYDDMM"), "(*.csv)|*.csv")
    LogMsg "Export to: " & sFileName
    If (Len(sFileName) > 0) Then
        LogMsg "Export Started at " & Format(Now, "DD/MMM/YYYY HH:MM:SS")
        m_clsPosRecProcessor.ExportTradesCFI sFileName, True
        LogMsg "Export Finished at " & Format(Now, "DD/MMM/YYYY HH:MM:SS")
    Else
        LogMsg "Trade export procedure aborted."
    End If
    
    ExportAllTrades = True
    Exit Function
ErrHandler:
    LogMsg "Error on export trade procedure."
    ExportAllTrades = False
End Function

Private Function ExecutePositionNetting() As Boolean
    Dim Count&
    On Error GoTo EH
    
    m_Parent = LogMsg("Process 'Postition Netting'")
    LogTaskStarted
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
        
    If m_clsPosRecProcessor Is Nothing Then
        Set m_clsPosRecProcessor = New clsPosReconcileProcessor
        m_clsPosRecProcessor.Init Me
    End If
    
    Count = RunPositionNetting
    
    ExecutePositionNetting = True
Done:
    On Error Resume Next
    LogTaskFinished , Count, "Positions"
    m_Parent = ""
    fNeedRefreshCurrent = True
    Exit Function
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    LogMsg LastErrorDescription
    GoTo Done
End Function

Private Function RunPositionNetting&()
    Dim collPos As New clsRecPosColl, collTrader As New clsRecPosTraderColl, collUnd As New clsRecPosUndColl
    Dim aPos As clsRecPosAtom, sLogMsg$, nCount#, i#, sParent$
    Dim s$, bFormatType As Byte
    Dim Items As Variant, dtUTCTradeDate As Date, bIsLoadPrice As Boolean, sLogParent, sFirstString As String
    Dim bRet As Boolean, nRet&, bClearSim As Boolean, bClearMan As Boolean, bIsSaveHistory As Boolean
    On Error GoTo EH
    
    dtUTCTradeDate = Date
                        
    bClearSim = True
    bClearMan = True
    bIsSaveHistory = True
    m_clsPosRecProcessor.SetFileFormatForNetting
    
    bRet = frmNettingParams.Execute("Confirm position netting params", "Netting date:", dtUTCTradeDate, bClearSim, bClearMan, bIsSaveHistory)
    LogMsg "Position netting date : " & dtUTCTradeDate
    
    If Not bRet Then
        LogMsg "Position netting canceled."
    Else
        If Not PositionsClearSimulatedAndManualTrades(bClearSim, bClearMan) Then Exit Function
        
        m_clsPosRecProcessor.TradeDate = dtUTCTradeDate
                
        sParent = m_Parent
        lblStatus.Caption = "Loading Positions from database..."
        m_Parent = LogMsg("Load Positions from database")
        pbProgress.Min = 0
        pbProgress.Max = 100
        pbProgress.Value = 0
        bRet = m_clsPosRecProcessor.LoadPositionsFromDatabase(True)
        m_Parent = sParent
        
        If bRet Then
            On Error GoTo FailedSave
            sParent = m_Parent
            lblStatus.Caption = "Saving Positions..."
            m_Parent = LogMsg("Save Positions")
            nRet = m_clsPosRecProcessor.SavePositionsToDatabase(bIsSaveHistory)
            RunPositionNetting = nRet
            m_Parent = sParent
        End If
    End If
    
    Exit Function
FailedSave:
    Dim Msg As String
    Msg = "Saving positions failed."
    GoTo EH
EH:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    If Len(Msg) = 0 Then
        Msg = "Error:" & Err.Description
    Else
        Msg = Msg & " " & Err.Description
    End If
    On Error Resume Next
    LogMsg Msg
End Function

Private Function GetDateFromString(sDate As String, sFormat As String) As Date
    On Error Resume Next
    Dim nYear, nMonth, nDay As Integer
    Dim nPosition As Integer
    Dim dtDate As Date
    
    nPosition = InStr(1, sFormat, YEAR_FORMAT)
    If nPosition > 0 Then
        nYear = CInt(Mid(sDate, nPosition, Len(YEAR_FORMAT)))
    End If
    
    nPosition = InStr(1, sFormat, MONTH_FORMAT)
    If nPosition > 0 Then
        nMonth = CInt(Mid(sDate, nPosition, Len(MONTH_FORMAT)))
    End If
    
    nPosition = InStr(1, sFormat, DAY_FORMAT)
    If nPosition > 0 Then
        nDay = CInt(Mid(sDate, nPosition, Len(DAY_FORMAT)))
    End If
    
    If nYear > 0 And nMonth > 0 And nDay > 0 Then
        dtDate = DateSerial(nYear, nMonth, nDay)
    Else
        dtDate = Now
    End If
    GetDateFromString = dtDate
End Function

Private Function GetStorageMaskFromFileMask(sFileName As String) As String
    On Error Resume Next
    Dim sFileMask As String
    
    sFileMask = sFileName
    sFileMask = Replace(sFileMask, YEAR_FORMAT, "????")
    sFileMask = Replace(sFileMask, MONTH_FORMAT, "??")
    sFileMask = Replace(sFileMask, DAY_FORMAT, "??")
    
    GetStorageMaskFromFileMask = sFileMask
End Function

Private Function DecryptFile(ByVal sIn$, sOut$) As Boolean
    On Error Resume Next
    Dim sAppName$, sParams$, sOutFile$
    Dim sUserName$, sUserGroup$
    Dim sPass$
    
    sUserName = CurrentNtUserName
    sUserGroup = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\Users\" & sUserName, "UserGroup", "")
    sPass = g_aMainXMLParams.GetStringValue(PRODUCT_XML_KEY & "Asp\PGP", sUserGroup, "--")
    If sPass = "--" Then
        DecryptFile = True
        Exit Function
    End If
    
    sAppName = GetStrKeyValueEx("SOFTWARE\GNU\GNUPG", "gpgProgram")
    If sAppName = "" Then sAppName = "gpg.exe"
    
    Dim hPassFile&
    sOutFile = sIn & ".dec"
    hPassFile = CreatePassFile(sPass, GetDir(sAppName) & "pgpass.dat")
    If hPassFile <> INVALID_HANDLE_VALUE Then
        sParams = " -o """ & sOutFile & """ --passphrase-fd " & hPassFile & " --batch --decrypt """ & sIn & """"
    Else
        sParams = " -o """ & sOutFile & """ --batch --decrypt """ & sIn & """"
    End If
    
    Kill (sOutFile)
    Dim si As STARTUPINFO
    Dim pi As PROCESS_INFORMATION
    Dim sa As SECURITY_ATTRIBUTES
    
    sa.nLength = Len(si)
    sa.bInheritHandle = True
    sa.lpSecurityDescriptor = 0
    
    If CreateProcess(sAppName, sParams, sa, ByVal 0&, True, CREATE_NO_WINDOW, ByVal 0&, GetDir(sAppName), si, pi) = False Then
        LogMsg ("Fail to execute decrypting process.")
        CloseHandle hPassFile
        DecryptFile = False
        Exit Function
    End If
    
    WaitForSingleObject pi.hProcess, 300000
    CloseHandle pi.hProcess
    CloseHandle pi.hThread
    CloseHandle hPassFile
    
    sOut = Dir(sOutFile)
    If sOut = "" Then
        LogMsg ("Error during decrypting file.")
        DecryptFile = False
        Exit Function
    End If
     
    sOut = GetDir(sOutFile) & sOut
    LogMsg ("Decrypting file.")
    DecryptFile = True
End Function

Private Function GetDir(ByVal sFullName$) As String
    On Error Resume Next
    Dim sDir$
    Dim nPos&
    
    nPos = InStrRev(sFullName, "\")
    sDir = Mid(sFullName, 1, nPos)
    GetDir = sDir
End Function

Private Function CreatePassFile(ByVal sPass$, ByVal sName$) As Long
    On Error Resume Next
    
    Dim nFile&
    nFile = FreeFile
    Open sName For Binary Access Write As #nFile
    Put #nFile, , sPass
    Close #nFile
    
    Dim hFile&
    hFile = INVALID_HANDLE_VALUE
    
    Dim si As SECURITY_ATTRIBUTES
    si.nLength = Len(si)
    si.bInheritHandle = True
    si.lpSecurityDescriptor = 0
    
    hFile = CreateFile(sName, GENERIC_WRITE Or GENERIC_READ, FILE_SHARE_READ Or FILE_SHARE_WRITE, si, OPEN_EXISTING, FILE_ATTRIBUTE_NOT_CONTENT_INDEXED Or FILE_FLAG_DELETE_ON_CLOSE Or FILE_ATTRIBUTE_TEMPORARY, 0)
    
    CreatePassFile = hFile
End Function

