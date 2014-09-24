VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IVDataFeeder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "IVDataFeeder"
Private Const DefaultTenor = 252

Public Tenor As Long
Private ReconnectLimit As Long

Private WithEvents pHttpFeeder As HttpFeeder
Attribute pHttpFeeder.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean
Private AccessGranted As Boolean

Public Event LogMessage(ByVal Message As String)
Public Event StockIndexBeta(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Beta As Double)
Public Event SymbolSkewKurtsis(ByVal Symbol As String, ByVal Skew As Double, ByVal Kurt As Double)
Public Event SaveStock(ByVal SymbolsNum As Long, ByVal Symbol As String, ByVal Company As String, ByVal PrimExchange As String)
Public Event SaveSymbol(ByVal SymbolsNum As Long, ByVal Symbol As String, ByVal SymbolType As String, ByVal Company As String, ByVal PrimExchange As String)
Public Event SymbolAverageVolumes(ByVal Symbol As String, ByVal Volume10Day As Long, ByVal Volume5Expiration As Long)

Public Event ProgressStart()
Public Event ProgressStop()
Public Event Progress(ByVal ProgressPercent As Long)

Public Event SymbolIVPoint(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double)
Public Event SymbolIVBandPoint(ByVal Symbol As String, ByVal StrikeMoneyness As Long, ByVal ExpPeriod As Long, ByVal IVBandLow As Double, ByVal IVBandHigh As Double)
Public Event SymbolMSUpdateDate(ByVal Symbol As String, ByVal LastUpdate As Date)
    
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Tenor = DefaultTenor
    Set pHttpFeeder = New HttpFeeder
    ReconnectLimit = VolGetSettingLong("ReconnectLimit", 3)
    g_SID = ""
    AccessGranted = True
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    Set pHttpFeeder = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetHttpFeeder() As HttpFeeder
    Set GetHttpFeeder = pHttpFeeder
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pHttpFeeder_LogMessage(ByVal Message As String)
    RaiseEvent LogMessage(Message)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function DeltaHedgeAspDataCall(ByVal Operation As String, ByVal Symbols As String, ByVal Flags As Long) As String
Dim Address As String
Dim DateString As String
Dim Buffer As String
Dim SaveKey As String
Dim Reconnect As Long
    
    LastError = ""
    Reconnect = 0
Try:
    Reconnect = Reconnect + 1
    If Reconnect > 1 Then
        RaiseEvent LogMessage("Http error: Reconnect attempt #" & Format$(Reconnect))
    End If
    If Len(g_SID) = 0 Then
        With New IVAccount
            .VolAppLogin = pHttpFeeder.VolServerLogin
            .VolAppPassword = pHttpFeeder.VolServerPassword
            Set .pHttpFeeder = pHttpFeeder
            AccessGranted = .IVInit
            Set .pHttpFeeder = Nothing
        End With
    End If
    
    DateString = LastDateString
    
    Address = "f=" & Operation
    If Len(g_SID) > 0 Then
        Address = Address & "&SID=" & g_SID
        SaveKey = g_SID
        g_SID = ""
    Else
        LastError = "Server did not authorize this request"
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    Address = Address & _
        "&SA=" & Symbols & _
        "&D=" & DateString & _
        "&T=" & Format$(Tenor) & _
        "&BM=" & CStr(Flags)
    
    Buffer = pHttpFeeder.HttpCall("delta_hedge_HF.asp?" & Address)
    
    If CancelRequested Then
        CancelAccepted = True
        Exit Function
    End If
    
    If Len(Buffer) = 0 Then
        'LastError = "Server has returned an empty string"
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    'Access denied
    If InStr(Buffer, "Access denied") > 0 Then
        LastError = "Access denied"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Access denied. You should supply correct account and password"
    End If
    
    If InStr(Buffer, "Timeout expired") > 0 Then
        LastError = "Timeout expired"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Server request timeout expired."
    End If
    
'    If InStr(Buffer, "You are not authorized to view this page") > 0 Then
'        LastError = "You are not authorized to view this page"
'        Err.Raise vbObjectError, "Load IV data", LastError
'    End If
    
    If Not FindBrakets(Buffer, Buffer, "BEGIN IV DATA" & vbCrLf, "END IV DATA" & vbCrLf) Then
'        LastError = "Server has returned an improperly formated page"
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    If Len(Buffer) = 0 Then
        RaiseEvent LogMessage("Load IV data: Server didn't return any data")
    End If
    
    DeltaHedgeAspDataCall = Buffer
    g_SID = SaveKey

End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function DeltaHedgeAspDataCall2(ByVal Operation As String, ByVal ParamName As String, ByVal ParamValue As Variant) As String
Dim Address As String
Dim DateString As String
Dim Buffer As String
Dim SaveKey As String
Dim Reconnect As Long
    
    LastError = ""
    Reconnect = 0
Try:
    Reconnect = Reconnect + 1
    If Reconnect > 1 Then
        RaiseEvent LogMessage("Http error: Reconnect attempt #" & Format$(Reconnect))
    End If
    If Len(g_SID) = 0 Then
        With New IVAccount
            .VolAppLogin = pHttpFeeder.VolServerLogin
            .VolAppPassword = pHttpFeeder.VolServerPassword
            Set .pHttpFeeder = pHttpFeeder
            AccessGranted = .IVInit
            Set .pHttpFeeder = Nothing
        End With
    End If
    
    Address = "f=" & Operation
    If Len(g_SID) > 0 Then
        Address = Address & "&SID=" & g_SID
        SaveKey = g_SID
        g_SID = ""
    Else
        LastError = "Server did not authorize this request"
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    Address = Address & _
        "&" & ParamName & "=" & ParamValue

    Buffer = pHttpFeeder.HttpCall("delta_hedge_HF.asp?" & Address)
    
    If CancelRequested Then
        CancelAccepted = True
        Exit Function
    End If
    
    If Len(Buffer) = 0 Then
        'LastError = "Server has returned an empty string"
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    'Access denied
    If InStr(Buffer, "Access denied") > 0 Then
        LastError = "Access denied"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Access denied. You should supply correct account and password"
    End If
    
    If InStr(Buffer, "Timeout expired") > 0 Then
        LastError = "Timeout expired"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Server request timeout expired."
    End If
    
    
    If Not FindBrakets(Buffer, Buffer, "BEGIN IV DATA" & vbCrLf, "END IV DATA" & vbCrLf) Then
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    If Len(Buffer) = 0 Then
        RaiseEvent LogMessage("Load IV data: Server didn't return any data")
    End If
    
    DeltaHedgeAspDataCall2 = Buffer
    g_SID = SaveKey

End Function

Private Function DeltaHedgeAspDataCall3(ByVal Operation As String, ByVal Symbols As String, Optional ByVal UpdateDate As Date = 0) As String
Dim Address As String
Dim DateString As String
Dim Buffer As String
Dim SaveKey As String
Dim Reconnect As Long
    
    LastError = ""
    Reconnect = 0
Try:
    Reconnect = Reconnect + 1
    If Reconnect > 1 Then
        RaiseEvent LogMessage("Http error: Reconnect attempt #" & Format$(Reconnect))
    End If
    If Len(g_SID) = 0 Then
        With New IVAccount
            .VolAppLogin = pHttpFeeder.VolServerLogin
            .VolAppPassword = pHttpFeeder.VolServerPassword
            Set .pHttpFeeder = pHttpFeeder
            AccessGranted = .IVInit
            Set .pHttpFeeder = Nothing
        End With
    End If
    
    If UpdateDate = 0 Then
        DateString = LastDateString
    Else
        DateString = FormatDate(UpdateDate)
    End If
    
    Address = "f=" & Operation
    If Len(g_SID) > 0 Then
        Address = Address & "&SID=" & g_SID
        SaveKey = g_SID
        g_SID = ""
    Else
        LastError = "Server did not authorize this request"
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    Address = Address & _
        "&SA=" & Symbols & _
        "&D=" & DateString
    
    Buffer = pHttpFeeder.HttpCall("delta_hedge_HF.asp?" & Address)
    
    If CancelRequested Then
        CancelAccepted = True
        Exit Function
    End If
    
    If Len(Buffer) = 0 Then
        'LastError = "Server has returned an empty string"
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    'Access denied
    If InStr(Buffer, "Access denied") > 0 Then
        LastError = "Access denied"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Access denied. You should supply correct account and password"
    End If
    
    If InStr(Buffer, "Timeout expired") > 0 Then
        LastError = "Timeout expired"
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", "Server request timeout expired."
    End If
    
'    If InStr(Buffer, "You are not authorized to view this page") > 0 Then
'        LastError = "You are not authorized to view this page"
'        Err.Raise vbObjectError, "Load IV data", LastError
'    End If
    
    If Not FindBrakets(Buffer, Buffer, "BEGIN IV DATA" & vbCrLf, "END IV DATA" & vbCrLf) Then
'        LastError = "Server has returned an improperly formated page"
        LastError = "IV service temporary unavailable. Please try later."
        If AccessGranted Then
            RaiseEvent LogMessage("Load IV data: " & LastError)
            If Reconnect < ReconnectLimit Then GoTo Try
        End If
        Err.Raise vbObjectError, "Load IV data", LastError
    End If
    
    If Len(Buffer) = 0 Then
        RaiseEvent LogMessage("Load IV data: Server didn't return any data")
    End If
    
    DeltaHedgeAspDataCall3 = Buffer
    g_SID = SaveKey
End Function

'
Public Function GetStockIndexBeta(IndexSymbols As String, ByVal StockSymbols As String) As Boolean
Dim Symbols As String
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    Symbols = IndexSymbols & ";" & StockSymbols
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SY", Symbols, 8)   '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
Dim str As String

    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) > 0 Then
        For Each L In Lines
            If Not FindBrakets(L, S, ": ") Then
                GoTo Fail
            End If
            Set Items = SplitItems(S, ",")
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim IndexSymbol As String
            Dim StockSymbol As String
            Dim Weight As Double
            Dim Beta As Double
            IndexSymbol = GetItem(Items, 2)
            StockSymbol = GetItem(Items, 1)
            
            str = IndexSymbol
            InValidateStockSymbol (str)
            
            If InStr(IndexSymbols, str) = 0 Then
                Swap StockSymbol, IndexSymbol
            End If
            
            Beta = GetItemDouble(Items, 3)
            RaiseEvent StockIndexBeta(StockSymbol, IndexSymbol, Beta)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockIndexBeta = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStockIndexBeta = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock/Index Beta data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock/Index Beta data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolSkewKurtsis(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("EH", Symbols, 8)   '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) > 0 Then
        For Each L In Lines
            If Not FindBrakets(L, S, ": ") Then
                GoTo Fail
            End If
            Set Items = SplitItems(S, ",")
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim Skew As Double
            Dim Kurt As Double
            Symbol = GetItem(Items, 1)
            Skew = GetItemDouble(Items, 2)
            Kurt = GetItemDouble(Items, 3)
            RaiseEvent SymbolSkewKurtsis(Symbol, Skew, Kurt)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetSymbolSkewKurtsis = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetSymbolSkewKurtsis = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Symbol Skew/Kurtosis data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Symbol Skew/Kurtosis data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolAverageVolumes(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("UV", Symbols, 8)   '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) > 0 Then
        For Each L In Lines
            If Not FindBrakets(L, S, ": ") Then
                GoTo Fail
            End If
            Set Items = SplitItems(S, ",")
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim Volume10Day As Long
            Dim Volume5Expiration As Long
            Symbol = GetItem(Items, 1)
            Volume10Day = GetItem(Items, 2)
            Volume5Expiration = GetItem(Items, 3)
            RaiseEvent SymbolAverageVolumes(Symbol, Volume10Day, Volume5Expiration)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetSymbolAverageVolumes = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetSymbolAverageVolumes = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Symbol Average Volumes data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Symbol Average Volumes data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockList(ByVal IsOptional As Integer) As Boolean
' Don't Function this Sub!!!!!!!!
' use GetSymbolList instead!!!!!
' FOR BINARY COMPATIBILITY ONLY!

'Dim Buffer As String
'    On Error GoTo Herr
'    TaskStarted
'    '//////////////////////////////////////////////////
'    Buffer = DeltaHedgeAspDataCall2("SG", "IO", IsOptional)  '/
'    '////////////////////////////////////////////////
'
'    If CancelRequested Then
'        CancelAccepted = True
'        GoTo Fail
'    End If
'
'Dim Lines As Variant
'Dim Items As Collection
'Dim L As Variant
'Dim S As String
'    Lines = Split(Buffer, vbCrLf)
'    If UBound(Lines) > 0 Then
'        For Each L In Lines
'            If Not FindBrakets(L, S, ": ") Then
'                RaiseEvent LogMessage("Bad String from IVolatility:" & L)
'                GoTo NextString
'            End If
'
'            Set Items = SplitItems(S, ",")
'            If Items.Count < 3 Then
'                RaiseEvent LogMessage("Bad String from IVolatility:" & S)
'                GoTo NextString
'            End If
'            Dim Symbol As String
'            Dim Company As String
'            Dim PrimExchange As String
'            Symbol = GetItem(Items, 1)
'            Company = GetItem(Items, 2)
'            PrimExchange = GetItem(Items, 3)
'            RaiseEvent SaveStock(UBound(Lines), Symbol, Company, PrimExchange)
'            DoEvents
'            If CancelRequested Then
'                CancelAccepted = True
'                GoTo Fail
'            End If
'NextString:
'        Next
'        GetStockList = True
'    End If
'    TaskFinished
'    Exit Function
'Fail:
'    TaskFinished
'    Exit Function
'Herr:
'    Dim Message As String
'    Message = Err.Description
'    TaskFinished
'    RaiseEvent LogMessage("Load IV data: Loading Stock list from IV server failed due to the following error: " & Message)
'    Err.Raise vbObjectError, "Load IV data", "Loading Stock from IV server failed due to the following error: " & vbCrLf & Message
    GetStockList = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolList(ByVal IsOptional As Integer) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall2("SG", "IO", IsOptional)  '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) > 0 Then
        For Each L In Lines
            If Not FindBrakets(L, S, ": ") Then
                RaiseEvent LogMessage("Bad String from IVolatility:" & L)
                GoTo NextString
            End If
                        
            Set Items = SplitItems(S, ",")
            If Items.Count < 4 Then
                RaiseEvent LogMessage("Bad String from IVolatility:" & S)
                GoTo NextString
            End If
            Dim Symbol As String
            Dim SymbolType As String
            Dim Company As String
            Dim PrimExchange As String
            Symbol = GetItem(Items, 1)
            Company = GetItem(Items, 2)
            PrimExchange = GetItem(Items, 3)
            SymbolType = GetItem(Items, 4)
            RaiseEvent SaveSymbol(UBound(Lines), Symbol, SymbolType, Company, PrimExchange)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
NextString:
        Next
        GetSymbolList = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetSymbolList = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Symbols list from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Symbols from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolIVSurface(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("OD", Symbols, 8)   '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Const OpenBracket As String = "BEGIN STOCK DATA" & vbCrLf
Const CloseBracket As String = "END STOCK DATA" & vbCrLf
Dim StockBuff As String
Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
Dim i As Long
Dim length As Long
Dim CurrentProgress As Long
Dim NewProgress As Long
        
    While FindBrakets(Buffer, StockBuff, OpenBracket, CloseBracket)
    
        CurrentProgress = 0
        RaiseEvent ProgressStart
            
        length = Len(Buffer) - Len(StockBuff) - Len(OpenBracket) - Len(CloseBracket)
        
        If length > 0 Then
            Buffer = Right(Buffer, length)
        Else
            Buffer = ""
        End If
                
        Lines = Split(StockBuff, vbCrLf)
            
    If UBound(Lines) > 0 Then
                
            Dim Symbol As String

            If Not FindBrakets(Lines(0), Symbol, ": ") Then
                GoTo Fail
            End If
    
            NewProgress = 100 / UBound(Lines)
            
            If NewProgress <> CurrentProgress Then
                CurrentProgress = NewProgress
                RaiseEvent Progress(CurrentProgress)
            End If
            
            i = 1
            While i < UBound(Lines)
                If Not FindBrakets(Lines(i), S, ": ") Then
                    GoTo Fail
                End If
                
                Set Items = SplitItems(S, ",")

                If Items.Count < 3 Then
                    GoTo Fail
                End If
    
                Dim Strike As Double
                Dim Expiration As Date
                Dim Vola As Double
                
                Expiration = GetItemDate(Items, 1)
                    
                Strike = GetItemDouble(Items, 2)
                    
                Vola = GetItemDouble(Items, 3)
                
                RaiseEvent SymbolIVPoint(Symbol, Strike, Expiration, Vola)
                
                NewProgress = 100 * (i + 1) / UBound(Lines)
            
                If NewProgress <> CurrentProgress Then
                    CurrentProgress = NewProgress
                    RaiseEvent Progress(CurrentProgress)
                End If
                
                DoEvents
                If CancelRequested Then
                    CancelAccepted = True
                    GoTo Fail
                End If
                i = i + 1
            Wend
            GetSymbolIVSurface = True
        End If
    
        RaiseEvent ProgressStop
    Wend

    TaskFinished
    Exit Function
Fail:
    GetSymbolIVSurface = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Symbol IV surface from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Symbol IV surface from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolIVBands(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("OS", Symbols, 8)   '/
    '////////////////////////////////////////////////

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

Const OpenBracket As String = "BEGIN STOCK DATA" & vbCrLf
Const CloseBracket As String = "END STOCK DATA" & vbCrLf
Dim StockBuff As String
Dim Lines As Variant
Dim Items As Collection
Dim L As Variant
Dim S As String
Dim i As Long
Dim length As Long
Dim CurrentProgress As Long
Dim NewProgress As Long
        
    While FindBrakets(Buffer, StockBuff, OpenBracket, CloseBracket)
    
        CurrentProgress = 0
        RaiseEvent ProgressStart
            
        length = Len(Buffer) - Len(StockBuff) - Len(OpenBracket) - Len(CloseBracket)
        
        If length > 0 Then
            Buffer = Right(Buffer, length)
        Else
            Buffer = ""
        End If
                
        Lines = Split(StockBuff, vbCrLf)
            
    If UBound(Lines) > 0 Then
                
            Dim Symbol As String

            If Not FindBrakets(Lines(0), Symbol, ": ") Then
                GoTo Fail
            End If
    
            NewProgress = 100 / UBound(Lines)
            
            If NewProgress <> CurrentProgress Then
                CurrentProgress = NewProgress
                RaiseEvent Progress(CurrentProgress)
            End If
            
            i = 1
            While i < UBound(Lines)
                If Not FindBrakets(Lines(i), S, ": ") Then
                    GoTo Fail
                End If
                
                Set Items = SplitItems(S, ",")

                If Items.Count < 4 Then
                    GoTo Fail
                End If
    
                Dim StrikeMoneyness As Long
                Dim ExpPeriod As Long
                Dim IVBandLow As Double
                Dim IVBandHigh As Double
                
                    
                StrikeMoneyness = GetItem(Items, 1)
                    
                ExpPeriod = GetItem(Items, 2)
                
                IVBandLow = GetItemDouble(Items, 3)
                IVBandHigh = GetItemDouble(Items, 4)
                
                RaiseEvent SymbolIVBandPoint(Symbol, StrikeMoneyness, ExpPeriod, IVBandLow, IVBandHigh)
                
                NewProgress = 100 * (i + 1) / UBound(Lines)
            
                If NewProgress <> CurrentProgress Then
                    CurrentProgress = NewProgress
                    RaiseEvent Progress(CurrentProgress)
                End If
                
                DoEvents
                If CancelRequested Then
                    CancelAccepted = True
                    GoTo Fail
                End If
                i = i + 1
            Wend
            GetSymbolIVBands = True
        End If
    
        RaiseEvent ProgressStop
    Wend

    TaskFinished
    Exit Function
Fail:
    GetSymbolIVBands = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Symbol IV Bands from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Symbol IV Bands from IV server failed due to the following error: " & vbCrLf & Message
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
    pHttpFeeder.CancelTask
End Function

Public Function GetSymbolsMSUpdateDateFromIV(ByVal Symbols As String, ByVal LastUpdateDate As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    
    Buffer = DeltaHedgeAspDataCall3("VV", Symbols, LastUpdateDate)

    If CancelRequested Then
        CancelAccepted = True
        GoTo Fail
    End If

    Dim Items As Collection
    Dim Lines As Variant
    Dim L As Variant
    Dim S As String
    
    Lines = Split(Buffer, vbCrLf)

    If UBound(Lines) > 0 Then
        For Each L In Lines
            If Not FindBrakets(L, S, ": ") Then
                GoTo Fail
            End If
            Set Items = SplitItems(S, ",")
            If Items.Count < 1 Then
                GoTo Fail
            End If
            Dim sSymbol As String
            Dim dtSymbolDate As Date
            sSymbol = GetItem(Items, 1)
            dtSymbolDate = GetItemDate(Items, 3, 0)
            RaiseEvent SymbolMSUpdateDate(sSymbol, dtSymbolDate)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetSymbolsMSUpdateDateFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetSymbolsMSUpdateDateFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading symbol market structure update date from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading symbol market structure update date from IV server failed due to the following error: " & vbCrLf & Message
End Function

