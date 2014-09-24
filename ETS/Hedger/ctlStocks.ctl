VERSION 5.00
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.UserControl ctlStocks 
   BackColor       =   &H00996666&
   ClientHeight    =   6285
   ClientLeft      =   0
   ClientTop       =   0
   ClientWidth     =   9645
   ScaleHeight     =   6285
   ScaleWidth      =   9645
   Begin VB.PictureBox picLeft 
      Appearance      =   0  'Flat
      BackColor       =   &H00999999&
      BorderStyle     =   0  'None
      ForeColor       =   &H80000008&
      Height          =   6075
      Left            =   0
      ScaleHeight     =   6075
      ScaleWidth      =   1620
      TabIndex        =   0
      Top             =   0
      Width           =   1620
      Begin VB.CheckBox Check1 
         Appearance      =   0  'Flat
         BackColor       =   &H00999999&
         Caption         =   "All stocks"
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00E0FFFF&
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   6
         ToolTipText     =   "Show stocks: all / current portfolio "
         Top             =   600
         Width           =   1335
      End
      Begin ElladaFlatControls.ActiveLabel btn 
         Height          =   315
         Index           =   1
         Left            =   120
         TabIndex        =   1
         Tag             =   "red"
         ToolTipText     =   "Start volatility and correlations data feed for Indexes and Stocks"
         Top             =   3840
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
         TabIndex        =   2
         Tag             =   "red"
         ToolTipText     =   "Start price feed for Indexes and Stocks"
         Top             =   4440
         Visible         =   0   'False
         Width           =   1275
         _ExtentX        =   2249
         _ExtentY        =   556
         BackColor       =   10066329
         ForeColor       =   14745599
         Enabled         =   0   'False
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
      Begin VB.Label Label1 
         BackStyle       =   0  'Transparent
         Caption         =   "Stocks:"
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
         Index           =   1
         Left            =   120
         TabIndex        =   7
         Top             =   240
         Width           =   1335
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
         TabIndex        =   3
         Top             =   3600
         Width           =   1335
      End
      Begin VB.Line Line1 
         BorderColor     =   &H00B0D0D0&
         Index           =   0
         X1              =   120
         X2              =   1440
         Y1              =   3480
         Y2              =   3480
      End
   End
   Begin EgarHedgerSQL.ctlIndexesData ctlIndexesData1 
      Height          =   1695
      Left            =   1680
      TabIndex        =   4
      Top             =   120
      Width           =   7215
      _ExtentX        =   12726
      _ExtentY        =   2990
   End
   Begin EgarHedgerSQL.ctlStocksData ctlStocksData1 
      Height          =   3495
      Left            =   1680
      TabIndex        =   5
      Top             =   1920
      Width           =   7215
      _ExtentX        =   12726
      _ExtentY        =   6165
   End
End
Attribute VB_Name = "ctlStocks"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const Border = 40
Public fVisible As Boolean

Private AutoStart As Boolean
Private WithEvents PortfolioHedge As PortfolioHedger
Attribute PortfolioHedge.VB_VarHelpID = -1
Private Cancel As Boolean
Private pStocksFilter As enmStocksFilter
Public NeedRefresh As Boolean

Private Enum enmBTN
    BTN_LOAD_PRICES = 0
    BTN_LOAD_IVDATA
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Initialize()
Dim X As Single
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    BackColor = GCOLOR_TAB_5
    
    Set PortfolioHedge = gPortfolioHedge
    With picLeft
        .Top = 0
        X = .Width + Border
    End With
    With ctlIndexesData1
        .Left = X
        .Top = Border
    End With
    With ctlStocksData1
        .Left = X
        .Top = Border + ctlIndexesData1.Top + ctlIndexesData1.Height
    End With
    pStocksFilter = STOCKS_PORTFOLIO
    NeedRefresh = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Resize()
Dim X As Single
    On Error Resume Next
    picLeft.Height = Max(0, ScaleHeight)
    X = ScaleWidth - picLeft.Width - 2 * Border
    With ctlIndexesData1
        .Width = Max(0, X)
    End With
    With ctlStocksData1
        .Width = Max(0, X)
        .Height = Max(0, ScaleHeight - .Top - Border)
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub UserControl_Terminate()
    On Error Resume Next
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
Private Sub PortfolioHedge_GotStockIndexCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockIndexCorrData StockSymbol, IndexSymbol, Corr, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStockIndexVCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double, ByVal DataStatus As enmDATA_STATUS)
    On Error Resume Next
    ctlStocksData1.SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr, DataStatus
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    EnableControls False
    Cancel = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotStopTask(ByVal Caption As String)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    EnableControls True
    Cancel = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_GotCancelTask()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    CancelTask
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EnableControls(ByVal Enable As Boolean)
Dim i As Long
    On Error Resume Next
    With btn
        For i = .LBound To .UBound
            .Item(i).Enabled = Enable
        Next
    End With
    With Check1
        For i = .LBound To .UBound
            .Item(i).Enabled = Enable
        Next
    End With
    ctlIndexesData1.Editable = Enable
    ctlStocksData1.Editable = Enable
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
    On Error Resume Next
    Cancel = False
    PortfolioHedge.StartTask Caption, True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub StopTask(Optional ByVal Caption As String = "")
    On Error Resume Next
    PortfolioHedge.StopTask Caption
    Cancel = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub CancelTask()
    On Error Resume Next
    Cancel = True
    ctlStocksData1.Cancel = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btn_Click(Index As Integer)
    On Error Resume Next
    PortfolioHedge.NeedSplitPortfolio = True
    Select Case Index
    Case BTN_LOAD_PRICES
        LoadPrices
    Case BTN_LOAD_IVDATA
        LoadIVData
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadPrices()
    On Error GoTo Herr
    Cancel = False
    If Not frmMain.LoadIndexPrices Then
        CancelTask
    End If
    If Not Cancel Then
        If pStocksFilter = STOCKS_SUPPORTED Then
            frmMain.LoadStockPrices True
        Else
            frmMain.LoadStockPrices False
        End If
    End If
    Exit Sub
Herr:
    LastErrDescription = Err.Description
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadIVData()
    On Error GoTo Herr
    Cancel = False
    If pStocksFilter = STOCKS_SUPPORTED Then
        frmMain.LoadStockIVData True
    Else
        frmMain.LoadStockIVData False
    End If
    Exit Sub
Herr:
    LastErrDescription = Err.Description
    ShowError
End Sub

'///////////////////////////////////////////////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(Optional ByVal StocksFilter As enmStocksFilter = STOCKS_SUPPORTED) As VbMsgBoxResult
    On Error Resume Next
    pStocksFilter = StocksFilter
    If StocksFilter = STOCKS_SUPPORTED Then
        'Caption = "Supported Stocks"
    Else
        'Caption = "Selected Stocks"
    End If
    ReadDataTask
    Execute = vbOK
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Clear()
    On Error Resume Next
    If pStocksFilter = STOCKS_SUPPORTED Then
        ' Don't clear
    Else
        ctlIndexesData1.Clear
        ctlStocksData1.Clear
        NeedRefresh = True
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Refresh()
    If NeedRefresh Then
        pStocksFilter = STOCKS_PORTFOLIO
        Check1(0).Value = 0
        ReadDataTask
        NeedRefresh = False
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ReadDataTask()
    On Error Resume Next
    StartTask "Preparing Stocks list...", False
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
Private Sub Check1_Click(Index As Integer)
    On Error Resume Next
    If Index = 0 Then
        If Check1(0).Value = 0 Then
            Execute STOCKS_PORTFOLIO
        Else
            Execute STOCKS_SUPPORTED
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_ClearResults()
    On Error Resume Next
    Clear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub PortfolioHedge_NewPortfolio(rs As ADODB.Recordset)
    On Error Resume Next
    If fVisible Then
        Refresh
    Else
        NeedRefresh = True
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetGrid(ByVal Index As Integer) As VSFlexGrid
    On Error Resume Next
    If Index = 0 Then
        Set GetGrid = ctlIndexesData1.GetGrid
    Else
        Set GetGrid = ctlStocksData1.GetGrid
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy the grid content to the clipboard
'
Public Function CopyToClipboard(ByVal All As Boolean) As Boolean
    On Error GoTo Herr
    CopyToClipboard = ctlStocksData1.CopyToClipboard(All)
    Exit Function
Herr:
    ShowError "Copy the grid content to the clipboard failed due to the following error:"
End Function

