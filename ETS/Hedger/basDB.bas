Attribute VB_Name = "basDB"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public CN As ADODB.Connection
Public g_DBVersion As String


Public Function OpenDataBase(ByVal hWnd As Long) As Boolean
    On Error GoTo EH

    OpenDataBase = False
    If gDBW Is Nothing Then Set gDBW = New clsDataSource
    If CN Is Nothing Then Set CN = New ADODB.Connection
    
    CN.ConnectionString = g_Params.DbConnection
    CN.ConnectionTimeout = 30
    CN.CursorLocation = adUseClient
    If CN.ConnectionString <> "" Then
        CN.Open
    Else
        CN.Provider = "SQLOLEDB"
        Err.Raise vbObjectError + 1023, "Application Settings", "Invalid database settings."
    End If
    
    OpenDataBase = True
    Exit Function
EH:
    gCmn.ErrorMsgBox "Fail to connect to database."
    On Error Resume Next
    OpenDataBase = ChangeDatabase(hWnd, CN)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub CloseDataBase()
    Dim rs As ADODB.Recordset
    On Error Resume Next
    If Not CN Is Nothing Then
        With CN
            If .State <> adStateClosed Then
                .Close
            End If
        End With
    End If
    Set CN = Nothing
End Sub


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ChangeDatabase(ByVal hWnd As Long, Optional ByRef aConn As ADODB.Connection = Nothing) As Boolean
    On Error GoTo EH
    Dim dlgDB As New MSDASC.DataLinks
    Dim cnn As ADODB.Connection, sConnString$
    ChangeDatabase = False

    dlgDB.hWnd = hWnd
    If aConn Is Nothing Then
        Set cnn = New ADODB.Connection
        cnn.ConnectionString = g_Params.DbConnection
        If cnn.ConnectionString = "" Then cnn.Provider = "SQLOLEDB"
        cnn.ConnectionTimeout = 30
    Else
        Set cnn = aConn
    End If
    
    Do While True
        On Error Resume Next
        cnn.Properties("Persist Security Info") = True
        On Error GoTo EH
        
        If dlgDB.PromptEdit(cnn) Then
            On Error Resume Next
            sConnString = cnn.ConnectionString
            Err.Clear
            cnn.Open
            If Err.Number = 0 Then
                g_Params.DbConnection = sConnString
                ChangeDatabase = True
                Exit Do
            Else
                gCmn.ErrorMsgBox "Fail to change database settings."
            End If
            On Error GoTo EH
        Else
            Exit Do
        End If
    Loop
    
    Exit Function
EH:
    gCmn.ErrorMsgBox "Fail to change database settings."
    On Error Resume Next
    Set cnn = Nothing
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function OpenRSDynamic(ByVal Sql As String) As ADODB.Recordset
    Set OpenRSDynamic = New ADODB.Recordset
    With OpenRSDynamic
        .LockType = adLockOptimistic
        .CursorType = adOpenKeyset
        .Open OpenCMDynamic(Sql)
    End With
    On Error GoTo Failed
    If OpenRSDynamic.RecordCount <> 0 Then
    
    End If
    Exit Function
Failed:
    Err.Raise vbObjectError, "Open recordset", "Failed to open recordset: '" & Sql & "'"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function OpenCMDynamic(ByVal Sql As String) As ADODB.Command
    Set OpenCMDynamic = New ADODB.Command
    With OpenCMDynamic
        Set .ActiveConnection = CN
        .CommandText = Sql
        .CommandType = adCmdText
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ExecuteSql(ByVal Sql As String)
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = Sql
        .CommandType = adCmdText
        .Execute
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function DuplicateRS(rs As ADODB.Recordset) As ADODB.Recordset
Dim f As ADODB.Field
Dim i As Long
Dim n As Long
    On Error GoTo Herr
    n = rs.Fields.Count
    Set DuplicateRS = New ADODB.Recordset
    With DuplicateRS
        For i = 1 To n
            Set f = rs.Fields.Item(i - 1)
            .Fields.Append f.Name, f.type, f.DefinedSize, adFldIsNullable
        Next
        .Open
        If rs.RecordCount <> 0 Then
            rs.MoveFirst
            Do While Not rs.EOF
                .AddNew
                For i = 1 To n
                    Set f = rs.Fields.Item(i - 1)
                    .Fields(f.Name) = f.Value
                Next
                .Update
                rs.MoveNext
            Loop
        End If
    End With
    Exit Function
Herr:
    Debug.Print "DuplicateRS failed: " & Err.Description
    Debug.Assert False
    Set DuplicateRS = rs
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRS(rs As ADODB.Recordset, ByVal Criteria As String) As Boolean
    With rs
        If .RecordCount <> 0 Then
            .MoveFirst
            .Find Criteria, 0, adSearchForward, adBookmarkFirst
            If Not .EOF Then
                FindRS = True
            End If
        End If
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRSStockSymbol(rs As ADODB.Recordset, ByVal StockSymbol As String) As Boolean
    If Len(StockSymbol) > 0 Then
        FindRSStockSymbol = FindRS(rs, "StockSymbol='" & StockSymbol & "'")
    Else
        FindRSStockSymbol = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRSIndexSymbol(rs As ADODB.Recordset, ByVal IndexSymbol As String) As Boolean
    If Len(IndexSymbol) > 0 Then
        FindRSIndexSymbol = FindRS(rs, "IndexSymbol='" & IndexSymbol & "'")
    Else
        FindRSIndexSymbol = False
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRS2(rs As ADODB.Recordset, ByVal Criteria1 As String, ByVal Criteria2 As String) As Boolean
    If rs Is Nothing Then
        Debug.Assert False
    Else
        With rs
            .Filter = Criteria1
            If .RecordCount <> 0 Then
                .MoveFirst
                .Find Criteria2, 0, adSearchForward, adBookmarkFirst
                If Not .EOF Then
                    FindRS2 = True
                End If
            End If
        End With
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRS2IndexSymbols(rs As ADODB.Recordset, IndexSymbol1 As String, IndexSymbol2 As String) As Boolean
    If IndexSymbol1 > IndexSymbol2 Then
        Swap IndexSymbol1, IndexSymbol2
    End If
    FindRS2IndexSymbols = FindRS2(rs, _
        "IndexSymbol1='" & IndexSymbol1 & "'", _
        "IndexSymbol2='" & IndexSymbol2 & "'")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRS2StockSymbols(rs As ADODB.Recordset, StockSymbol1 As String, StockSymbol2 As String) As Boolean
    If StockSymbol1 > StockSymbol2 Then
        Swap StockSymbol1, StockSymbol2
    End If
    FindRS2StockSymbols = FindRS2(rs, _
        "StockSymbol1='" & StockSymbol1 & "'", _
        "StockSymbol2='" & StockSymbol2 & "'")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function FindRS2StockIndex(rs As ADODB.Recordset, StockSymbol As String, IndexSymbol As String) As Boolean
    FindRS2StockIndex = FindRS2(rs, _
        "IndexSymbol='" & IndexSymbol & "'", _
        "StockSymbol='" & StockSymbol & "'")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetDBVersion() As Boolean
Dim rs As ADODB.Recordset
Dim cm As ADODB.Command
    On Error GoTo Herr
    Dim Sql As String
    Sql = "EXEC usp_DataInfo_Get"
    Set cm = OpenCMDynamic(Sql)
    'Set rs = OpenRSDynamic(Sql)
    Set rs = cm.Execute
    
    If Not rs Is Nothing Then
        With rs
            .MoveFirst
            Do While Not .EOF
                If !vcKeyName = "version" Then
                    g_DBVersion = !vcKeyValue
                    GetDBVersion = True
                    Exit Function
                End If
                .MoveNext
            Loop
        End With
    End If
    Err.Raise vbObjectError, "GetDBVersion", "Version record was not found"
    Exit Function
Herr:
    ShowError "Error while getting database version info."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetStockReportData(ByVal StockSymbol As String) As ADODB.Recordset
Dim rs As ADODB.Recordset
Dim cm As ADODB.Command
Dim Sql As String
'    On Error GoTo Herr
    ValidateStockSymbol StockSymbol
    Sql = "EXEC GetStockReportData '" & StockSymbol & "'"
'    Set cm = OpenCMDynamic(Sql)
'    Set GetStockReportData = cm.Execute
    Set GetStockReportData = OpenRSDynamic(Sql)
'    StockPrice = GetStockPrice(StockSymbol)
'    StockHV = GetStockHV(StockSymbol)
'    StockIV = GetStockIV(StockSymbol)
'    GetStockIndexCorr2 StockSymbol, "SPX", Corr1, VCorr1
'    GetStockIndexCorr2 StockSymbol, "NDX", Corr2, VCorr2
'    GetStockIndexCorr2 StockSymbol, "OEX", Corr3, VCorr3
'    GetStockIndexCorr2 StockSymbol, "DJX", Corr4, VCorr4
    Exit Function
Herr:
    LastErrDescription = Err.Description
    ShowError "Call to DB.GetStockReportData failed."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'usp_StockCorrelationDH_Get
'    @vcSymbol1 varchar(8),
'    @vcSymbol2 varchar(8),
'    @fCorrelationPrice float = Null Output,
'    @fCorrelationVola  float = Null Output
'
Public Function usp_StockStockCorrelation_Get( _
    ByVal Symbol1 As String, _
    ByVal Symbol2 As String, _
    Corr As Double, _
    VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockCorrelationDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol1)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, Symbol2)
        .Parameters.Append .CreateParameter("2", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("3", adDouble, adParamOutput)
        .Execute
        Corr = DBRead(.Parameters.Item(2), 0)
        VCorr = DBRead(.Parameters.Item(3), 0)
    End With
    usp_StockStockCorrelation_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockStockCorrelation_Set( _
    ByVal Symbol1 As String, _
    ByVal Symbol2 As String, _
    ByVal Corr As Double, _
    ByVal VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockCorrelationDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol1)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, Symbol2)
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(Corr, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(VCorr, 0))
        .Execute
    End With
    usp_StockStockCorrelation_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexIndexCorrelation_Get( _
    ByVal Symbol1 As String, _
    ByVal Symbol2 As String, _
    Corr As Double, _
    VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexCorrelationDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol1)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, Symbol2)
        .Parameters.Append .CreateParameter("2", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("3", adDouble, adParamOutput)
        .Execute
        Corr = DBRead(.Parameters.Item(2), 0)
        VCorr = DBRead(.Parameters.Item(3), 0)
    End With
    usp_IndexIndexCorrelation_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexIndexCorrelation_Set( _
    ByVal Symbol1 As String, _
    ByVal Symbol2 As String, _
    ByVal Corr As Double, _
    ByVal VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexCorrelationDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol1)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, Symbol2)
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(Corr, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(VCorr, 0))
        .Execute
    End With
    usp_IndexIndexCorrelation_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockIndexCorrelation_Get( _
    ByVal StockSymbol As String, _
    ByVal IndexSymbol As String, _
    Corr As Double, _
    VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexStockCorrelationDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, IndexSymbol)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, StockSymbol)
        .Parameters.Append .CreateParameter("2", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("3", adDouble, adParamOutput)
        .Execute
        Corr = DBRead(.Parameters.Item(2), 0)
        VCorr = DBRead(.Parameters.Item(3), 0)
    End With
    usp_StockIndexCorrelation_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexDefinition_Save( _
    ByVal StockSymbol As String, _
    ByVal IndexSymbol As String, _
    ByVal Weight As Double, _
    ByVal Beta As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexDefinition_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adInteger, adParamInput, , Null)
        .Parameters.Append .CreateParameter("1", adInteger, adParamInput, , Null)
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(Weight, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(Beta, 0))
        .Parameters.Append .CreateParameter("4", adVarChar, adParamInput, 20, IndexSymbol)
        .Parameters.Append .CreateParameter("5", adVarChar, adParamInput, 20, StockSymbol)
        .Execute
    End With
    usp_IndexDefinition_Save = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockIndexCorrelation_Set( _
    ByVal StockSymbol As String, _
    ByVal IndexSymbol As String, _
    ByVal Corr As Double, _
    ByVal VCorr As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexStockCorrelationDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, IndexSymbol)
        .Parameters.Append .CreateParameter("1", adVarChar, adParamInput, 20, StockSymbol)
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(Corr, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(VCorr, 0))
        .Execute
    End With
    usp_StockIndexCorrelation_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexDataHistoryDH_Get( _
    ByVal Symbol As String, _
    IV As Double, _
    HV As Double, _
    VolOfVol As Double, _
    Vega As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexDataHistoryDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("2", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("3", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("4", adDouble, adParamOutput)
        .Execute
        IV = DBRead(.Parameters.Item(1), 0)
        HV = DBRead(.Parameters.Item(2), 0)
        VolOfVol = DBRead(.Parameters.Item(3), 0)
        Vega = DBRead(.Parameters.Item(4), 0)
    End With
    usp_IndexDataHistoryDH_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexDataHistoryDH_Set( _
    ByVal Symbol As String, _
    ByVal IV As Double, _
    ByVal HV As Double, _
    ByVal VolOfVol As Double, _
    ByVal Vega As Double _
) As Boolean
    
#If ETS Then

    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexDataHistory_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Refresh
        .Parameters.Item("@vcIndexTicker").Value = Symbol
        .Parameters.Item("@dtDate").Value = Date
        .Parameters.Item("@iTenorID").Value = 8
        .Parameters.Item("@fCurrentIV").Value = DBWrite(IV, 0)
        .Parameters.Item("@fCurrentHV").Value = DBWrite(HV, 0)
        .Parameters.Item("@fCurrentVolOfVol").Value = DBWrite(VolOfVol, 0)
        .Parameters.Item("@fCurrentVega").Value = DBWrite(Vega, 0)
        .Execute
    End With
    
#Else
    
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_IndexDataHistoryDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamInput, , DBWrite(IV, 0))
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(HV, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(VolOfVol, 0))
        .Parameters.Append .CreateParameter("4", adDouble, adParamInput, , DBWrite(Vega, 0))
        .Execute
    End With
    
#End If

    usp_IndexDataHistoryDH_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockDataHistoryDH_Get( _
    ByVal Symbol As String, _
    IV As Double, _
    HV As Double, _
    VolOfVol As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockDataHistoryDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("2", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("3", adDouble, adParamOutput)
        .Execute
        IV = DBRead(.Parameters.Item(1), 0)
        HV = DBRead(.Parameters.Item(2), 0)
        VolOfVol = DBRead(.Parameters.Item(3), 0)
    End With
    usp_StockDataHistoryDH_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockDataHistoryDH_Set( _
    ByVal Symbol As String, _
    ByVal IV As Double, _
    ByVal HV As Double, _
    ByVal VolOfVol As Double _
) As Boolean

#If ETS Then

    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockDataHistory_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Refresh
        .Parameters.Item("@vcStockTicker").Value = Symbol
        .Parameters.Item("@dtDate").Value = Date
        .Parameters.Item("@iTenorID").Value = 8
        .Parameters.Item("@fIVMean").Value = DBWrite(IV, 0)
        .Parameters.Item("@fCurrentHV").Value = DBWrite(HV, 0)
        .Parameters.Item("@fCurrentVolOfVol").Value = DBWrite(VolOfVol, 0)
        .Execute
    End With
    
#Else
    
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockDataHistoryDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamInput, , DBWrite(IV, 0))
        .Parameters.Append .CreateParameter("2", adDouble, adParamInput, , DBWrite(HV, 0))
        .Parameters.Append .CreateParameter("3", adDouble, adParamInput, , DBWrite(VolOfVol, 0))
        .Execute
    End With

#End If

    usp_StockDataHistoryDH_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_PriceCloseDH_Get( _
    ByVal Symbol As String, _
    PriceClose As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_PriceCloseDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamOutput)
        .Execute
        PriceClose = DBRead(.Parameters.Item(1), 0)
    End With
    usp_PriceCloseDH_Get = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_PriceCloseDH_Set( _
    ByVal Symbol As String, _
    ByVal PriceClose As Double _
) As Boolean
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_PriceCloseDH_Save"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 20, Symbol)
        .Parameters.Append .CreateParameter("1", adDouble, adParamInput, , DBWrite(PriceClose, 0))
        .Execute
    End With
    usp_PriceCloseDH_Set = True
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockReportDH_Get( _
    Optional ByVal SymbolList As String = "" _
) As ADODB.Recordset
    With New ADODB.Command
        Set .ActiveConnection = CN
        .CommandText = "usp_StockReportDH_Get"
        .CommandType = adCmdStoredProc
        .Parameters.Append .CreateParameter("0", adVarChar, adParamInput, 8000, DBWrite(SymbolList, ""))
        Set usp_StockReportDH_Get = .Execute
    End With
    On Error GoTo Failed
    Exit Function
Failed:
    Err.Raise vbObjectError, "Open recordset", "Failed to open recordset: 'usp_StockReportDH_Get'"

End Function

