VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DHIVLoader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True

'
Option Explicit

Private Const THIS = "DHIVLoader"

Private WithEvents pDHIVFeeder As DHIVFeeder
Attribute pDHIVFeeder.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean

Public Event Status(ByVal Message As String)
Public Event LogMessage(ByVal Message As String)
Public Event ProgressStart()
Public Event ProgressStop()
Public Event Progress(ByVal ProgressPercent As Long)

Public Event GotStockPriceData4(ByVal StockSymbol As String, ByVal PriceClose As Double, ByVal PriceDate As Date)
Public Event GotIndexPriceData4(ByVal IndexSymbol As String, ByVal PriceClose As Double, ByVal PriceDate As Date)
Public Event GotStockSymbolData4(ByVal StockSymbol As String, ByVal IndexSymbol As String)

Public Event GotStockIndexCorrData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotStockStockCorrData4(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotIndexIndexCorrData4(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Public Event GotStockHVData4(ByVal StockSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
Public Event GotIndexHVData4(ByVal IndexSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
Public Event GotIndexVegaData4(ByVal IndexSymbol As String, ByVal Vega As Double)

Public Event GotStockIndexBetaData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Weight As Double, ByVal Beta As Double)

Public Event GotIndexIVData5(ByVal IndexSymbol As String, ByVal IVIndexVola As Double, ByVal HV As Double, ByVal IVMin As Double, ByVal IVMax As Double)
Public Event GotStockIVData5(ByVal StockSymbol As String, ByVal IVIndexVola As Double, ByVal HV As Double, ByVal IVMin As Double, ByVal IVMax As Double)

Public Event GotStockIndexCorrData5(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double)
Public Event GotStockStockCorrData5(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal VCorr As Double)
Public Event GotIndexIndexCorrData5(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal VCorr As Double)

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
Public CurentStockCountPercent As Long
Public CurentIndexCountPercent As Long
Public CurentStockStockCountPercent As Long
Public CurentStockIndexCountPercent As Long
Public CurentIndexIndexCountPercent As Long

Public g_SymbolsCountLimit As Long

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set pDHIVFeeder = New DHIVFeeder
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
    Set pDHIVFeeder = Nothing
    VolSetSettingLong "SymbolsCountLimit", g_SymbolsCountLimit
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetDHIVFeeder() As DHIVFeeder
    Set GetDHIVFeeder = pDHIVFeeder
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_LogMessage(ByVal Message As String)
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
Dim Criteria As String
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
            Criteria = "IndexSymbol='" & IndexSymbol & "'"
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
Dim Criteria As String
    If rsStockData Is Nothing Then
        StocksListClear
    Else
        If Not ValidateStockSymbol(StockSymbol) Then
            Exit Sub
        End If
        With rsStockData
            If .RecordCount > 0 Then
                .MoveFirst
            End If
            Criteria = "StockSymbol='" & StockSymbol & "'"
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
Dim IndexSymbol
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
Dim StockSymbol
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

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetStockSymbolsEx() As Collection
Dim c1 As Collection
Dim c2 As New Collection
Dim u As String
Dim v As String
Dim i As Long
Dim j As Long
Dim n As Long
    Set c1 = GetStockSymbols
    n = c1.Count
    For i = 1 To n
        u = c1.Item(i)
        c2.Add u
        For j = i + 1 To n
            v = c1.Item(j)
            c2.Add u & ";" & v
        Next
    Next
    Set GetStockSymbolsEx = c2
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockSymbolData4(ByVal StockSymbol As String, ByVal IndexSymbol As String)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    Debug.Print "GotStockSymbolData", StockSymbol
    RaiseEvent GotStockSymbolData4(StockSymbol, IndexSymbol)
'    RaiseEvent Status("Loading Stock List From IVolatility " & StockSymbol)
    
    On Error Resume Next
    Dim i As Long
    CurentStockCount = CurentStockCount + 1
    i = 100 * CurentStockCount / TotalStockCount
    If i <> CurentStockCountPercent Then
        CurentStockCountPercent = i
'        RaiseEvent Status("Loading Stock List From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    
    With rsStockData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "StockSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If .EOF Then
            .AddNew
            !StockSymbol = StockSymbol
        End If
        !Supported = True
        !Selected = True
        .Update
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockHVData4(ByVal StockSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    ' Check if it is index?
    With rsIndexData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "IndexSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If Not .EOF Then
            !HV = HV
            !IV = IV
            !VolOfVol = VolOfVol
            .Update
            RaiseEvent GotIndexHVData4(StockSymbol, HV, IV, VolOfVol)
'            RaiseEvent Status("Loading Index HV From IVolatility for " & StockSymbol)
            
            On Error Resume Next
            Dim i As Long
            CurentIndexCount = CurentIndexCount + 1
            i = 100 * CurentIndexCount / TotalIndexCount
            If i <> CurentIndexCountPercent Then
                CurentIndexCountPercent = i
'                RaiseEvent Status("Loading Index HV From IVolatility " & Format$(i) & "%")
                RaiseEvent Progress(i)
            End If
            
            Exit Sub
        End If
    End With
    
    ' Not index, then stock
    RaiseEvent GotStockHVData4(StockSymbol, HV, IV, VolOfVol)
'    RaiseEvent Status("Loading Stock HV From IVolatility for " & StockSymbol)
    
    On Error Resume Next
    CurentStockCount = CurentStockCount + 1
    i = 100 * CurentStockCount / TotalStockCount
    If i <> CurentStockCountPercent Then
        CurentStockCountPercent = i
'        RaiseEvent Status("Loading Stock HV From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    
    With rsStockData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "StockSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If .EOF Then
            .AddNew
            !StockSymbol = StockSymbol
        End If
        !HV = HV
        !IV = IV
        !VolOfVol = VolOfVol
        .Update
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockIVData5(ByVal StockSymbol As String, ByVal IVIndexVola As Double, ByVal HV As Double, ByVal IVMin As Double, ByVal IVMax As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    ' Check if it is index?
    With rsIndexData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "IndexSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If Not .EOF Then
            RaiseEvent GotIndexIVData5(StockSymbol, IVIndexVola, HV, IVMin, IVMax)
            
            On Error Resume Next
            Dim i As Long
            CurentIndexCount = CurentIndexCount + 1
            i = 100 * CurentIndexCount / TotalIndexCount
            If i <> CurentIndexCountPercent Then
                CurentIndexCountPercent = i
                RaiseEvent Progress(i)
            End If
            
            Exit Sub
        End If
    End With
    
    ' Not index, then stock
    RaiseEvent GotStockIVData5(StockSymbol, IVIndexVola, HV, IVMin, IVMax)
    
    On Error Resume Next
    CurentStockCount = CurentStockCount + 1
    i = 100 * CurentStockCount / TotalStockCount
    If i <> CurentStockCountPercent Then
        CurentStockCountPercent = i
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockPriceData4(ByVal StockSymbol As String, ByVal ClosePrice As Double, ByVal PriceDate As Date)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    ' Check if it is index?
    With rsIndexData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "IndexSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If Not .EOF Then
            !ClosePrice = ClosePrice
            .Update
            RaiseEvent GotIndexPriceData4(StockSymbol, ClosePrice, PriceDate)
'            RaiseEvent Status("Loading Index Price From IVolatility for " & StockSymbol)
            
            On Error Resume Next
            Dim i As Long
            CurentIndexCount = CurentIndexCount + 1
            i = 100 * CurentIndexCount / TotalIndexCount
            If i <> CurentIndexCountPercent Then
                CurentIndexCountPercent = i
'                RaiseEvent Status("Loading Index Price From IVolatility " & Format$(i) & "%")
                RaiseEvent Progress(i)
            End If
            
            Exit Sub
        End If
    End With
    
    ' Not index, then stock
    RaiseEvent GotStockPriceData4(StockSymbol, ClosePrice, PriceDate)
'    RaiseEvent Status("Loading Stock Price From IVolatility for " & StockSymbol)
    
    On Error Resume Next
    CurentStockCount = CurentStockCount + 1
    i = 100 * CurentStockCount / TotalStockCount
    If i <> CurentStockCountPercent Then
        CurentStockCountPercent = i
'        RaiseEvent Status("Loading Stock Price From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    
    With rsStockData
        If .RecordCount > 0 Then
            .MoveFirst
        End If
        Criteria = "StockSymbol='" & StockSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If .EOF Then
            .AddNew
            !StockSymbol = StockSymbol
        End If
        !ClosePrice = ClosePrice
        .Update
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotIndexVegaData4(ByVal IndexSymbol As String, ByVal Vega As Double)
    If Not ValidateStockSymbol(IndexSymbol) Then
        Exit Sub
    End If
    RaiseEvent GotIndexVegaData4(IndexSymbol, Vega)
    
    On Error Resume Next
    Dim i As Long
    CurentIndexCount = CurentIndexCount + 1
    i = 100 * CurentIndexCount / TotalIndexCount
    If i <> CurentIndexCountPercent Then
        CurentIndexCountPercent = i
'        RaiseEvent Status("Loading Index Vega From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockIndexBetaData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Weight As Double, ByVal Beta As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    RaiseEvent GotStockIndexBetaData4(StockSymbol, IndexSymbol, Weight, Beta)
    
    On Error Resume Next
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
Private Sub pDHIVFeeder_GotStockIndexCorrData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    RaiseEvent GotStockIndexCorrData4(StockSymbol, IndexSymbol, Corr, VCorr)
'    RaiseEvent Status("Loading Stock/Index Corr From IVolatility for " & StockSymbol & "/" & IndexSymbol)
    
    On Error Resume Next
    Dim i As Long
    CurentStockIndexCount = CurentStockIndexCount + 1
    i = 100 * CurentStockIndexCount / TotalStockIndexCount
    If i <> CurentStockIndexCountPercent Then
        CurentStockIndexCountPercent = i
        RaiseEvent Status("Loading Stock/Index Corr From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    
'    With rsStockIndexData.Clone
'        .Filter = "StockSymbol='" & StockSymbol & "'"
'        If .RecordCount > 0 Then
'            .MoveFirst
'        End If
'        Criteria = "IndexSymbol='" & IndexSymbol & "'"
'        .Find Criteria, 0, adSearchForward, adBookmarkFirst
'        If .EOF Then
'            .AddNew
'            !StockSymbol = StockSymbol
'            !IndexSymbol = IndexSymbol
'        End If
'        !Corr = Corr
'        !VCorr = VCorr
'        .Update
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockIndexCorrData5(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol) Then
        Exit Sub
    End If
    
    RaiseEvent GotStockIndexCorrData5(StockSymbol, IndexSymbol, VCorr)
    
    On Error Resume Next
    Dim i As Long
    CurentStockIndexCount = CurentStockIndexCount + 1
    i = 100 * CurentStockIndexCount / TotalStockIndexCount
    If i <> CurentStockIndexCountPercent Then
        CurentStockIndexCountPercent = i
        RaiseEvent Status("Loading Stock/Index Corr From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockStockCorrData4(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol1) Then
        Exit Sub
    End If
    If Not ValidateStockSymbol(StockSymbol2) Then
        Exit Sub
    End If
    
    RaiseEvent GotStockStockCorrData4(StockSymbol1, StockSymbol2, Corr, VCorr)
'    RaiseEvent Status("Loading Stock/Stock Corr From IVolatility for " & StockSymbol1 & "/" & StockSymbol2)
    
    On Error Resume Next
    Dim i As Long
    CurentStockStockCount = CurentStockStockCount + 1
    i = 100 * CurentStockStockCount / TotalStockStockCount
    If i <> CurentStockStockCountPercent Then
        CurentStockStockCountPercent = i
        RaiseEvent Status("Loading Stock/Stock Corr From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

'    If StockSymbol1 > StockSymbol2 Then
'        Criteria = StockSymbol1
'        StockSymbol1 = StockSymbol2
'        StockSymbol2 = Criteria
'    End If
'    With rsStockStockData.Clone
'        .Filter = "StockSymbol1='" & StockSymbol1 & "'"
'        If .RecordCount > 0 Then
'            .MoveFirst
'        End If
'        Criteria = "StockSymbol2='" & StockSymbol2 & "'"
'        .Find Criteria, 0, adSearchForward, adBookmarkFirst
'        If .EOF Then
'            .AddNew
'            !StockSymbol1 = StockSymbol1
'            !StockSymbol2 = StockSymbol2
'        End If
'        !Corr = Corr
'        !VCorr = VCorr
'        .Update
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotStockStockCorrData5(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(StockSymbol1) Then
        Exit Sub
    End If
    If Not ValidateStockSymbol(StockSymbol2) Then
        Exit Sub
    End If
    
    RaiseEvent GotStockStockCorrData5(StockSymbol1, StockSymbol2, VCorr)
    
    On Error Resume Next
    Dim i As Long
    CurentStockStockCount = CurentStockStockCount + 1
    i = 100 * CurentStockStockCount / TotalStockStockCount
    If i <> CurentStockStockCountPercent Then
        CurentStockStockCountPercent = i
        RaiseEvent Status("Loading Stock/Stock Corr From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotIndexIndexCorrData4(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(IndexSymbol1) Then
        Exit Sub
    End If
    If Not ValidateStockSymbol(IndexSymbol2) Then
        Exit Sub
    End If
    
    RaiseEvent GotIndexIndexCorrData4(IndexSymbol1, IndexSymbol2, Corr, VCorr)
    'RaiseEvent Status("Loading Index/Index Corr From IVolatility for " & IndexSymbol1 & "/" & IndexSymbol2)
    
    On Error Resume Next
    Dim i As Long
    CurentIndexIndexCount = CurentIndexIndexCount + 1
    i = 100 * CurentIndexIndexCount / TotalIndexIndexCount
    If i <> CurentIndexIndexCountPercent Then
        CurentIndexIndexCountPercent = i
'        RaiseEvent Status("Loading Index/Index Corr From IVolatility " & Format$(i) & "%")
        RaiseEvent Progress(i)
    End If

'    If IndexSymbol1 > IndexSymbol2 Then
'        Criteria = IndexSymbol1
'        IndexSymbol1 = IndexSymbol2
'        IndexSymbol2 = Criteria
'    End If
'    With rsIndexIndexData.Clone
'        .Filter = "IndexSymbol1='" & IndexSymbol1 & "'"
'        If .RecordCount > 0 Then
'            .MoveFirst
'        End If
'        Criteria = "IndexSymbol2='" & IndexSymbol2 & "'"
'        .Find Criteria, 0, adSearchForward, adBookmarkFirst
'        If .EOF Then
'            .AddNew
'            !IndexSymbol1 = IndexSymbol1
'            !IndexSymbol2 = IndexSymbol2
'        End If
'        !Corr = Corr
'        !VCorr = VCorr
'        .Update
'    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub pDHIVFeeder_GotIndexIndexCorrData5(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal VCorr As Double)
Dim Criteria As String
    
    If Not ValidateStockSymbol(IndexSymbol1) Then
        Exit Sub
    End If
    If Not ValidateStockSymbol(IndexSymbol2) Then
        Exit Sub
    End If
    
    RaiseEvent GotIndexIndexCorrData5(IndexSymbol1, IndexSymbol2, VCorr)
    
    On Error Resume Next
    Dim i As Long
    CurentIndexIndexCount = CurentIndexIndexCount + 1
    i = 100 * CurentIndexIndexCount / TotalIndexIndexCount
    If i <> CurentIndexIndexCountPercent Then
        CurentIndexIndexCountPercent = i
        RaiseEvent Progress(i)
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksHVFromIV() As Boolean
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockCount = 0
    CurentStockCountPercent = 0
'    RaiseEvent Status("Loading Stocks HV From IVolatility")
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stocks HV From IVolatility: requesting data..")
                pDHIVFeeder.GetStocksHVFromIV StockSymbols
            End If
        End If
    Next
    Dim Msg As String
    Msg = "Loading Stocks HV From IVolatility: " & _
        Format$(CurentStockCountPercent) & "% data points received (" & _
        Format$(CurentStockCount) & " from " & _
        Format$(TotalStockCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStocksHVFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksIVData5() As Boolean
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockCount = 0
    CurentStockCountPercent = 0
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stocks IV From IVolatility: requesting data..")
                pDHIVFeeder.GetStocksIVData5 StockSymbols
            End If
        End If
    Next
    Dim Msg As String
    Msg = "Loading Stocks IV From IVolatility: " & _
        Format$(CurentStockCountPercent) & "% data points received (" & _
        Format$(CurentStockCount) & " from " & _
        Format$(TotalStockCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStocksIVData5 = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksPriceFromIV() As Boolean
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockCount = 0
    CurentStockCountPercent = 0
'    RaiseEvent Status("Loading Stock Price From IVolatility")
    For Each StockSymbols In GetStockSymbols
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stocks Price From IVolatility: requesting data..")
                pDHIVFeeder.GetStockPriceFromIV StockSymbols
            End If
        End If
    Next
    Dim Msg As String
    Msg = "Loading Stocks Price From IVolatility: " & _
        Format$(CurentStockCountPercent) & "% data points received (" & _
        Format$(CurentStockCount) & " from " & _
        Format$(TotalStockCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStocksPriceFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexHVFromIV() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentIndexCount = 0
    CurentIndexCountPercent = 0
'    RaiseEvent Status("Loading Index HV From IVolatility")
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index HV From IVolatility: requesting data..")
            pDHIVFeeder.GetStocksHVFromIV IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index HV From IVolatility: " & _
        Format$(CurentIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexCount) & " from " & _
        Format$(TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexHVFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexIVData5() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentIndexCount = 0
    CurentIndexCountPercent = 0
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index IV From IVolatility: requesting data..")
            pDHIVFeeder.GetStocksIVData5 IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index IV From IVolatility: " & _
        Format$(CurentIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexCount) & " from " & _
        Format$(TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexIVData5 = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexesVegaFromIV() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentIndexCount = 0
    CurentIndexCountPercent = 0
'    RaiseEvent Status("Loading Index Vega From IVolatility")
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index Vega From IVolatility: requesting data..")
            pDHIVFeeder.GetIndexVegaFromIV IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index Vega From IVolatility: " & _
        Format$(CurentIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexCount) & " from " & _
        Format$(TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexesVegaFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexesPriceFromIV() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentIndexCount = 0
    CurentIndexCountPercent = 0
'    RaiseEvent Status("Loading Index Prices From IVolatility")
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index Price From IVolatility: requesting data..")
            pDHIVFeeder.GetStockPriceFromIV IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index Price From IVolatility: " & _
        Format$(CurentIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexCount) & " from " & _
        Format$(TotalIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexesPriceFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockStockCorrFromIV() As Boolean
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockStockCount = 0
    CurentStockStockCountPercent = 0
'    RaiseEvent Status("Loading Stock/Stock Corr From IVolatility")
    For Each StockSymbols In GetStockSymbolsEx
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stock/Stock Corr From IVolatility: requesting data..")
                pDHIVFeeder.GetStockStockCorrFromIV StockSymbols
            End If
        End If
    Next
    Dim Msg As String
    Msg = "Loading Stock/Stock Corr From IVolatility: " & _
        Format$(CurentStockStockCountPercent) & "% data points received (" & _
        Format$(CurentStockStockCount) & " from " & _
        Format$(TotalStockStockCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStockStockCorrFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockStockVCorr5() As Boolean
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockStockCount = 0
    CurentStockStockCountPercent = 0
'    RaiseEvent Status("Loading Stock/Stock Corr From IVolatility")
    For Each StockSymbols In GetStockSymbolsEx
        If Len(StockSymbols) > 0 Then
            If Not CancelRequested Then
                RaiseEvent Status("Loading Stock/Stock VCorr From IVolatility: requesting data..")
                pDHIVFeeder.GetStockStockVCorr5 StockSymbols
            End If
        End If
    Next
    Dim Msg As String
    Msg = "Loading Stock/Stock VCorr From IVolatility: " & _
        Format$(CurentStockStockCountPercent) & "% data points received (" & _
        Format$(CurentStockStockCount) & " from " & _
        Format$(TotalStockStockCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStockStockVCorr5 = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexIndexCorrFromIV() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
'    RaiseEvent Status("Loading Index/Index Corr From IVolatility")
    CurentIndexIndexCount = 0
    CurentIndexIndexCountPercent = 0
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index/Index Corr From IVolatility: requesting data..")
            pDHIVFeeder.GetIndexIndexCorrFromIV IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index/Index Corr From IVolatility: " & _
        Format$(CurentIndexIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexIndexCount) & " from " & _
        Format$(TotalIndexIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexIndexCorrFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexIndexVCorr5() As Boolean
Dim IndexSymbols
    TaskStarted
    RaiseEvent ProgressStart
    CurentIndexIndexCount = 0
    CurentIndexIndexCountPercent = 0
    IndexSymbols = GetIndexSymbols
    If Len(IndexSymbols) > 0 Then
        If Not CancelRequested Then
            RaiseEvent Status("Loading Index/Index VCorr From IVolatility: requesting data..")
            pDHIVFeeder.GetIndexIndexVCorr5 IndexSymbols
        End If
    End If
    Dim Msg As String
    Msg = "Loading Index/Index VCorr From IVolatility: " & _
        Format$(CurentIndexIndexCountPercent) & "% data points received (" & _
        Format$(CurentIndexIndexCount) & " from " & _
        Format$(TotalIndexIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadIndexIndexVCorr5 = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIndexCorrFromIV() As Boolean
Dim IndexSymbols As String
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
'    RaiseEvent Status("Loading Stock/Index Corr From IVolatility")
    IndexSymbols = GetIndexSymbols
    CurentStockIndexCount = 0
    CurentStockIndexCountPercent = 0
    If Len(IndexSymbols) > 0 Then
        For Each StockSymbols In GetStockSymbols
            If Len(StockSymbols) > 0 Then
                If Not CancelRequested Then
                    RaiseEvent Status("Loading Stock/Index Corr From IVolatility: requesting data..")
                    pDHIVFeeder.GetStockIndexCorrFromIV IndexSymbols, StockSymbols
                End If
            End If
        Next
    End If
    Dim Msg As String
    Msg = "Loading Stock/Index Corr From IVolatility: " & _
        Format$(CurentStockIndexCountPercent) & "% data points received (" & _
        Format$(CurentStockIndexCount) & " from " & _
        Format$(TotalStockIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStockIndexCorrFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIndexVCorr5() As Boolean
Dim IndexSymbols As String
Dim StockSymbols
    TaskStarted
    RaiseEvent ProgressStart
    IndexSymbols = GetIndexSymbols
    CurentStockIndexCount = 0
    CurentStockIndexCountPercent = 0
    If Len(IndexSymbols) > 0 Then
        For Each StockSymbols In GetStockSymbols
            If Len(StockSymbols) > 0 Then
                If Not CancelRequested Then
                    RaiseEvent Status("Loading Stock/Index VCorr From IVolatility: requesting data..")
                    pDHIVFeeder.GetStockIndexVCorr5 IndexSymbols, StockSymbols
                End If
            End If
        Next
    End If
    Dim Msg As String
    Msg = "Loading Stock/Index VCorr From IVolatility: " & _
        Format$(CurentStockIndexCountPercent) & "% data points received (" & _
        Format$(CurentStockIndexCount) & " from " & _
        Format$(TotalStockIndexCount) & " expected)."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStockIndexVCorr5 = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIndexBetaFromIV() As Boolean
Dim IndexSymbols As String
Dim StockSymbols
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
                    pDHIVFeeder.GetStockIndexBetaFromIV IndexSymbols, StockSymbols
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
    LoadStockIndexBetaFromIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksListFromIV() As Boolean
    TaskStarted
    RaiseEvent ProgressStart
    CurentStockCount = 0
    CurentStockCountPercent = 0
    'Find TotalStockCount
    GetStockSymbols
    If TotalStockCount = 0 Then
        ' Real stock count on Dec 2001
        'TotalStockCount = 540
        ' Duplicated stock count
        TotalStockCount = 740
    End If
    RaiseEvent Status("Loading Stock List From IVolatility: requesting data..")
    pDHIVFeeder.GetStocksListFromIV
    Dim Msg As String
    GetStockSymbols
    Msg = "Loading Stock List From IVolatility: " & _
        Format$(TotalStockCount) & " data points received."
    RaiseEvent Status(Msg)
    RaiseEvent LogMessage(Msg)
    RaiseEvent ProgressStop
    LoadStocksListFromIV = TaskFinished
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
    pDHIVFeeder.CancelTask
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get HVTenor() As Long
    HVTenor = pDHIVFeeder.HVTenor
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let HVTenor(ByVal vNewValue As Long)
    pDHIVFeeder.HVTenor = vNewValue
End Property

