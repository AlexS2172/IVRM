VERSION 5.00
Object = "{0D452EE1-E08F-101A-852E-02608C4D0BB4}#2.0#0"; "FM20.DLL"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "mscomctl.ocx"
Object = "{E5B04F18-C63E-465D-B0C6-E598BBF429FE}#2.0#0"; "ElladaFlatControls.ocx"
Begin VB.Form frmImportTrades 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Import Trades"
   ClientHeight    =   3570
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   4455
   Icon            =   "frmImportTrades.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   3570
   ScaleWidth      =   4455
   ShowInTaskbar   =   0   'False
   StartUpPosition =   1  'CenterOwner
   Begin VB.PictureBox picLoading 
      Appearance      =   0  'Flat
      BackColor       =   &H00E0E0E0&
      ForeColor       =   &H80000008&
      Height          =   1455
      Left            =   240
      ScaleHeight     =   1425
      ScaleWidth      =   3945
      TabIndex        =   10
      Top             =   1020
      Visible         =   0   'False
      Width           =   3975
      Begin ElladaFlatControls.FlatButton FlatButton1 
         Height          =   300
         Left            =   1560
         TabIndex        =   11
         Top             =   960
         Width           =   855
         _ExtentX        =   1508
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "&Cancel"
      End
      Begin MSComctlLib.ProgressBar pbProgress 
         Height          =   195
         Left            =   240
         TabIndex        =   13
         Top             =   120
         Visible         =   0   'False
         Width           =   3495
         _ExtentX        =   6165
         _ExtentY        =   344
         _Version        =   393216
         BorderStyle     =   1
         Appearance      =   0
         Max             =   1000
      End
      Begin VB.Label lblStatus 
         Alignment       =   2  'Center
         Appearance      =   0  'Flat
         BackColor       =   &H80000005&
         BackStyle       =   0  'Transparent
         Caption         =   "Loading.."
         BeginProperty Font 
            Name            =   "MS Sans Serif"
            Size            =   9.75
            Charset         =   204
            Weight          =   700
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         ForeColor       =   &H00FF0000&
         Height          =   375
         Left            =   240
         TabIndex        =   12
         Top             =   480
         Width           =   3495
      End
   End
   Begin VB.PictureBox Picture1 
      Appearance      =   0  'Flat
      ForeColor       =   &H80000008&
      Height          =   2835
      Left            =   120
      ScaleHeight     =   2805
      ScaleWidth      =   4215
      TabIndex        =   7
      Top             =   120
      Width           =   4245
      Begin ElladaFlatControls.FlatButton cmbNewFilter 
         Height          =   300
         Left            =   480
         TabIndex        =   1
         Top             =   1320
         Width           =   855
         _ExtentX        =   1508
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "&New"
      End
      Begin ElladaFlatControls.FlatButton cmbEditFilter 
         Height          =   300
         Left            =   1680
         TabIndex        =   2
         Top             =   1320
         Width           =   855
         _ExtentX        =   1508
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "&Edit"
      End
      Begin ElladaFlatControls.FlatButton cmbDelFilter 
         Height          =   300
         Left            =   2880
         TabIndex        =   3
         Top             =   1320
         Width           =   855
         _ExtentX        =   1508
         _ExtentY        =   529
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "MS Sans Serif"
            Size            =   8.25
            Charset         =   204
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
         Caption         =   "&Del"
      End
      Begin MSForms.ComboBox cmbFilters 
         Height          =   300
         Left            =   480
         TabIndex        =   0
         Top             =   600
         Width           =   3255
         VariousPropertyBits=   679499803
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "5741;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         Value           =   "cmbFilters"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin MSForms.ComboBox cmbStocks 
         Height          =   300
         Left            =   2280
         TabIndex        =   4
         Top             =   2320
         Width           =   1455
         VariousPropertyBits=   679499803
         BorderStyle     =   1
         DisplayStyle    =   3
         Size            =   "2566;529"
         MatchEntry      =   1
         ShowDropButtonWhen=   1
         Value           =   "cboContracts"
         SpecialEffect   =   0
         FontHeight      =   165
         FontCharSet     =   204
         FontPitchAndFamily=   2
      End
      Begin VB.Label lbl1 
         Alignment       =   2  'Center
         AutoSize        =   -1  'True
         Caption         =   "Filter"
         Height          =   255
         Left            =   480
         TabIndex        =   9
         Top             =   255
         Width           =   555
         WordWrap        =   -1  'True
      End
      Begin VB.Shape Shape1 
         Height          =   1455
         Left            =   240
         Top             =   360
         Width           =   3735
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         BackStyle       =   0  'Transparent
         Caption         =   "Stock"
         Height          =   195
         Left            =   720
         TabIndex        =   8
         Top             =   2400
         Width           =   1275
         WordWrap        =   -1  'True
      End
   End
   Begin ElladaFlatControls.FlatButton btnCancel 
      Cancel          =   -1  'True
      Height          =   300
      Left            =   3000
      TabIndex        =   6
      Top             =   3180
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&Cancel"
   End
   Begin ElladaFlatControls.FlatButton btnOk 
      Default         =   -1  'True
      Height          =   300
      Left            =   1560
      TabIndex        =   5
      Top             =   3180
      Width           =   1335
      _ExtentX        =   2355
      _ExtentY        =   529
      BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
         Name            =   "MS Sans Serif"
         Size            =   8.25
         Charset         =   204
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Caption         =   "&OK"
   End
End
Attribute VB_Name = "frmImportTrades"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents TrackDataPriceInfo1 As PRICEPROVIDERSLib.PriceInfo
Attribute TrackDataPriceInfo1.VB_VarHelpID = -1

Private fGotAllPrices As Boolean
Private RequestCount As Long
Private fGotStockPrice As Boolean
Private fGotOptionPrice As Boolean

Private mBusy As Boolean
Private mCancel As Boolean

Private WithEvents mCN As ADODB.Connection
Attribute mCN.VB_VarHelpID = -1
Private Const TimeOut = 200 'sec
Private WaitFlag As Boolean

Private rsStockPrices As ADODB.Recordset
Private rsStockTrades As ADODB.Recordset
Private rsOptionPrices As ADODB.Recordset
Private rsOptionTrades As ADODB.Recordset

Private LastErrorNumber As Long
Private LastErrorDescription As String
Private LastErrorSource As String
Private LastStatus As Long
Private LastRecordset As ADODB.Recordset
Private IsOK As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Execute(rs As ADODB.Recordset) As Boolean
    IsOK = False
    LoadData
    Me.Show vbModal
    Execute = IsOK
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnCancel_Click()
    On Error Resume Next
    mCancel = True
    If mBusy Then
    Else
        Me.Hide
        Unload Me
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub btnOK_Click()
Dim FilterID As Long
Dim StockID As Long
    On Error GoTo Herr
     
    With cmbFilters
        If .ListIndex > 0 Then
            FilterID = .Value
        Else
            FilterID = 0
        End If
    End With
    
    With cmbStocks
        If .ListIndex > 0 Then
            StockID = .Value
        Else
            StockID = 0
        End If
    End With
    
    LoadTrades FilterID, StockID
    
    Me.Hide
    Unload Me
    
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadData()
Dim rsFilters As ADODB.Recordset
Dim rsStocks As ADODB.Recordset
Dim Symbol As String
Dim ID As Long
    
    With cmbFilters
        .Clear
        .ColumnCount = 1
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "<None>"
        .Column(1, .ListCount - 1) = 0
        .ListIndex = 0
    End With
    Set rsFilters = gDBW.usp_FilterList_Get
    If Not rsFilters Is Nothing Then
        With rsFilters
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    ID = DBRead(!iFilterID, 0)
                    Symbol = DBRead(!vcFilterName, "")
                    If ID <> 0 And Len(Symbol) > 0 Then
                        With cmbFilters
                            .AddItem Symbol
                            .Column(1, .ListCount - 1) = ID
                        End With
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If
    
    With cmbStocks
        .Clear
        .ColumnCount = 1
        .TextColumn = 1
        .BoundColumn = 2
        .AddItem "<None>"
        .Column(1, .ListCount - 1) = 0
        .ListIndex = 0
    End With
    
    Set rsStocks = gDBW.usp_ContractSymbol_Get(0, True, True, True, False)
    If Not rsStocks Is Nothing Then
        With rsStocks
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    ID = DBRead(!iContractID, 0)
                    Symbol = DBRead(!vcSymbol, "")
                    If ID <> 0 And Len(Symbol) > 0 Then
                        With cmbStocks
                            .AddItem Symbol
                            .Column(1, .ListCount - 1) = ID
                        End With
                    End If
                    .MoveNext
                Loop
            End If
        End With
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LoadTrades(FilterID, StockID)
    On Error GoTo Herr
    Dim f As Boolean
    StartTask
    lblStatus = "Loading.."
    
    EnableConttols False
    picLoading.Visible = True
    Me.MousePointer = vbArrowHourglass
    
    '//////////////////////////
    f = GetPositionData(StockID, FilterID)
    '////////////////////////
    If Not f Then
        GoTo Done
    End If
    If g_ReportPricesFeed Then
        Idle
        lblStatus = "Loading prices.."
        FeedPrices
    End If

    Idle
    lblStatus = "Calculating.."
    CalculateTrades
    IsOK = True
    Idle
Done:
    StopTask
    EnableConttols True
    Exit Sub
Herr:
    ShowError
    On Error Resume Next
    StopTask
    picLoading.Visible = False
    Me.MousePointer = vbNormal
    EnableConttols True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetPositionData( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0, _
    Optional ByVal BegDate As Date = 0#, _
    Optional ByVal EndDate As Date = 0# _
) As Boolean
    
    Set mCN = CN
    Dim nTrades As Long
    
    ' Load stocks list
    Set rsStockPrices = GetRS(LoadStockPrices(StockID, FilterID))
    Idle
    
    ' Load options list
'    Set rsOptionPrices = GetRS(LoadOptionPrices(StockID, FilterID))
'    Idle
    
    ' Load stocks trades
    Set rsStockTrades = GetRS(LoadStockTrades(StockID, FilterID))
    Idle
    
    ' Load options trades
    Set rsOptionTrades = GetRS(LoadOptionTrades(StockID, FilterID))
    Idle
    
    nTrades = rsOptionTrades.RecordCount + rsStockTrades.RecordCount
    If nTrades = 0 Then
        LogEvent EVENT_WARNING, "Zero trades count in the Import Trades data"
        Exit Function
    End If
    LogEvent EVENT_INFO, "Trades imported: " & Format$(nTrades)
    GetPositionData = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetRS(cm As ADODB.Command) As ADODB.Recordset
Dim tStart As Date
    
    LastErrorNumber = 0
    LastErrorDescription = ""
    LastStatus = 0
    Set LastRecordset = Nothing
    
    ' Flag indicating that the caller have to wait for request completion
    WaitFlag = False
    tStart = Now
    With cm
        .CommandTimeout = TimeOut  ' 30 sec by default
        ' Execute SQL request asynchronously
        .Execute , , adAsyncExecute
    End With
    
    On Error GoTo Failed
    ' Wait for the request completion
    Do
        If WaitFlag Then
            Exit Do
        End If
        If DateDiff("s", tStart, Now) > TimeOut Then
            Err.Raise vbObject, "Reports", "Request timeout expired"
        End If
        Idle
    Loop
    
    On Error GoTo 0
    If LastStatus = adStatusOK Then
        Set GetRS = DuplicateRS(LastRecordset)
        Set LastRecordset = Nothing
    Else
        Err.Raise LastErrorNumber, "Reports", LastErrorDescription
    End If
    
    Exit Function
Failed:
    Dim n, S
    n = Err.Number
    S = Err.Description
    On Error Resume Next
    cm.Cancel
    On Error GoTo 0
    Err.Raise n, "Reports", S
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Creates ADO command object retrieving
' records for Report
'
' iContractID
' vcSymbol
' fPriceBid
' fPriceAsk
' fPriceLast
' iContractTypeID
'
Public Function LoadStockPrices( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0 _
) As ADODB.Command
    Debug.Print "usp_ReportStock_Get", StockID, FilterID
    Set LoadStockPrices = New ADODB.Command
    With LoadStockPrices
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportStock_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Creates ADO command object retrieving
' records for Report
'
' iUnderlyingContractID
' vcSymbol
' dtExpiry
' fStrike
' iIsCall
' fPriceBid
' fPriceAsk
' fPriceLast
' iContractTypeID
'
Public Function LoadOptionPrices( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0 _
) As ADODB.Command
    Debug.Print "usp_ReportMarketStructure_Get", StockID, FilterID
    Set LoadOptionPrices = New ADODB.Command
    With LoadOptionPrices
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportMarketStructure_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Creates ADO command object retrieving
' records for Report
'
' iContractID
' iQuantity
' dtTradeDate
' fPrice
' iContractTypeID
'
Public Function LoadStockTrades( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0, _
    Optional ByVal BegDate As Date = 0#, _
    Optional ByVal EndDate As Date = 0# _
) As ADODB.Command
    Debug.Print "usp_ReportStockTrade_Get", StockID, FilterID
    Set LoadStockTrades = New ADODB.Command
    With LoadStockTrades
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportStockTrade_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'usp_ReportStockTradeTotal_Get
'    @iStockID int = Null,
'    @iFilterID int = Null,
'    @dtBeginDate datetime = Null,
'    @dtEndDate datetime = Null
'Output:
'    iContractID,
'    iTotalQuantity,
'    fTotalQuantityPrice,
'    iContractTypeID
'
Public Function LoadStockTradesTotal( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0, _
    Optional ByVal BegDate As Date = 0#, _
    Optional ByVal EndDate As Date = 0# _
) As ADODB.Command
    Debug.Print "usp_ReportStockTradeTotal_Get", StockID, FilterID, BegDate, EndDate
    Set LoadStockTradesTotal = New ADODB.Command
    With LoadStockTradesTotal
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportStockTradeTotal_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
            .Item(3).Value = DBWrite(BegDate, 0#)
            .Item(4).Value = DBWrite(EndDate, 0#)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Creates ADO command object retrieving
' records for Report
'
' iUnderlyingContractID
' dtExpiry
' fStrike
' iIsCall
' iQuantity
' dtTradeDate
' fPrice
' iContractTypeID
'
Public Function LoadOptionTrades( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0, _
    Optional ByVal BegDate As Date = 0#, _
    Optional ByVal EndDate As Date = 0# _
) As ADODB.Command
    Debug.Print "usp_ReportTrade_Get", StockID, FilterID
    Set LoadOptionTrades = New ADODB.Command
    With LoadOptionTrades
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportTrade_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'usp_ReportTradeTotal_Get
'    @iStockID int = Null,
'    @iFilterID int = Null,
'    @dtBeginDate datetime = Null,
'    @dtEndDate datetime = Null
'
'Output:
'    iUnderlyingContractID,
'    dtExpiry,
'    fStrike,
'    iTotalCallQuantity,
'    iTotalPutQuantity,
'    fTotalCallQuantityPrice,
'    fTotalPutQuantityPrice,
'    iContractTypeID
'
Public Function LoadOptionTradesTotal( _
    Optional ByVal StockID As Long = 0, _
    Optional ByVal FilterID As Long = 0, _
    Optional ByVal BegDate As Date = 0#, _
    Optional ByVal EndDate As Date = 0# _
) As ADODB.Command
    Debug.Print "usp_ReportTradeTotal_Get", StockID, FilterID, BegDate, EndDate
    Set LoadOptionTradesTotal = New ADODB.Command
    With LoadOptionTradesTotal
        Set .ActiveConnection = mCN
        .CommandText = "usp_ReportTradeTotal_Get"
        .CommandType = adCmdStoredProc
        With .Parameters
            .Refresh
            .Item(1).Value = DBWrite(StockID, 0)
            .Item(2).Value = DBWrite(FilterID, 0)
            .Item(3).Value = DBWrite(BegDate, 0#)
            .Item(4).Value = DBWrite(EndDate, 0#)
        End With
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbEditFilter_Click()
Dim FilterID As Long
Dim FilterName As String
    On Error GoTo Herr
    With cmbFilters
        If .ListIndex > 0 Then
            FilterID = .Value
            If FilterID <> 0 Then
                If frmEntFilter.ExecuteEdit(FilterID, FilterName) Then
                    If FilterID <> 0 And Len(FilterName) > 0 Then
                        .List(.ListIndex) = FilterName
                    End If
                End If
            End If
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbNewFilter_Click()
Dim FilterID As Long
Dim FilterName As String
    On Error GoTo Herr
    FilterID = 0
    FilterName = ""
    If frmEntFilter.ExecuteNew(FilterID, FilterName) Then
        If FilterID <> 0 And Len(FilterName) > 0 Then
            With cmbFilters
                .AddItem FilterName
                .Column(1, .ListCount - 1) = FilterID
                .ListIndex = .ListCount - 1
            End With
        End If
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbDelFilter_Click()
Dim FilterID As Long
    On Error GoTo Herr
    With cmbFilters
        If .ListIndex > 0 Then
            FilterID = .Value
            If gDBW.usp_Filter_Del(FilterID) Then
                .RemoveItem .ListIndex
                .ListIndex = 0
            End If
        End If
    End With
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub EnableConttols(ByVal Enable As Boolean)
    On Error Resume Next
    cmbFilters.Enabled = Enable
    cmbStocks.Enabled = Enable
    btnOk.Enabled = Enable
    btnCancel.Enabled = Enable
    cmbNewFilter.Enabled = Enable
    cmbEditFilter.Enabled = Enable
    cmbDelFilter.Enabled = Enable
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub cmbStocks_GotFocus()
    On Error Resume Next
    gCmn.SelectText cmbStocks
End Sub

Private Sub Form_Load()
    frmMain.Enabled = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub mCN_ExecuteComplete(ByVal RecordsAffected As Long, ByVal pError As ADODB.Error, adStatus As ADODB.EventStatusEnum, ByVal pCommand As ADODB.Command, ByVal pRecordset As ADODB.Recordset, ByVal pConnection As ADODB.Connection)
    On Error Resume Next
    WaitFlag = True
    If Not pError Is Nothing Then
        LastErrorNumber = pError.Number
        LastErrorDescription = pError.Description
    End If
    LastStatus = adStatusOK
    Set LastRecordset = pRecordset
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub FlatButton1_Click()
    On Error Resume Next
    mCancel = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load stock symbols list
'
Private Function GetAllStocks() As ADODB.Recordset
    Set GetAllStocks = rsStockPrices.Clone
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Load options for stock symbol
'
Private Function GetStockOptions(ByVal StockID As Long) As ADODB.Recordset
    Set GetStockOptions = rsOptionTrades.Clone
    GetStockOptions.Filter = "iUnderlyingContractID=" & Format$(StockID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function CalculateTrades()
Dim rs As ADODB.Recordset
    Set rs = New ADODB.Recordset
    With rs
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "Expiration", adDate, , adFldIsNullable
            .Append "Delta", adDouble, , adFldIsNullable
            .Append "StockPrice", adDouble, , adFldIsNullable
            .Append "Vega", adDouble, , adFldIsNullable
            .Append "IV", adDouble, , adFldIsNullable
            .Append "Unsupported", adBoolean
        End With
        .Open
    End With

Dim StockID As Long
Dim StockSymbol As String
Dim StockPrice As Double
Dim StockDivDate As Date
Dim StockDivAmt As Double
Dim StockDivFreq As Long

Dim Expiry As Date
Dim Strike As Double
Dim IsCall As Boolean

Dim Quantity As Double
Dim Delta As Double
Dim Vega As Double
Dim ContractSize As Long
Dim ContractType As ContractTypes
Dim fNoVega As Boolean
Dim fNoDelta As Boolean
Dim StockIV As Double
    
    fNoVega = True
    fNoDelta = True

    LoadInterestRates
    With rsStockPrices
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                
                StockID = DBRead(!iContractID, 0)
                StockSymbol = DBRead(!vcSymbol, "")
                StockPrice = DBRead(GetPrice(!fPriceBid, !fPriceAsk, !fPriceClose), 0)
                ContractType = DBRead(!iContractTypeID, 0)
                StockDivDate = DBRead(!dtDivDate, 0)
                StockDivFreq = DBRead(!iDivFreq, 0)
                StockDivAmt = DBRead(!fDivAmt, 0)
                StockIV = gPortfolioHedge.GetStockIV(StockSymbol)
                 '!!!
                Dim IsHTB As Boolean
                Dim StockPosition As Long
                IsHTB = 0 <> DBRead(!bIsHTB, 0)
                StockPosition = 0
                
                Quantity = 0
                With rsStockTrades
                    .Filter = "iContractID=" & Format$(StockID)
                    If .RecordCount <> 0 Then
                        .MoveFirst
                        Do While Not .EOF
                            Quantity = Quantity + DBRead(!iQuantity, 0)
                            Idle
                            .MoveNext
                        Loop
                    End If
                End With
                StockPosition = Quantity * StockPrice
                
                If Quantity <> 0 Then
                    With rs
                        .AddNew
                        !StockSymbol = StockSymbol
                        !Delta = Quantity
                        !StockPrice = StockPrice
                        !Vega = 0
                        !IV = StockIV
                        !Unsupported = False
                        .Update
                        If fNoDelta And Quantity <> 0 Then
                            fNoDelta = False
                        End If
                    End With
                End If
                
                Dim ExpiryTag As Date
                ExpiryTag = 0
                With rsOptionTrades
                    .Filter = "iUnderlyingContractID=" & Format$(StockID)
                    If .RecordCount <> 0 Then
                        '///////////////////
                        SetStockData StockSymbol, ContractType, StockPrice, StockDivFreq, StockDivAmt, StockDivDate, IsHTB, StockPosition
                        '///////////////////
                        
                        .Sort = "dtExpiry"
                        .MoveFirst
                        Do While Not .EOF
                            'ContractSize = DBRead(!iLotSize, 100)
                            'If ContractSize = 0 Then ContractSize = 100
                            ContractSize = 100
                            Quantity = DBRead(!iQuantity, 0) * ContractSize
                            If Quantity <> 0 Then
                                Expiry = DBRead(!dtExpiry, 0#)
                                
                                IsCall = DBRead(!iIsCall, 0) <> 0
                                Strike = DBRead(!fStrike, 0)
                                
                                Delta = 0
                                Vega = 0
                                
                                If Date > Expiry Then GoTo Skip
#If SIMULATE Then
                                Delta = 1
                                Vega = 1
#Else
                                With m_Greeks
                                    .nMask = GT_ALL
                                    .dDelta = 0
                                    .dVega = 0
                                End With
                                If GetGreeks(Strike, Expiry, IsCall) Then
                                    With m_Greeks
                                        Delta = .dDelta
                                        Vega = .dVega
                                    End With
                                End If
#End If
                                Delta = Delta * Quantity
                                Vega = Vega * Quantity
                                
                                With rs
                                    If Expiry <> ExpiryTag Then
                                        .AddNew
                                        !StockSymbol = StockSymbol
                                        !Expiration = Expiry 'NormalExpiry(Expiry)
                                        !StockPrice = StockPrice
                                        !IV = StockIV
                                        !Unsupported = False
                                        !Delta = Delta
                                        !Vega = Vega
                                        .Update
                                        ExpiryTag = Expiry
                                    Else
                                        !Delta = !Delta + Delta
                                        !Vega = !Vega + Vega
                                        .Update
                                    End If
                                    If fNoDelta And Delta <> 0 Then
                                        fNoDelta = False
                                    End If
                                    If fNoVega And Vega <> 0 Then
                                        fNoVega = False
                                    End If
                                End With
Skip:
                            End If
                            
                            Idle
                            .MoveNext
                        Loop
                    End If
                End With
                
                Idle
                .MoveNext
            Loop
        End If
    End With
    
    If rs.RecordCount <> 0 Then
        LogEvent EVENT_INFO, "positions generated: " & Format$(rs.RecordCount)
    Else
        LogEvent EVENT_INFO, "No positions generated"
    End If
    gPortfolioHedge.LoadPorfolioFromTrades rs, fNoVega, fNoDelta
    
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NormalExpiry(ByVal Expiry As Date) As Date
    NormalExpiry = DateSerial(Year(Expiry), Month(Expiry), 1)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub StartTask()
    mBusy = True
    mCancel = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub StopTask()
    mBusy = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Idle()
    DoEvents
    Sleep 10
    If mBusy And mCancel Then
        Err.Raise vbObjectError, , "Operation calceled"
    End If
End Sub

'////////////////////////////////////////////////
' Real time prices
'////////////////////////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function FeedPrices()
    If gPortfolioHedge.fNeedReconnectMT Then
        LastErrDescription = "You need to restart application to connect to price provider."
        Err.Raise vbObjectError, "Price Loader", LastErrDescription
        Exit Function
    End If
    pbProgress.Visible = True
    ExecuteLoadStockPrices
    If LastErrorNumber Then
        pbProgress.Visible = False
        Err.Raise LastErrorNumber, LastErrorSource, LastErrorDescription
    End If
    pbProgress.Visible = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteLoadStockPrices() As Boolean
Dim rsStocks As ADODB.Recordset
Dim StockSymbol As String
Dim StockID As Long
Dim Exchange As String
Dim Count As Long
    On Error GoTo Herr
    pbProgress.Min = 0
    pbProgress.Max = 100
    pbProgress.Value = 0
    Set rsStocks = GetAllStocks
    If rsStocks Is Nothing Then
        Debug.Assert False
    Else
        If g_PriceProvider Is Nothing Then
            Set g_PriceProvider = New PRICEPROVIDERSLib.PriceInfo
        End If
        If TrackDataPriceInfo1 Is Nothing Then
            Set TrackDataPriceInfo1 = g_PriceProvider
        End If
'        Dim Provider1 As PRICEPROVIDERSLib.IProvider
'        Set Provider1 = TrackDataPriceInfo1
'        If Not Provider1 Is Nothing Then
'            Provider1.type = g_PriceProviderType
'        End If
'        TrackDataPriceInfo1.Connect
        fGotAllPrices = False
        RequestCount = 0
        With rsStocks
            If .RecordCount <> 0 Then
                .MoveLast
                .MoveFirst
                pbProgress.Max = .RecordCount
                .MoveFirst
                Count = 0
                Do While Not .EOF
                    Count = Count + 1
                    StockSymbol = !vcSymbol
                    StockID = DBRead(!iContractID, 0)
                    With rsStockTrades
                        .Filter = "iContractID=" & Format$(StockID)
                        If .RecordCount = 0 Then
                            With rsOptionTrades
                                .Filter = "iUnderlyingContractID=" & Format$(StockID)
                                If .RecordCount = 0 Then
                                    GoTo Skip
                                End If
                            End With
                        End If
                    End With
                    Dim IsIndex As Boolean
                    IsIndex = 1 = DBRead(!iContractTypeID, 0)
                    Exchange = "" 'DBRead(!vcExchangeCode, "")
                    ExecuteLoadStockPricesBySymbol StockSymbol, Exchange, 30, IsIndex
Skip:
                    pbProgress.Value = Count
                    Idle
                    .MoveNext
                Loop
                '/// Wait for all requests are closed //
                Do
                    If fGotAllPrices Then
                        Exit Do
                    End If
                    Idle
                Loop
            Else
                Debug.Assert False
            End If
        End With
    End If
    ExecuteLoadStockPrices = True
Done:
    On Error Resume Next
'    TrackDataPriceInfo1.Disconnect
    Exit Function
Herr:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
    GoTo Done
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function ExecuteLoadStockPricesBySymbol(ByVal StockSymbol As String, ByVal Exchange As String, ByVal TimeOut As Long, ByVal IsIndex As Boolean)
Dim QParams As PRICEPROVIDERSLib.QuoteUpdateParams
Dim t1 As Date
    On Error GoTo Herr
    With QParams
        .Exchange = Exchange
        .Symbol = StockSymbol
        If IsIndex Then
            .type = enIDX
        Else
            .type = enSTK
        End If
    End With
    fGotStockPrice = False
    t1 = Now
    RequestCount = RequestCount + 1
    TrackDataPriceInfo1.RequestLastQuote QParams
    If TimeOut > 0 Then
        '/// Wait for request is closed //
        Do
            If fGotStockPrice Then
                Exit Do
            End If
            If DateDiff("s", t1, Now) > TimeOut Then
                Err.Raise vbObjectError, "", "Request timeout expired"
            End If
            Idle
        Loop
    End If
    ExecuteLoadStockPricesBySymbol = True
    Exit Function
Herr:
    Dim Msg As String
    Msg = "Load stock price for '" & StockSymbol & "' failed: " & Err.Description
    On Error Resume Next
'    LogMsg Msg
    TrackDataPriceInfo1.CancelLastQuote
    On Error GoTo 0
    Err.Raise vbObjectError, "Reports", Msg
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub TrackDataPriceInfo1_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
    On Error Resume Next
    Dim Msg As String
    Msg = "Load close price for '" & Request.Symbol & "' failed: " & Description
    LogMsg Msg
    If ReqType = enRequestLastQuote Then
        RequestCount = RequestCount - 1
        If RequestCount <= 0 Then
            fGotAllPrices = True
        End If
        If Request.type = enSTK Then
            fGotStockPrice = True
        ElseIf Request.type = enOPT Then
            fGotOptionPrice = True
        End If
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub TrackDataPriceInfo1_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
Dim Symbol As String
Dim Exchange As String
Dim ClosePrice As Double
Dim LastPrice As Double
Dim PriceBid As Double
Dim PriceAsk As Double
Dim IsIndex As Boolean
    On Error GoTo Herr
    RequestCount = RequestCount - 1
    If RequestCount <= 0 Then
        fGotAllPrices = True
    End If
    IsIndex = False
    With Params
        Symbol = .Symbol
        Exchange = .Exchange
        Select Case .type
        Case enOPT
            GoTo SaveOptionData
        Case enIDX
            IsIndex = True
            GoTo SaveStockData
        Case enSTK
            GoTo SaveStockData
        End Select
    End With
    Exit Sub

SaveStockData:
    fGotStockPrice = True
    With Results
        PriceAsk = .AskPrice
'        .AskSize
        PriceBid = .BidPrice
'        .BidSize
        ClosePrice = .ClosePrice
'        .Currency
'        .Exchange
'        .HighPrice
        LastPrice = .LastPrice
'        .LotSize
'        .LowPrice
'        .OpenInterest
'        .OpenPrice
'        .SeqNum
'        .Skipped
'        .TotalRequests
'        .UpdateDateTime
'        .Volume
'        .WaitTime
    End With
    SaveStockPrice Symbol, Exchange, ClosePrice, PriceBid, PriceAsk, LastPrice
    Exit Sub

SaveOptionData:
    fGotOptionPrice = True
    With Results
        PriceAsk = .AskPrice
'        .AskSize
        PriceBid = .BidPrice
'        .BidSize
        ClosePrice = .ClosePrice
'        .Currency
'        .Exchange
'        .HighPrice
        LastPrice = .LastPrice
'        .LotSize
'        .LowPrice
'        .OpenInterest
'        .OpenPrice
'        .SeqNum
'        .Skipped
'        .TotalRequests
'        .UpdateDateTime
'        .Volume
'        .WaitTime
    End With
    SaveOptionPrice Symbol, Exchange, ClosePrice, PriceBid, PriceAsk, LastPrice
    Exit Sub
Herr:
    LastErrorNumber = Err.Number
    LastErrorDescription = Err.Description
    LastErrorSource = Err.Source
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveStockPrice(ByVal Symbol As String, ByVal Exchange As String, ByVal ClosePrice As Double, ByVal PriceBid As Double, ByVal PriceAsk As Double, ByVal LastPrice As Double)
Dim ContractID As Long
    If PriceBid = 0 And PriceAsk = 0 Then
        PriceBid = LastPrice
        PriceAsk = LastPrice
    End If
    With rsStockPrices.Clone
        Symbol = Replace(Symbol, "'", "''")
        .Filter = "vcSymbol='" & Symbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                ContractID = !iContractID
                !fPriceClose = DBWrite(ClosePrice, 0)
                !fPriceBid = DBWrite(PriceBid, 0)
                !fPriceAsk = DBWrite(PriceAsk, 0)
                !fPriceLast = DBWrite(LastPrice, 0)
                .Update
                ' Update trades records
                With rsStockTrades.Clone
                    .Filter = "iContractID=" & Format$(ContractID)
                    If .RecordCount <> 0 Then
                        .MoveFirst
                        Do While Not .EOF
                            !fPriceClose = DBWrite(ClosePrice, 0)
                            !fPriceBid = DBWrite(PriceBid, 0)
                            !fPriceAsk = DBWrite(PriceAsk, 0)
                            !fPriceLast = DBWrite(LastPrice, 0)
                            .Update
                            .MoveNext
                        Loop
                    End If
                End With
                .MoveNext
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function SaveOptionPrice(ByVal Symbol As String, ByVal Exchange As String, ByVal ClosePrice As Double, ByVal PriceBid As Double, ByVal PriceAsk As Double, ByVal LastPrice As Double)
Dim ContractID As Long
    If PriceBid = 0 And PriceAsk = 0 Then
        PriceBid = LastPrice
        PriceAsk = LastPrice
    End If
    Symbol = Replace(Symbol, "'", "''")
    With rsOptionPrices.Clone
        .Filter = "vcSymbol='" & Symbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                !fPriceClose = DBWrite(ClosePrice, 0)
                !fPriceBid = DBWrite(PriceBid, 0)
                !fPriceAsk = DBWrite(PriceAsk, 0)
                !fPriceLast = DBWrite(LastPrice, 0)
                .Update
                .MoveNext
            Loop
        End If
    End With
    ' Update trades records
    With rsOptionTrades.Clone
        .Filter = "vcSymbol='" & Symbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                !fPriceClose = DBWrite(ClosePrice, 0)
                !fPriceBid = DBWrite(PriceBid, 0)
                !fPriceAsk = DBWrite(PriceAsk, 0)
                !fPriceLast = DBWrite(LastPrice, 0)
                .Update
                .MoveNext
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Form_Unload(Cancel As Integer)
    On Error Resume Next
    frmMain.Enabled = True
    If Not TrackDataPriceInfo1 Is Nothing Then
        TrackDataPriceInfo1.CancelLastQuote
'        TrackDataPriceInfo1.Disconnect
        Set TrackDataPriceInfo1 = Nothing
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LogMsg(ByVal Msg As String) As String
    On Error Resume Next
    LogEvent EVENT_WARNING, Msg
End Function

