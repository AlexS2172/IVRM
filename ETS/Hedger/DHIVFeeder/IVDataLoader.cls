VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IVDataLoader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True

'
Option Explicit

Private Const THIS = "IVDataLoader"

Private WithEvents pIVFeeder As IVDataFeeder
Attribute pIVFeeder.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean

Public Event Status(ByVal Message As String)
Public Event LogMessage(ByVal Message As String)
Public Event ProgressStart()
Public Event ProgressStop()
Public Event Progress(ByVal ProgressPercent As Long)

Public Event StockIndexBeta(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Beta As Double)
Public Event SymbolSkewKurtsis(ByVal Symbol As String, ByVal Skew As Double, ByVal Kurt As Double)
Public Event SaveStock(ByVal Symbol As String, ByVal Company As String, ByVal PrimExchange As String)
Public Event SaveSymbol(ByVal Symbol As String, ByVal SymbolType As String, ByVal Company As String, ByVal PrimExchange As String)
Public Event SymbolIVPoint(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double)
Public Event SymbolIVBandPoint(ByVal Symbol As String, ByVal StrikeMoneyness As Long, ByVal ExpPeriod As Long, ByVal IVBandLow As Double, ByVal IVBandHigh As Double)
Public Event SymbolAverageVolumes(ByVal Symbol As String, ByVal Volume10Day As Long, ByVal Volume5Expiration As Long)
Public Event SymbolMSUpdateDate(ByVal Symbol As String, ByVal LastUpdate As Date)

Public rsIndexData As ADODB.Recordset
Public rsStockData As ADODB.Recordset
Public rsStockIndexData As ADODB.Recordset
Public rsStockStockData As ADODB.Recordset
Public rsIndexIndexData As ADODB.Recordset

Public TotalStockCount As Long
Public TotalIndexCount As Long
Public TotalStockStockCount As Long
Public TotalStockIndexCount As Long
Public TotalIndexIndexCount As Long
Public CurentStockCount As Long
Public CurentIndexCount As Long
Public CurentStockStockCount As Long
Public CurentStockIndexCount As Long
Public CurentIndexIndexCount As Long
Public CurentSymbolCount As Long
Public CurentStockCountPercent As Long
Public CurentIndexCountPercent As Long
Public CurentStockStockCountPercent As Long
Public CurentStockIndexCountPercent As Long
Public CurentIndexIndexCountPercent As Long
Public CurentSymbolCountPercent As Long
Public CurentSymbol As String

Public g_SymbolsCountLimit As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set pIVFeeder = New IVDataFeeder
    IndexListClear
    StocksListClear
    StockIndexListClear
    StockStockListClear
    IndexIndexListClear
    On Error Resume Next
    g_SymbolsCountLimit = 100
    g_SymbolsCountLimit = VolGetSettingLong("SymbolsCountLimit", 100)
    If g_SymbolsCountLimit < 10 Then
        g_SymbolsCountLimit = 100
    End If
    VolSetSettingLong "SymbolsCountLimit", g_SymbolsCountLimit
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    On Error Resume Next
    Set pIVFeeder = Nothing
    VolSetSettingLong "SymbolsCountLimit", g_SymbolsCountLimit
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetDHIVFeeder() As IVDataFeeder
    Set GetDHIVFeeder = pIVFeeder
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pIVFeeder_LogMessage(ByVal Message As String)
    RaiseEvent LogMessage(Message)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub IndexListClear()
    Set rsIndexData = New ADODB.Recordset
    With rsIndexData
        .Fields.Append "IndexSymbol", adVarChar, 20
        .Fields.Append "Selected", adBoolean
        .Fields.Append "HV", adDouble
        .Fields.Append "IV", adDouble
        .Fields.Append "VolOfVol", adDouble
        .Fields.Append "Vega", adDouble
        .Fields.Append "ClosePrice", adDouble
        .Open
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StocksListClear()
    Set rsStockData = New ADODB.Recordset
    With rsStockData
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "Selected", adBoolean
        .Fields.Append "HV", adDouble
        .Fields.Append "IV", adDouble
        .Fields.Append "VolOfVol", adDouble
        .Fields.Append "ClosePrice", adDouble
        .Fields.Append "Supported", adBoolean
        .Open
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StockIndexListClear()
    Set rsStockIndexData = New ADODB.Recordset
    With rsStockIndexData
        .Fields.Append "IndexSymbol", adBSTR, 20
        .Fields.Append "StockSymbol", adBSTR, 20
        .Fields.Append "Corr", adDouble
        .Fields.Append "VCorr", adDouble
        .Open
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StockStockListClear()
    Set rsStockStockData = New ADODB.Recordset
    With rsStockStockData
        .Fields.Append "StockSymbol1", adVarChar, 20
        .Fields.Append "StockSymbol2", adVarChar, 20
        .Fields.Append "Corr", adDouble
        .Fields.Append "VCorr", adDouble
        .Open
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub IndexIndexListClear()
    Set rsIndexIndexData = New ADODB.Recordset
    With rsIndexIndexData
        .Fields.Append "IndexSymbol1", adVarChar, 20
        .Fields.Append "IndexSymbol2", adVarChar, 20
        .Fields.Append "Corr", adDouble
        .Fields.Append "VCorr", adDouble
        .Open
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub IndexListAddSymbol(ByVal IndexSymbol As String)
Dim Criteria, tmpIndexSymbol As String

tmpIndexSymbol = IndexSymbol
InValidateStockSymbol tmpIndexSymbol

     If rsIndexData Is Nothing Then
        IndexListClear
    Else
        If Not ValidateStockSymbol(IndexSymbol) Then
            Exit Sub
        End If
        With rsIndexData
            If .RecordCount > 0 Then
                .MoveFirst
            End If
            Criteria = "IndexSymbol='" & tmpIndexSymbol & "'"
            .Find Criteria, 0, adSearchForward, adBookmarkFirst
            If .EOF Then
                .AddNew
                !IndexSymbol = IndexSymbol
            End If
            !Selected = True
            .Update
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StocksListAddSymbol(ByVal StockSymbol As String)
Dim Criteria, tmpStockSymbol As String

tmpStockSymbol = StockSymbol
InValidateStockSymbol tmpStockSymbol

    If rsStockData Is Nothing Then
        StocksListClear
    Else
        With rsStockData
            If .RecordCount > 0 Then
                .MoveFirst
            End If
            Criteria = "StockSymbol='" & tmpStockSymbol & "'"
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
Private Function GetIndexSymbols() As String
Dim IndexSymbol As Variant
Dim IndexSymbols As String
    IndexSymbols = ""
    TotalIndexCount = 0
    With rsIndexData.Clone(adLockReadOnly)
        If .RecordCount > 0 Then
            .Sort = "IndexSymbol"
            .MoveFirst
            Do While Not .EOF
                IndexSymbol = !IndexSymbol
                If !Selected Then
                    If Len(IndexSymbols) = 0 Then
                        IndexSymbols = QuotedSymbol(IndexSymbol)
                    Else
                        IndexSymbols = IndexSymbols & "," & QuotedSymbol(IndexSymbol)
                    End If
                    TotalIndexCount = TotalIndexCount + 1
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    GetIndexSymbols = IndexSymbols
    TotalIndexIndexCount = TotalIndexCount * (TotalIndexCount - 1) / 2
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetStockSymbols() As Collection
Dim StockSymbol As Variant
Dim StockSymbols As String
Dim Count As Long
    Set GetStockSymbols = New Collection
    StockSymbols = ""
    TotalStockCount = 0
    With rsStockData.Clone(adLockReadOnly)
        If .RecordCount > 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            Do While Not .EOF
                StockSymbol = !StockSymbol
                If !Selected Then
                    If Len(StockSymbols) = 0 Then
                        StockSymbols = QuotedSymbol(StockSymbol)
                    Else
                        StockSymbols = StockSymbols & "," & QuotedSymbol(StockSymbol)
                    End If
                    Count = Count + 1
                    TotalStockCount = TotalStockCount + 1
                    If Count >= g_SymbolsCountLimit Then
                        GetStockSymbols.Add StockSymbols
                        StockSymbols = ""
                        Count = 0
                    End If
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    If Len(StockSymbols) > 0 Then
        GetStockSymbols.Add StockSymbols
    End If
    TotalStockStockCount = TotalStockCount * (TotalStockCount - 1) / 2
    TotalStockIndexCount = TotalStockCount * TotalIndexCount
End Function

Private Sub pIVFeeder_Progress(ByVal ProgressPercent As Long)
    RaiseEvent Progress(ProgressPercent)
End Sub

Private Sub pIVFeeder_ProgressStart()
    RaiseEvent ProgressStart
End Sub

Private Sub pIVFeeder_ProgressStop()
    RaiseEvent ProgressStop
End Sub

Private Sub pIVFeeder_SaveStock(ByVal SymbolsNum As Long, ByVal Symbol As String, ByVal Company As String, ByVal PrimExchange As String)
    On Error Resume Next
' Don't used this Sub!!!!!!!!
' use pIVFeeder_SaveSymbol instead!!!!!
' FOR BINARY COMPATIBILITY ONLY!
'
'    If Not ValidateStockSymbol(Symbol) Then
'        Exit Sub
'    End If
'
'    RaiseEvent SaveStock(Symbol, Company, PrimExchange)
'
'    Dim i As Long
'    CurentStockCount = CurentStockCount + 1
'    i = 100 * CurentStockCount / SymbolsNum
'    If i <> CurentStockCountPercent Then
'        CurentStockCountPercent = i
'        RaiseEvent Status("Loading Stock List From IVolatility " & Format$(i) & "%")
'        RaiseEvent Progress(i)
'    End If

End Sub

Private Sub pIVFeeder_SaveSymbol(ByVal SymbolsNum As Long, ByVal Symbol As String, ByVal SymbolType As String, ByVal Company As String, ByVal PrimExchange As String)
    On Error Resume Next
        
    RaiseEvent SaveSymbol(Symbol, SymbolType, Company, PrimExchange)
    
    Dim i As Long
    CurentSymbolCount = CurentSymbolCount + 1
    i = 100 * CurentSymbolCount / SymbolsNum
    If i <> CurentSymbolCountPercent Then
        CurentSymbolCountPercent = i
        RaiseEvent Status("Loading Symbol List From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If


End Sub

Private Sub pIVFeeder_StockIndexBeta(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Beta As Double)
    On Error Resume Next
        
    RaiseEvent StockIndexBeta(StockSymbol, IndexSymbol, Beta)
    
    Dim i As Long
    CurentStockIndexCount = CurentStockIndexCount + 1
    i = 100 * CurentStockIndexCount / TotalStockIndexCount
    If i <> CurentStockIndexCountPercent Then
        CurentStockIndexCountPercent = i
        RaiseEvent Status("Loading Stock/Index Beta From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIndexBeta() As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    On Error GoTo EH
    TaskStarted
    RaiseEvent ProgressStart
    IndexSymbols = GetIndexSymbols
    CurentStockIndexCount = 0
    CurentStockIndexCountPercent = 0
    If Len(IndexSymbols) > 0 Then
        For Each StockSymbols In GetStockSymbols
            If Len(StockSymbols) > 0 Then
                If Not CancelRequested Then
                    RaiseEvent Status("Loading Stock/Index Beta From IVolatility: requesting data..")
                    pIVFeeder.GetStockIndexBeta IndexSymbols, StockSymbols
                End If
            End If
        Next
    End If
    Dim Msg As String
    Msg = "Loading Stock/Index Beta From IVolatility: " & _
        Format$(CurentStockIndexCountPercent) & "% data points received (" & _
        Format$(CurentStockIndexCount) & " from " & _
        Format$(TotalStockIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStockIndexBeta = TaskFinished
    Exit Function
EH:
    LoadStockIndexBeta = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function

Private Sub pIVFeeder_SymbolAverageVolumes(ByVal Symbol As String, ByVal Volume10Day As Long, ByVal Volume5Expiration As Long)
    On Error Resume Next
        
    RaiseEvent SymbolAverageVolumes(Symbol, Volume10Day, Volume5Expiration)
    
    Dim i As Long
    CurentSymbolCount = CurentSymbolCount + 1
    i = 100 * CurentSymbolCount / (TotalStockCount + TotalIndexCount)
    If i <> CurentSymbolCountPercent Then
        CurentSymbolCountPercent = i
        RaiseEvent Status("Loading Symbol Average Volumes From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
End Sub

Private Sub pIVFeeder_SymbolIVBandPoint(ByVal Symbol As String, ByVal StrikeMoneyness As Long, ByVal ExpPeriod As Long, ByVal IVBandLow As Double, ByVal IVBandHigh As Double)
    On Error Resume Next
        
    RaiseEvent SymbolIVBandPoint(Symbol, StrikeMoneyness, ExpPeriod, IVBandLow, IVBandHigh)
    
    If CurentSymbol <> Symbol Then
        CurentSymbol = Symbol
        Dim i As Long
        CurentSymbolCount = CurentSymbolCount + 1
        i = 100 * CurentSymbolCount / (TotalStockCount + TotalIndexCount)
        If i <> CurentSymbolCountPercent Then
            CurentSymbolCountPercent = i
            RaiseEvent Status("Loading Symbol IV Bands From IVolatility " & Format$(i) & "%")
        End If
    End If

End Sub

Private Sub pIVFeeder_SymbolIVPoint(ByVal Symbol As String, ByVal Strike As Double, ByVal Expiration As Date, ByVal Vola As Double)
    On Error Resume Next
        
    RaiseEvent SymbolIVPoint(Symbol, Strike, Expiration, Vola)
    
    If CurentSymbol <> Symbol Then
        CurentSymbol = Symbol
        Dim i As Long
        CurentSymbolCount = CurentSymbolCount + 1
        i = 100 * CurentSymbolCount / (TotalStockCount + TotalIndexCount)
        If i <> CurentSymbolCountPercent Then
            CurentSymbolCountPercent = i
            RaiseEvent Status("Loading Symbol Default IV Surface From IVolatility " & Format$(i) & "%")
        End If
    End If
End Sub

Private Sub pIVFeeder_SymbolMSUpdateDate(ByVal Symbol As String, ByVal LastUpdate As Date)
    On Error Resume Next
    
    RaiseEvent SymbolMSUpdateDate(Symbol, LastUpdate)
    
    Dim i As Long
    CurentSymbolCount = CurentSymbolCount + 1
    i = 100 * CurentSymbolCount / (TotalStockCount + TotalIndexCount)
    If i <> CurentSymbolCountPercent Then
        CurentSymbolCountPercent = i
        RaiseEvent Status("Loading symbol last update date from IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Private Sub pIVFeeder_SymbolSkewKurtsis(ByVal Symbol As String, ByVal Skew As Double, ByVal Kurt As Double)
    On Error Resume Next
        
    RaiseEvent SymbolSkewKurtsis(Symbol, Skew, Kurt)
    
    Dim i As Long
    CurentSymbolCount = CurentSymbolCount + 1
    i = 100 * CurentSymbolCount / (TotalStockCount + TotalIndexCount)
    If i <> CurentSymbolCountPercent Then
        CurentSymbolCountPercent = i
        RaiseEvent Status("Loading Symbol Skew/Kurtosis From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolSkewKurtsis() As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    On Error GoTo EH
    TaskStarted
    RaiseEvent ProgressStart
    IndexSymbols = GetIndexSymbols
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
        
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Symbol Skew/Kurtosis From IVolatility: requesting data..")
                pIVFeeder.GetSymbolSkewKurtsis StockSymbols
            End If
        End If
    Next
    
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Symbol Skew/Kurtosis From IVolatility: requesting data..")
            pIVFeeder.GetSymbolSkewKurtsis IndexSymbols
        End If
    End If
    
    Dim Msg As String
    Msg = "Loading Symbol Skew Kurtosis From IVolatility: " & _
        Format$(CurentSymbolCountPercent) & "% data points received (" & _
        Format$(CurentSymbolCount) & " from " & _
        Format$(TotalStockCount + TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadSymbolSkewKurtsis = TaskFinished
    Exit Function
EH:
    LoadSymbolSkewKurtsis = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolAverageVolumes() As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    On Error GoTo EH
    TaskStarted
    RaiseEvent ProgressStart
    IndexSymbols = GetIndexSymbols
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
        
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Symbol Average Volumes From IVolatility: requesting data..")
                pIVFeeder.GetSymbolAverageVolumes StockSymbols
            End If
        End If
    Next
    
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Symbol Average Volumes From IVolatility: requesting data..")
            pIVFeeder.GetSymbolAverageVolumes IndexSymbols
        End If
    End If
    
    Dim Msg As String
    Msg = "Loading Symbol Average Volumes From IVolatility: " & _
        Format$(CurentSymbolCountPercent) & "% data points received (" & _
        Format$(CurentSymbolCount) & " from " & _
        Format$(TotalStockCount + TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadSymbolAverageVolumes = TaskFinished
    Exit Function
EH:
    LoadSymbolAverageVolumes = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockList(ByVal IsOptional As Integer) As Boolean
' Don't used this Function!!!!!!!!
' use LoadSymbolList instead!!!!!
' FOR BINARY COMPATIBILITY ONLY!
    
'    TaskStarted
'    RaiseEvent ProgressStart
'
'    CurentStockCount = 0
'    CurentStockCountPercent = 0
'
'    RaiseEvent Status("Loading Stock List From IVolatility: requesting data..")
'
'    pIVFeeder.GetStockList IsOptional
'
'    RaiseEvent ProgressStop
'
'    LoadStockList = TaskFinished
LoadStockList = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolList(ByVal IsOptional As Integer) As Boolean
    
    On Error GoTo EH
    TaskStarted
    RaiseEvent ProgressStart
        
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
    
    RaiseEvent Status("Loading Symbol List From IVolatility: requesting data..")
    
    pIVFeeder.GetSymbolList IsOptional
        
    RaiseEvent ProgressStop
    
    LoadSymbolList = TaskFinished
    Exit Function
EH:
    LoadSymbolList = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolDefaultIVSurface() As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    CurentSymbol = ""
    
    On Error GoTo EH
    TaskStarted
    IndexSymbols = GetIndexSymbols
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
        
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Symbol Default IV surface From IVolatility: requesting data..")
                pIVFeeder.GetSymbolIVSurface StockSymbols
            End If
        End If
    Next
    
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Symbol Default IV surface From IVolatility: requesting data..")
            pIVFeeder.GetSymbolIVSurface IndexSymbols
        End If
    End If
    
    Dim Msg As String
    Msg = "Loading Symbol Default IV surface From IVolatility: " & _
        Format$(CurentSymbolCountPercent) & "% symbol surfaces received (" & _
        Format$(CurentSymbolCount) & " from " & _
        Format$(TotalStockCount + TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    LoadSymbolDefaultIVSurface = TaskFinished
    Exit Function
EH:
    LoadSymbolDefaultIVSurface = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolIVBands() As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    CurentSymbol = ""
    
    On Error GoTo EH
    TaskStarted
    IndexSymbols = GetIndexSymbols
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
        
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Symbol IV Bands From IVolatility: requesting data..")
                pIVFeeder.GetSymbolIVBands StockSymbols
            End If
        End If
    Next
    
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Symbol IV Bands From IVolatility: requesting data..")
            pIVFeeder.GetSymbolIVBands IndexSymbols
        End If
    End If
    
    Dim Msg As String
    Msg = "Loading Symbol IV Bands From IVolatility: " & _
        Format$(CurentSymbolCountPercent) & "% symbol IV Bands received (" & _
        Format$(CurentSymbolCount) & " from " & _
        Format$(TotalStockCount + TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    LoadSymbolIVBands = TaskFinished
    Exit Function
EH:
    LoadSymbolIVBands = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
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
Public Function Busy() As Boolean
    Busy = TaskInProgress
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CancelTask() As Boolean
    CancelTask = Busy
    CancelRequested = True
    CancelAccepted = False
    pIVFeeder.CancelTask
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get Tenor() As Long
    Tenor = pIVFeeder.Tenor
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let Tenor(ByVal vNewValue As Long)
    pIVFeeder.Tenor = vNewValue
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadSymbolMSUpdateDateFromIV(ByVal LastUpdateDate As Date) As Boolean
    Dim IndexSymbols As String
    Dim StockSymbols As Variant
    
    On Error GoTo EH
    LoadSymbolMSUpdateDateFromIV = False
    TaskStarted
    RaiseEvent ProgressStart
    IndexSymbols = GetIndexSymbols
    CurentSymbolCount = 0
    CurentSymbolCountPercent = 0
        
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stocks Market Structure Update Dates From IVolatility: requesting data..")
                pIVFeeder.GetSymbolsMSUpdateDateFromIV StockSymbols, LastUpdateDate
            End If
        End If
    Next
    
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Indexes Market Structure Update Dates From IVolatility: requesting data..")
            pIVFeeder.GetSymbolsMSUpdateDateFromIV IndexSymbols, LastUpdateDate
        End If
    End If
    
    Dim Msg As String
    Msg = "Loading Symbols Market Structure Update Dates From IVolatility: " & _
        Format$(CurentSymbolCountPercent) & "% data points received (" & _
        Format$(CurentSymbolCount) & " from " & _
        Format$(TotalStockCount + TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadSymbolMSUpdateDateFromIV = TaskFinished
    Exit Function
EH:
    LoadSymbolMSUpdateDateFromIV = TaskFinished
    Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
End Function


