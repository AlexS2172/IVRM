VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DHMTFeeder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private WithEvents MyTracker As PRICEPROVIDERSLib.PriceInfo
Attribute MyTracker.VB_VarHelpID = -1

Public Event GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double)
Public Event GotOptionPriceData(ByVal OptionSymbol As String, ByVal OptionPrice As Double)
Public Event Status(ByVal Message As String)

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean

Private mPriceProviderType As Long
Private fConnected As Boolean

Private tStart As Date
Private fDone As Boolean
Private fSuccess As Boolean
Private fCancel As Boolean
Private Const TIMEOUT = 30 'sec
Private dPrice As Double

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set MyTracker = New PRICEPROVIDERSLib.PriceInfo
    fConnected = False
    mPriceProviderType = -1
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    On Error Resume Next
    If fConnected Then
        MyTracker.Disconnect
        fConnected = False
    End If
    Set MyTracker = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub MyTracker_OnError(ByVal ErrorNumber As PRICEPROVIDERSLib.ErrorNumberEnum, ByVal Description As String, ByVal ReqType As PRICEPROVIDERSLib.RequestsTypeEnum, ByVal Request As Variant)
Dim Msg As String
    On Error Resume Next
    If ReqType = enRequestLastQuote Then
        Msg = Request.Symbol
    End If
    RaiseEvent Status("Price Provider Error: '" & Msg & "' " & Description)
    CancelRequested = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LogError()
    RaiseEvent Status("DHMTFeeder Error: " & Err.Description)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub MyTracker_OnLastQuote(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
    On Error GoTo Herr
    Debug.Print "Mytracker_OnLastQuote", Params.Symbol, Results.LastPrice
    With Results
        dPrice = GetPrice(.BidPrice, .AskPrice, .LastPrice)
    End With
    fDone = True
    Exit Sub
Herr:
    LogError
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub MyTracker_OnQuoteUpdate(Params As PRICEPROVIDERSLib.QuoteUpdateParams, Results As PRICEPROVIDERSLib.QuoteUpdateInfo)
'    On Error GoTo Herr
'    Debug.Print "Mytracker_OnQuoteUpdate"
'    Exit Sub
'Herr:
'    LogError
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Connect(ByVal PriceProviderType As Long) As Boolean
    RaiseEvent Status("Connecting to Price Provider..")
    fConnected = False
    mPriceProviderType = PriceProviderType
    
    Dim Provider1 As PRICEPROVIDERSLib.IProvider
    Set Provider1 = MyTracker
    If Not Provider1 Is Nothing Then
        Provider1.Type = PriceProviderType
    End If
    
    MyTracker.Connect
    
    fConnected = True
    Connect = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Disconnect() As Boolean
    On Error Resume Next
    MyTracker.CancelLastQuote
    MyTracker.Disconnect
    fConnected = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetMytrackStockPrice(ByVal StockSymbol As String, StockPrice As Double) As Boolean
Dim P As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error GoTo Herr
    TaskStarted
    P.Exchange = ""
    InValidateStockSymbolMT StockSymbol
    P.Symbol = StockSymbol
    Select Case StockSymbol
    Case "NDX", "SPX", "OEX", "DJX"
        P.Type = enIDX
    Case Else
        P.Type = enSTK
    End Select
    tStart = Now
    fDone = False
    dPrice = 0
    If Not fConnected Then
'        Connect mPriceProviderType
    End If
    MyTracker.RequestLastQuote P
    Do While Not fDone
        DoEvents
        Sleep 10
        If DateDiff("s", tStart, Now) > TIMEOUT Then
           CancelRequested = True
           RaiseEvent Status("Price Provider: Request Timeout expired.")
        End If
        If CancelRequested Then
            CancelAccepted = True
            On Error Resume Next
            MyTracker.CancelLastQuote
            Exit Do
        End If
    Loop
    GetMytrackStockPrice = TaskFinished
    If GetMytrackStockPrice Then
        StockPrice = dPrice
        RaiseEvent GotStockPriceData(StockSymbol, StockPrice)
    End If
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent Status("Price Feeder: Loading Stock Price from server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Price Feeder: ", "Loading Stock Price from server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetMytrackOptionPrice(ByVal OptionSymbol As String, OptionPrice As Double) As Boolean
Dim P As PRICEPROVIDERSLib.QuoteUpdateParams
    On Error GoTo Herr
    TaskStarted
    P.Exchange = ""
    P.Symbol = OptionSymbol
    P.Type = enOPT
    tStart = Now
    fDone = False
    dPrice = 0
    If Not fConnected Then
'        Connect mPriceProviderType
    End If
    MyTracker.RequestLastQuote P
    Do While Not fDone
        DoEvents
        Sleep 10
        If DateDiff("s", tStart, Now) > TIMEOUT Then
           CancelRequested = True
        End If
        If CancelRequested Then
            CancelAccepted = True
            On Error Resume Next
            MyTracker.CancelLastQuote
            Exit Do
        End If
    Loop
    GetMytrackOptionPrice = TaskFinished
    If GetMytrackOptionPrice Then
        OptionPrice = dPrice
        RaiseEvent GotOptionPriceData(OptionSymbol, OptionPrice)
    End If
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent Status("Price  Feeder: Loading Option Price from server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Price Feeder: ", "Loading Option Price from server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskStarted() As Boolean
    TaskStarted = Not TaskInProgress
    If TaskStarted Then
        TaskInProgress = True
        CancelRequested = False
        CancelAccepted = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskFinished() As Boolean
    TaskFinished = TaskInProgress And Not CancelRequested
    TaskInProgress = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskCanceled() As Boolean
    TaskCanceled = TaskInProgress And CancelAccepted
    TaskInProgress = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Busy() As Boolean
    Busy = TaskInProgress
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CancelTask() As Boolean
    CancelTask = Busy
    CancelRequested = True
    CancelAccepted = False
    On Error Resume Next
    MyTracker.CancelLastQuote
End Function

