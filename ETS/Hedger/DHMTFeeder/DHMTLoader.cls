VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DHMTLoader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "DHMTLoader"

Private WithEvents DHMTFeeder1 As DHMTFeeder
Attribute DHMTFeeder1.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean

Public Event GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double)
Public Event GotOptionPriceData(ByVal OptionSymbol As String, ByVal OptionPrice As Double)
Public Event GotReply(ByVal Message As String)
Public Event Status(ByVal Message As String)
Public Event ProgressStart()
Public Event ProgressStop()
Public Event Progress(ByVal ProgressPercent As Long)

Public rsStockData As ADODB.Recordset

Public TotalStockCount As Long
Public CurentStockCount As Long
Public CurentStockCountPercent As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set DHMTFeeder1 = New DHMTFeeder
    StocksListClear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    Set DHMTFeeder1 = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Connect(ByVal PriceProviderType As Long) As Boolean
    Connect = DHMTFeeder1.Connect(PriceProviderType)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function Disconnect() As Boolean
    Disconnect = DHMTFeeder1.Disconnect
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StocksListClear()
    Set rsStockData = New ADODB.Recordset
    With rsStockData
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "Selected", adBoolean
        .Fields.Append "StockPrice", adDouble
        .Fields.Append "LastUpdate", adDate
        .Open
    End With
End Sub
'
''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Function ValidateStockSymbol(StockSymbol As String) As Boolean
'Dim i As Long
'    i = InStr(StockSymbol, "'")
'    If i > 0 Then
'        ValidateStockSymbol = False
'        StockSymbol = Left(StockSymbol, i - 1) & """" & Mid(StockSymbol, i + 1)
'        ValidateStockSymbol = True
'    Else
'        ValidateStockSymbol = True
'    End If
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StocksListAddSymbol(ByVal StockSymbol As String)
Dim Criteria As String
    If rsStockData Is Nothing Then
        StocksListClear
    Else
        If Not ValidateStockSymbol(StockSymbol) Then
            Exit Sub
        End If
        With rsStockData
            Criteria = "StockSymbol='" & StockSymbol & "'"
            If .RecordCount > 0 Then
                .MoveFirst
            End If
            .Find Criteria, 0, adSearchForward, adBookmarkFirst
            If .EOF Then
                .AddNew
                !StockSymbol = StockSymbol
            End If
            !Selected = True
            .Update
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksPriceFromMT() As Boolean
Dim StockSymbol As String
Dim StockPrice As Double
    If Not rsStockData Is Nothing Then
        TaskStarted
        RaiseEvent ProgressStart
        CurentStockCount = 0
        CurentStockCountPercent = 0
        With rsStockData
            TotalStockCount = .RecordCount
            If .RecordCount > 0 Then
                .Sort = "StockSymbol"
                .MoveFirst
                Do While Not .EOF
                    StockSymbol = !StockSymbol
                    If !Selected Then
                        'RaiseEvent Status("Loading Stock Price from Price Provider for " & StockSymbol)
                        If DHMTFeeder1.GetMytrackStockPrice(StockSymbol, StockPrice) Then
                            !StockPrice = StockPrice
                            !LastUpdate = Now
                            .Update
                        End If
                    End If
                    .MoveNext
                    DoEvents
                    If CancelRequested Then
                        CancelAccepted = True
                        Exit Do
                    End If
                Loop
            End If
        End With
        Dim Msg As String
        Msg = "Loading Stocks Price From Price Provider: " & _
            Format$(CurentStockCountPercent) & "% data points received (" & _
            Format$(CurentStockCount) & " from " & _
            Format$(TotalStockCount) & " expected)."
        RaiseEvent Status(Msg)
        RaiseEvent ProgressStop
        LoadStocksPriceFromMT = TaskFinished
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTFeeder1_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double)
Dim Message As String
    On Error GoTo Herr
    Message = "GotStockPriceData " & StockSymbol & " " & Format$(StockPrice)
    RaiseEvent GotReply(Message)
    If Not ValidateStockSymbol(StockSymbol) Then
        RaiseEvent Status("GotStockPriceData Error: Invalid Stock Symbol " & StockSymbol)
        Exit Sub
    End If
    If StockPrice <= 0 Then
        RaiseEvent Status("GotStockPriceData Error: Invalid Stock Price " & Format$(StockPrice))
        Exit Sub
    End If
    RaiseEvent GotStockPriceData(StockSymbol, StockPrice)
    On Error Resume Next
    Dim i As Long
    CurentStockCount = CurentStockCount + 1
    i = 100 * CurentStockCount / TotalStockCount
    If i <> CurentStockCountPercent Then
        CurentStockCountPercent = i
'        RaiseEvent Status("Loading Stock Price From iVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    Exit Sub
Herr:
    RaiseEvent Status("GotStockPriceData Error: " & Err.Description)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTFeeder1_GotOptionPriceData(ByVal OptionSymbol As String, ByVal OptionPrice As Double)
Dim Message As String
    On Error GoTo Herr
    Message = "GotOptionPriceData " & OptionSymbol & " " & Format$(OptionPrice)
    RaiseEvent GotReply(Message)
    RaiseEvent GotOptionPriceData(OptionSymbol, OptionPrice)
    Exit Sub
Herr:
    RaiseEvent Status("GotOptionPriceData Error: " & Err.Description)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTFeeder1_Status(ByVal Message As String)
    On Error Resume Next
    RaiseEvent Status(Message)
End Sub

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
    DHMTFeeder1.CancelTask
End Function

