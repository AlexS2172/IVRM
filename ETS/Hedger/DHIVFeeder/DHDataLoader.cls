VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "DHDataLoader"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = True
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "DHDataLoader"

Private HttpFeeder1 As HttpFeeder

Public rsIndexData As ADODB.Recordset
Public rsStockData As ADODB.Recordset

Public Event NewStockData(IndexSymbol As String, rs As Recordset)

Public fLoadStocksCorrelationFromIV As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    Set HttpFeeder1 = New HttpFeeder
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    Set HttpFeeder1 = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function InitIndices()
    If rsIndexData Is Nothing Then
        Set rsIndexData = New ADODB.Recordset
        With rsIndexData
            .Fields.Append "vcIndexSymbol", adBSTR, 20
            .Fields.Append "IndexPrice", adDouble, , adFldIsNullable
            .Fields.Append "Yield", adDouble, , adFldIsNullable
            .Fields.Append "LotSize", adInteger, , adFldIsNullable
            
            .Fields.Append "bSelected", adBoolean
            .Fields.Append "iCountOfHedgedStocks", adInteger, , adFldIsNullable
            .Fields.Append "fQValue", adDouble, , adFldIsNullable
            .Fields.Append "fPremium", adDouble, , adFldIsNullable
            
            .Fields.Append "ExpirationDate", adDBDate, , adFldIsNullable
            .Fields.Append "Strike", adDouble, , adFldIsNullable
            .Fields.Append "CallPut", adBSTR, 3, adFldIsNullable
            .Fields.Append "OptionPrice", adDouble, , adFldIsNullable
            .Fields.Append "OptionDelta", adDouble, , adFldIsNullable
            .Fields.Append "OptionIV", adDouble, , adFldIsNullable
            .Fields.Append "iContractsAmount", adInteger, , adFldIsNullable
            .Fields.Append "iIndexContractsAmount", adInteger, , adFldIsNullable
            
            .Open
            .AddNew
            !vcIndexSymbol = "SPX"
            !bSelected = True
            .Update
            .AddNew
            !vcIndexSymbol = "NDX"
            !bSelected = True
            .Update
            .AddNew
            !vcIndexSymbol = "OEX"
            !bSelected = True
            .Update
            .AddNew
            !vcIndexSymbol = "DJX"
            !bSelected = True
            .Update
        End With
    Else
        With rsIndexData
            If .RecordCount > 0 Then
                .MoveFirst
                Do While Not .EOF
                    !iCountOfHedgedStocks = Null
                    !fQValue = Null
                    !fPremium = Null
                    .Update
                    .MoveNext
                Loop
            End If
        End With
    End If

End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockData()
Dim IndexSymbol As String
    Set rsStockData = New ADODB.Recordset
    
    With rsStockData
        .Fields.Append "vcIndexSymbol", adBSTR, 20
        .Fields.Append "vcStockSymbol", adBSTR, 20
        .Fields.Append "fEquivalentIndexIV", adDouble
        .Fields.Append "fBeta", adDouble
        .Fields.Append "fCorr", adDouble
        .Fields.Append "fSpecificVariance", adDouble
        .Fields.Append "fContributionToIndexVolatility", adDouble
        .Fields.Append "fCurrentHV", adDouble
        .Fields.Append "fCurrentIV", adDouble
        .Open
    End With
    
    '!!!
'    fLoadStocksCorrelationFromIV = False
    fLoadStocksCorrelationFromIV = True
    With rsIndexData
        If .RecordCount > 0 Then
            .MoveFirst
            Do While Not .EOF
                IndexSymbol = !vcIndexSymbol
                LoadStocksCorrelation rsStockData, IndexSymbol
                .MoveNext
            Loop
        End If
    End With

End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksCorrelation(rs As ADODB.Recordset, ByVal IndexSymbol As String) As Boolean
    On Error GoTo HerrIV
    If fLoadStocksCorrelationFromIV Then
        LoadStocksCorrelation = LoadStocksCorrelationFromIV(rs, IndexSymbol)
        fLoadStocksCorrelationFromIV = LoadStocksCorrelation
    End If
    If Not LoadStocksCorrelation Then
        LoadStocksCorrelation = LoadStocksCorrelationFromFile(rs, IndexSymbol)
    End If
    Exit Function
HerrIV:
    MsgBox "Loading stocks correlation data from IV for Index symbol: " _
        & IndexSymbol _
        & " failed due to the following error:" & vbCrLf _
        & Err.Description
    Resume Next
    Exit Function
Herr:
    MsgBox "Loading stocks correlation data from file for Index symbol: " _
        & IndexSymbol _
        & " failed due to the following error:" & vbCrLf _
        & Err.Description
    Resume Next
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksCorrelationFromFile(rs As ADODB.Recordset, ByVal IndexSymbol As String) As Boolean
Dim FileName As String
Dim Buffer As String
    On Error GoTo Herr
    FileName = App.Path & "\Data\" & IndexSymbol & ".txt"
    Buffer = ReadFromFile(FileName)
    LoadStocksCorrelationFromFile = LoadStocksCorrelationFromText(rs, IndexSymbol, Buffer)
    Exit Function
Herr:
    Err.Raise vbObjectError, "Load IV data", "Loading Stocks correlation data from file failed due to the following error: " & vbCrLf & Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksCorrelationFromText(rs As ADODB.Recordset, ByVal IndexSymbol As String, ByVal Buffer As String) As Boolean
Dim Lines As Collection
Dim Items As Collection
Dim L As Variant
Dim i1 As Variant
Dim i2 As Variant
Dim i3 As Variant
Dim StockSymbol As String
Dim EquivalentIndexIV As Double
Dim BetaToIndex As Double
Dim CorrelationToIndex As Double
Dim SpecificVariance As Double
Dim ContributionToIndexVolatility As Double
Dim CurrentHV As Double
Dim CurrentIV As Double

Dim S As String
    On Error GoTo Herr
    If Buffer = "" Then
        Err.Raise vbObjectError, "Load Stocks/Index correlation data", "Buffer is empty."
        Exit Function
    End If
    Set Lines = SplitLines(Buffer)
    If Not Lines Is Nothing Then
        For Each L In Lines
            i1 = InStr(L, ":")
            If i1 > 0 Then
                L = Mid(L, i1 + 1)
            Else
                Exit Function
            End If
            
            Set Items = SplitItems(L, ",")
            StockSymbol = GetItem(Items, 1)
            EquivalentIndexIV = GetItemDouble(Items, 2)
            BetaToIndex = GetItemDouble(Items, 3)
            CorrelationToIndex = GetItemDouble(Items, 4)
            SpecificVariance = GetItemDouble(Items, 5)
            ContributionToIndexVolatility = GetItemDouble(Items, 6)
            CurrentHV = GetItemDouble(Items, 7)
            CurrentIV = GetItemDouble(Items, 8)
            
            With rs
                .AddNew
                !vcIndexSymbol = IndexSymbol
                !vcStockSymbol = StockSymbol
                !fEquivalentIndexIV = EquivalentIndexIV
                !fBeta = BetaToIndex
                !fCorr = CorrelationToIndex
                !fSpecificVariance = SpecificVariance
                !fContributionToIndexVolatility = ContributionToIndexVolatility
                !fCurrentHV = CurrentHV
                !fCurrentIV = CurrentIV
                .Update
            End With
            DoEvents
        Next
        RaiseEvent NewStockData(IndexSymbol, rs)
    End If
    LoadStocksCorrelationFromText = True
    Exit Function
Herr:
    Err.Raise vbObjectError, "Load IV data", "Parsing of the text buffer failed due to the followitf error: " & vbCrLf & Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStocksCorrelationFromIV(rs As ADODB.Recordset, ByVal IndexSymbol As String) As Boolean
Dim Address As String
Dim Buffer As String
Dim Moment As Date
    On Error GoTo Herr
    'www.ivolatility.com
    '192.168.8.7
    Moment = Date - 1
    Address = "www.ivolatility.com/data_load/ivfeed_data_load.asp?si=0" & _
        "&I=" & IndexSymbol & _
        "&T=30" & _
        "&D=" & Format$(Month(Moment), "00") & "-" & Format$(Day(Moment), "00") & "-" & Format$(Year(Moment), "0000")
    '//////////////////////////////////////////
    Buffer = HttpFeeder1.HttpCall(Address)  '/
    '////////////////////////////////////////
    LoadStocksCorrelationFromIV = LoadStocksCorrelationFromText(rs, IndexSymbol, Buffer)
    Exit Function
Herr:
    Err.Raise vbObjectError, "Load IV data", "Loading Stocks correlation data from iV server failed due to the following error: " & vbCrLf & Err.Description
End Function

