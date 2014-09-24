VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DHIVFeeder"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "DHIVFeeder"
Private Const DefaultTenor = 252

Public HVTenor As Long
Private ReconnectLimit As Long

Private WithEvents pHttpFeeder As HttpFeeder
Attribute pHttpFeeder.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean
Private AccessGranted As Boolean

Public Event LogMessage(ByVal Message As String)

Public Event GotStockPriceData4(ByVal StockSymbol As String, ByVal PriceClose As Double, ByVal PriceDate As Date)
Public Event GotStockSymbolData4(ByVal StockSymbol As String, ByVal IndexSymbol As String)

Public Event GotStockIndexCorrData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotStockStockCorrData4(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotIndexIndexCorrData4(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotStockHVData4(ByVal StockSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
Public Event GotIndexVegaData4(ByVal IndexSymbol As String, ByVal Vega As Double)

Public Event GotStockIndexBetaData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Weight As Double, ByVal Beta As Double)

Public Event GotStockIVData5(ByVal StockSymbol As String, ByVal IVIndexVola As Double, ByVal HV As Double, ByVal IVMin As Double, ByVal IVMax As Double)
Public Event GotStockIndexCorrData5(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double)
Public Event GotStockStockCorrData5(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal VCorr As Double)
Public Event GotIndexIndexCorrData5(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal VCorr As Double)

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    HVTenor = DefaultTenor
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
Private Function DeltaHedgeAspDataCall(ByVal Operation As String, ByVal Symbols As String) As String
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
        "&T=" & Format$(HVTenor)
    
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStocksHVFromIV(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SR", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 4 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim SymbolHV As Double
            Dim SymbolIV As Double
            Dim SymbolVolOfVol As Double
            Symbol = GetItem(Items, 1)
            SymbolVolOfVol = GetItemDouble(Items, 2)
            SymbolHV = GetItemDouble(Items, 3)
            SymbolIV = GetItemDouble(Items, 4)
            RaiseEvent GotStockHVData4(Symbol, SymbolHV, SymbolIV, SymbolVolOfVol)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStocksHVFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStocksHVFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock's HV data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock's HV data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStocksIVData5(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("WV", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 5 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim IVIndexVola As Double
            Dim HV As Double
            Dim IVMin As Double
            Dim IVMax As Double
            Symbol = GetItem(Items, 1)
            IVIndexVola = GetItemDouble(Items, 2)
            HV = GetItemDouble(Items, 3)
            IVMin = GetItemDouble(Items, 4)
            IVMax = GetItemDouble(Items, 5)
            RaiseEvent GotStockIVData5(Symbol, IVIndexVola, HV, IVMin, IVMax)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStocksIVData5 = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStocksIVData5 = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock's IV data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock's IV data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockStockCorrFromIV(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SD", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 4 Then
                GoTo Fail
            End If
            Dim Symbol1 As String
            Dim Symbol2 As String
            Dim Corr As Double
            Dim VCorr As Double
            Symbol1 = GetItem(Items, 1)
            Symbol2 = GetItem(Items, 2)
            Corr = GetItemDouble(Items, 3)
            VCorr = GetItemDouble(Items, 4)
            RaiseEvent GotStockStockCorrData4(Symbol1, Symbol2, Corr, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockStockCorrFromIV = True
    End If
Done:
    TaskFinished
    Exit Function
Fail:
    GetStockStockCorrFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock/Stock correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock/Stock correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockStockVCorr5(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SY", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim Symbol1 As String
            Dim Symbol2 As String
            Dim VCorr As Double
            Symbol1 = GetItem(Items, 1)
            Symbol2 = GetItem(Items, 2)
            VCorr = GetItemDouble(Items, 3)
            RaiseEvent GotStockStockCorrData5(Symbol1, Symbol2, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockStockVCorr5 = True
    End If
Done:
    TaskFinished
    Exit Function
Fail:
    GetStockStockVCorr5 = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock/Stock correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock/Stock correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexIndexCorrFromIV(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SD", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 4 Then
                GoTo Fail
            End If
            Dim Symbol1 As String
            Dim Symbol2 As String
            Dim Corr As Double
            Dim VCorr As Double
            Symbol1 = GetItem(Items, 1)
            Symbol2 = GetItem(Items, 2)
            Corr = GetItemDouble(Items, 3)
            VCorr = GetItemDouble(Items, 4)
            RaiseEvent GotIndexIndexCorrData4(Symbol1, Symbol2, Corr, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetIndexIndexCorrFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetIndexIndexCorrFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Index/Index correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Index/Index correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexIndexVCorr5(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SY", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim Symbol1 As String
            Dim Symbol2 As String
            Dim VCorr As Double
            Symbol1 = GetItem(Items, 1)
            Symbol2 = GetItem(Items, 2)
            VCorr = GetItemDouble(Items, 3)
            RaiseEvent GotIndexIndexCorrData5(Symbol1, Symbol2, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetIndexIndexVCorr5 = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetIndexIndexVCorr5 = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Index/Index correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Index/Index correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIndexCorrFromIV(IndexSymbols As String, ByVal StockSymbols As String) As Boolean
Dim Symbols As String
Dim Buffer As String
Dim StockIndexPairsBuf As String
Dim PairsWithNoCorr As String
    On Error GoTo Herr
    TaskStarted
    StockIndexPairsBuf = MakeStockIndexSymbolPairs(IndexSymbols, StockSymbols)
    
    Symbols = IndexSymbols & ";" & StockSymbols
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SD", Symbols)   '/
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
            If Items.Count < 4 Then
                GoTo Fail
            End If
            Dim IndexSymbol As String
            Dim StockSymbol As String
            Dim Corr As Double
            Dim VCorr As Double
            StockSymbol = GetItem(Items, 1)
            IndexSymbol = GetItem(Items, 2)
            
            If InStr(QuotedSymbol(IndexSymbols), QuotedSymbol(IndexSymbol)) = 0 Then
                Swap StockSymbol, IndexSymbol
            End If
            
            Dim Needle As String
            Needle = StockSymbol & "," & IndexSymbol & ":"
            StockIndexPairsBuf = Replace(StockIndexPairsBuf, Needle, "", 1, 1)

            'If InStr(1, StockIndexPairsBuf, "'" & StockSymbol & "','" & IndexSymbol & "'") = 0 Then
                'RaiseEvent LogMessage("No correlation values found on IVolatility.com for: " & StockSymbol & ", " & IndexSymbol)
            'End If
            
            Corr = GetItemDouble(Items, 3)
            VCorr = GetItemDouble(Items, 4)
            RaiseEvent GotStockIndexCorrData4(StockSymbol, IndexSymbol, Corr, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        If Len(StockIndexPairsBuf) > 0 Then _
            LogPairsWithNoCorr (StockIndexPairsBuf)
        GetStockIndexCorrFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    If Len(StockIndexPairsBuf) > 0 Then _
        LogPairsWithNoCorr (StockIndexPairsBuf)
    GetStockIndexCorrFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock/Index correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock/Index correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIndexVCorr5(IndexSymbols As String, ByVal StockSymbols As String) As Boolean
Dim Symbols As String
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    Symbols = IndexSymbols & ";" & StockSymbols
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SY", Symbols)   '/
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
            Dim IndexSymbol As String
            Dim StockSymbol As String
            Dim Corr As Double
            Dim VCorr As Double
            StockSymbol = GetItem(Items, 1)
            IndexSymbol = GetItem(Items, 2)
            If InStr(QuotedSymbol(IndexSymbols), QuotedSymbol(IndexSymbol)) = 0 Then
                Swap StockSymbol, IndexSymbol
            End If
            VCorr = GetItemDouble(Items, 3)
            RaiseEvent GotStockIndexCorrData5(StockSymbol, IndexSymbol, VCorr)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockIndexVCorr5 = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStockIndexVCorr5 = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock/Index correlation data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock/Index correlation data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIndexBetaFromIV(IndexSymbols As String, ByVal StockSymbols As String) As Boolean
Dim Symbols As String
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    Symbols = IndexSymbols & ";" & StockSymbols
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("SS", Symbols)   '/
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
            If Items.Count < 4 Then
                GoTo Fail
            End If
            Dim IndexSymbol As String
            Dim StockSymbol As String
            Dim Weight As Double
            Dim Beta As Double
            IndexSymbol = GetItem(Items, 2)
            StockSymbol = GetItem(Items, 1)
            If InStr(QuotedSymbol(IndexSymbols), QuotedSymbol(IndexSymbol)) = 0 Then
                Swap StockSymbol, IndexSymbol
            End If
            Weight = GetItemDouble(Items, 3)
            Beta = GetItemDouble(Items, 4)
            RaiseEvent GotStockIndexBetaData4(StockSymbol, IndexSymbol, Weight, Beta)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockIndexBetaFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStockIndexBetaFromIV = False
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
Public Function GetStockPriceFromIV(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("CP", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 3 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim PriceDate As Date
            Dim PriceClose As Double
            Symbol = GetItem(Items, 1)
            PriceDate = GetItemDouble(Items, 2)
            PriceClose = GetItemDouble(Items, 3)
            RaiseEvent GotStockPriceData4(Symbol, PriceClose, PriceDate)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStockPriceFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStockPriceFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock's Price data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock's Price data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexVegaFromIV(ByVal Symbols As String) As Boolean
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("CE", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count < 2 Then
                GoTo Fail
            End If
            Dim Symbol As String
            Dim Vega As Double
            Symbol = GetItem(Items, 1)
            Vega = GetItemDouble(Items, 2)
            RaiseEvent GotIndexVegaData4(Symbol, Vega)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetIndexVegaFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetIndexVegaFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Index Vega data from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Index Vega data from IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStocksListFromIV() As Boolean
Dim Symbols As String
Dim Buffer As String
    On Error GoTo Herr
    TaskStarted
    Symbols = ""
    '//////////////////////////////////////////////////
    Buffer = DeltaHedgeAspDataCall("CS", Symbols)   '/
    '////////////////////////////////////////////////

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
            If Items.Count <> 2 Then
                GoTo Fail
            End If
            Dim StockSymbol As String
            Dim IndexSymbol As String
            Dim ID As Long
            IndexSymbol = GetItem(Items, 1)
            StockSymbol = GetItem(Items, 2)
            RaiseEvent GotStockSymbolData4(StockSymbol, IndexSymbol)
            DoEvents
            If CancelRequested Then
                CancelAccepted = True
                GoTo Fail
            End If
        Next
        GetStocksListFromIV = True
    End If
    TaskFinished
    Exit Function
Fail:
    GetStocksListFromIV = False
    TaskFinished
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    TaskFinished
    RaiseEvent LogMessage("Load IV data: Loading Stock's List from IV server failed due to the following error: " & Message)
    Err.Raise vbObjectError, "Load IV data", "Loading Stock's List from IV server failed due to the following error: " & vbCrLf & Message
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function MakeStockIndexSymbolPairs(ByVal ISymbols As String, ByVal SSymbols As String) As String
Dim IndexSymbolsArr As Variant
Dim StockSymbolsArr As Variant
Dim IndexSymbolElem As Variant
Dim StockSymbolElem As Variant
Dim Buf As String
    IndexSymbolsArr = Split(Replace(ISymbols, "'", ""), ",")
    StockSymbolsArr = Split(Replace(SSymbols, "'", ""), ",")
    For Each StockSymbolElem In StockSymbolsArr
        For Each IndexSymbolElem In IndexSymbolsArr
            Buf = Buf & StockSymbolElem & "," & IndexSymbolElem & ":"
        Next
    Next
    
    MakeStockIndexSymbolPairs = Buf
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub LogPairsWithNoCorr(PairsBuf As String)
Dim PairsArr As Variant
Dim PairElem As Variant
    PairsArr = Split(PairsBuf, ":")
    For Each PairElem In PairsArr
        If Len(PairElem) > 0 Then _
            RaiseEvent LogMessage("No correlation found on IVolatility for pair: " & PairElem)
    Next
End Sub
