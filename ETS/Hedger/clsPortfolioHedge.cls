VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "PortfolioHedger"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
Option Compare Text

Public Enum enmDATA_STATUS
    DATA_NORMAL
    DATA_FEED
    DATA_MANUAL
End Enum

Private Const DEFAULT_CORRELATION_BARIER = 0.2
Private Const DEFAULT_GAIN_BARIER = 0.2
Private Const DEFAULT_TERM = 252

Public fPerExpiration As Boolean
Public dPerExpirationMonth As Long
Public dPerExpirationYear As Long

Public g_dGainBarrier As Double
Public CorrelationBarrier As Double
Public HedgeMode As String

Public g_dCorrelationBarrier As Double

Public g_SelectSPX As Boolean
Public g_SelectNDX As Boolean
Public g_SelectOEX As Boolean
Public g_SelectDJX As Boolean

Public fVegaMode As Boolean
Public Event VegaModeChanged(ByVal NewVegaMode As Boolean)
Public Event AdviseVegaMode(ByVal NewVegaMode As Boolean)

Public fShortPortfolio As Boolean
Public Event ShortPortfolioModeChanged(ByVal NewShortPortfolioMode As Boolean)

Public g_lTerm As Long
Public Event HVTermChanged(ByVal NewHVTerm As Long)

Public Event Status(ByVal Message As String)
Public Event LogMessage(ByVal Message As String)
Public Event GotStartTask(ByVal Caption As String, ByVal CanCancel As Boolean)
Public Event GotStopTask(ByVal Caption As String)
Public Event GotCancelTask()
Public Event ProgressStart()
Public Event ProgressStop()
Public Event Progress(ByVal ProgressPercent As Long)
Public Event ShowHelpTipsChanged()

Public Event ClearResults()
Public Event NewExpiry(rs As Recordset)
Public Event NewPortfolio(rs As Recordset)
Public Event NewSubPortfolio(rs As Recordset)
Public Event NewIndexData(rs As Recordset)
Public Event GotSubportfolioTotals(ByVal Q As Double, ByVal D2 As Double, ByVal d3 As Double, ByVal Gain As Double, ByVal Q1 As Double, ByVal Q2 As Double, ByVal Q3 As Double, ByVal Q4 As Double, ByVal HedgeSymbol As String)
Public Event GotSubportfolioTotals2(ByVal d1 As Long, ByVal D2 As Long, ByVal d3 As Double, ByVal D4 As Double, ByVal HedgeSymbol As String)

Public Event GotSymbolsChanged(rsChangedSymbols As ADODB.Recordset)
Public Event GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotIndexPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotIndexHVData(ByVal IndexSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotStockHVData3(ByVal StockSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotIndexHVData3(ByVal IndexSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotStockIndexCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotStockIndexVCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotIndexVega(ByVal IndexSymbol As String, ByVal Vega As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
Public Event GotIndexIVData(ByVal IndexSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)

Public rsPortfolioData As ADODB.Recordset
Public rsPositions As ADODB.Recordset
Public rsIndexOptions As ADODB.Recordset
Public rsExpiry As ADODB.Recordset

Public rsSymbolsList As ADODB.Recordset
Public rsTerms As ADODB.Recordset

Public fNeedReconnectMT As Boolean
Public FeedInProgress As Boolean
Public fLoadStocksCorrelationFromIV As Boolean
Public NeedSplitPortfolio As Boolean
Public NeedSavePortfolio As Boolean
Public NeedUpdateExpiry As Boolean

Public mPortfolioName As String
Public Event PortfolioNameChanged(ByVal sNewValue As String)

Private IndexIndexCorrelator As New CIndexIndexCorrelator
Private StockStockCorrelator As CStockStockCorrelator
Private IsMapped As Boolean

Private WithEvents DHIVLoader5 As DHIVFeeder5.DHIVLoader
Attribute DHIVLoader5.VB_VarHelpID = -1
Private WithEvents DHMTLoader1 As DHMTFeeder2.DHMTLoader
Attribute DHMTLoader1.VB_VarHelpID = -1
Private WithEvents DHHttpFeeder As HttpFeeder
Attribute DHHttpFeeder.VB_VarHelpID = -1

Private TaskInProgress As Boolean
Private CancelRequested As Boolean
Private CancelAccepted As Boolean
Public fNeedRefreshPortfolio As Boolean
Public fNeedRescan As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Initialize()
    On Error GoTo Herr
    LoadHedgeParameters
    IsMapped = False
    fNeedRescan = False
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadHedgeParameters()
    On Error Resume Next
    VegaMode = MyGetSettingBool("VegaHedge", False)
    CorrelationBarrier = MyGetSettingDouble("CorrelationBarrier", DEFAULT_CORRELATION_BARIER)
    g_dGainBarrier = MyGetSettingDouble("GainBarrier", DEFAULT_GAIN_BARIER)
    g_lTerm = MyGetSettingLong("Term", DEFAULT_TERM)
    ValidateHVTerm
    g_SelectSPX = MyGetSettingBool("SelectSPX", True)
    g_SelectNDX = MyGetSettingBool("SelectNDX", True)
    g_SelectOEX = MyGetSettingBool("SelectOEX", True)
    g_SelectDJX = MyGetSettingBool("SelectDJX", True)
'    fPerExpiration = MyGetSettingBool("PerExpiration", True)
    fPerExpiration = True
    dPerExpirationMonth = MyGetSettingLong("PerExpirationMonth", Month(Date))
    dPerExpirationMonth = 0
    dPerExpirationYear = MyGetSettingLong("PerExpirationYear", Year(Date))
    dPerExpirationYear = 0
    fShortPortfolio = MyGetSettingBool("ShortPortfolio", True)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SaveHedgeParameters()
    On Error Resume Next
    MySaveSettingDouble "CorrelationBarrier", CorrelationBarrier
    MySaveSettingDouble "GainBarrier", g_dGainBarrier
    MySaveSettingBool "SelectSPX", g_SelectSPX
    MySaveSettingBool "SelectNDX", g_SelectNDX
    MySaveSettingBool "SelectOEX", g_SelectOEX
    MySaveSettingBool "SelectDJX", g_SelectDJX
    MySaveSettingBool "PerExpiration", fPerExpiration
    MySaveSettingLong "PerExpirationMonth", dPerExpirationMonth
    MySaveSettingLong "PerExpirationYear", dPerExpirationYear
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Class_Terminate()
    On Error Resume Next
    SaveHedgeParameters
    If Not DHIVLoader5 Is Nothing Then
        DHIVLoader5.CancelTask
    End If
    If Not DHMTLoader1 Is Nothing Then
        DHMTLoader1.CancelTask
    End If
    Set DHIVLoader5 = Nothing
    Set DHMTLoader1 = Nothing
    Set DHHttpFeeder = Nothing
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NewPortfolio() As Boolean
    LoadPorfolioFromText "", False
    HedgeMode = "0"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IsEmptyPortfolio() As Boolean
    If rsPortfolioData Is Nothing Then
        IsEmptyPortfolio = True
        Exit Function
    ElseIf rsPortfolioData.RecordCount = 0 Then
        IsEmptyPortfolio = True
        Exit Function
    End If
    With rsPortfolioData
        .MoveFirst
        Do While Not .EOF
            If Len(!StockSymbol) > 0 Then
                IsEmptyPortfolio = False
                Exit Function
            End If
            .MoveNext
            DoEvents
        Loop
    End With
    IsEmptyPortfolio = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadPorfolioFromTrades(ByVal rs As ADODB.Recordset, ByVal fNoVega As Boolean, ByVal fNoDelta As Boolean) As Boolean
    
    Set rsPortfolioData = rs
    GetExpiry
    
    ' optimize
    
    If Not gRuntime Then Exit Function
    RaiseEvent NewPortfolio(rs)
    If rsPortfolioData.RecordCount <> 0 Then
        If fVegaMode Then
            If fNoVega And Not fNoDelta Then
                ' Advise to set Delta mode
                RaiseEvent AdviseVegaMode(False)
            End If
        Else
            If fNoDelta And Not fNoVega Then
                ' Advise to set Vega mode
                RaiseEvent AdviseVegaMode(True)
            End If
        End If
    End If
    NeedSplitPortfolio = True
    LoadPorfolioFromTrades = True
    fNeedRescan = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Parses text stream and builds porfolio
'
Public Function LoadPorfolioFromText(ByVal Buffer As String, ByVal FromFile As Boolean) As Boolean
Dim Lines As Variant
Dim Items As Collection
Dim l As Variant
Dim v As Variant
Dim i1 As Variant
Dim i2 As Variant
Dim i3 As Variant
Dim StockSymbol As String
Dim isStock As Boolean
Dim Expiry As Date
Dim Result As Double
Dim Delta As Double
Dim Price As Double
Dim Vega As Double
Dim HV As Double
Dim IV As Double
Dim S As String
Dim rs As New ADODB.Recordset
Dim Separator As String
Dim BlackList As New ADODB.Recordset
Dim fBadRecords As Boolean
Dim fUnsupported As Boolean
Dim Percent As Long
Dim Percent1 As Long
Dim Count As Long
Dim CountMax As Long
Dim fNoDelta As Boolean
Dim fNoVega As Boolean
    
    fBadRecords = False
    fNoDelta = True
    fNoVega = True
    With BlackList
        .Fields.Append "StockSymbol", adVarChar, 20
        .Open
    End With
    With rs
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "Expiration", adDate, , adFldIsNullable
        .Fields.Append "Delta", adDouble, , adFldIsNullable
        .Fields.Append "StockPrice", adDouble, , adFldIsNullable
        .Fields.Append "Vega", adDouble, , adFldIsNullable
        .Fields.Append "IV", adDouble, , adFldIsNullable
        .Fields.Append "Unsupported", adBoolean
        .Open
    End With
    If Buffer = "" Then
        GoTo Done
    End If
    Lines = Split(Buffer, vbCrLf)
    If UBound(Lines) > 0 Then
        Separator = vbTab
'        l = Lines(0)
'        Set Items = SplitItems(l, Separator)
'            Err.Raise vbObjectError, "Load Portfolio", "Format can not be recognized."
'            Exit Function
'        End If
        
        RaiseEvent ProgressStart
        CountMax = UBound(Lines) - LBound(Lines) + 1
        Count = 0
        
        For Each l In Lines
            
            DoEvents
            Count = Count + 1
            Percent1 = Count * 100& \ CountMax
            If Percent1 <> Percent Then
                Percent = Percent1
                RaiseEvent Progress(Percent)
            End If
            
            If Len(l) = 0 Then GoTo Skip
            Set Items = SplitItems(l, Separator)
            If Items.Count < 3 Or Items.Count > 7 Then
                fBadRecords = True
                GoTo Skip
            End If
            Dim fSkipDelta As Boolean
            fSkipDelta = False
            If VegaMode Then
                If Items.Count = 4 Then
                    fSkipDelta = True
                End If
            End If
            
            With rs
                On Error Resume Next
                StockSymbol = UCase(GetItem(Items, 1))
                If StockSymbol = "" Then
                    fBadRecords = True
                    GoTo Skip
                End If
                If StockSymbol = "SYMBOL" Then
                    fBadRecords = True
                    GoTo Skip
                End If
                If IsNumeric(StockSymbol) Then
                    fBadRecords = True
                    GoTo Skip
                End If
                If IsDate(StockSymbol) Then
                    fBadRecords = True
                    GoTo Skip
                End If
                If UCase(StockSymbol) = "UL" Then
                    fBadRecords = True
                    GoTo Skip
                End If
                
                fUnsupported = False
                If Not CheckStockSymbol(StockSymbol) Then
                    fUnsupported = True
                    With BlackList
                        If .RecordCount <> 0 Then
                            .MoveFirst
                        End If
                        .Find "StockSymbol='" & StockSymbol & "'", 0, adSearchForward, adBookmarkFirst
                        If .EOF Then
                            LogSave "Unsupported Symbol '" & StockSymbol & "'"
                            If .RecordCount <> 0 Then
                                ' Don't ask too many times
                                'If vbCancel = gCmn.MyMsgBox("There are more unsupported symbols in portfolio. Do you want to continue?", vbOKCancel Or vbExclamation) Then
                                '    Exit Function
                                'End If
                            Else
'                                If vbCancel = gCmn.MyMsgBox("The symbol '" & StockSymbol & "' is not found in the list of supported stocks. Do you want to continue?", vbOKCancel Or vbExclamation) Then
'                                    Exit Function
'                                End If
                                frmDataLog.ShowHistory
                            End If
                            .AddNew
                            !StockSymbol = StockSymbol
                            .Update
'                            GoTo Skip
                        End If
                    End With
                End If
                
                .AddNew
                !StockSymbol = StockSymbol
                !Unsupported = fUnsupported
                If Items.Count >= 2 Then
                    i2 = GetItem(Items, 2)
                    If i2 = "" Or UCase(i2) = "UL" Then
                        isStock = True
                    Else
                        isStock = False
                        If FromFile Then
                            If ScanDateMy(i2, Expiry) Then
                                !Expiration = Expiry
                            End If
                        Else
                            If ScanDate(i2, Expiry) Then
                                !Expiration = Expiry
                            End If
                        End If
                    End If
                End If
                
                If fSkipDelta Then
                    If Items.Count >= 3 Then
                        Vega = GetItemDouble(Items, 3)
                        !Vega = Vega
                        If Vega <> 0 Then
                            fNoVega = False
                        End If
                    End If
                    
                    IV = 0
                    If Items.Count >= 4 Then
                        IV = GetItemDouble(Items, 4)
                    End If
                    If IV = 0 Then
                        IV = GetStockIV(StockSymbol)
                    End If
                    If IV > 0 Then
                        !IV = IV
                    End If
                Else
                    
                    If Items.Count >= 3 Then
                        Delta = GetItemDouble(Items, 3)
                        !Delta = Delta
                        If Delta <> 0 Then
                            fNoDelta = False
                        End If
                    End If
                    
                    Price = 0
                    If Items.Count >= 4 Then
                        Price = GetItemDouble(Items, 4)
                    End If
                    If Price = 0 Then
                        Price = GetStockPrice(StockSymbol)
                    End If
                    If Price > 0 Then
                        !StockPrice = Price
                    End If
                    
                    If Items.Count >= 5 Then
                        Vega = GetItemDouble(Items, 5)
                        !Vega = Vega
                        If Vega <> 0 Then
                            fNoVega = False
                        End If
                    End If
                    
                    IV = 0
                    If Items.Count >= 6 Then
                        IV = GetItemDouble(Items, 6)
                    End If
                    If IV = 0 Then
                        IV = GetStockIV(StockSymbol)
                    End If
                    If IV > 0 Then
                        !IV = IV
                    End If
                End If
                .Update
            End With
Skip:
        On Error GoTo 0
        Next
        RaiseEvent ProgressStop
Done:
        If (Buffer = "") Or ((Not fBadRecords) And rs.RecordCount <> 0) Then
            If BlackList.RecordCount <> 0 Then
'                gCmn.MyMsgBox "There were " & Format$(BlackList.RecordCount) & " unsupported symbols found in the portfolio. They will be marked with pink color."
            End If
            Set rsPortfolioData = rs
            GetExpiry
            If Not gRuntime Then Exit Function
            RaiseEvent NewPortfolio(rs)
            If rsPortfolioData.RecordCount <> 0 Then
                If fVegaMode Then
                    If fNoVega And Not fNoDelta Then
                        ' Advise to set Delta mode
                        RaiseEvent AdviseVegaMode(False)
                    End If
                Else
                    If fNoDelta And Not fNoVega Then
                        ' Advise to set Vega mode
                        RaiseEvent AdviseVegaMode(True)
                    End If
                End If
            End If
            NeedSplitPortfolio = True
            LoadPorfolioFromText = True
        Else
            LastErrDescription = "Format doesn't meet specification. " & vbCrLf _
                & "There should be 6 coulumns:" & vbCrLf _
                & "Symbol, Expiry, Delta, Price, Vega, IV"
                
            ShowError "Error while loading portfolio"
        End If
    End If
    fNeedRescan = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SavePorfolioToText(ByVal ToFile As Boolean) As String
Dim S As String
Dim l As String
Dim Percent As Long
Dim Percent1 As Long
Dim Count As Long
Dim CountMax As Long
    S = ""
    If Not rsPortfolioData Is Nothing Then
        With rsPortfolioData
            If .RecordCount <> 0 Then
         
                RaiseEvent ProgressStart
                CountMax = .RecordCount
                Count = 0
               
               .Sort = "StockSymbol, Expiration"
                .MoveFirst
                Do While Not .EOF
                    
                    Count = Count + 1
                    Percent1 = Count * 100& \ CountMax
                    If Percent1 <> Percent Then
                        Percent = Percent1
                        RaiseEvent Progress(Percent)
                    End If
                    
                    l = Format$(!StockSymbol)
                    If Len(l) > 0 Then
                        If ToFile Then
                            If IsNull(!Expiration) Then
                                l = l & vbTab & "UL"
                            Else
                                l = l & vbTab & Format$(Year(!Expiration), "0000") & Format$(Month(!Expiration), "00") & Format$(Day(!Expiration), "00")
                            End If
                        Else
                            If IsNull(!Expiration) Then
                                l = l & vbTab & "UL"
                            Else
                                l = l & vbTab & Format$(Year(!Expiration), "0000") & "/" & Format$(Month(!Expiration), "00") & "/" & Format$(Day(!Expiration), "00")
                            End If
                        End If
                        l = l & vbTab & DBFormat(!Delta)
                        l = l & vbTab & DBFormat(!StockPrice)
                        l = l & vbTab & DBFormat(!Vega)
                        l = l & vbTab & DBFormat(!IV)
                        S = S & l & vbCrLf
                    End If
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If
    RaiseEvent ProgressStop
    SavePorfolioToText = S
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function InitIndices()
    If rsIndexOptions Is Nothing Then
        Set rsIndexOptions = New ADODB.Recordset
        With rsIndexOptions
            .Fields.Append "Selected", adBoolean
            .Fields.Append "IndexSymbol", adVarChar, 20
            .Fields.Append "QLong", adDouble, , adFldIsNullable
            .Fields.Append "QShort", adDouble, , adFldIsNullable
            .Fields.Append "IndexPrice", adDouble, , adFldIsNullable
            .Fields.Append "LongContractsAmount", adInteger, , adFldIsNullable
            .Fields.Append "ShortContractsAmount", adInteger, , adFldIsNullable
            .Open
            Dim v As Variant
            For Each v In Array("SPX", "NDX", "OEX", "DJX")
                .AddNew
                !IndexSymbol = v
                Select Case v
                Case "SPX"
                    !Selected = g_SelectSPX
                Case "NDX"
                    !Selected = g_SelectNDX
                Case "OEX"
                    !Selected = g_SelectOEX
                Case "DJX"
                    !Selected = g_SelectDJX
                End Select
                !IndexPrice = GetStockPrice(v)
                .Update
            Next
        End With
    Else
        With rsIndexOptions
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    !QLong = Null
                    !QShort = Null
                    !LongContractsAmount = Null
                    !ShortContractsAmount = Null
                    .Update
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If

End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetIndexSymbols() As Collection
Dim IndexSymbol As String
    Set GetIndexSymbols = New Collection
    With rsIndexOptions
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If Not IsNull(!IndexSymbol) Then
                    IndexSymbol = !IndexSymbol
                    If Len(IndexSymbol) > 0 Then
                        If IndexSymbol <> "NONE" Then
                            GetIndexSymbols.Add IndexSymbol
                        End If
                    End If
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateIndexOptions()
    If Not rsIndexOptions Is Nothing Then
        With rsIndexOptions
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    !IndexPrice = GetStockPrice(!IndexSymbol)
                    .Update
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Prepare new Positions recordset
'
Private Function NewPositions() As ADODB.Recordset
    Set NewPositions = New ADODB.Recordset
    With NewPositions
        With .Fields
            .Append "StockSymbol", adVarChar, 20, adFldIsNullable
            .Append "SymbolID", adInteger
            .Append "Expiry", adDate, , adFldIsNullable
            .Append "DeltaReal", adDouble, , adFldIsNullable
            .Append "VegaReal", adDouble, , adFldIsNullable
            .Append "StockPriceReal", adDouble, , adFldIsNullable
            .Append "HVReal", adDouble, , adFldIsNullable
            .Append "VoVReal", adDouble, , adFldIsNullable
            .Append "Delta", adDouble, , adFldIsNullable
            .Append "StockPrice", adDouble, , adFldIsNullable
            .Append "HV", adDouble, , adFldIsNullable
            .Append "IV", adDouble, , adFldIsNullable
            .Append "ShortHedge", adBoolean
            .Append "LongHedge", adBoolean
            .Append "ShortHedgeOld", adBoolean
            .Append "LongHedgeOld", adBoolean
            .Append "Corr1", adDouble, , adFldIsNullable
            .Append "Corr2", adDouble, , adFldIsNullable
            .Append "Corr3", adDouble, , adFldIsNullable
            .Append "Corr4", adDouble, , adFldIsNullable
            .Append "DSV0", adDouble, , adFldIsNullable
            .Append "DSV", adDouble, , adFldIsNullable
            .Append "Gain", adDouble, , adFldIsNullable
            .Append "Q0", adDouble, , adFldIsNullable
            .Append "Q", adDouble, , adFldIsNullable
            .Append "QAbs", adDouble, , adFldIsNullable
            .Append "Q1", adDouble, , adFldIsNullable
            .Append "Q2", adDouble, , adFldIsNullable
            .Append "Q3", adDouble, , adFldIsNullable
            .Append "Q4", adDouble, , adFldIsNullable
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DeselectStockSymbol(ByVal StockSymbol As String) As Boolean
Dim bm As Variant
    With rsPortfolioData.Clone
        .Filter = "StockSymbol = '" & StockSymbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If Not !Unsupported Then
                    !Unsupported = True
                    .Update
                    DeselectStockSymbol = True
                    fNeedRefreshPortfolio = True
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IsIndex(ByVal Symbol As String) As Boolean
    Select Case Symbol
    Case "SPX", "NDX", "OEX", "DJX"
        IsIndex = True
    Case Else
        IsIndex = False
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SelectStocks() As Boolean
Dim bm As Variant
    With rsPortfolioData.Clone
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If Len(!StockSymbol) = 0 Then GoTo Skip
                If !Unsupported Then
'                    If VegaMode Then
'                        If !IV = 0 Then GoTo Skip
'                        If GetStockVoV(!StockSymbol) = 0 Then GoTo Skip
'                    Else
'                        If !StockPrice = 0 Then GoTo Skip
'                    End If
'                    If 0 = IIf(g_SelectSPX, GetStockIndexCorr(!StockSymbol, "SPX"), 1) Then GoTo Skip
'                    If 0 = IIf(g_SelectNDX, GetStockIndexCorr(!StockSymbol, "NDX"), 1) Then GoTo Skip
'                    If 0 = IIf(g_SelectOEX, GetStockIndexCorr(!StockSymbol, "OEX"), 1) Then GoTo Skip
'                    If 0 = IIf(g_SelectDJX, GetStockIndexCorr(!StockSymbol, "DJX"), 1) Then GoTo Skip
                    If IsIndex(!StockSymbol) Then
                        GoTo Skip
                    End If
                    If CheckStockSymbol(!StockSymbol) Then
                        !Unsupported = False
                        .Update
                        SelectStocks = True
                        fNeedRefreshPortfolio = True
                    End If
                End If
Skip:
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Scan PortfolioData recordset and
' fill Positions recordset
'
Private Sub GetPositions()
Dim StockSymbol As String
Dim Expiry As Variant
Dim Delta As Double
Dim Vega As Double
Dim DeltaOrVega As Double
Dim StockPrice As Double
Dim HV As Double
Dim IV As Double
Dim StockPriceOrIV As Double
Dim StockHVorVoV As Double
Dim Corr1 As Double
Dim Corr2 As Double
Dim Corr3 As Double
Dim Corr4 As Double
Dim SymbolID As Long
Dim DSV0 As Double
Dim fDistinct As Boolean
Dim fAgregatePositions As Boolean
Dim fLogErrors As Boolean
Dim rsStocks1 As New ADODB.Recordset
Dim Percent As Long
Dim Percent1 As Long
Dim Count As Long
Dim CountMax As Long
    
    With rsStocks1
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "SymbolID", adInteger
            .Append "ZeroCorrCount", adInteger
            .Append "ZeroCorrCount1", adInteger
            .Append "Remove", adBoolean
        End With
        .Open
    End With
    
    Set rsPositions = NewPositions
    If VegaMode Then
        fAgregatePositions = False
        fPerExpiration = True
        If dPerExpirationYear = 0 Then
            fPerExpiration = False
        End If
        If dPerExpirationMonth = 0 Then
            fPerExpiration = False
        End If
    Else
        fAgregatePositions = True
        fPerExpiration = False
    End If
    '//////////////////
    SelectStocks    '/
    '////////////////
    fLogErrors = False
    
    Set StockStockCorrelator = New CStockStockCorrelator
    StockStockCorrelator.Clear
    
    With rsPortfolioData.Clone
        If .RecordCount <> 0 Then
            
            RaiseEvent Status("Filtering positions..")
            RaiseEvent ProgressStart
            CountMax = .RecordCount
            Count = 0
            
            .Sort = "StockSymbol"
            
            .MoveFirst
            StockSymbol = ""
            Do While Not .EOF
                
                Count = Count + 1
                Percent1 = Count * 100& \ CountMax
                If Percent1 <> Percent Then
                    Percent = Percent1
                    RaiseEvent Progress(Percent)
                End If
                
                If Len(!StockSymbol) > 0 Then
                    If !Unsupported Then
                        GoTo Skip
                    End If
                    
                    If IsIndex(!StockSymbol) Then
                        If DeselectStockSymbol(!StockSymbol) Then
                            fLogErrors = True
                            LogSave "Positions for '" & !StockSymbol & "' was excluded as it is Index"
                            GoTo Skip
                        End If
                    End If
                    
                    Expiry = !Expiration
                    If Not fAgregatePositions Then
                        If fPerExpiration Then
                        If dPerExpirationYear <> 0 Then
                        If dPerExpirationMonth <> 0 Then
                            If IsNull(Expiry) Then GoTo Skip
                            If Not IsDate(Expiry) Then GoTo Skip
                            Dim d As Date
                            d = Expiry
                            If dPerExpirationMonth <> Month(d) Then GoTo Skip
                            If dPerExpirationYear <> Year(d) Then GoTo Skip
                        End If
                        End If
                        End If
                    End If
                    
                    Vega = DBDouble(!Vega)
                    Delta = DBDouble(!Delta)
                    If VegaMode Then
                        DeltaOrVega = Vega
                    Else
                        DeltaOrVega = Delta
                    End If
                    If DeltaOrVega = 0 Then
                        GoTo Skip
                    End If
                    
                    StockPrice = DBDouble(!StockPrice)
                    If StockPrice = 0 Then
                        StockPrice = GetStockPrice(!StockSymbol)
                        !StockPrice = StockPrice
                        .Update
                        fNeedRefreshPortfolio = True
                    End If
                    If StockPrice = 0 Then
                        If DeselectStockSymbol(!StockSymbol) Then
                            fLogErrors = True
                            LogSave "Zero Stock Price for '" & !StockSymbol & "'"
                            GoTo Skip
                        End If
                    End If
                    HV = GetStockHV(!StockSymbol)
                    If HV = 0 Then
                        If DeselectStockSymbol(!StockSymbol) Then
                            fLogErrors = True
                            LogSave "Zero Stock HV for '" & !StockSymbol & "'"
                            GoTo Skip
                        End If
                    End If
                    IV = DBDouble(!IV)
                    If IV = 0 Then
                        IV = GetStockIV(!StockSymbol)
                        !IV = IV
                        .Update
                        fNeedRefreshPortfolio = True
                    End If
                    If VegaMode Then
                        If IV = 0 Then
                            If DeselectStockSymbol(!StockSymbol) Then
                                fLogErrors = True
                                LogSave "Zero Stock IV for '" & !StockSymbol & "'"
                                GoTo Skip
                            End If
                        End If
                    End If
                    If VegaMode Then
                        StockPriceOrIV = IV
                    Else
                        StockPriceOrIV = StockPrice
                    End If
                    If StockSymbol <> !StockSymbol Then
                        StockSymbol = !StockSymbol
                        '!!! How to use index selection ???
                        Corr1 = IIf(g_SelectSPX, GetStockIndexCorr(StockSymbol, "SPX"), 0)
                        Corr2 = IIf(g_SelectNDX, GetStockIndexCorr(StockSymbol, "NDX"), 0)
                        Corr3 = IIf(g_SelectOEX, GetStockIndexCorr(StockSymbol, "OEX"), 0)
                        Corr4 = IIf(g_SelectDJX, GetStockIndexCorr(StockSymbol, "DJX"), 0)
                        If g_SelectSPX And Corr1 = 0 Then
                            If DeselectStockSymbol(StockSymbol) Then
                                fLogErrors = True
                                LogSave "Zero Stock/Index Corr for '" & StockSymbol & "' and SPX"
                                GoTo Skip
                            End If
                        End If
                        If g_SelectNDX And Corr2 = 0 Then
                            If DeselectStockSymbol(StockSymbol) Then
                                fLogErrors = True
                                LogSave "Zero Stock/Index Corr for '" & StockSymbol & "' and NDX"
                                GoTo Skip
                            End If
                        End If
                        If g_SelectOEX And Corr3 = 0 Then
                            If DeselectStockSymbol(StockSymbol) Then
                                fLogErrors = True
                                LogSave "Zero Stock/Index Corr for '" & StockSymbol & "' and OEX"
                                GoTo Skip
                            End If
                        End If
                        If g_SelectDJX And Corr4 = 0 Then
                            If DeselectStockSymbol(StockSymbol) Then
                                fLogErrors = True
                                LogSave "Zero Stock/Index Corr for '" & StockSymbol & "' and DJX"
                                GoTo Skip
                            End If
                        End If
                        If VegaMode Then
                            StockHVorVoV = GetStockVoV(StockSymbol)
                            If StockHVorVoV = 0 Then
                                If DeselectStockSymbol(StockSymbol) Then
                                    fLogErrors = True
                                    LogSave "Zero Stock VolOfVol for '" & StockSymbol & "'"
                                    GoTo Skip
                                End If
                            End If
                        Else
                            StockHVorVoV = GetStockHV(StockSymbol)
                            If StockHVorVoV = 0 Then
                                If DeselectStockSymbol(StockSymbol) Then
                                    fLogErrors = True
                                    LogSave "Zero Stock HV for '" & StockSymbol & "'"
                                    GoTo Skip
                                End If
                            End If
                        End If
                        DSV0 = 1 * StockPriceOrIV * StockHVorVoV / Sqr(252)
                        
                        StockStockCorrelator.AddStock StockSymbol
                        SymbolID = StockStockCorrelator.SymbolID(StockSymbol)
                        With rsStocks1
                            .AddNew
                            !StockSymbol = StockSymbol
                            !SymbolID = SymbolID
                            !ZeroCorrCount = 0
                            !ZeroCorrCount1 = 0
                            !Remove = False
                            .Update
                        End With
                        
                        fDistinct = True
                    Else
                        If fAgregatePositions Then
                            fDistinct = False
                        Else
                            fDistinct = True
                        End If
                    End If
                    
                    If fDistinct Then
                        With rsPositions
                            .AddNew
                            !SymbolID = SymbolID
                            !StockSymbol = StockSymbol
                            If fAgregatePositions Then
                                ' Expiry <- NULL
                            Else
                                !Expiry = Expiry
                            End If
                            !StockPriceReal = StockPrice
                            !HVReal = HV
                            !DeltaReal = Delta
                            !VegaReal = Vega
                            !StockPrice = StockPriceOrIV
                            !Delta = DeltaOrVega
                            !HV = StockHVorVoV
                            !IV = IV
                            !Corr1 = Corr1
                            !Corr2 = Corr2
                            !Corr3 = Corr3
                            !Corr4 = Corr4
                            !DSV0 = DSV0
                            !DSV = DSV0 * DeltaOrVega
                            !ShortHedge = False
                            !LongHedge = False
                            !ShortHedgeOld = False
                            !LongHedgeOld = False
                            !Gain = 0
                            !Q0 = 0
                            !Q = 0
                            !Q1 = 0
                            !Q2 = 0
                            !Q3 = 0
                            !Q4 = 0
                            .Update
                        End With
                    Else
                        With rsPositions
                            !DeltaReal = !DeltaReal + Delta
                            !VegaReal = !VegaReal + Vega
                            !Delta = !Delta + DeltaOrVega
                            !DSV = DSV0 * !Delta
                            .Update
                        End With
                    End If
                End If
Skip:
                .MoveNext
                DoEvents
            Loop
            RaiseEvent ProgressStop
        End If
    End With
    
    StockStockCorrelator.AllocateMemory
    Dim bm As Variant
    Dim StockSymbol1 As String
    Dim StockSymbol2 As String
    Dim SymbolID1 As Long
    Dim SymbolID2 As Long
    Dim Corr As Double
    Dim ZeroCorrCount1 As Long
    With rsStocks1
        '///////////////////////////
        ' Read Stock/Stock Corr
        '
        If .RecordCount <> 0 Then
            
            RaiseEvent Status("Checking correlations..")
            RaiseEvent ProgressStart
            CountMax = .RecordCount
            Count = 0
            
            .Sort = "StockSymbol"
            .MoveFirst
            Do While Not .EOF
                
                Count = Count + 1
                Percent1 = Count * 100& \ CountMax
                If Percent1 <> Percent Then
                    Percent = Percent1
                    RaiseEvent Progress(Percent)
                End If
                
                StockSymbol1 = !StockSymbol
                SymbolID1 = !SymbolID
                ZeroCorrCount1 = !ZeroCorrCount
                bm = .Bookmark
                .MoveNext
                Do While Not .EOF
                    StockSymbol2 = !StockSymbol
                    SymbolID1 = !SymbolID
                    Corr = GetStockStockCorr(StockSymbol1, StockSymbol2)
                    StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2) = Corr
                    If Corr = 0 Then
                        fLogErrors = True
                        LogSave "Zero Stock/Stock Corr for '" & StockSymbol1 & "' and '" & StockSymbol2 & "'"
                        !ZeroCorrCount = !ZeroCorrCount + 1
                        .Update
                        ZeroCorrCount1 = ZeroCorrCount1 + 1
                    End If
                    .MoveNext
                    DoEvents
                Loop
                .Bookmark = bm
                !ZeroCorrCount = ZeroCorrCount1
                .Update
                .MoveNext
                DoEvents
            Loop
            RaiseEvent ProgressStop

            '///////////////////////////
            ' Check Stock/Stock Corr
            '
            Dim fNothingToRemove As Boolean
            fNothingToRemove = False
            Do While Not fNothingToRemove
                With .Clone
                    .Filter = "ZeroCorrCount<>0 and Remove=false"
                    If .RecordCount <> 0 Then
                        .Sort = "ZeroCorrCount desc"
                        .MoveFirst
                        StockSymbol1 = !StockSymbol
                        Debug.Print "Remove Stock " & StockSymbol1, !ZeroCorrCount
                        !Remove = True
                        !ZeroCorrCount1 = !ZeroCorrCount
                        !ZeroCorrCount = 0
                        .Update
                        ' Recalculate ZeroCorrCount for remaining stocks
                        With .Clone
                            .Filter = "ZeroCorrCount<>0 and Remove=false"
                            If .RecordCount <> 0 Then
                                .MoveFirst
                                Do While Not .EOF
                                    StockSymbol2 = !StockSymbol
                                    If StockSymbol1 <> StockSymbol2 Then
                                        Corr = StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2)
                                        If Corr = 0 Then
                                            !ZeroCorrCount = !ZeroCorrCount - 1
                                            Debug.Assert !ZeroCorrCount >= 0
                                            .Update
                                        End If
                                    End If
                                    .MoveNext
                                    DoEvents
                                Loop
                            End If
                        End With
                    Else
                        fNothingToRemove = True
                    End If
                End With
                DoEvents
            Loop
            ' Do remove positions for removed stocks
            With .Clone
                .Filter = "Remove=true"
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                         StockSymbol = !StockSymbol
                        If DeselectStockSymbol(StockSymbol) Then
                        End If
                            fLogErrors = True
                            ZeroCorrCount1 = !ZeroCorrCount1
                            
                            LogSave "Stock '" & StockSymbol & _
                                "' was removed as it has zero " & _
                                IIf(VegaMode, "IV", "price") & _
                                " correlation with " & ZeroCorrCount1 & _
                                " other stock" & IIf(ZeroCorrCount1 > 1, "s", "")
                             
                             With rsPositions.Clone
                                .Filter = "StockSymbol='" & StockSymbol & "'"
                                If .RecordCount <> 0 Then
                                    .MoveFirst
                                    Do While Not .EOF
                                        .Delete
                                        .MoveNext
                                        DoEvents
                                    Loop
                                End If
                            End With
                        .MoveNext
                        DoEvents
                    Loop
                End If
            End With
        End If
    End With
    
    If fLogErrors Then
        frmDataLog.ShowHistory
'        gCmn.MyMsgBox "There were stocks found with zero price or IV.", vbExclamation
    End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Copy prices to Portfolio data
'
Public Sub UpdatePortfolio()
Dim StockSymbol As String
Dim StockPrice As Double
Dim HV As Double
    With rsPortfolioData
        If .RecordCount <> 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            StockSymbol = ""
            StockPrice = 0
            Do While Not .EOF
                If Len(!StockSymbol) > 0 Then
                    If StockSymbol <> !StockSymbol Then
                        StockSymbol = !StockSymbol
                        StockPrice = GetStockPrice(StockSymbol)
                    End If
                    If StockPrice <> 0 Then
                        !StockPrice = StockPrice
                    End If
                    .Update
                End If
               .MoveNext
                DoEvents
            Loop
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdatePositions()
Dim StockSymbol As String
Dim StockPrice As Double
Dim HV As Double
Dim StockPriceReal As Double
Dim HVReal As Double
Dim StockPriceOrHV As Double
Dim StockHVorVoV As Double
Dim Corr1 As Double
Dim Corr2 As Double
Dim Corr3 As Double
Dim Corr4 As Double
Dim DSV0 As Double
    If Not rsPositions Is Nothing Then
        With rsPositions
            If .RecordCount <> 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            StockSymbol = ""
            StockPrice = 0
            HV = 0
            Do While Not .EOF
                If StockSymbol <> !StockSymbol Then
                    StockSymbol = !StockSymbol
                    StockPrice = GetStockPrice(StockSymbol)
                    HV = GetStockHV(StockSymbol)
                    If StockPrice <> 0 Then
                        StockPriceReal = StockPrice
                    Else
                        StockPriceReal = DBDouble(!StockPriceReal)
                    End If
                    If HV <> 0 Then
                        HVReal = HV
                    Else
                        HVReal = DBDouble(!HVReal)
                    End If
                    Corr1 = IIf(g_SelectSPX, GetStockIndexCorr(StockSymbol, "SPX"), 0)
                    Corr2 = IIf(g_SelectNDX, GetStockIndexCorr(StockSymbol, "NDX"), 0)
                    Corr3 = IIf(g_SelectOEX, GetStockIndexCorr(StockSymbol, "OEX"), 0)
                    Corr4 = IIf(g_SelectDJX, GetStockIndexCorr(StockSymbol, "DJX"), 0)
                    If VegaMode Then
                        StockPriceOrHV = HVReal
                        StockHVorVoV = GetStockVoV(StockSymbol)
                    Else
                        StockPriceOrHV = StockPriceReal
                        StockHVorVoV = GetStockHV(StockSymbol)
                    End If
                    DSV0 = 1 * StockPriceOrHV * StockHVorVoV / Sqr(252)
                Else
                    StockPriceReal = IIf(StockPrice <> 0, StockPrice, DBDouble(!StockPriceReal))
                    HVReal = IIf(HV <> 0, HV, DBDouble(!HVReal))
                    StockPriceOrHV = IIf(VegaMode, HVReal, StockPriceReal)
                End If
                If StockPrice <> 0 Then
                    !StockPriceReal = StockPrice
                End If
                If HV <> 0 Then
                    !HVReal = HV
                End If
                !StockPrice = StockPriceOrHV
                !HV = StockHVorVoV
                !Corr1 = Corr1
                !Corr2 = Corr2
                !Corr3 = Corr3
                !Corr4 = Corr4
                !DSV0 = DSV0
                If VegaMode Then
                    !DSV = DSV0 * DBDouble(!VegaReal)
                Else
                    !DSV = DSV0 * DBDouble(!DeltaReal)
                End If
                .Update
                .MoveNext
                DoEvents
            Loop
            End If
        End With
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Scan input recordset and
' fill output recordset with unique StockSymbols
'
Public Function GetSymbols(rs As ADODB.Recordset) As ADODB.Recordset
Dim StockSymbol As String
Dim rs2 As ADODB.Recordset
    Set rs2 = New ADODB.Recordset
    With rs2
        With .Fields
            .Append "StockSymbol", adVarChar, 20, adFldIsNullable
        End With
        .Open
    End With
    With rs.Clone
        If .RecordCount <> 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            StockSymbol = ""
            Do While Not .EOF
                If StockSymbol <> !StockSymbol Then
                    StockSymbol = !StockSymbol
                    With rs2
                        .AddNew
                        !StockSymbol = StockSymbol
                        .Update
                    End With
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    Set GetSymbols = rs2
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolDeltaTotal(rs As ADODB.Recordset, ByVal StockSymbol As Double) As Double
Dim Delta As Double
    Delta = 0
    With rs.Clone
        .Filter = "StockSymbol='" & StockSymbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If Not IsNull(!Delta) Then
                    Delta = Delta + !Delta
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    GetSymbolDeltaTotal = Delta
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSymbolVegaTotal(rs As ADODB.Recordset, ByVal StockSymbol As Double) As Double
Dim Vega As Double
    Vega = 0
    With rs.Clone
        .Filter = "StockSymbol='" & StockSymbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If Not IsNull(!Vega) Then
                    Vega = Vega + !Vega
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    GetSymbolVegaTotal = Vega
End Function
    
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdateIndexIndexCorrelator()
Dim i As Long
Dim j As Long
Dim d As Double
Dim IV As Double
Dim HV As Double
Dim IndexSymbol As String
    Set IndexIndexCorrelator = New CIndexIndexCorrelator
    With IndexIndexCorrelator
        .IndexSelected("SPX") = g_SelectSPX
        .IndexSelected("NDX") = g_SelectNDX
        .IndexSelected("OEX") = g_SelectOEX
        .IndexSelected("DJX") = g_SelectDJX
        For i = 1 To .IndexCount
            IndexSymbol = .IndexSymbol(i)
            If VegaMode Then
            
                d = GetIndexVoV(IndexSymbol)
                If d = 0 Then
                    IndexSymbol = .IndexSymbol(i)
                    Err.Raise vbObjectError, , "Zero Index VolOfVol for '" & IndexSymbol & "'"
                End If
                IV = GetIndexIV(IndexSymbol)
                If d = 0 Then
                    IndexSymbol = .IndexSymbol(i)
                    Err.Raise vbObjectError, , "Zero Index IV for '" & IndexSymbol & "'"
                End If
                .IndexHV(IndexSymbol) = d * IV * 100 / Sqr(252)
                
                d = GetIndexVega(IndexSymbol)
                If d = 0 Then
                    Err.Raise vbObjectError, , "Zero Index Vega for '" & IndexSymbol & "'"
                End If
                HV = GetIndexHV(IndexSymbol)
                If HV = 0 Then
                    Err.Raise vbObjectError, , "Zero Index HV for '" & IndexSymbol & "'"
                End If
                .IndexPrice(IndexSymbol) = d * HV
            
            Else
                ' Delta mode
                HV = GetIndexHV(IndexSymbol)
                If HV = 0 Then
                    Err.Raise vbObjectError, , "Zero Index HV for '" & IndexSymbol & "'"
                End If
                .IndexHV(IndexSymbol) = HV
                d = GetStockPrice(IndexSymbol)
                If d = 0 Then
                    Err.Raise vbObjectError, , "Zero Index Price for '" & IndexSymbol & "'"
                End If
                .IndexPrice(IndexSymbol) = d
            End If
            For j = i + 1 To .IndexCount
                d = GetIndexIndexCorr(IndexSymbol, .IndexSymbol(j))
                If d = 0 Then
                    Err.Raise vbObjectError, , "Zero Index/Index Correlation between '" & IndexSymbol & "' and '" & .IndexSymbol(j) & "'"
                End If
                .IndexIndexCorr(.IndexSymbol(i), .IndexSymbol(j)) = d
            Next
        Next
        .CalculateIMatrix
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SplitPortfolio() As Boolean
Dim StockSymbol As String
Dim Q As Double
Dim D2 As Double
Dim d3 As Double
Dim Gain As Double
Dim Gain0 As Double
Dim Q1 As Double
Dim Q2 As Double
Dim Q3 As Double
Dim Q4 As Double
Dim HedgeSymbol As String
Dim Percent As Long
Dim Percent1 As Long
Dim Count As Long
Dim CountMax As Long
    
    On Error GoTo Herr
    If Not gRuntime Then Exit Function
    
    StartTask "Calculating...", True
    
    CancelRequested = False
    RaiseEvent Status("Preparing stock data..")
    LogSave "Calculation started " & Now
    
'    If Not IsMapped Then
'        '!!!
'        Requery
'        MapStockStockCorr
'        IsMapped = True
'    End If
    If CancelRequested Then GoTo Done
    DoEvents
    
    InitIndices
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Getting positions..")
    '//////////////////
    GetPositions    '/
    '////////////////
    
    HedgeMode = ""
    HedgeSymbol = ""
    Gain0 = 0
    
    UpdateIndexIndexCorrelator
    
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Calculating variations..")
    With rsPositions
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                !ShortHedge = True
                !LongHedge = True
                .Update
                .MoveNext
                DoEvents
            Loop
        End If
    End With
    CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, ""
    '//////////////////////////
    If Gain >= g_dGainBarrier Then
        GoTo Done
    End If
    
    If CancelRequested Then GoTo Done
    Gain0 = Gain
    
    ' Split on Long and Short
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Spliting portfolio..")
    HedgeMode = "0"
    
    Dim dStockSymbol As String
    Dim DSV As Double
    Dim Q0 As Double
    With rsPositions
        If .RecordCount <> 0 Then
            
            RaiseEvent ProgressStart
            CountMax = .RecordCount
            Count = 0
            
            .MoveFirst
            dStockSymbol = ""
            Do While Not .EOF
                StockSymbol = !StockSymbol
                If Not gRuntime Then Exit Function
                Count = Count + 1
                Percent1 = Count * 100& \ CountMax
                If Percent1 <> Percent Then
                    Percent = Percent1
                    RaiseEvent Progress(Percent)
                End If
                
'                RaiseEvent Status("Split position " & StockSymbol)
                If Len(StockSymbol) = 0 Then GoTo Skip
                '!!!
                If dStockSymbol <> StockSymbol Then
                    DSV = !DSV0
                    With IndexIndexCorrelator
                        .VarPort = DSV * DSV
                        .CorPortInd("SPX") = rsPositions!Corr1
                        .CorPortInd("NDX") = rsPositions!Corr2
                        .CorPortInd("OEX") = rsPositions!Corr3
                        .CorPortInd("DJX") = rsPositions!Corr4
                        .CalculateQVector
                        Q0 = .QTotal
                        Gain = .Gain
                    End With
                    dStockSymbol = StockSymbol
                End If
                Q = Q0 * DBDouble(!Delta)
                !Q0 = Q0
                !Gain = Gain
'                Debug.Print StockSymbol; vbTab, Q; vbTab, Gain
                !Q = Q
                !QAbs = Abs(Q)
                If Q > 0 Then
                    !ShortHedge = True
                    !ShortHedgeOld = True
                    !LongHedge = False
                    !LongHedgeOld = False
                ElseIf Q < 0 Then
                    !ShortHedge = False
                    !ShortHedgeOld = False
                    !LongHedge = True
                    !LongHedgeOld = True
                Else
                    !ShortHedge = False
                    !LongHedge = False
                End If
                .Update
Skip:
                If CancelRequested Then GoTo Done
                .MoveNext
                DoEvents
            Loop
            RaiseEvent ProgressStop
        End If
    End With
    
Dim BestGain As Double
Dim BestQ As Double
Dim v As Variant
    For Each v In Array("SHORT", "LONG")
        HedgeSymbol = v
#If CALC_MODE = 0 Then
        CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
        '//////////////////////////
#Else
        Dim VarPort0 As Double
        Dim CorPortInd10 As Double
        Dim CorPortInd20 As Double
        Dim CorPortInd30 As Double
        Dim CorPortInd40 As Double
        CalculateHedgeVariations _
            VarPort0, _
            CorPortInd10, _
            CorPortInd20, _
            CorPortInd30, _
            CorPortInd40, _
            HedgeSymbol

        CalculateSubportfolioTotalsShort _
            VarPort0, _
            CorPortInd10, _
            CorPortInd20, _
            CorPortInd30, _
            CorPortInd40, _
            Q, D2, d3, Gain, _
            Q1, Q2, Q3, Q4
#End If
        BestGain = Gain
        BestQ = Q
        If BestGain >= g_dGainBarrier Then
            GoTo TryInsert
        End If
        ' Remove positions
        With rsPositions.Clone
            If HedgeSymbol = "SHORT" Then
                .Filter = "ShortHedge=True"
            Else
                .Filter = "LongHedge=True"
            End If
            RaiseEvent Status("Removing " & HedgeSymbol & " positions..")
            If .RecordCount <> 0 Then
                RaiseEvent ProgressStart
                CountMax = .RecordCount
                Count = 0
                .Sort = "Gain asc, QAbs asc"
                .MoveFirst
                Do While Not .EOF
                    If Not gRuntime Then Exit Function
                    Count = Count + 1
                    Percent1 = Count * 100& \ CountMax
                    If Percent1 <> Percent Then
                        Percent = Percent1
                        RaiseEvent Progress(Percent)
                    End If
                    RaiseEvent Status("Removing " & HedgeSymbol & " position " & Count & " of " & CountMax)
'                    Debug.Print "Remove " & HedgeSymbol & " position " & !StockSymbol
                    ' Remove position from subportfolio
                    If HedgeSymbol = "SHORT" Then
                        !ShortHedge = False
                    Else
                        !LongHedge = False
                    End If
                    .Update
#If CALC_MODE = 0 Then
                    '!!! Optimize
                    CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
#Else
                    '//////////////////////////
                    Dim VarPort As Double
                    Dim CorPortInd1 As Double
                    Dim CorPortInd2 As Double
                    Dim CorPortInd3 As Double
                    Dim CorPortInd4 As Double
                    Dim DSV1 As Double
                    Dim DSVSumm As Double
'                    CalculateHedgeVariations _
'                        VarPort, _
'                        CorPortInd1, _
'                        CorPortInd2, _
'                        CorPortInd3, _
'                        CorPortInd4, _
'                        HedgeSymbol
'                    Debug.Print "C2", VarPort
                    
                    Dim SymbolID1 As Long
                    SymbolID1 = !SymbolID
                    
                    DSV1 = !DSV
                    DSVSumm = 0
                    With .Clone
                        If HedgeSymbol = "SHORT" Then
                            .Filter = "ShortHedge=True"
                        Else
                            .Filter = "LongHedge=True"
                        End If
                        If .RecordCount <> 0 Then
                            .MoveFirst
                            Do While Not .EOF
                                DSVSumm = DSVSumm + !DSV * StockStockCorrelator.dStockStockCorr(SymbolID1, !SymbolID)
                                .MoveNext
                                DoEvents
                            Loop
                        End If
                    End With
'                    If DSVSumm <> 0 Then
                        VarPort = VarPort0 - DSV1 * (DSV1 + DSVSumm + DSVSumm)
'                    End If
                    CorPortInd1 = CorPortInd10 - !Corr1 * DSV1
                    CorPortInd2 = CorPortInd20 - !Corr2 * DSV1
                    CorPortInd3 = CorPortInd30 - !Corr3 * DSV1
                    CorPortInd4 = CorPortInd40 - !Corr4 * DSV1
                    CalculateSubportfolioTotalsShort _
                        VarPort, _
                        CorPortInd1, _
                        CorPortInd2, _
                        CorPortInd3, _
                        CorPortInd4, _
                        Q, D2, d3, Gain, _
                        Q1, Q2, Q3, Q4
                    '//////////////////////////
#End If
                    'Debug.Print "C0", Q, Gain
                    If (Gain > BestGain) Or (Gain = BestGain And Q > BestQ) Then
                        BestGain = Gain
                        BestQ = Q
#If CALC_MODE = 0 Then
#Else
                        ' Commit remove position
                        CorPortInd10 = CorPortInd1
                        CorPortInd20 = CorPortInd2
                        CorPortInd30 = CorPortInd3
                        CorPortInd40 = CorPortInd4
                        VarPort0 = VarPort
#End If
                        If BestGain >= g_dGainBarrier Then
                            Exit Do
                        End If
                    Else
                        ' Rollback remove position
                        If HedgeSymbol = "SHORT" Then
                            !ShortHedge = True
                        Else
                            !LongHedge = True
                        End If
                        .Update
                    End If
                    If CancelRequested Then GoTo Done
                    .MoveNext
                    DoEvents
                Loop
                RaiseEvent ProgressStop
            End If
        End With

TryInsert:
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Inserting " & HedgeSymbol & " positions..")
        With rsPositions.Clone
            ' Insert positions
            If HedgeSymbol = "SHORT" Then
                .Filter = "LongHedgeOld=True"
            Else
                .Filter = "ShortHedgeOld=True"
            End If
            If .RecordCount <> 0 Then
                RaiseEvent ProgressStart
                CountMax = .RecordCount
                Count = 0
                .Sort = "Gain desc, QAbs desc"
                .MoveFirst
                Do While Not .EOF
                    If Not gRuntime Then Exit Function
                    Count = Count + 1
                    Percent1 = Count * 100& \ CountMax
                    If Percent1 <> Percent Then
                        Percent = Percent1
                        RaiseEvent Progress(Percent)
                    End If
                    RaiseEvent Status("Inserting " & HedgeSymbol & " position " & Count & " of " & CountMax)
'                    Debug.Print "Insert " & HedgeSymbol & " position " & !StockSymbol
                    ' Insert position
                    If HedgeSymbol = "SHORT" Then
                        !ShortHedge = True
                    Else
                        !LongHedge = True
                    End If
                    .Update
#If CALC_MODE = 0 Then
                    '!!! Optimize
                    CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
#Else
                    '//////////////////////////
'                    CalculateHedgeVariations _
'                        VarPort, _
'                        CorPortInd1, _
'                        CorPortInd2, _
'                        CorPortInd3, _
'                        CorPortInd4, _
'                        HedgeSymbol
'                    Debug.Print "D2", VarPort
                    SymbolID1 = !SymbolID
                    DSV1 = !DSV
                    DSVSumm = 0
                    With .Clone
                        If HedgeSymbol = "SHORT" Then
                            .Filter = "ShortHedge=True"
                        Else
                            .Filter = "LongHedge=True"
                        End If
                        If .RecordCount <> 0 Then
                            .MoveFirst
                            Do While Not .EOF
                                DSVSumm = DSVSumm + !DSV * StockStockCorrelator.dStockStockCorr(SymbolID1, !SymbolID)
                                .MoveNext
                                DoEvents
                            Loop
                        End If
                    End With
'                    If DSVSumm <> 0 Then
                        VarPort = VarPort0 + DSV1 * (DSV1 + DSVSumm + DSVSumm)
'                    End If
                    CorPortInd1 = CorPortInd10 + !Corr1 * DSV1
                    CorPortInd2 = CorPortInd20 + !Corr2 * DSV1
                    CorPortInd3 = CorPortInd30 + !Corr3 * DSV1
                    CorPortInd4 = CorPortInd40 + !Corr4 * DSV1
                    CalculateSubportfolioTotalsShort _
                        VarPort, _
                        CorPortInd1, _
                        CorPortInd2, _
                        CorPortInd3, _
                        CorPortInd4, _
                        Q, D2, d3, Gain, _
                        Q1, Q2, Q3, Q4
                    '//////////////////////////
#End If
'                    Debug.Print "D0", Q, Gain
                    If Gain < Min(BestGain, g_dGainBarrier) Then
                        ' Rollback insert position
                        If HedgeSymbol = "SHORT" Then
                            !ShortHedge = False
                        Else
                            !LongHedge = False
                        End If
                        .Update
                    Else
                        BestGain = Gain
                        BestQ = Q
#If CALC_MODE = 0 Then
#Else
                        ' Commit insert position
                        CorPortInd10 = CorPortInd1
                        CorPortInd20 = CorPortInd2
                        CorPortInd30 = CorPortInd3
                        CorPortInd40 = CorPortInd4
                        VarPort0 = VarPort
#End If
                    End If
                    If CancelRequested Then GoTo Done
                    .MoveNext
                    DoEvents
                Loop
                RaiseEvent ProgressStop
            End If
        End With
    Next
    
    '//////////////////////////
    If BestGain < Gain0 Then
        ' Rollback to Optimal hedge
        HedgeMode = ""
        With rsPositions
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    !ShortHedge = True
                    !LongHedge = True
                    .Update
                    If CancelRequested Then GoTo Done
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If
    '//////////////////////////

Done:
    If CancelRequested Then
        LogSave "Calculation aborted: " & Now
    Else
        SplitPortfolio = True
        NeedSplitPortfolio = False
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Calculating totals..")
        RaiseEvent NewSubPortfolio(rsPositions)
        LogSave "Calculation finished " & Now
    End If
    Exit Function
Herr:
    LastErrDescription = Err.Description
    frmDataLog.ShowHistory
    LogSave "Calculation failed: " & LastErrDescription
    ShowError "Calculation failed"
    If Not gRuntime Then Exit Function
    RaiseEvent ClearResults
'    NeedSplitPortfolio = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSubportfolioFilter(ByVal HedgeSymbol As String) As String
    If Len(HedgeSymbol) > 0 Then
        Select Case HedgeSymbol
        Case "ALL"
            GetSubportfolioFilter = ""
        Case "SHORT"
            GetSubportfolioFilter = "ShortHedge=True"
        Case "NOT_SHORT"
            GetSubportfolioFilter = "ShortHedge=False"
        Case "LONG"
            GetSubportfolioFilter = "LongHedge=True"
        Case "NOT_LONG"
            GetSubportfolioFilter = "LongHedge=False"
        Case "OPT"
            GetSubportfolioFilter = "(ShortHedge=True) and (LongHedge=True)"
        Case "NONE"
            GetSubportfolioFilter = "(ShortHedge=false) or (LongHedge=false)"
        Case Else
            GetSubportfolioFilter = ""
        End Select
    Else
        GetSubportfolioFilter = ""
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetSubportfolioCaption(ByVal HedgeSymbol As String) As String
    If Len(HedgeSymbol) > 0 Then
        Select Case HedgeSymbol
        Case "SHORT"
            GetSubportfolioCaption = "SHORT"
        Case "LONG"
            GetSubportfolioCaption = "LONG"
        Case "OPT"
            GetSubportfolioCaption = "HEDGE"
        Case "NONE"
            GetSubportfolioCaption = "NONE"
        Case "NOT_SHORT"
            GetSubportfolioCaption = "NONE"
        Case "NOT_LONG"
            GetSubportfolioCaption = "NONE"
        Case Else
            GetSubportfolioCaption = ""
        End Select
    Else
        GetSubportfolioCaption = ""
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CalculateSubportfolioTotals2( _
    ByVal HedgeSymbol As String, _
    PositionsCount As Long, _
    EquivalentPosition As Double _
)
Dim c As Long
Dim d As Double
    PositionsCount = 0
    EquivalentPosition = 0
    If rsPositions Is Nothing Then
        Exit Sub
    End If
    With rsPositions.Clone
        .Filter = GetSubportfolioFilter(HedgeSymbol)
        If .RecordCount <> 0 Then
            .MoveFirst
            c = 0
            d = 0
            Do While Not .EOF
                c = c + 1
                If VegaMode Then
                    d = d + DBDouble(!Delta)    ' !!! Vega in reality
                Else
                    d = d + DBDouble(!Delta) * DBDouble(!StockPrice)
                End If
                .MoveNext
                If Not gRuntime Then Exit Sub
                DoEvents
            Loop
            PositionsCount = c
            EquivalentPosition = d
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CalculateSubportfolioTotals( _
    Q As Double, _
    NonHedgedDeviation As Double, _
    HedgedDeviation As Double, _
    Gain As Double, _
    Q1 As Double, _
    Q2 As Double, _
    Q3 As Double, _
    Q4 As Double, _
    ByVal HedgeSymbol As String _
)
Dim VarPort As Double
Dim CorPortInd1 As Double
Dim CorPortInd2 As Double
Dim CorPortInd3 As Double
Dim CorPortInd4 As Double
    
    CalculateHedgeVariations _
        VarPort, _
        CorPortInd1, _
        CorPortInd2, _
        CorPortInd3, _
        CorPortInd4, _
        HedgeSymbol
    
    CalculateSubportfolioTotalsShort _
        VarPort, _
        CorPortInd1, _
        CorPortInd2, _
        CorPortInd3, _
        CorPortInd4, _
        Q, NonHedgedDeviation, HedgedDeviation, Gain, _
        Q1, Q2, Q3, Q4
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CalculateSubportfolioTotalsShort( _
    ByVal VarPort As Double, _
    ByVal CorPortInd1 As Double, _
    ByVal CorPortInd2 As Double, _
    ByVal CorPortInd3 As Double, _
    ByVal CorPortInd4 As Double, _
    Q As Double, _
    NonHedgedDeviation As Double, _
    HedgedDeviation As Double, _
    Gain As Double, _
    Q1 As Double, _
    Q2 As Double, _
    Q3 As Double, _
    Q4 As Double _
)
    If VarPort > 0 Then
        With IndexIndexCorrelator
            If VegaMode Then
                NonHedgedDeviation = 100 * Sqr(VarPort) / Sqr(252)
                .VarPort = VarPort * 10000 / 252
            Else
                NonHedgedDeviation = Sqr(VarPort)
                .VarPort = VarPort
            End If
            .CorPortInd("SPX") = CorPortInd1 / Sqr(VarPort)
            .CorPortInd("NDX") = CorPortInd2 / Sqr(VarPort)
            .CorPortInd("OEX") = CorPortInd3 / Sqr(VarPort)
            .CorPortInd("DJX") = CorPortInd4 / Sqr(VarPort)
            .CalculateQVector
'            If VegaMode Then
'                Dim IV As Double
'                IV = 0
'                IV = GetIndexIV("SPX")
'                If IV = 0 Then
'                    Q1 = 0
'                Else
'                    Q1 = .QByIndex("SPX") / IV
'                End If
'                IV = 0
'                IV = GetIndexIV("NDX")
'                If IV = 0 Then
'                    Q2 = 0
'                Else
'                    Q2 = .QByIndex("NDX") / IV
'                End If
'                IV = 0
'                IV = GetIndexIV("OEX")
'                If IV = 0 Then
'                    Q3 = 0
'                Else
'                    Q3 = .QByIndex("OEX") / IV
'                End If
'                IV = 0
'                IV = GetIndexIV("DJX")
'                If IV = 0 Then
'                    Q4 = 0
'                Else
'                    Q4 = .QByIndex("DJX") / IV
'                End If
'                Q = Q1 + Q2 + Q3 + Q4
'            Else
                Q1 = .QByIndex("SPX")
                Q2 = .QByIndex("NDX")
                Q3 = .QByIndex("OEX")
                Q4 = .QByIndex("DJX")
                Q = .QTotal
'            End If
            Gain = .Gain
            Dim d As Double
            d = .d
            If d > 0 Then
                HedgedDeviation = NonHedgedDeviation * Sqr(d)
            Else
                HedgedDeviation = 0
            End If
        End With
    Else
        Q = 0
        NonHedgedDeviation = 0
        HedgedDeviation = 0
        Gain = 0
        Q1 = 0
        Q2 = 0
        Q3 = 0
        Q4 = 0
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CalculateContractAmmounts()
Dim Q As Double
Dim D2 As Double
Dim d3 As Double
Dim Gain As Double
Dim Q1 As Double
Dim Q2 As Double
Dim Q3 As Double
Dim Q4 As Double
Dim HedgeSymbol As String
Dim v As Variant
Dim OriginalPositionsCount As Long
Dim HedgedPositions As Long
Dim EquivalentPositions As Double
Dim EquivalentHedgedPositions As Double
    
    On Error GoTo Herr
'    If Not IsMapped Then
'        '!!!
'        Requery
'        MapStockStockCorr
'        IsMapped = True
'    End If
    
    CalculateSubportfolioTotals2 "", OriginalPositionsCount, EquivalentPositions
    
    CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "ALL"
    If Not gRuntime Then Exit Sub
    RaiseEvent GotSubportfolioTotals(Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "ALL")
    CalculateSubportfolioTotals2 "ALL", HedgedPositions, EquivalentHedgedPositions
    If Not gRuntime Then Exit Sub
    RaiseEvent GotSubportfolioTotals2(OriginalPositionsCount, HedgedPositions, EquivalentHedgedPositions, EquivalentPositions - EquivalentHedgedPositions, "ALL")
    
    If HedgeMode = "" Then
        CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "OPT"
'        SetQByIndex "SHORT", -Q1, -Q2, -Q3, -Q4
'        SetQByIndex "LONG", 0, 0, 0, 0
        If Not gRuntime Then Exit Sub
        RaiseEvent GotSubportfolioTotals(Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "OPT")
        CalculateSubportfolioTotals2 "OPT", HedgedPositions, EquivalentHedgedPositions
        If Not gRuntime Then Exit Sub
        RaiseEvent GotSubportfolioTotals2(OriginalPositionsCount, HedgedPositions, EquivalentHedgedPositions, EquivalentPositions - EquivalentHedgedPositions, "OPT")
        
        CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "NONE"
        If Not gRuntime Then Exit Sub
        RaiseEvent GotSubportfolioTotals(Q, D2, d3, Gain, Q1, Q2, Q3, Q4, "NONE")
        CalculateSubportfolioTotals2 "NONE", HedgedPositions, EquivalentHedgedPositions
        If Not gRuntime Then Exit Sub
        RaiseEvent GotSubportfolioTotals2(OriginalPositionsCount, HedgedPositions, EquivalentHedgedPositions, EquivalentPositions - EquivalentHedgedPositions, "NONE")
    
    Else
        For Each v In Array("SHORT", "LONG")
            HedgeSymbol = v
            CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
'            SetQByIndex HedgeSymbol, -Q1, -Q2, -Q3, -Q4
            If Not gRuntime Then Exit Sub
            RaiseEvent GotSubportfolioTotals(Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol)
            CalculateSubportfolioTotals2 HedgeSymbol, HedgedPositions, EquivalentHedgedPositions
            If Not gRuntime Then Exit Sub
            RaiseEvent GotSubportfolioTotals2(OriginalPositionsCount, HedgedPositions, EquivalentHedgedPositions, EquivalentPositions - EquivalentHedgedPositions, HedgeSymbol)
            
            HedgeSymbol = "NOT_" & v
            CalculateSubportfolioTotals Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol
            If Not gRuntime Then Exit Sub
            RaiseEvent GotSubportfolioTotals(Q, D2, d3, Gain, Q1, Q2, Q3, Q4, HedgeSymbol)
            CalculateSubportfolioTotals2 HedgeSymbol, HedgedPositions, EquivalentHedgedPositions
            If Not gRuntime Then Exit Sub
            RaiseEvent GotSubportfolioTotals2(OriginalPositionsCount, HedgedPositions, EquivalentHedgedPositions, EquivalentPositions - EquivalentHedgedPositions, HedgeSymbol)
        Next
    End If
    If Not gRuntime Then Exit Sub
    RaiseEvent NewIndexData(rsIndexOptions)
    Exit Sub
Herr:
    ShowError
    If Not gRuntime Then Exit Sub
    RaiseEvent ClearResults
'    On Error Resume Next
'    If HedgeMode = "" Then
'        SetQByIndex "SHORT", 0, 0, 0, 0
'        SetQByIndex "LONG", 0, 0, 0, 0
'        RaiseEvent GotSubportfolioTotals(0, 0, 0, 0, 0, 0, 0, 0, "OPT")
'        RaiseEvent GotSubportfolioTotals2(0, 0, 0, 0, "OPT")
'        RaiseEvent GotSubportfolioTotals(0, 0, 0, 0, 0, 0, 0, 0, "NONE")
'        RaiseEvent GotSubportfolioTotals2(0, 0, 0, 0, "NONE")
'    Else
'        For Each V In Array("SHORT", "LONG")
'            HedgeSymbol = V
'            SetQByIndex HedgeSymbol, 0, 0, 0, 0
'            RaiseEvent GotSubportfolioTotals(0, 0, 0, 0, 0, 0, 0, 0, HedgeSymbol)
'            RaiseEvent GotSubportfolioTotals2(0, 0, 0, 0, HedgeSymbol)
'        Next
'        RaiseEvent GotSubportfolioTotals(0, 0, 0, 0, 0, 0, 0, 0, "NONE")
'        RaiseEvent GotSubportfolioTotals2(0, 0, 0, 0, "NONE")
'    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetStocksList(Optional ByVal IndexSymbol As String) As ADODB.Recordset
Dim StockSymbol As String
Dim Price As Double
Dim Delta As Double
Dim VarVec As Double
Dim HV As Double
Dim DSV As Double
Dim Corr As Double
Dim SymbolID As Long

    Set GetStocksList = New ADODB.Recordset
    With GetStocksList
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "SymbolID", adInteger
        .Fields.Append "Price", adDouble
        .Fields.Append "Delta", adDouble
        .Fields.Append "VarVec", adDouble
        .Fields.Append "DSV", adDouble
        .Fields.Append "CorrInd", adDouble
        .Open
    End With
    If rsPositions Is Nothing Then
        Exit Function
    End If
    With rsPositions.Clone
        If Len(IndexSymbol) > 0 Then
            .Filter = "HedgingIndexSymbol='" & IndexSymbol & "'"
        End If
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                StockSymbol = !StockSymbol
                SymbolID = StockStockCorrelator.SymbolID(StockSymbol)
                Corr = !Corr
                Delta = !Delta
                Price = !StockPrice
                HV = !HV
                DSV = Delta * Price * HV / Sqr(252)
                With GetStocksList
                    .AddNew
                    !StockSymbol = StockSymbol
                    !SymbolID = SymbolID
                    !Price = Price
                    !Delta = Delta
                    !DSV = DSV
                    !CorrInd = Corr * DSV
                    .Update
                End With
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetStocksSubList(Optional ByVal HedgeSymbol As String) As ADODB.Recordset
Dim StockSymbol As String
Dim DSV As Double
Dim Corr1 As Double
Dim Corr2 As Double
Dim Corr3 As Double
Dim Corr4 As Double
Dim SymbolID As Long
    Set GetStocksSubList = New ADODB.Recordset
    With GetStocksSubList
        .Fields.Append "StockSymbol", adVarChar, 20
        .Fields.Append "SymbolID", adInteger
        .Fields.Append "DSV", adDouble
        .Fields.Append "Corr1", adDouble
        .Fields.Append "Corr2", adDouble
        .Fields.Append "Corr3", adDouble
        .Fields.Append "Corr4", adDouble
        .Open
    End With
    If rsPositions Is Nothing Then
        Exit Function
    End If
    With rsPositions.Clone
        .Filter = GetSubportfolioFilter(HedgeSymbol)
        If .RecordCount <> 0 Then
            .Sort = "StockSymbol"
            .MoveFirst
            StockSymbol = ""
            DSV = 0
            Do While Not .EOF
                If StockSymbol <> !StockSymbol Then
                    StockSymbol = !StockSymbol
                    SymbolID = !SymbolID
                    DSV = DBDouble(!DSV)
                    Corr1 = !Corr1
                    Corr2 = !Corr2
                    Corr3 = !Corr3
                    Corr4 = !Corr4
                    With GetStocksSubList
                        .AddNew
                        !StockSymbol = StockSymbol
                        !SymbolID = SymbolID
                        !DSV = DSV
                        !Corr1 = Corr1
                        !Corr2 = Corr2
                        !Corr3 = Corr3
                        !Corr4 = Corr4
                        .Update
                    End With
                    DSV = 0
                Else
                    DSV = DBDouble(!DSV)
                    With GetStocksSubList
                        !DSV = !DSV + DSV
                        .Update
                    End With
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CalculateHedgeVariations( _
    dVarPort As Double, _
    dCorPortInd1 As Double, _
    dCorPortInd2 As Double, _
    dCorPortInd3 As Double, _
    dCorPortInd4 As Double, _
    Optional ByVal HedgeSymbol As String _
)
Dim rsStockList As ADODB.Recordset

Dim Symbol1 As String
Dim Symbol2 As String
Dim Corr As Double
Dim bm As Variant

Dim VarPort As Double
Dim VarPortInd As Double
Dim DSV1 As Double
Dim DSV2 As Double

Dim StockSymbolID1 As Long
Dim StockSymbolID2 As Long

Dim CorInd(1 To 4) As Double
Dim CorPortInd(1 To 4) As Double
Dim i As Long
Dim Msg As String
    
    Set rsStockList = GetStocksSubList(HedgeSymbol)
    VarPort = 0
    For i = 1 To 4
        CorPortInd(i) = 0
    Next
    dCorPortInd1 = 0
    dCorPortInd2 = 0
    dCorPortInd3 = 0
    dCorPortInd4 = 0
    If Not rsStockList Is Nothing Then
        With rsStockList
            If .RecordCount <> 0 Then
                .Sort = "StockSymbol"
                .MoveFirst
                Do While Not .EOF
                    Symbol1 = !StockSymbol
                    StockSymbolID1 = !SymbolID
                    bm = .Bookmark
                    
                    DSV1 = !DSV
                    CorInd(1) = !Corr1
                    CorInd(2) = !Corr2
                    CorInd(3) = !Corr3
                    CorInd(4) = !Corr4
                    For i = 1 To 4
                        CorPortInd(i) = CorPortInd(i) + CorInd(i) * DSV1
                        If CorInd(i) = 0 Then
                            Select Case i
                            Case 1: Symbol2 = IIf(g_SelectSPX, "SPX", "")
                            Case 2: Symbol2 = IIf(g_SelectNDX, "NDX", "")
                            Case 3: Symbol2 = IIf(g_SelectOEX, "OEX", "")
                            Case 4: Symbol2 = IIf(g_SelectDJX, "DJX", "")
                            End Select
                            If Symbol2 <> "" Then
                                'Msg = "Zero Correlation between '" & Symbol1 & "' and '" & Symbol2 & "'"
                                Msg = "Cannot find Price/IV correlations between '" & Symbol1 & "' and '" & Symbol2 & "'" & ". Please go on Stocks page and run 'Get IV data'."
                                Err.Raise vbObjectError + 1, , Msg
                            End If
                        End If
                    Next
                    
                    VarPortInd = 0
                    .MoveNext
                    Do While Not .EOF
                        Symbol2 = !StockSymbol
                        StockSymbolID2 = !SymbolID
                        DSV2 = !DSV
                        Corr = StockStockCorrelator.dStockStockCorr(StockSymbolID1, StockSymbolID2)
                        If Corr = 0 Then
                            'Msg = "Zero Correlation between '" & Symbol1 & "' and '" & Symbol2 & "'"
                            Msg = "Cannot find Price/IV correlations between '" & Symbol1 & "' and '" & Symbol2 & "'" & ". Please go on Stocks page and run 'Get IV data'."
                            Err.Raise vbObjectError + 1, , Msg
                        End If
                        VarPortInd = VarPortInd + DSV2 * Corr
                        .MoveNext
                        DoEvents
                        If Not gRuntime Then Exit Sub
                        DoEvents
                    Loop
                    VarPort = VarPort + DSV1 * (DSV1 + VarPortInd + VarPortInd)
                    
                    .Bookmark = bm
                    .MoveNext
                    If Not gRuntime Then Exit Sub
                    DoEvents
                Loop
            End If
        End With
    End If
    
    dVarPort = VarPort
    dCorPortInd1 = CorPortInd(1)
    dCorPortInd2 = CorPortInd(2)
    dCorPortInd3 = CorPortInd(3)
    dCorPortInd4 = CorPortInd(4)
    
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DSVSum( _
    ByVal SymbolID1 As Long, _
    Optional ByVal HedgeSymbol As String _
) As Double
    DSVSum = 0
    With GetStocksSubList(HedgeSymbol)
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                DSVSum = DSVSum + !DSV * StockStockCorrelator.dStockStockCorr(SymbolID1, !SymbolID)
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReconnectIV() As Boolean
    Set DHIVLoader5 = Nothing
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NeedReconnectMT() As Boolean
    fNeedReconnectMT = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ReconnectMT() As Boolean
    If Not DHMTLoader1 Is Nothing Then
        DHMTLoader1.Disconnect
        If DHMTLoader1.Connect(g_Params.PriceProviderType) Then
            fNeedReconnectMT = False
        Else
            LastErrDescription = "Failed to connect to Price Provider"
            Err.Raise vbObjectError, "Price Feeder", LastErrDescription
        End If
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ConnectMT() As Boolean
    LunchMTLoader
    If DHMTLoader1.Connect(g_Params.PriceProviderType) Then
        fNeedReconnectMT = False
    Else
        LastErrDescription = "Failed to connect to Price Provider"
        Err.Raise vbObjectError, "Price Feeder", LastErrDescription
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DisconnectMT() As Boolean
    On Error Resume Next
    If Not DHMTLoader1 Is Nothing Then
        DHMTLoader1.Disconnect
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LunchMTLoader()
    If fNeedReconnectMT Then
        LastErrDescription = "You need to restart application to connect to price provider."
        Err.Raise vbObjectError, "Price Loader", LastErrDescription
        Exit Sub
    End If
    If DHMTLoader1 Is Nothing Then
        If Not gRuntime Then Exit Sub
        RaiseEvent Status("Initializing Price feeder..")
        On Error Resume Next
        Set DHMTLoader1 = New DHMTFeeder2.DHMTLoader
        On Error GoTo 0
        If DHMTLoader1 Is Nothing Then
            LastErrDescription = "Price Loader component can not be started." & vbCrLf & "Repair application installation."
            If Not gRuntime Then Exit Sub
            Err.Raise vbObjectError, "Price Loader", LastErrDescription
            Exit Sub
        End If
    End If
    If fNeedReconnectMT Then
        RaiseEvent Status("Connecting to Price Provider..")
        ReconnectMT
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LunchIVLoader()
    If DHIVLoader5 Is Nothing Then
        If Not gRuntime Then Exit Sub
        RaiseEvent Status("Initializing IV feeder..")
        On Error Resume Next
        Set DHIVLoader5 = New DHIVFeeder5.DHIVLoader
        On Error GoTo 0
        If DHIVLoader5 Is Nothing Then
            LastErrDescription = "IV Loader component can not be started." & vbCrLf & "Repair application installation."
            Err.Raise vbObjectError, "IV Loader", LastErrDescription
            Exit Sub
        End If
        On Error Resume Next
        If DHIVLoader5.g_SymbolsCountLimit > 1000 Then
            DHIVLoader5.g_SymbolsCountLimit = 1000
        End If
        Set DHHttpFeeder = DHIVLoader5.GetDHIVFeeder.GetHttpFeeder
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadIndexPrices() As Boolean
    On Error GoTo Herr
    TaskStarted
    FeedInProgress = True
    
    LoadIndexPrices = LoadIndexPricesMT
    
    UpdateIndexOptions
    FeedInProgress = False
    LoadIndexPrices = TaskFinished
    Exit Function
Herr:
    FeedInProgress = False
    If Not CancelRequested Then
        Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadIndexPricesMT() As Boolean
    LunchMTLoader
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Preparing Indexes List")
    With DHMTLoader1
        .StocksListClear
        Dim IndexSymbol As Variant
        For Each IndexSymbol In GetIndexSymbols
            .StocksListAddSymbol IndexSymbol
        Next
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Index Prices")
        LoadIndexPricesMT = .LoadStocksPriceFromMT
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function LoadIndexPricesIV()
    LunchIVLoader
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Preparing Indexes List")
    With DHIVLoader5
        .IndexListClear
        Dim IndexSymbol As Variant
        For Each IndexSymbol In GetIndexSymbols
            .IndexListAddSymbol IndexSymbol
        Next
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Index Prices From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Index Prices From IVolatility")
        LoadIndexPricesIV = .LoadIndexesPriceFromIV
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockPricesIV(Optional ByVal AllSymbols As Boolean = False) As Boolean
Dim StockSymbol As String
Dim StockPrice As Double
Dim Criteria As String
    
    TaskStarted
    
    LunchIVLoader
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Preparing Stocks List")
    DHIVLoader5.StocksListClear
    If AllSymbols Then
        ' Copy All Stock symbols to price loader
        With rsSymbolsList
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If Not IsNull(!StockSymbol) Then
                        StockSymbol = !StockSymbol
                        If Len(StockSymbol) > 0 And !Supported Then
                            With DHIVLoader5
                                .StocksListAddSymbol StockSymbol
                            End With
                        End If
                    End If
                    DoEvents
                    If CancelRequested Then
                        CancelAccepted = True
                        GoTo Done
                    End If
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    Else
        ' Copy Portfolio Stock symbols to price loader
        '!!! optimize: only distinct symbols
        With rsPortfolioData
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If Not IsNull(!StockSymbol) Then
                        StockSymbol = !StockSymbol
                        If Len(StockSymbol) > 0 Then
                        If CheckStockSymbol(StockSymbol) Then
                            With DHIVLoader5
                                .StocksListAddSymbol StockSymbol
                            End With
                        End If
                        End If
                    End If
                    DoEvents
                    If CancelRequested Then
                        CancelAccepted = True
                        GoTo Done
                    End If
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Loading Stocks Prices From IVolatility")
    If Not gRuntime Then Exit Function
    RaiseEvent LogMessage("Loading Stocks Prices From IVolatility")
    If DHIVLoader5.LoadStocksPriceFromIV Then
        If CancelRequested Then
            CancelAccepted = True
            GoTo Done
        End If
    End If
Done:
    LoadStockPricesIV = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockPricesMT(Optional ByVal AllSymbols As Boolean = False) As Boolean
Dim StockSymbol As String
Dim StockPrice As Double
Dim Criteria As String
    
    TaskStarted
        
    LunchMTLoader
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Preparing Stocks List")
    DHMTLoader1.StocksListClear
    If AllSymbols Then
        ' Copy All Stock symbols to price loader
        With rsSymbolsList
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If Not IsNull(!StockSymbol) Then
                        StockSymbol = !StockSymbol
                        If Len(StockSymbol) > 0 And !Supported Then
                            With DHMTLoader1
                                .StocksListAddSymbol StockSymbol
                            End With
                        End If
                    End If
                    .MoveNext
                    DoEvents
                    If CancelRequested Then
                        CancelAccepted = True
                        GoTo Done
                    End If
                    DoEvents
                Loop
            End If
        End With
    Else
        ' Copy Portfolio Stock symbols to price loader
        With rsPortfolioData
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    If Not IsNull(!StockSymbol) Then
                        StockSymbol = !StockSymbol
                        If Len(StockSymbol) > 0 Then
                        If CheckStockSymbol(StockSymbol) Then
                            With DHMTLoader1
                                .StocksListAddSymbol StockSymbol
                            End With
                        End If
                        End If
                    End If
                    .MoveNext
                    DoEvents
                    If CancelRequested Then
                        CancelAccepted = True
                        GoTo Done
                    End If
                    DoEvents
                Loop
            End If
        End With
    End If
    
    If CancelRequested Then
        CancelAccepted = True
        GoTo Done
    End If
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Loading Stock Prices")
    If DHMTLoader1.LoadStocksPriceFromMT Then
        If CancelRequested Then
            CancelAccepted = True
            GoTo Done
        End If
    End If
Done:
    LoadStockPricesMT = TaskFinished
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockIVData(Optional ByVal AllSymbols As Boolean = False) As Boolean
Dim StockSymbol As String
Dim IndexSymbol As String
Dim Criteria As String
Dim ClosePrice As Double
Dim rsChangedSymbols As ADODB.Recordset
Dim nStocks&, nIndices&
    
    On Error GoTo Herr
    TaskStarted
    FeedInProgress = True
    
    LunchIVLoader
    DHIVLoader5.HVTenor = g_lTerm
    
    If CancelRequested Then GoTo Canceled
    If Not gRuntime Then Exit Function
    RaiseEvent Status("Preparing Stocks List")
    DHIVLoader5.IndexListClear
    DHIVLoader5.StocksListClear

    If CancelRequested Then GoTo Canceled
    
    With DHIVLoader5
        If g_SelectSPX Then .IndexListAddSymbol "SPX"
        If g_SelectNDX Then .IndexListAddSymbol "NDX"
        If g_SelectOEX Then .IndexListAddSymbol "OEX"
        If g_SelectDJX Then .IndexListAddSymbol "DJX"
    End With
    
    If CancelRequested Then GoTo Canceled
    Set rsChangedSymbols = New ADODB.Recordset
    With rsChangedSymbols
        With .Fields
            .Append "StockSymbol", adVarChar, 20
            .Append "Supported", adBoolean
        End With
        .Open
    End With
    If AllSymbols Then
''        If vbYes = MsgBox("Do you want to reload list of Supported stocks?", vbYesNo) Then
'            IsMapped = False
'            If Not gRuntime Then Exit Function
'            RaiseEvent Status("Loading Stocks List From IVolatility")
'            If DHIVLoader5.LoadStocksListFromIV Then
'                ' Synchronize
'                With rsSymbolsList
'                    If .RecordCount <> 0 Then
'                        .MoveFirst
'                        Do While Not .EOF
'                            StockSymbol = !StockSymbol
'                            If !Supported Xor !Updated Then
'                                Dim f As Boolean
'                                f = !Updated
'                                If f Then
'                                    ' Symbol came to list
'                                    Debug.Print "Symbol '" & StockSymbol & "' came to the list"
'                                    LogSave "Symbol '" & StockSymbol & "' was added to the list of supported stocks"
'                                Else
'                                    ' Symbol gone from list
'                                    Debug.Print "Symbol '" & StockSymbol & "' gone from the list"
'                                    LogSave "Symbol '" & StockSymbol & "' was removed from the list of supported stocks"
'                                End If
'                                With rsChangedSymbols
'                                    .AddNew
'                                    !StockSymbol = StockSymbol
'                                    !Supported = f
'                                    .Update
'                                End With
'                            End If
'                            !Supported = !Updated
'                            !Updated = False
'                            On Error Resume Next
'                            Err.Clear
'                            .Update
'                            If Err Then
'                                .CancelUpdate
'                            End If
'                            On Error GoTo Herr
'                            .MoveNext
'                            DoEvents
'                        Loop
'                    End If
'                End With
'                If CancelRequested Then GoTo Canceled
'                With rsChangedSymbols
'                    If .RecordCount <> 0 Then
'                        If Not gRuntime Then Exit Function
'                        RaiseEvent GotSymbolsChanged(rsChangedSymbols)
'                    End If
'                End With
'            End If
''        Else
            DHIVLoader5.StocksListClear
            With rsSymbolsList
                If .RecordCount <> 0 Then
                    .MoveFirst
                    Do While Not .EOF
                        If !Supported Then
                            DHIVLoader5.StocksListAddSymbol !StockSymbol
                        End If
                        .MoveNext
                        DoEvents
                    Loop
                End If
            End With
''        End If
    Else
        With rsPortfolioData
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    StockSymbol = !StockSymbol
                    If StockSymbol <> "" Then
                    If CheckStockSymbol(StockSymbol) Then
                        With DHIVLoader5
                            .StocksListAddSymbol StockSymbol
                        End With
                    End If
                    End If
                    .MoveNext
                    DoEvents
                    If CancelRequested Then GoTo Canceled
                    DoEvents
                Loop
            End If
        End With
    End If
    
    On Error Resume Next
    On Error GoTo Herr
    
    nIndices = DHIVLoader5.rsIndexData.RecordCount
    nStocks = DHIVLoader5.rsStockData.RecordCount
    
    If nIndices > 0 Then
        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Indexes HV From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Indexes HV From IVolatility")
        DHIVLoader5.LoadIndexHVFromIV

        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Indexes Vega From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Indexes Vega From IVolatility")
        DHIVLoader5.LoadIndexesVegaFromIV
    End If

    If nStocks > 0 Then
        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Stocks HV From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Stocks HV From IVolatility")
        DHIVLoader5.LoadStocksHVFromIV
    End If

    If nIndices > 1 Then
        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Index/Index Corr From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Index/Index Corr From IVolatility")
        DHIVLoader5.LoadIndexIndexCorrFromIV
    End If

    If nIndices > 0 And nStocks > 0 Then
        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Stock/Index Corr From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Stock/Index Corr From IVolatility")
        DHIVLoader5.LoadStockIndexCorrFromIV
    End If

'    If CancelRequested Then GoTo Canceled
'    If Not gRuntime Then Exit Function
'    RaiseEvent Status("Loading Stock/Index Beta From IVolatility")
'    If Not gRuntime Then Exit Function
'    RaiseEvent LogMessage("Loading Stock/Index Beta From IVolatility")
'    DHIVLoader5.LoadStockIndexBetaFromIV
    
'    If AllSymbols Then
'        If vbOK <> MsgBox( _
'            "Do you want to load all available Stock/Stock correlation data? This may take a long time. You can safely stop the feed at any time and resume later." _
'            , vbOKCancel) Then
'            GoTo Done
'        End If
'    End If

    If nStocks > 1 Then
        If CancelRequested Then GoTo Canceled
        If Not gRuntime Then Exit Function
        RaiseEvent Status("Loading Stock/Stock Corr From IVolatility")
        If Not gRuntime Then Exit Function
        RaiseEvent LogMessage("Loading Stock/Stock Corr From IVolatility")
        DHIVLoader5.LoadStockStockCorrFromIV
    End If

Done:
    TaskFinished
    If Not gRuntime Then Exit Function
    RaiseEvent Status("")
    FeedInProgress = False
    Exit Function
Canceled:
    CancelAccepted = True
    GoTo Done
    Exit Function
Herr:
    FeedInProgress = False
'    If Not CancelRequested Then
        Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
'    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotIndexHVData4(ByVal IndexSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Index HV From IVolatility for " & IndexSymbol)
'    RaiseEvent LogMessage("Loading Index HV From IVolatility for " & IndexSymbol)
    SetIndexHVData3 IndexSymbol, 0, HV, IV, VolOfVol, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotIndexHVData", IndexSymbol, HV, IV, VolOfVol
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotIndexPriceData4(ByVal IndexSymbol As String, ByVal ClosePrice As Double, ByVal PriceDate As Date)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Index Price From IVolatility for " & IndexSymbol)
'    RaiseEvent LogMessage("Loading Index Price From IVolatility for " & IndexSymbol)
    SetIndexPrice IndexSymbol, ClosePrice, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotIndexPriceData", IndexSymbol, ClosePrice
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotIndexVegaData4(ByVal IndexSymbol As String, ByVal Vega As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Index Vega From IVolatility for " & IndexSymbol)
'    RaiseEvent LogMessage("Loading Index Vega From IVolatility for " & IndexSymbol)
    SetIndexVega IndexSymbol, Vega, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotIndexVega", IndexSymbol, Vega
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotStockStockCorrData4(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Stock/Stock Corr From IVolatility for " & StockSymbol1 & "/" & StockSymbol2)
    SetStockStockCorr StockSymbol1, StockSymbol2, Corr / 10000, VCorr / 10000
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockStockCorr", StockSymbol1, StockSymbol2, Corr, VCorr
#End If
    IsMapped = False
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotIndexIndexCorrData4(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    'RaiseEvent Status("Loading Index/Index Corr From IVolatility for " & IndexSymbol1 & "/" & IndexSymbol2)
'    RaiseEvent LogMessage("Loading Index/Index Corr From IVolatility for " & IndexSymbol1 & "/" & IndexSymbol2)
    SetIndexIndexCorr IndexSymbol1, IndexSymbol2, Corr / 10000, VCorr / 10000
#If DEBUG_REPLY Then
    DebugSaveEx "GotIndexIndexCorr", IndexSymbol1, IndexSymbol2, Corr, VCorr
#End If
    IsMapped = False
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotStockSymbolData4(ByVal StockSymbol As String, ByVal IndexSymbol As String)
Dim Criteria As String
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Stock List From IVolatility " & StockSymbol)
'    RaiseEvent LogMessage("Loading Stock List From IVolatility " & StockSymbol)
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockSymbolData", StockSymbol
#End If
    Criteria = "StockSymbol='" & StockSymbol & "'"
    With rsSymbolsList
        If Not FindRSStockSymbol(rsSymbolsList, StockSymbol) Then
            .AddNew
            !StockSymbol = StockSymbol
        End If
        !Updated = True
        If IsNull(!Supported) Then
            !Supported = False
        End If
        !Selected = False
        On Error Resume Next
        Err.Clear
        .Update
        If Err Then
            .CancelUpdate
        End If
    End With
    IsMapped = False
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_LogMessage(ByVal Message As String)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent LogMessage(Message)
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_Progress(ByVal ProgressPercent As Long)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent Progress(ProgressPercent)
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_ProgressStart()
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent ProgressStart
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_ProgressStop()
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent ProgressStop
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTLoader1_Progress(ByVal ProgressPercent As Long)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent Progress(ProgressPercent)
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTLoader1_ProgressStart()
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent ProgressStart
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTLoader1_ProgressStop()
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent ProgressStop
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_Status(ByVal Message As String)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent Status(Message)
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTLoader1_GotStockPriceData(ByVal StockSymbol As String, ByVal StockPrice As Double)
    On Error GoTo Herr
    Select Case StockSymbol
    Case "SPX", "NDX", "OEX", "DJX"
        SetIndexPrice StockSymbol, StockPrice, DATA_FEED
    Case Else
        SetStockPrice StockSymbol, StockPrice, DATA_FEED
    End Select
#If DEBUG_REPLY Then
    DebugSaveEx "DHMTLoader1_GotStockPriceData", StockSymbol, StockPrice
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

Private Sub DHIVLoader5_GotStockIndexBetaData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Weight As Double, ByVal Beta As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    SetStockIndexWeightBetaData StockSymbol, IndexSymbol, Weight, Beta, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockIndexBetaData4", StockSymbol, IndexSymbol, Weight, Beta
#End If
    Exit Sub
Herr:
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotStockIndexCorrData4(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal VCorr As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Stock/Index Corr From IVolatility for " & StockSymbol & "/" & IndexSymbol)
'    RaiseEvent LogMessage("Loading Stock/Index Corr From IVolatility for " & StockSymbol & "/" & IndexSymbol)
    SetStockIndexCorrData StockSymbol, IndexSymbol, Corr / 10000, DATA_FEED
    SetStockIndexVCorrData StockSymbol, IndexSymbol, VCorr / 10000, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockIndexCorrData", StockSymbol, IndexSymbol, Corr, VCorr
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotStockPriceData4(ByVal StockSymbol As String, ByVal ClosePrice As Double, ByVal PriceDate As Date)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Stock Price From IVolatility for " & StockSymbol)
'    RaiseEvent LogMessage("Loading Stock Price From IVolatility for " & StockSymbol)
    SetStockPrice StockSymbol, ClosePrice, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockPriceData", StockSymbol, ClosePrice
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHIVLoader5_GotStockHVData4(ByVal StockSymbol As String, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
'    RaiseEvent Status("Loading Stock HV From IVolatility for " & StockSymbol)
'    RaiseEvent LogMessage("Loading Stock HV From IVolatility for " & StockSymbol)
    SetStockHVData3 StockSymbol, 0, HV, IV, VolOfVol, DATA_FEED
#If DEBUG_REPLY Then
    DebugSaveEx "GotStockHVData", StockSymbol, HV, IV, VolOfVol
#End If
    Exit Sub
Herr:
'    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHMTLoader1_Status(ByVal Message As String)
    On Error GoTo Herr
    If Not gRuntime Then Exit Sub
    RaiseEvent Status(Message)
    If Not gRuntime Then Exit Sub
    RaiseEvent LogMessage(Message)
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function CheckStockSymbol(StockSymbol As String) As Boolean
    ' Check for apostrophe etc
    ValidateStockSymbol StockSymbol
    If FindRSStockSymbol(rsSymbolsList, StockSymbol) Then
        CheckStockSymbol = rsSymbolsList!Supported
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SelectStockSymbols(rsStocks As ADODB.Recordset) As Boolean
Dim Sql As String
    Sql = "EXEC usp_DeselectAllStocks"
    ExecuteSql Sql
    
    rsSymbolsList.Requery
    If Not rsStocks Is Nothing Then
        With rsStocks
            If .RecordCount <> 0 Then
                .MoveFirst
                Do While Not .EOF
                    SelectStockSymbol !StockSymbol
                    .MoveNext
                Loop
            End If
        End With
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SelectStockSymbol(ByVal StockSymbol As String) As Boolean
    ValidateStockSymbol StockSymbol
    If FindRSStockSymbol(rsSymbolsList, StockSymbol) Then
        rsSymbolsList!Selected = True
        rsSymbolsList.Update
        SelectStockSymbol = True
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockPrice(ByVal StockSymbol As String) As Double
Dim Price As Double
    usp_PriceCloseDH_Get StockSymbol, Price
    GetStockPrice = Price
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexPrice(ByVal IndexSymbol As String, ByVal IndexPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    SetSymbolPrice IndexSymbol, IndexPrice, DataStatus
    If Not gRuntime Then Exit Sub
    RaiseEvent GotIndexPriceData(IndexSymbol, IndexPrice, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockPrice(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    SetSymbolPrice StockSymbol, StockPrice, DataStatus
    UpdatePortfolioPrice StockSymbol, StockPrice, DataStatus
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStockPriceData(StockSymbol, StockPrice, DataStatus)
End Sub
    
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdatePortfolioPrice(ByVal StockSymbol As String, ByVal StockPrice As Double, ByVal DataStatus As enmDATA_STATUS)
    With rsPortfolioData.Clone
        .Filter = "StockSymbol='" & StockSymbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If StockPrice <> 0 Then
                    !StockPrice = StockPrice
                    .Update
                End If
               .MoveNext
                DoEvents
            Loop
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub UpdatePortfolioIV(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    With rsPortfolioData.Clone
        .Filter = "StockSymbol='" & StockSymbol & "'"
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If IV <> 0 Then
                    !IV = IV
                    .Update
                End If
               .MoveNext
                DoEvents
            Loop
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetSymbolPrice(ByVal StockSymbol As String, ByVal Price As Double, ByVal DataStatus As enmDATA_STATUS)
    If Len(StockSymbol) > 0 Then
        usp_PriceCloseDH_Set StockSymbol, Price
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockHV(ByVal StockSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
    If usp_StockDataHistoryDH_Get(StockSymbol, IV, HV, VoV) Then
        GetStockHV = HV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIV(ByVal StockSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
    If usp_StockDataHistoryDH_Get(StockSymbol, IV, HV, VoV) Then
        GetStockIV = IV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockVoV(ByVal StockSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
    If usp_StockDataHistoryDH_Get(StockSymbol, IV, HV, VoV) Then
        GetStockVoV = VoV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockHVData(ByVal StockSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_StockDataHistoryDH_Set StockSymbol, 0, HV, 0
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStockHVData(StockSymbol, HV, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockIVData(ByVal StockSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_StockDataHistoryDH_Set StockSymbol, IV, 0, 0
    UpdatePortfolioIV StockSymbol, IV, DataStatus
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStockIVData(StockSymbol, IV, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockHVData3(ByVal StockSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_StockDataHistoryDH_Set StockSymbol, IV, HV, VolOfVol
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStockHVData3(StockSymbol, Tenor, HV, IV, VolOfVol, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexHV(ByVal IndexSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
Dim Vega As Double
    If usp_IndexDataHistoryDH_Get(IndexSymbol, IV, HV, VoV, Vega) Then
        GetIndexHV = HV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexIV(ByVal IndexSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
Dim Vega As Double
    If usp_IndexDataHistoryDH_Get(IndexSymbol, IV, HV, VoV, Vega) Then
        GetIndexIV = IV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexVoV(ByVal IndexSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
Dim Vega As Double
    If usp_IndexDataHistoryDH_Get(IndexSymbol, IV, HV, VoV, Vega) Then
        GetIndexVoV = VoV
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetIndexVega(ByVal IndexSymbol As String) As Double
Dim IV As Double
Dim HV As Double
Dim VoV As Double
Dim Vega As Double
    If usp_IndexDataHistoryDH_Get(IndexSymbol, IV, HV, VoV, Vega) Then
        GetIndexVega = Vega
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexVega(ByVal IndexSymbol As String, ByVal Vega As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_IndexDataHistoryDH_Set IndexSymbol, 0, 0, 0, Vega
    If Not gRuntime Then Exit Sub
    RaiseEvent GotIndexVega(IndexSymbol, Vega, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexHVData(ByVal IndexSymbol As String, ByVal HV As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_IndexDataHistoryDH_Set IndexSymbol, 0, HV, 0, 0
    If Not gRuntime Then Exit Sub
    RaiseEvent GotIndexHVData(IndexSymbol, HV, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexIVData(ByVal IndexSymbol As String, ByVal IV As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_IndexDataHistoryDH_Set IndexSymbol, IV, 0, 0, 0
    If Not gRuntime Then Exit Sub
    RaiseEvent GotIndexIVData(IndexSymbol, IV, DataStatus)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexHVData3(ByVal IndexSymbol As String, ByVal Tenor As Long, ByVal HV As Double, ByVal IV As Double, ByVal VolOfVol As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_IndexDataHistoryDH_Set IndexSymbol, IV, HV, VolOfVol, 0
    If Not gRuntime Then Exit Sub
    RaiseEvent GotIndexHVData3(IndexSymbol, Tenor, HV, IV, VolOfVol, DataStatus)
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Private Sub MapStockStockCorr()
'    '!!!
'    Debug.Assert False
'    Exit Sub
'    Set StockStockCorrelator = New CStockStockCorrelator
'    StockStockCorrelator.Clear
'
'    If Not rsSymbolsList Is Nothing Then
'        With rsSymbolsList
'            If .RecordCount <> 0 Then
'                .MoveFirst
'                Do While Not .EOF
'                    StockStockCorrelator.AddStock !StockSymbol
'                    .MoveNext
'                Loop
'            End If
'        End With
'    End If
'
'    StockStockCorrelator.AllocateMemory
'
'    UpdateStockStockCorrelator
'
'End Sub
'
''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'' DB->Matrix
''
'Public Sub UpdateStockStockCorrelator()
'Dim Count As Long
'    If Not StockStockCorrelator Is Nothing Then
'    If Not rsStockStockCorr3 Is Nothing Then
'        With rsStockStockCorr3
'            If .RecordCount <> 0 Then
'                .MoveFirst
'                Count = 0
'                If fVegaMode Then
'                    Do While Not .EOF
'                        StockStockCorrelator.StockStockCorr(!StockSymbol1, !StockSymbol2) = DBDouble(!VCorr)
'                        Count = Count + 1
'                        If Count > 1000 Then
'                            If CancelRequested Then GoTo Done
'                            DoEvents
'                            Count = 0
'                        End If
'                        .MoveNext
'                    Loop
'                Else
'                    Do While Not .EOF
'                        StockStockCorrelator.StockStockCorr(!StockSymbol1, !StockSymbol2) = DBDouble(!Corr)
'                        Count = Count + 1
'                        If Count > 1000 Then
'                            If CancelRequested Then GoTo Done
'                            DoEvents
'                            Count = 0
'                        End If
'                        .MoveNext
'                    Loop
'                End If
'            End If
'        End With
'    End If
'    End If
'Done:
'    If CancelRequested Then
'
'    Else
'
'    End If
'End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'' DB<-Matrix
''
'Public Sub UpdateStockStockCorrelator2()
'    If Not StockStockCorrelator Is Nothing Then
'    If Not rsStockStockCorr3 Is Nothing Then
'        With rsStockStockCorr3
'            If .RecordCount <> 0 Then
'                .MoveFirst
'                If fVegaMode Then
'                    Do While Not .EOF
'                        !VCorr = StockStockCorrelator.StockStockCorr(!StockSymbol1, !StockSymbol2)
'                        .Update
'                        .MoveNext
'                    Loop
'                Else
'                    Do While Not .EOF
'                        !Corr = StockStockCorrelator.StockStockCorr(!StockSymbol1, !StockSymbol2)
'                        .Update
'                        .MoveNext
'                    Loop
'                End If
'            End If
'        End With
'    End If
'    End If
'End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetStockStockCorrelator(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    If Not StockStockCorrelator Is Nothing Then
        If fVegaMode Then
            StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2) = DBDouble(VCorr)
        Else
            StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2) = DBDouble(Corr)
        End If
    End If
End Sub

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Public Function GetStockStockCorrelator(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String) As Double
'    If Not StockStockCorrelator Is Nothing Then
'        If fVegaMode Then
'            GetStockStockCorrelator = StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2)
'        Else
'            GetStockStockCorrelator = StockStockCorrelator.StockStockCorr(StockSymbol1, StockSymbol2)
'        End If
'    End If
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetStockStockCorr(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String) As Double
Dim Corr As Double
Dim VCorr As Double
    If StockSymbol1 > StockSymbol2 Then
        Swap StockSymbol1, StockSymbol2
    End If
    usp_StockStockCorrelation_Get StockSymbol1, StockSymbol2, Corr, VCorr
    If fVegaMode Then
        GetStockStockCorr = VCorr
    Else
        GetStockStockCorr = Corr
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetStockStockCorr(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    SetStockStockCorr1 StockSymbol1, StockSymbol2, Corr, VCorr
    SetStockStockCorrelator StockSymbol1, StockSymbol2, Corr, VCorr
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetStockStockCorr1(ByVal StockSymbol1 As String, ByVal StockSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    If StockSymbol1 > StockSymbol2 Then
        Swap StockSymbol1, StockSymbol2
    End If
    If Abs(Corr) > 1 Then
        Corr = 0
    End If
    If Abs(VCorr) > 1 Then
        VCorr = 0
    End If
    usp_StockStockCorrelation_Set StockSymbol1, StockSymbol2, Corr, VCorr
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetIndexIndexCorr(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String) As Double
Dim Corr As Double
Dim VCorr As Double
    If IndexSymbol1 > IndexSymbol2 Then
        Swap IndexSymbol1, IndexSymbol2
    End If
    usp_IndexIndexCorrelation_Get IndexSymbol1, IndexSymbol2, Corr, VCorr
    If fVegaMode Then
        GetIndexIndexCorr = VCorr
    Else
        GetIndexIndexCorr = Corr
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub SetIndexIndexCorr(ByVal IndexSymbol1 As String, ByVal IndexSymbol2 As String, ByVal Corr As Double, ByVal VCorr As Double)
    If IndexSymbol1 > IndexSymbol2 Then
        Swap IndexSymbol1, IndexSymbol2
    End If
    If Abs(Corr) > 1 Then
        Corr = 0
    End If
    If Abs(VCorr) > 1 Then
        VCorr = 0
    End If
    usp_IndexIndexCorrelation_Set IndexSymbol1, IndexSymbol2, Corr, VCorr
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIndexCorr(ByVal StockSymbol As String, ByVal IndexSymbol As String) As Double
Dim Corr As Double
Dim VCorr As Double
    GetStockIndexCorr2 StockSymbol, IndexSymbol, Corr, VCorr
    If fVegaMode Then
        GetStockIndexCorr = VCorr
    Else
        GetStockIndexCorr = Corr
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockIndexCorr2(ByVal StockSymbol As String, ByVal IndexSymbol As String, Corr As Double, VCorr As Double) As Boolean
    usp_StockIndexCorrelation_Get StockSymbol, IndexSymbol, Corr, VCorr
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockIndexWeightBetaData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Weight As Double, ByVal Beta As Double, ByVal DataStatus As enmDATA_STATUS)
    usp_IndexDefinition_Save StockSymbol, IndexSymbol, Weight, Beta
'    SetStockIndexBetaData StockSymbol, IndexSymbol, Beta, DATA_FEED
'    SetStockIndexWeightData StockSymbol, IndexSymbol, Weight, DATA_FEED
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockIndexCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal Corr As Double, ByVal DataStatus As enmDATA_STATUS)
    If Abs(Corr) > 1 Then
        Corr = 0
    End If
    usp_StockIndexCorrelation_Set StockSymbol, IndexSymbol, Corr, 0
    If Not gRuntime Then Exit Function
    RaiseEvent GotStockIndexCorrData(StockSymbol, IndexSymbol, Corr, DataStatus)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function SetStockIndexVCorrData(ByVal StockSymbol As String, ByVal IndexSymbol As String, ByVal VCorr As Double, ByVal DataStatus As enmDATA_STATUS)
    If Abs(VCorr) > 1 Then
        VCorr = 0
    End If
    usp_StockIndexCorrelation_Set StockSymbol, IndexSymbol, 0, VCorr
    If Not gRuntime Then Exit Function
    RaiseEvent GotStockIndexVCorrData(StockSymbol, IndexSymbol, VCorr, DataStatus)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskStarted() As Boolean
    TaskStarted = Not TaskInProgress
'    If TaskStarted Then
        TaskInProgress = True
        CancelRequested = False
        CancelAccepted = False
'    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskFinished() As Boolean
    TaskFinished = TaskInProgress And Not CancelRequested
    TaskInProgress = False
    FeedInProgress = False
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function TaskCanceled() As Boolean
    TaskCanceled = TaskInProgress And CancelAccepted
    TaskInProgress = False
    FeedInProgress = False
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
    CancelLoad
    If Not gRuntime Then Exit Function
    RaiseEvent GotCancelTask
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub CancelLoad()
    On Error GoTo Herr
    If Not DHIVLoader5 Is Nothing Then
        DHIVLoader5.CancelTask
    End If
    If Not DHMTLoader1 Is Nothing Then
        DHMTLoader1.CancelTask
    End If
    Exit Sub
Herr:
    ShowError
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function LoadStockPrices(Optional ByVal AllSymbols As Boolean = False) As Boolean
    On Error GoTo Herr
    FeedInProgress = True
    
    LoadStockPrices = LoadStockPricesMT(AllSymbols)
    
    FeedInProgress = False
    Exit Function
Herr:
    FeedInProgress = False
    If Not CancelRequested Then
        Err.Raise Err.Number, Err.Source, Err.Description, Err.HelpFile, Err.HelpContext
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetIndexSelected(ByVal IndexSymbol As String, ByVal IndexSelected As Boolean)
Dim Criteria As String
    With rsIndexOptions
        If .RecordCount <> 0 Then
            .MoveFirst
        End If
        Criteria = "IndexSymbol='" & IndexSymbol & "'"
        .Find Criteria, 0, adSearchForward, adBookmarkFirst
        If Not .EOF Then
            If !Selected <> IndexSelected Then
                !Selected = IndexSelected
                .Update
                NeedSplitPortfolio = True
            End If
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StartTask(Optional ByVal Caption As String = "", Optional ByVal CanCancel As Boolean = False)
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStartTask(Caption, CanCancel)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub StopTask(Optional ByVal Caption As String = "")
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    RaiseEvent GotStopTask(Caption)
End Sub

#If DEBUG_REPLY > 1 Then

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_GotReply(ByVal Message As String)
    DebugSave "DHHttpFeeder_GotReply:"
    DebugSave Message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_GotRequest(ByVal Message As String)
    DebugSave "DHHttpFeeder_GotRequest:"
    DebugSave Message
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_LogMessage(ByVal Message As String)
    DebugSave "DHHttpFeeder_LogMessage:"
    DebugSave Message
End Sub

#End If

#If DEBUG_REPLY > 3 Then
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub DHHttpFeeder_StateChanged(ByVal State As Long, ByVal Message As String)
    DebugSave "DHHttpFeeder_StateChanged:"
    DebugSave State, Message
End Sub

#End If

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub SetQByIndex(ByVal HedgeSymbol As String, ByVal QSPX As Double, ByVal QNDX As Double, ByVal QOEX As Double, ByVal QDJX As Double)
Dim IndexPrice As Double
    With rsIndexOptions
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If IsNull(!IndexPrice) Then
                    IndexPrice = 0
                Else
                    IndexPrice = !IndexPrice
                End If
                If HedgeSymbol = "SHORT" Then
                    Select Case !IndexSymbol
                    Case "SPX"
                        !QShort = QSPX
                    Case "NDX"
                        !QShort = QNDX
                    Case "OEX"
                        !QShort = QOEX
                    Case "DJX"
                        !QShort = QDJX
                    End Select
                    If IndexPrice = 0 Then
                        !ShortContractsAmount = Null
                    Else
                        !ShortContractsAmount = !QShort / IndexPrice
                    End If
                ElseIf HedgeSymbol = "LONG" Then
                    Select Case !IndexSymbol
                    Case "SPX"
                        !QLong = QSPX
                    Case "NDX"
                        !QLong = QNDX
                    Case "OEX"
                        !QLong = QOEX
                    Case "DJX"
                        !QLong = QDJX
                    End Select
                    '~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                    ' Calculate Index Contracts ammount
                    '
                    If IndexPrice = 0 Then
                        !LongContractsAmount = Null
                    Else
                        !LongContractsAmount = !QLong / IndexPrice
                    End If
                Else
                    Select Case !IndexSymbol
                    Case "SPX"
                        !QShort = QSPX
                    Case "NDX"
                        !QShort = QNDX
                    Case "OEX"
                        !QShort = QOEX
                    Case "DJX"
                        !QShort = QDJX
                    End Select
                    If IndexPrice = 0 Then
                        !ShortContractsAmount = Null
                    Else
                        !ShortContractsAmount = !QShort / IndexPrice
                    End If
                End If
                .MoveNext
                DoEvents
            Loop
        End If
    End With
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get VegaMode() As Boolean
    VegaMode = fVegaMode
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let VegaMode(ByVal vNewValue As Boolean)
    IsMapped = False
'    If fVegaMode <> vNewValue Then
        fVegaMode = vNewValue
        If Not gRuntime Then Exit Property
        RaiseEvent VegaModeChanged(fVegaMode)
        MySaveSettingBool "VegaHedge", fVegaMode
'    End If
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get ShortPortfolio() As Boolean
    ShortPortfolio = fShortPortfolio
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let ShortPortfolio(ByVal vNewValue As Boolean)
    If fShortPortfolio <> vNewValue Then
        fShortPortfolio = vNewValue
        If Not gRuntime Then Exit Property
        RaiseEvent ShortPortfolioModeChanged(fShortPortfolio)
        MySaveSettingLong "ShortPortfolio", fShortPortfolio
    End If
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub ValidateHVTerm()
    Select Case g_lTerm
    Case 252
        ' OK
    Case Else
        g_lTerm = DEFAULT_TERM
    End Select
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get HVTerm() As Long
    HVTerm = g_lTerm
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let HVTerm(ByVal vNewValue As Long)
    IsMapped = False
    ValidateHVTerm
    If g_lTerm <> vNewValue Then
        g_lTerm = vNewValue
        '///////////////
        LoadStockData   '//
        '/////////////
        If Not gRuntime Then Exit Property
        RaiseEvent HVTermChanged(g_lTerm)
        MySaveSettingLong "Term", g_lTerm
    End If
    
    If Not rsTerms Is Nothing Then
        With rsTerms
            If .RecordCount <> 0 Then
                .MoveFirst
                .Find "Term=" & g_lTerm
                If Not .EOF Then
                    !Selected = True
                    .Update
                End If
            End If
        End With
    End If
    
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LoadStockData()

#If ETS Then

    Set rsSymbolsList = New ADODB.Recordset
    With rsSymbolsList
        With .Fields
            .Append "StockID", adInteger
            .Append "StockSymbol", adVarChar, 20
            .Append "Supported", adBoolean
            .Append "Selected", adBoolean
            .Append "Updated", adBoolean
        End With
        .Open
    End With
        
    Dim rsStockData As ADODB.Recordset
    Dim StockSymbol As String
    Dim i As Long
    Set rsStockData = usp_StockReportDH_Get("")
    i = 0
    With rsStockData
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                StockSymbol = !Symbol
                i = i + 1
                With rsSymbolsList
                    .AddNew
                    !StockID = i
                    !StockSymbol = StockSymbol
                    !Supported = True
                    !Selected = False
                    !Updated = False
                    .Update
                End With
                .MoveNext
            Loop
        End If
    End With
    
    Set rsTerms = New ADODB.Recordset
    With rsTerms
        With .Fields
            .Append "Term", adInteger
            .Append "Selected", adBoolean
            .Append "Supported", adBoolean
        End With
        .Open
        .AddNew
        !Term = 252
        !Selected = True
        !Supported = True
        .Update
    End With
    
#Else
    
    Set rsSymbolsList = OpenRSDynamic("exec usp_SupportedStockSymbols_Get")
    Set rsTerms = OpenRSDynamic("exec usp_Terms_Get")

#End If

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function UnLoadStockData()
    On Error Resume Next
    rsSymbolsList.Close
    rsTerms.Close
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Sub Requery()
    On Error Resume Next
    rsSymbolsList.Requery
    rsTerms.Requery
    IsMapped = False
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub LogSave(ByVal Message As String)
    If Not gRuntime Then Exit Sub
    RaiseEvent LogMessage(Message)
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Get PortfolioName() As String
    PortfolioName = mPortfolioName
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Property Let PortfolioName(ByVal sNewValue As String)
    If Len(sNewValue) > 0 Then
        mPortfolioName = sNewValue
    Else
        mPortfolioName = "NewPortfolio.pfl"
    End If
    If Not gRuntime Then Exit Property
    RaiseEvent PortfolioNameChanged(mPortfolioName)
End Property

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintPortfolio()

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintSummary()

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintShortHedge()

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub PrintLongHedge()

End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function NewExpiry() As ADODB.Recordset
    Set NewExpiry = New ADODB.Recordset
    With NewExpiry
        With .Fields
            .Append "ExpMonth", adInteger
            .Append "ExpYear", adInteger
            .Append "ExpDay", adInteger
            .Append "RefCount", adInteger
            .Append "ExpDate", adDate
            .Append "ExpString", adVarChar, 20
        End With
        .Open
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetExpiry() As Boolean
    On Error Resume Next
    Set rsExpiry = NewExpiry
    If Not rsPortfolioData Is Nothing Then
        With rsPortfolioData
            If .RecordCount Then
                .MoveFirst
                Do While Not .EOF
                    If Not IsNull(!Expiration) Then
                        AddExpiry !Expiration
                    End If
                    .MoveNext
                    DoEvents
                Loop
            End If
        End With
    End If
    If Not gRuntime Then Exit Function
    RaiseEvent NewExpiry(rsExpiry)
    GetExpiry = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ChangeExpiry(ByVal OldExpDate As Variant, ByVal NewExpDate As Variant) As Boolean
Dim m1 As Integer
Dim y1 As Integer
Dim m2 As Integer
Dim y2 As Integer
    On Error Resume Next
    
    If Not IsDate(OldExpDate) Then
        If IsDate(NewExpDate) Then
            AddExpiry NewExpDate
            GoTo Done
        End If
        Exit Function
    End If
    
    If Not IsDate(NewExpDate) Then
        If IsDate(OldExpDate) Then
            DelExpiry OldExpDate
            GoTo Done
        End If
        Exit Function
    End If
    
    m1 = Month(OldExpDate)
    y1 = Year(OldExpDate)
    m2 = Month(NewExpDate)
    y2 = Year(NewExpDate)
    If (m1 <> m2) Or (y1 <> y2) Then
        DelExpiry OldExpDate
        AddExpiry NewExpDate
    End If
    
Done:
    If NeedUpdateExpiry Then
        If Not gRuntime Then Exit Function
        RaiseEvent NewExpiry(rsExpiry)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function AddExpiry(ByVal ExpDate As Date) As Boolean
Dim m As Integer
Dim y As Integer
    If rsExpiry Is Nothing Then
        Exit Function
    End If
    m = Month(ExpDate)
    y = Year(ExpDate)
    With rsExpiry
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If !ExpYear = y Then
                    If !ExpMonth = m Then
                        GoTo Found
                    End If
                End If
                .MoveNext
                DoEvents
            Loop
        End If
        ' Not found
        .AddNew
        !ExpMonth = m
        !ExpYear = y
        !RefCount = 1
        .Update
        NeedUpdateExpiry = True
        Exit Function
Found:
        !RefCount = !RefCount + 1
        .Update
    End With
    AddExpiry = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DelExpiry(ByVal ExpDate As Date) As Boolean
Dim m As Integer
Dim y As Integer
    If rsExpiry Is Nothing Then
        Exit Function
    End If
    m = Month(ExpDate)
    y = Year(ExpDate)
    With rsExpiry
        If .RecordCount <> 0 Then
            .MoveFirst
            Do While Not .EOF
                If !ExpYear = y Then
                    If !ExpMonth = m Then
                        GoTo Found
                    End If
                End If
                .MoveNext
                DoEvents
            Loop
        End If
        ' Not found
        Exit Function
Found:
        If !RefCount > 1 Then
            !RefCount = !RefCount - 1
            .Update
        Else
            .Delete
            NeedUpdateExpiry = True
        End If
    End With
    DelExpiry = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowHelpTagsUpdate()
    On Error Resume Next
    If Not gRuntime Then Exit Sub
    RaiseEvent ShowHelpTipsChanged
End Sub

