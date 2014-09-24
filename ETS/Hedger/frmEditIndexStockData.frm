VERSION 5.00
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmEditIndexStockData 
   Appearance      =   0  'Flat
   BackColor       =   &H00008000&
   Caption         =   "Stocks/Index Data"
   ClientHeight    =   6990
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   10200
   Icon            =   "frmEditIndexStockData.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   6990
   ScaleWidth      =   10200
   StartUpPosition =   2  'CenterScreen
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      Height          =   255
      Left            =   0
      TabIndex        =   9
      Top             =   6735
      Width           =   10200
      _ExtentX        =   17992
      _ExtentY        =   450
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin VB.PictureBox picLeft 
      Appearance      =   0  'Flat
      BackColor       =   &H00999999&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   5355
      Left            =   0
      ScaleHeight     =   5355
      ScaleWidth      =   1620
      TabIndex        =   4
      Top             =   720
      Width           =   1620
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   5
         Tag             =   "red"
         ToolTipText     =   "Start volatility and correlations data feed for Indexes and Stocks"
         Top             =   3720
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8454016
         Caption         =   "Get IV data"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   0
         Left            =   120
         TabIndex        =   6
         Tag             =   "red"
         ToolTipText     =   "Start price feed for Indexes and Stocks"
         Top             =   3360
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8454016
         Caption         =   "Get prices"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   2
         Left            =   120
         TabIndex        =   7
         Tag             =   "red"
         ToolTipText     =   "Close window"
         Top             =   4680
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         HoverForeColor  =   8454016
         Caption         =   "Close"
         PressForeColor  =   8454016
         DisabledForeColor=   12632256
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   0
         X1              =   120
         X2              =   1440
         Y1              =   3000
         Y2              =   3000
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   1
         X1              =   120
         X2              =   1440
         Y1              =   4440
         Y2              =   4440
      End
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Import data:"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00000000&
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   8
         Top             =   3120
         Width           =   1335
      End
   End
   Begin EgarHedger.ctlTopPanel2 ctlTopPanel21 
      Height          =   765
      Left            =   0
      TabIndex        =   3
      Top             =   0
      Width           =   9015
      _ExtentX        =   15901
      _ExtentY        =   1349
   End
   Begin EgarHedger.ctlIndexesData ctlIndexesData1 
      Height          =   1695
      Left            =   1680
      TabIndex        =   2
      Top             =   840
      Width           =   7215
      _ExtentX        =   12726
      _ExtentY        =   2990
   End
   Begin EgarHedger.ctlStocksData ctlStocksData1 
      Height          =   3495
      Left            =   1680
      TabIndex        =   0
      Top             =   2640
      Width           =   7215
      _ExtentX        =   12726
      _ExtentY        =   6165
   End
   Begin VB.PictureBox Picture1 
      Height          =   0
      Left            =   0
      ScaleHeight     =   0
      ScaleWidth      =   0
      TabIndex        =   1
      Top             =   0
      Width           =   0
   End
End
Attribute VB_Name = "frmEditIndexStockData"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const Border = 40

Private AutoStart As Boolean
Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1
Private Cancel As Boolean
Private pStocksFilter As enmStocksFilter

Private Enum enmBTN
    BTN_CLOSE = 0
    BTN_LOAD_PRICES
    BTN_LOAD_IVDATA
    BTN_REFRESH
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(Optional ByVal StocksFilter As enmStocksFilter = STOCKS_SUPPORTED) As VbMsgBoxResult
    On Error Resume Next
    Load Me
    pStocksFilter = StocksFilter
    If StocksFilter = STOCKS_SUPPORTED Then
        Me.Caption = "Supported Stocks"
    End If
    AutoStart = False
'    Show vbModal, frmMain
    Show , frmMain
    DoEvents
    ReadDataTask
    
    Execute = vbOK
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btn_Click(Index As Integer)
    On Error Resume Next
    Select Case Index
    Case 0
        StartTask "Loading Prices...", True
        If Not frmMain.LoadIndexPrices Then
            Cancel = True
        End If
        If Not Cancel Then
            If pStocksFilter = STOCKS_SUPPORTED Then
                frmMain.LoadStockPrices True
            Else
                frmMain.LoadStockPrices False
            End If
        End If
        StopTask
    Case 1
        StartTask "Loading IV data...", True
        If pStocksFilter = STOCKS_SUPPORTED Then
            frmMain.LoadStockIVData True
        Else
            frmMain.LoadStockIVData False
        End If
        StopTask
    Case 2
        Unload Me
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlTopPanel21_ButtonClick(ByVal Index As BTN_TOP_PANEL2)
    On Error Resume Next
    Select Case Index
    Case BTN_TOP_PANEL2_CLOSE
        Unload Me
    Case BTN_TOP_PANEL2_LOAD_PRICES
        StartTask "Loading Prices...", True
        frmMain.LoadIndexPrices
        If Not Cancel Then
            If pStocksFilter = STOCKS_SUPPORTED Then
                frmMain.LoadStockPrices True
            Else
                frmMain.LoadStockPrices False
            End If
        End If
        StopTask
    Case BTN_TOP_PANEL2_LOAD_IV
        StartTask "Loading IV data...", True
        If pStocksFilter = STOCKS_SUPPORTED Then
            frmMain.LoadStockIVData True
        Else
            frmMain.LoadStockIVData False
        End If
        StopTask
    Case BTN_TOP_PANEL2_REFRESH
        ReadDataTask
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ctlTopPanel21_CancelClick()
    CancelTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Activate()
    On Error GoTo Herr
    If AutoStart Then
        AutoStart = False
        ReadDataTask
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ReadDataTask()
    On Error Resume Next
    StartTask "Preparing all Stocks list...", True
    ReadData
    StopTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ReadData()
    On Error GoTo Herr
    ctlIndexesData1.DisplayIndexData
    ctlStocksData1.DisplayStockData pStocksFilter
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Initialize()
    On Error Resume Next
    Set PortfolioHedge = gPortfolioHedge
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Terminate()
    On Error Resume Next
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Load()
Dim X As Single
    On Error GoTo Herr
'    With New clsCommon
'        .CustomizeForm Me
'    End With
    With picLeft
        .Top = ctlTopPanel21.Height
        X = .Width + Border
    End With
    With ctlIndexesData1
        .Left = X
        .Top = Border + ctlTopPanel21.Height
    End With
    With ctlStocksData1
        .Left = X
        .Top = Border + ctlIndexesData1.Top + ctlIndexesData1.Height
    End With
    AutoStart = False
    Exit Sub
Herr:
    ShowError
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub Form_QueryUnload(Cancel As Integer, UnloadMode As Integer)
'    If UnloadMode = vbFormControlMenu Then
'        Cancel = ctlTopPanel21.Busy
'    End If
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Resize()
Dim X As Single
    On Error Resume Next
    With ctlTopPanel21
        .Width = Max(0, Me.ScaleWidth)
    End With
    picLeft.Height = Max(0, ScaleHeight - ctlTopPanel21.Height)
    X = Me.ScaleWidth - picLeft.Width - 2 * Border
    With ctlIndexesData1
        .Width = Max(0, X)
    End With
    With ctlStocksData1
        .Width = Max(0, X)
        .Height = Max(0, ScaleHeight - .Top - Border - StatusBar1.Height)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    CancelTask
    Set PortfolioHedge = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexHVData(ByVal IndexSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexesData1.SetIndexHVData IndexSymbol, HV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexIVData(ByVal IndexSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexesData1.SetIndexIVData IndexSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexHVData3(ByVal IndexSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    If HV <> 0 Then
        ctlIndexesData1.SetIndexHVData IndexSymbol, HV, DataStatus
    End If
    If IV <> 0 Then
        ctlIndexesData1.SetIndexIVData IndexSymbol, IV, DataStatus
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexVega(ByVal IndexSymbol As String, ByVal Vega As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    If Vega <> 0 Then
        ctlIndexesData1.SetIndexVegaData IndexSymbol, Vega, DataStatus
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotIndexPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlIndexesData1.SetIndexPriceData StockSymbol, StockPrice, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockHVData StockSymbol, HV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockIVData StockSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockHVData3(ByVal StockSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockHVData StockSymbol, HV, DataStatus
    ctlStocksData1.SetStockIVData StockSymbol, IV, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockPriceData StockSymbol, StockPrice, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'

Private Sub PortfolioHedge_GotStockIndexCorrData3(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, VCorr, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_Status(ByVal Message As String)
    On Error Resume Next
    SetStatus Message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    On Error Resume Next
    SetStatus Caption
    Cancel = False
    btn(0).Enabled = False
    btn(1).Enabled = False
    btn(2).Enabled = False
    ctlTopPanel21.StartTask Caption, CanCancel
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    On Error Resume Next
    ctlTopPanel21.StopTask Caption
    btn(0).Enabled = True
    btn(1).Enabled = True
    btn(2).Enabled = True
    SetStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
    On Error Resume Next
    PortfolioHedge.StartTask Caption, True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
    On Error Resume Next
    PortfolioHedge.StopTask Caption
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CancelTask()
    On Error GoTo Herr
    Cancel = True
    ctlStocksData1.Cancel = True
    If Not PortfolioHedge Is Nothing Then
        PortfolioHedge.CancelTask
    End If
    StopTask
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStatus(Optional ByVal Message As String = "")
    On Error Resume Next
    '// if simple tool bar
    Me.StatusBar1.SimpleText = Message
    '// else
    'Me.StatusBar1.Panels(0).Text = Message
End Sub

