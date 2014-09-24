VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsIvFeed"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
Option Explicit
Option Base 0

Private WithEvents ivFeed As InetCtlsObjects.Inet
Attribute ivFeed.VB_VarHelpID = -1

Public Event CancelFeed(ByRef bCancel As Boolean)
Public Event StateChanged(ByVal sState As String)

Private Const HTTP_TIMEOUT = 15 ' sec
Private m_bCancelFeed As Boolean
Private m_sReturnedData As String
Private m_sResult() As String


Private sIvServer As String
Private sIvAccount As String
Private sIvPassword As String
Private sIvPort As String
Private sIvKey As String


Private Sub Class_Initialize()
    Set ivFeed = New InetCtlsObjects.Inet
End Sub

Private Sub Class_Terminate()
    Set ivFeed = Nothing
End Sub

Private Sub ivFeed_StateChanged(ByVal State As Integer)
    Dim vtData As Variant
    Dim strData As String
    Dim bDone As Boolean
    
    RaiseEvent StateChanged(InetState(State))
    Select Case State
        ' ... Other cases not shown.
    Case icError ' 11
        ' In case of error, return ResponseCode and
        ' ResponseInfo.
        vtData = ivFeed.ResponseCode & ":" & ivFeed.ResponseInfo
    Case icResponseCompleted ' 12
        bDone = False
        ' Get first chunk.
        vtData = ivFeed.GetChunk(1024, icString)
        DoEvents
        Do While Not bDone
            strData = strData & vtData
            ' Get next chunk.
            vtData = ivFeed.GetChunk(1024, icString)
            DoEvents
            If Len(vtData) = 0 Then
                bDone = True
            End If
        Loop
        m_sReturnedData = strData
    End Select
End Sub

Private Function HttpCall(ByVal Address As String) As String
    Dim tmStart As Date
    
    On Error GoTo EH
    'Debug.Print "Http get: http://", Address
    With ivFeed
        If .StillExecuting = True Then
            .Cancel
        End If
        'Busy = True
        m_bCancelFeed = False
        tmStart = Now
        .Execute Address, "GET"
        Do While .StillExecuting
            RaiseEvent CancelFeed(m_bCancelFeed)
            If m_bCancelFeed Then
                .Cancel
                Exit Do
            End If
            DoEvents
            If DateDiff("s", tmStart, Now) > HTTP_TIMEOUT Then
                ' Timeout condition detected
                .Cancel
                Exit Do
            End If
        Loop
    End With
    If m_bCancelFeed Then
        HttpCall = ""
    Else
        HttpCall = m_sReturnedData
    End If
    'Debug.Print "Replay  :", "[" & HttpCall & "]"
    Exit Function
EH:
    Debug.Print "Http error:", Err.Number, Err.Description
End Function

Private Function InetState(ByVal State As Integer) As String
    Select Case State
    Case icNone '0
        InetState = ""
    Case icResolvingHost '1
        InetState = "Resolving Host"
    Case icHostResolved '2
        InetState = "Host Resolved"
    Case icConnecting '3
        InetState = "Connecting"
    Case icConnected '4
        InetState = "Connected"
    Case icRequesting '5
        InetState = "Requesting"
    Case icRequestSent '6
        InetState = "Request Sent"
    Case icReceivingResponse '7
        InetState = "Receiving Response"
    Case icResponseReceived '8
        InetState = "Response Received"
    Case icDisconnecting '9
        InetState = "Disconnecting"
    Case icDisconnected ' 10
        InetState = "Disconnected"
    Case icError '11
        InetState = "Error"
    Case icResponseCompleted '12
        InetState = "ResponseCompleted"
    Case Else
        InetState = Format$(State)
    End Select
End Function

Private Function ConnectToIV() As Boolean
    Dim sSessionKey As String
    sIvKey = ""
    sSessionKey = GetAuthKey(sIvAccount)
    If Trim(sSessionKey) = "" Or Len(sSessionKey) > 12 Then
        ConnectToIV = False
    Else
        sIvKey = GetMd5Key(sSessionKey & sIvPassword)
        ConnectToIV = True
    End If
End Function

Public Function IVInit() As Boolean
    IVInit = False
    
    sIvServer = GetStrKeyValueEx(APP_NAME, "IV", "Address")
    sIvAccount = GetStrKeyValueEx(APP_NAME, "IV", "Login")
    sIvPassword = GetStrKeyValueEx(APP_NAME, "IV", "Password")
    sIvPort = GetStrKeyValueEx(APP_NAME, "IV", "Port")
    
    If Len(sIvServer) > 0 And Len(sIvAccount) > 0 And Len(sIvPassword) > 0 Then
        If ConnectToIV Then
            IVInit = True
'        Else
'            Err.Raise vbObjectError, "IVInit", "Could't connect to iVolatility"
        End If
    Else
        Err.Raise vbObjectError, "IVInit", "iVolatility account is not specified."
    End If
End Function

Private Function GetAuthKey(sLogin As String) As String
    GetAuthKey = HttpCall(sIvServer & "/data_load/auth.asp?name=" & sLogin)
End Function

Private Function GetMd5Key(Key As String) As String
    Dim objMD5 As WORKFLOWICLib.MD5
    Set objMD5 = New WORKFLOWICLib.MD5
    GetMd5Key = objMD5.MDString(Key)
    Set objMD5 = Nothing
End Function

Private Function IndexCall(ByVal Symbol As String) As String
    IndexCall = AspDataCall("si=4&symbol=" & Symbol)
End Function

Private Function StockCall(ByVal Symbol As String) As String
    Dim sDateString As String
    sDateString = LastDateString
    StockCall = AspDataCall("si=1&sl=" & Symbol & "&sd=" & sDateString & "&ed=" & sDateString)
End Function

Private Function AspDataCall(ByVal sParams As String) As String
    AspDataCall = HttpCall(sIvServer & "/data_load/data_load.asp?au=" & sIvKey & "&" & sParams)
End Function

Private Function LastDateString() As String
    LastDateString = FormatDate(LastWorkingDay(Yesterday))
End Function

Private Function IsWorkingDay(ByVal Moment As Date) As Boolean
    IsWorkingDay = Not (IsWeekend(Moment) Or IsHolyday(Moment))
End Function

Private Function LastWorkingDay(ByVal Moment As Date) As Date
    LastWorkingDay = Moment
    Do While Not IsWorkingDay(LastWorkingDay)
        LastWorkingDay = PrevDay(LastWorkingDay)
    Loop
End Function

Private Function Yesterday() As Date
    Yesterday = PrevDay(Date)
End Function

Private Function PrevDay(ByVal Moment As Date) As Date
    PrevDay = Moment - 1
End Function

Private Function SplitResult(ByVal s As String) As Boolean
    Dim i As Integer
    
    On Error GoTo EH
    If s = "" Then
        SplitResult = False
    Else
        i = InStr(s, "<br>")
        If i > 0 Then
            s = Left$(s, i - 1)
        End If
        m_sResult = Split(s, ",")
        'DumpResult
        SplitResult = True
    End If
    Exit Function
EH:
    Debug.Print "Failed to split result"
    SplitResult = False
End Function

Public Function DownloadIndexStocks(ByVal sIndex As String, sStocks() As String) As Long
    On Error Resume Next
    Erase sStocks
    Dim lCnt As Long: lCnt = 0
    Dim lResCnt As Long: lResCnt = 0
    Dim sResult As String
    Dim sRow As String
    Dim sItems() As String
    Dim i As Long
    
    sResult = IndexCall(sIndex)
    If sResult <> "" Then
        While Len(sResult) > 0
            i = InStr(sResult, vbCrLf)
            If i > 0 Then
                sRow = Left(sResult, i - 1)
            Else
                sRow = sResult
            End If
                        
            lResCnt = -1
            sItems = Split(sRow, ",")
            lResCnt = UBound(sItems)
            If lResCnt > 1 Then
                If lCnt <> 0 Then ReDim Preserve sStocks(0 To lCnt) Else ReDim Preserve sStocks(0 To 0)
                sStocks(lCnt) = sItems(1)
                lCnt = lCnt + 1
            End If
            
            If i > 0 Then
                sResult = Right(sResult, Len(sResult) - i - 1)
            Else
                sResult = ""
            End If
        Wend
    End If
    
    DownloadIndexStocks = lCnt
End Function

Private Function GetStockDataFromIV(Symbol As String) As Boolean
    Dim s As String
    s = StockCall(Symbol)
    GetStockDataFromIV = SplitResult(s)
End Function

Public Function DownloadStock(ByVal Symbol As String, Beta As Double, Correlation As Double, Vola As Double, HV As Double) As Boolean
    Dim d As Date
    If GetStockDataFromIV(Symbol) Then
        If UBound(m_sResult) >= 4 Then
            If Symbol = m_sResult(1) Then
                d = DateValue(m_sResult(0))
                Beta = ValResult(2)
                Correlation = ValResult(3)
                Vola = ValResult(4)
                HV = ValResult(5)
                DownloadStock = True
                Exit Function
            End If
        End If
    End If
    Beta = 0
    Correlation = 0
    Vola = 0
    HV = 0
    DownloadStock = False
End Function

Private Function ValResult(ByVal Index As Long) As Variant
    If Index >= LBound(m_sResult) And Index <= UBound(m_sResult) Then
        ValResult = Val(m_sResult(Index))
    Else
        ValResult = 0
    End If
End Function

Private Function FormatDate(ByVal Moment As Date) As String
    FormatDate = _
        Format$(Month(Moment), "00") _
        & "/" & Format$(Day(Moment), "00") _
        & "/" & Format$(Year(Moment), "0000")
End Function

Private Function IsWeekend(ByVal Moment As Date) As Boolean
    If Weekday(Moment, vbMonday) > 5 Then
        IsWeekend = True
    Else
        IsWeekend = False
    End If
End Function

' Not implemented
Private Function IsHolyday(ByVal Moment As Date) As Boolean
    IsHolyday = False
End Function

