VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDBWork"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
Attribute VB_Ext_KEY = "SavedWithClassBuilder6" ,"Yes"
Attribute VB_Ext_KEY = "Top_Level" ,"Yes"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private m_DB As ADODB.Connection
Attribute m_DB.VB_VarHelpID = -1
Private m_bInTran As Boolean


Private Sub Class_Initialize()
    On Error Resume Next
    m_bInTran = False
End Sub

Public Function GetDbPath() As String
    On Error Resume Next
    GetDbPath = "N/A"
    GetDbPath = m_DB.Properties("Data Source Name").Value & "\" & m_DB.Properties("Current Catalog").Value
End Function

' checking database connection
Public Function CheckDBConnection(ByVal hwnd As Long, ByVal bFirstRun As Boolean) As Boolean
    On Error GoTo EH
    Dim cnn As New ADODB.Connection

    CheckDBConnection = False
    
    cnn.ConnectionString = g_Params.DbConnection
    cnn.ConnectionTimeout = 30
    If cnn.ConnectionString <> "" Then
        cnn.Open
    Else
        cnn.Provider = "SQLOLEDB"
        Err.Raise vbObjectError + 1023, "Application Settings", "Invalid database settings."
    End If
    
    CheckDBConnection = True
    Exit Function
EH:
    If Not bFirstRun Then gCmn.ErrorMsgBox "Fail to connect to database."
'    On Error Resume Next
'    CheckDBConnection = ChangeDatabase(hWnd, cnn)
End Function

'Public Function ChangeDatabase(ByVal hWnd As Long, Optional ByRef aConn As ADODB.Connection = Nothing, _
'            Optional ByVal bSaveChanges As Boolean = True, Optional ByRef sConnectionString As String = "") As Boolean
'    On Error GoTo EH
'    Dim dlgDB As New MSDASC.DataLinks
'    Dim cnn As ADODB.Connection, sConnString$
'    ChangeDatabase = False
'
'    dlgDB.hWnd = hWnd
'    If aConn Is Nothing Then
'        Set cnn = New ADODB.Connection
'        If bSaveChanges Then
'            cnn.ConnectionString = g_Params.DbConnection
'        Else
'            cnn.ConnectionString = sConnectionString
'        End If
'        If cnn.ConnectionString = "" Then cnn.Provider = "SQLOLEDB"
'        cnn.ConnectionTimeout = 30
'    Else
'        Set cnn = aConn
'    End If
'
'    Do While True
'        On Error Resume Next
'        cnn.Properties("Persist Security Info") = True
'        On Error GoTo EH
'
'        If dlgDB.PromptEdit(cnn) Then
'            On Error Resume Next
'            sConnString = cnn.ConnectionString
'            Err.Clear
'            cnn.Open
'            If Err.Number = 0 Then
'                If bSaveChanges Then
'                    g_Params.SystemDbConnection = sConnString
'                    g_Params.DbConnection = sConnString
'                Else
'                    sConnectionString = sConnString
'                End If
'                Set m_DB = Nothing
'                ChangeDatabase = True
'                Exit Do
'            Else
'                gCmn.ErrorMsgBox "Fail to change database settings."
'            End If
'            On Error GoTo EH
'        Else
'            Exit Do
'        End If
'    Loop
'
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox "Fail to change database settings."
'    On Error Resume Next
'    Set cnn = Nothing
'End Function

Public Function Connection() As ADODB.Connection
    Dim bOpen As Boolean
            
    If Not m_DB Is Nothing Then
        On Error Resume Next
        bOpen = True
        bOpen = (m_DB.State = ADODB.adStateClosed)
    Else
        On Error GoTo EH
        Set m_DB = New ADODB.Connection
        bOpen = True
    End If
        
    If bOpen Then
        On Error GoTo EH
        Set m_DB = New ADODB.Connection
        m_DB.CursorLocation = ADODB.adUseClient
        m_DB.ConnectionString = g_Params.DbConnection
        m_DB.ConnectionTimeout = 60
        m_DB.CommandTimeout = 120
        m_DB.Open
    End If
    
    Set Connection = m_DB
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to connect to database."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' creating parameter for executing a command as a string
'
Private Function ParseParam(ByVal v As Variant) As String
    Select Case VarType(v)
    Case vbDate
        ParseParam = "'" & Format$(v, "yyyymmdd") & "'"
    Case vbBoolean
        ParseParam = IIf(v, "1", "0")
    Case vbString
        ParseParam = "'" & Replace(v, "'", "''") & "'"
    Case vbNull
        ParseParam = "Null"
    Case vbLong
        ParseParam = CStr(v)
    Case Else
        ParseParam = Val(v)
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' creating parameter for executing a command with manual parameters appending
'
Private Function CreateParam(nIndex As Long, varValue As Variant) As ADODB.Parameter
    Dim aParam As ADODB.Parameter
    
    Set aParam = New ADODB.Parameter
    With aParam
        .Name = "p" & nIndex
        Select Case VarType(varValue)
            Case vbNull, vbEmpty
                .Type = adInteger
                .Value = Null
                
            Case vbLong, vbInteger
                .Type = adInteger
                .Value = varValue
                
            Case vbDate
                .Type = adDBTimeStamp
                .Value = CDate(varValue)
                
            Case vbBoolean, vbByte
                .Type = adBoolean
                .Value = CBool(varValue)
                
            Case vbString
                .Type = adVarChar
                .Size = IIf(Len(varValue) <> 0, Len(varValue), 1)
                .Value = CStr(varValue)
                
            Case Else
                .Type = adDouble
                .Value = CDbl(varValue)
                
        End Select
    End With
    Set CreateParam = aParam
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' universal procedure to execute stored procedures
' Returns static recordset or return value
'
Public Function Execute(ByVal bReturnRST As Boolean, ByVal sCommandText As String, ParamArray arrParams()) As Variant
Dim fConnected As Boolean
Dim fConnectionBroken As Boolean
Dim cmd As New ADODB.Command
Dim rst As New ADODB.Recordset
Dim parRet As ADODB.Parameter
Dim i As Long
    fConnectionBroken = False
Try:
    On Error GoTo ErrHnd
    With cmd
        Set .ActiveConnection = Connection
        fConnected = True
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = sCommandText

        If Not bReturnRST Then
            Set parRet = .CreateParameter("return", adInteger, adParamReturnValue)
            .Parameters.Append parRet
        End If
        
'        Debug.Print sCommandText;
        For i = LBound(arrParams) To UBound(arrParams)
'            Debug.Print ", " & DBRead(arrParams(i), "null");
            .Parameters.Append CreateParam(i, arrParams(i))
        Next
'        Debug.Print
        
        .Prepared = True
        If bReturnRST Then
            rst.CursorType = adOpenStatic
            rst.CursorLocation = adUseClient
            rst.Open cmd, , adOpenStatic
            Set Execute = rst
            Set rst = Nothing
        Else
            .Execute
            Execute = parRet.Value
        End If

        If .ActiveConnection.Errors.Count > 0 Then
            Dim Msg As String
'            Msg = "SQL server error 0x" & _
                Hex(.ActiveConnection.Errors.Item(0).Number) & _
                ". " & .ActiveConnection.Errors.Item(0).Description
            Msg = .ActiveConnection.Errors.Item(0).Description
            Debug.Print "Execute failed: " & Msg
            Err.Raise _
                vbObjectError, _
                "DBWork.Execute", _
                Msg
        End If

    End With
    Exit Function

Failed:
    On Error Resume Next
    Debug.Print "Try to resume connection"
'    Debug.Assert False
    m_DB.Close
    GoTo Try
    Exit Function

ErrHnd:
    Dim s As String
    s = Err.Description
    Debug.Print Hex(Err.Number), s
    
    ' Try to catch "connection failure"
    If InStr(s, "Connect") > 0 Then
        If Not fConnectionBroken Then
            fConnectionBroken = True
            Resume Failed
        End If
    End If
    
    s = "exec " & sCommandText
    For i = LBound(arrParams) To UBound(arrParams)
        s = s & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(s, 1) = "," Then
        s = Left$(s, Len(s) - 1)
    End If
    gCmn.ErrorHandler s, Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Call this for usp_XXX_Save(ID,...)
' if ID is null then assume that we are creating new item and
'
Public Function ExecuteSaveItem(ByVal sCommandText As String, ItemID As Variant, ParamArray arrParams()) As Variant
    On Error GoTo ErrHnd

    Dim cmd As New ADODB.Command
    Dim rst As New ADODB.Recordset
    Dim i&
    Dim parRet As ADODB.Parameter
    Dim parID As ADODB.Parameter

    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = sCommandText

        Set parRet = .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append parRet
        Set parID = .CreateParameter("ItemID", adInteger, adParamInputOutput, , ItemID)
        .Parameters.Append parID
        
'        Debug.Print sCommandText;
        For i = LBound(arrParams) To UBound(arrParams)
'            Debug.Print ", " & DBRead(arrParams(i), "null");
            .Parameters.Append CreateParam(i, arrParams(i))
        Next
'        Debug.Print
        
        .Prepared = True
        .Execute
        ExecuteSaveItem = parRet.Value
        ItemID = parID.Value

        If .ActiveConnection.Errors.Count > 0 Then
            Dim Msg As String
'            Msg = "SQL server error 0x" & _
                Hex(.ActiveConnection.Errors.Item(0).Number) & _
                ". " & .ActiveConnection.Errors.Item(0).Description
            Msg = .ActiveConnection.Errors.Item(0).Description
            Err.Raise _
                vbObjectError, _
                "DBWork.Execute", _
                Msg
        End If

    End With
Exit Function
ErrHnd:
    Dim s As String
    Debug.Print Err.Description
    s = "exec " & sCommandText
    For i = LBound(arrParams) To UBound(arrParams)
        s = s & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(s, 1) = "," Then
        s = Left$(s, Len(s) - 1)
    End If
    gCmn.ErrorHandler s '"DBWork.Execute"
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ExecuteSQL(ByVal bReturnRST As Boolean, ByVal sCommandText As String) As Variant
Dim fConnected As Boolean
Dim fConnectionBroken As Boolean
Dim cmd As New ADODB.Command
Dim rst As New ADODB.Recordset
Dim parRet As ADODB.Parameter
Dim i As Long
    fConnectionBroken = False
Try:
    On Error GoTo ErrHnd
    With cmd
        Set .ActiveConnection = Connection
        fConnected = True
        .CommandTimeout = 120

        .CommandType = adCmdText
        .CommandText = sCommandText

        If Not bReturnRST Then
            Set parRet = .CreateParameter("return", adInteger, adParamReturnValue)
            .Parameters.Append parRet
        End If
'
'        Debug.Print sCommandText
'
        .Prepared = True
        If bReturnRST Then
            rst.CursorType = adOpenStatic
            rst.CursorLocation = adUseClient
            rst.Open cmd, , adOpenStatic
            Set ExecuteSQL = rst
            Set rst = Nothing
        Else
            .Execute
            ExecuteSQL = parRet.Value
        End If

        If .ActiveConnection.Errors.Count > 0 Then
            Dim Msg As String
'            Msg = "SQL server error 0x" & _
                Hex(.ActiveConnection.Errors.Item(0).Number) & _
                ". " & .ActiveConnection.Errors.Item(0).Description
            Msg = .ActiveConnection.Errors.Item(0).Description
            Debug.Print "Execute failed: " & Msg
            Err.Raise _
                vbObjectError, _
                "DBWork.Execute", _
                Msg
        End If

    End With
    Exit Function

Failed:
    On Error Resume Next
    Debug.Print "Try to resume connection"
'    Debug.Assert False
    m_DB.Close
    GoTo Try
    Exit Function

ErrHnd:
    Dim s As String
    s = Err.Description
    Debug.Print Hex(Err.Number), s
    
    ' Try to catch "connection failure"
    If InStr(s, "Connection") > 0 Then
        If Not fConnectionBroken Then
            fConnectionBroken = True
            Resume Failed
        End If
    End If
    
    s = "exec " & sCommandText
    gCmn.ErrorHandler s, Err.Description
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'Checking state of recordset
'
Public Function CheckRS(ByVal xRs As ADODB.Recordset) As Boolean
    On Error GoTo e
    CheckRS = True
    If xRs Is Nothing Then
        CheckRS = False
    ElseIf xRs.State = ADODB.adStateClosed Then
        CheckRS = False
    ElseIf xRs.BOF And xRs.EOF Then
        CheckRS = False
    End If
    Exit Function
e:
    gCmn.ErrorHandler "DBWork.CheckRS"
End Function

'/////////////////////////////////////
' Wrapping for stored procedures    /
'///////////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Wrapping for stored procedures
'
Public Function usp_BoContractsSymbol_Get(ByVal iContractTypeID As Variant) As ADODB.Recordset
    Set usp_BoContractsSymbol_Get = Execute(True, "usp_BoContractsSymbol_Get", iContractTypeID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoContractBySymbol_Get(ByVal vcSymbol As String) As ADODB.Recordset
    Set usp_BoContractBySymbol_Get = Execute(True, "usp_BoContractBySymbol_Get", vcSymbol)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoPositionForReconcile_Get(ByVal dtUTCTradeDate As Date, ByVal bUseStategy As Boolean) As ADODB.Recordset
    Set usp_BoPositionForReconcile_Get = Execute(True, "usp_BoPositionForReconcile_Get", dtUTCTradeDate, bUseStategy)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_TradeForReconcile_Get() As ADODB.Recordset
    Set usp_TradeForReconcile_Get = Execute(True, "usp_TradeForReconcile_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Contract_Save( _
    ByVal iContract_ID As Long, _
    ByVal vcSymbol As String, _
    ByVal vcContractName As String, _
    ByVal iLotSize As Long, _
    ByVal fPrice_Bid As Double, _
    ByVal fPrice_Ask As Double, _
    ByVal iExchange_ID As Variant, _
    ByVal fLastPrice As Double, _
    ByVal fOpenPrice As Double, _
    ByVal fClosePrice As Double, _
    ByVal fLowPrice As Double, _
    ByVal fHighPrice As Double, _
    ByVal iVolume As Long, _
    ByVal iOpenInterest As Long, _
    Optional ByVal iVolume10Day As Variant = Null, _
    Optional ByVal iVolume5Expiration As Variant = Null _
) As Boolean
    usp_Contract_Save = 0 = Execute(False, "usp_Contract_Save", _
        iContract_ID, _
        vcSymbol, _
        vcContractName, _
        iLotSize, _
        fPrice_Bid, _
        fPrice_Ask, _
        iExchange_ID, _
        fLastPrice, _
        fOpenPrice, _
        fClosePrice, _
        fLowPrice, _
        fHighPrice, _
        iVolume, _
        iOpenInterest, _
        iVolume10Day, _
        iVolume5Expiration)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Stocks
'
Public Function usp_BoStock_Del(ByVal iStockID As Long) As Long
    usp_BoStock_Del = Execute(False, "usp_BoStock_Del", iStockID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Stock_Get( _
    Optional ByVal iGroupID As Long = 0, _
    Optional ByVal iStockID As Long = 0 _
) As ADODB.Recordset
    Set usp_Stock_Get = Execute(True, "usp_Stock_Get" _
        , DBWrite(iGroupID) _
        , DBWrite(iStockID) _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_StockWithPrice_Get( _
    Optional iGroupID As Variant = Null, _
    Optional iStockID As Variant = Null, _
    Optional bIsBestPriceOnly As Byte = 0) As ADODB.Recordset
    Set usp_StockWithPrice_Get = Execute(True, "usp_StockWithPrice_Get", iGroupID, iStockID, bIsBestPriceOnly)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Stock_Import( _
    ByVal iContractID As Long, _
    ByVal bIsStock As Boolean, _
    ByVal vcContractName As String, _
    ByVal iDivFreq As Long, _
    ByVal fDivAmt As Double, _
    ByVal dtDivDate As Date, _
    ByVal sExchange As String) As Long
Dim v As Variant
    If dtDivDate > #1/1/1990# Then
        v = dtDivDate
    Else
        v = 0&
    End If
    usp_Stock_Import = Execute(False, "usp_Stock_Import", _
        iContractID, _
        IIf(bIsStock, 1, 0), _
        vcContractName, _
        iDivFreq, _
        fDivAmt, _
        v, _
        sExchange _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Stock_Save( _
    Optional ByVal iStockID As Variant = Null, _
    Optional iPrimaryExchangeID As Variant = Null, _
    Optional vcSymbol As Variant = Null, _
    Optional vcContractName As Variant = Null, _
    Optional bIsHTB As Variant = Null, _
    Optional iDivFreq As Variant = Null, _
    Optional dDivDate As Variant = Null, _
    Optional fDivAmt As Variant = Null, _
    Optional iDivFreq2 As Variant = Null, _
    Optional dDivDate2 As Variant = Null, _
    Optional fDivAmt2 As Variant = Null, _
    Optional bUseDiv2 As Variant = Null, _
    Optional tiCalcOptionType As Variant = Null, _
    Optional tiCalcModelType As Variant = Null, _
    Optional iExpCalendarID As Variant = Null, _
    Optional tiIsActive As Variant = Null, _
    Optional fSkew As Variant = Null, _
    Optional fKurt As Variant = Null, _
    Optional tiCheckExistance As Byte = 1 _
) As Long
    
Dim v As Variant
Dim v2 As Variant
Dim tiUseDiv As Long

    v = dDivDate
    v2 = dDivDate2
    If Not IsNull(bUseDiv2) Then tiUseDiv = bUseDiv2
        
    If Not IsNull(dDivDate) Then
        If dDivDate < #1/1/1990# Then
            v = 0&
        End If
    End If
    If Not IsNull(dDivDate2) Then
        If dDivDate2 < #1/1/1990# Then
            v2 = 0&
        End If
    End If
    usp_Stock_Save = Execute(False, "usp_Stock_Save", _
        iStockID, _
        iPrimaryExchangeID, _
        vcSymbol, _
        vcContractName, _
        bIsHTB, _
        iDivFreq, _
        fDivAmt, _
        v, _
        IIf(IsNull(bUseDiv2), bUseDiv2, tiUseDiv), _
        fDivAmt2, _
        iDivFreq2, _
        v2, _
        tiCalcOptionType, _
        tiCalcModelType, _
        iExpCalendarID, _
        tiIsActive, _
        fSkew, _
        fKurt, _
        tiCheckExistance)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Options
'
Public Function usp_Option_Del(ByVal iOptionID As Long) As Boolean
    usp_Option_Del = 0 = Execute(False, "usp_Option_Del", iOptionID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_OptionPair_Del( _
    ByVal iOptionIDCall As Variant, _
    ByVal iOptionIDPut As Variant _
) As Boolean
    usp_OptionPair_Del = 0 = Execute(False, "usp_OptionPair_Del", _
        iOptionIDCall, _
        iOptionIDPut _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Option_Get( _
    iUnderlyingID As Variant, _
    dtExpiry As Variant, _
    iOptionID As Variant, _
    Optional tiBaseInfoOnly As Byte = 0, _
    Optional tiWithPosition As Byte = 0, _
    Optional iTraderID As Variant = Null _
) As ADODB.Recordset
    Set usp_Option_Get = Execute(True, "usp_Option_Get", iUnderlyingID, dtExpiry, iOptionID, tiBaseInfoOnly, tiWithPosition, iTraderID)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoOptionsForReconcile_Get( _
    iUnderlyingID As Long _
) As ADODB.Recordset
    Set usp_BoOptionsForReconcile_Get = Execute(True, "usp_BoOptionsForReconcile_Get", iUnderlyingID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoOptionWithPrice_Get( _
    iUnderlyingID As Long _
) As ADODB.Recordset
    Set usp_BoOptionWithPrice_Get = Execute(True, "usp_BoOptionWithPrice_Get", iUnderlyingID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFutureOptionWithPrice_Get( _
    iFutureID As Long _
) As ADODB.Recordset
    Set usp_BoFutureOptionWithPrice_Get = Execute(True, "usp_BoFutureOptionWithPrice_Get", iFutureID)
End Function

Public Function usp_BoFutureOption_Get( _
    Optional iFutureID As Variant = Null _
) As ADODB.Recordset
    Set usp_BoFutureOption_Get = Execute(True, "usp_BoFutureOption_Get", iFutureID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Option_Save( _
    ByVal iOptionID As Variant, _
    ByVal vcSymbol As String, _
    ByVal iExchangeId As Variant, _
    ByVal iUnderlyingContract_ID As Variant, _
    ByVal iIsCall As Boolean, _
    ByVal fStrike As Double, _
    ByVal dtExpiry As Date, _
    ByVal bIsManualyEntered As Boolean _
) As Long
    usp_Option_Save = Execute(False, "usp_Option_Save" _
        , iOptionID _
        , vcSymbol _
        , iExchangeId _
        , iUnderlyingContract_ID _
        , IIf(iIsCall, 1, 0) _
        , Round(fStrike, 4) _
        , dtExpiry _
        , IIf(bIsManualyEntered, 1, 0) _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ExpiredOptions_Del( _
    ByVal iUnderlyingID As Variant _
) As ADODB.Recordset
    Set usp_ExpiredOptions_Del = Execute(True, "usp_ExpiredOptions_Del" _
        , iUnderlyingID _
        )
End Function

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''    1. usp_ContractPriceTheo_Init_Save
''    2. usp_ContractPriceTheo_Save
''        @iContractID int,
''        @iExchangeID int = Null,
''        @fPriceTheo float = Null
''    3.usp_ContractPriceTheo_Complete_Save
''
'Public Function usp_ContractPriceTheo_Save( _
'    ByVal iContractID As Variant, _
'    ByVal iExchangeID As Variant, _
'    ByVal fPriceTheo As Variant _
') As Boolean
'    usp_ContractPriceTheo_Save = 0 = Execute(False, "usp_ContractPriceTheo_Save", iContractID, DBWrite(iExchangeID, 0), fPriceTheo)
'End Function
'
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' usp_ContractPrice_Save
'    @iContractPriceID int = Null output,
'    @iContractID int = Null,
'    @iExchangeID int = Null,
'    @fPriceBid float = Null,
'    @fPriceAsk float = Null,
'    @iLotSize int = Null,
'    @fPriceLast float = Null,
'    @fPriceOpen float = Null,
'    @fPriceClose float = Null,
'    @fPriceLow float = Null,
'    @fPriceHigh float = Null,
'    @iVolume int = Null,
'    @iOpenInterest int = Null,
'    @vcSymbol varchar(8) = Null,
'    @iContractTypeID int = Null, --2-stock 3-option
'    @vcExchangeCode varchar(8) = Null
'
Public Function usp_ContractPrice_Save( _
    iContractPriceID As Variant, _
    ByVal iContractID As Variant, _
    ByVal iExchangeId As Variant, _
    ByVal fPriceBid As Variant, _
    ByVal fPriceAsk As Variant, _
    ByVal iLotSize As Variant, _
    ByVal fPriceLast As Variant, _
    ByVal fPriceOpen As Variant, _
    ByVal fPriceClose As Variant, _
    ByVal fPriceLow As Variant, _
    ByVal fPriceHigh As Variant, _
    ByVal iVolume As Variant, _
    ByVal iOpenInterest As Variant, _
    ByVal vcSymbol As Variant, _
    ByVal iContractTypeID As Variant, _
    ByVal vcExchangeCode As Variant _
) As Boolean
    usp_ContractPrice_Save = 0 = Execute(False, "usp_ContractPrice_Save", _
        iContractPriceID, _
        iContractID, _
        iExchangeId, _
        fPriceBid, _
        fPriceAsk, _
        iLotSize, _
        fPriceLast, _
        fPriceOpen, _
        fPriceClose, _
        fPriceLow, _
        fPriceHigh, _
        iVolume, _
        iOpenInterest, _
        vcSymbol, _
        iContractTypeID, _
        vcExchangeCode _
        )
End Function

Public Function usp_BoFuture_Del(ByVal iFutureID As Long, ByVal tiIsRaiseError As Byte) As Long
    usp_BoFuture_Del = Execute(False, "usp_BoFuture_Del", iFutureID, tiIsRaiseError)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Indexes
'
Public Function usp_BoIndex_Del( _
    ByVal iIndexID As Long _
) As Long
    usp_BoIndex_Del = Execute(False, "usp_BoIndex_Del" _
        , iIndexID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Index_Get( _
    Optional ByVal iGroupID As Long = 0, _
    Optional ByVal iIndexID As Long = 0 _
) As ADODB.Recordset
    Set usp_Index_Get = Execute(True, "usp_Index_Get" _
        , DBWrite(iGroupID) _
        , DBWrite(iIndexID) _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Index_Save( _
    Optional iIndexID As Variant = Null, _
    Optional vcSymbol As Variant = Null, _
    Optional vcContractName As Variant = Null, _
    Optional fYield As Variant = Null, _
    Optional bIsHTB As Variant = Null, _
    Optional tiCalcOptionType As Variant = Null, _
    Optional tiCalcModelType As Variant = Null, _
    Optional iExpCalendarID As Variant = Null, _
    Optional IsBasket As Variant = Null, _
    Optional tiIsActive As Variant = Null, _
    Optional fSkew As Variant = Null, _
    Optional fKurt As Variant = Null, _
    Optional tiCheckExistance As Byte = 1 _
) As Long
    usp_Index_Save = Execute(False, "usp_Index_Save" _
        , iIndexID _
        , vcSymbol _
        , vcContractName _
        , fYield _
        , bIsHTB _
        , tiCalcOptionType _
        , tiCalcModelType _
        , iExpCalendarID _
        , IsBasket _
        , tiIsActive _
        , fSkew _
        , fKurt _
        , tiCheckExistance)
End Function


Public Function usp_BoFuture_Save(Optional iFutureID As Variant = Null, _
                                Optional iFutureRootID As Variant = Null, _
                                Optional vcFutureSymbol As Variant = Null, _
                                Optional vcFutureName As Variant = Null, _
                                Optional vcExportSymbol As Variant = Null, _
                                Optional dtMaturityDate As Variant = Null, _
                                Optional iExpCalendarID As Variant = Null, _
                                Optional tiCalcOptionType As Variant = Null, _
                                Optional tiIsActive As Variant = Null, _
                                Optional iUndPriceProfileID As Variant = Null, _
                                Optional iOptPriceProfileID As Variant = Null, _
                                Optional tiIsRaiseError As Byte = 1) As Long
    
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_BoFuture_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iFutureID", adInteger, adParamInputOutput, , iFutureID)
        .Parameters.Append .CreateParameter("@iFutureRootID", adInteger, adParamInput, , iFutureRootID)
        .Parameters.Append .CreateParameter("@vcFutureSymbol", adVarChar, adParamInput, 20, vcFutureSymbol)
        .Parameters.Append .CreateParameter("@vcFutureName", adVarChar, adParamInput, 255, vcFutureName)
        .Parameters.Append .CreateParameter("@vcExportSymbol", adVarChar, adParamInput, 20, vcExportSymbol)
        .Parameters.Append .CreateParameter("@dtMaturityDate", adDBDate, adParamInput, , dtMaturityDate)
        .Parameters.Append .CreateParameter("@iExpCalendarID", adInteger, adParamInput, , iExpCalendarID)
        .Parameters.Append .CreateParameter("@tiCalcOptionType", adTinyInt, adParamInput, , tiCalcOptionType)
        .Parameters.Append .CreateParameter("@tiIsActive", adTinyInt, adParamInput, , tiIsActive)
        .Parameters.Append .CreateParameter("@iUndPriceProfileID", adInteger, adParamInputOutput, , iUndPriceProfileID)
        .Parameters.Append .CreateParameter("@iOptPriceProfileID", adInteger, adParamInputOutput, , iOptPriceProfileID)
        .Parameters.Append .CreateParameter("@tiIsRaiseError", adTinyInt, adParamInput, , tiIsRaiseError)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        If Not IsMissing(iFutureID) Then iFutureID = gCmn.ReadLng(.Parameters("@iFutureID"))
        If Not IsMissing(iUndPriceProfileID) Then iUndPriceProfileID = gCmn.ReadLng(.Parameters("@iUndPriceProfileID"))
        If Not IsMissing(iOptPriceProfileID) Then iOptPriceProfileID = gCmn.ReadLng(.Parameters("@iOptPriceProfileID"))
        
        usp_BoFuture_Save = gCmn.ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to save futures."
End Function

Public Function usp_BoFutureOption_Save(Optional iFutureOptionID As Variant = Null, _
                                Optional iFutureID As Variant = Null, _
                                Optional vcFutureOptionSymbol As Variant = Null, _
                                Optional vcFutureOptionName As Variant = Null, _
                                Optional vcExportSymbol As Variant = Null, _
                                Optional tiIsCall As Variant = Null, _
                                Optional fStrike As Variant = Null, _
                                Optional dtExpiryDate As Variant = Null, _
                                Optional tiIsRaiseError As Byte = 1) As Long
    
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_BoFutureOption_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iFutureOptionID", adInteger, adParamInputOutput, , iFutureOptionID)
        .Parameters.Append .CreateParameter("@iFutureID", adInteger, adParamInput, , iFutureID)
        .Parameters.Append .CreateParameter("@vcFutureOptionSymbol", adVarChar, adParamInput, 20, vcFutureOptionSymbol)
        .Parameters.Append .CreateParameter("@vcFutureOptionName", adVarChar, adParamInput, 255, vcFutureOptionName)
        .Parameters.Append .CreateParameter("@vcExportSymbol", adVarChar, adParamInput, 20, vcExportSymbol)
        .Parameters.Append .CreateParameter("@tiIsCall", adTinyInt, adParamInput, , tiIsCall)
        .Parameters.Append .CreateParameter("@fStrike", adDouble, adParamInput, , fStrike)
        .Parameters.Append .CreateParameter("@dtExpiryDate", adDBDate, adParamInput, , dtExpiryDate)
        .Parameters.Append .CreateParameter("@tiIsRaiseError", adTinyInt, adParamInput, , tiIsRaiseError)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        If Not IsMissing(iFutureOptionID) Then iFutureOptionID = gCmn.ReadLng(.Parameters("@iFutureOptionID"))
        
        usp_BoFutureOption_Save = gCmn.ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to save futures option."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Contracts In Groups
'
Public Function usp_ContractInGroupByContract_Del(ByVal iContractID As Long, ByVal iGroupID As Variant) As Boolean
    usp_ContractInGroupByContract_Del = 0 = Execute(False, "usp_ContractInGroupByContract_Del", iContractID, iGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInGroupByContract_Get(ByVal iContractID As Long) As ADODB.Recordset
    Set usp_ContractInGroupByContract_Get = Execute(True, "usp_ContractInGroupByContract_Get", iContractID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInGroupByContract_Save(ByVal iContractID As Long, ByVal iGroupID As Long) As Long
    usp_ContractInGroupByContract_Save = Execute(False, "usp_ContractInGroupByContract_Save", iContractID, iGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInGroup_Import(strXML As String) As ADODB.Recordset
    Set usp_ContractInGroup_Import = Execute(True, "usp_ContractInGroup_Import", strXML)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInGroup_Get(ByVal iGroupID As Variant, ByVal iContractID As Variant) As ADODB.Recordset
    Set usp_ContractInGroup_Get = Execute(True, "usp_ContractInGroup_Get", iGroupID, iContractID)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Brokers
'
Public Function usp_Broker_Del(ByVal iBrokerID As Long, ByVal tiIsRaiseError As Byte) As Boolean
    usp_Broker_Del = 0 = Execute(False, "usp_Broker_Del", iBrokerID, tiIsRaiseError)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Broker_Get(ByVal iBrokerID As Variant) As ADODB.Recordset
    Set usp_Broker_Get = Execute(True, "usp_Broker_Get", iBrokerID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Broker_Save(ByVal iBrokerID As Variant, _
                                ByVal vcBrokerName As String, _
                                ByVal vcDescription As String, _
                                ByVal iClearing As Long, _
                                ByVal fUndComm As Double, _
                                ByVal fOptRegComm As Double, _
                                ByVal fOptSpreadComm As Double, _
                                ByVal fLowPremComm As Double _
                                ) As Long
    usp_Broker_Save = Execute(False, "usp_Broker_Save", _
                                iBrokerID, _
                                vcBrokerName, _
                                vcDescription, _
                                iClearing, _
                                fUndComm, _
                                fOptRegComm, _
                                fOptSpreadComm, _
                                fLowPremComm _
                                )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Exchanges
'
Public Function usp_Exchange_Del(ByVal iExchangeId As Long) As Boolean
    usp_Exchange_Del = 0 = Execute(False, "usp_Exchange_Del", iExchangeId)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Exchange_Get(ByVal iExchangeId As Variant) As ADODB.Recordset
    Set usp_Exchange_Get = Execute(True, "usp_Exchange_Get", iExchangeId)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Exchange_Save(ByVal iExchangeId As Variant, ByVal vcExchangeName As String, _
                                ByVal vcDescription As String, ByVal vcExchangeCode As Variant, _
                                ByVal tiIsUnderlying As Variant, ByVal tiIsOption As Variant, _
                                ByVal vcOccCode As Variant) As Long
Dim Ret As Long
    Ret = Execute(False, "usp_Exchange_Save", iExchangeId, vcExchangeName, vcDescription, vcExchangeCode, tiIsUnderlying, tiIsOption, vcOccCode)
    usp_Exchange_Save = Ret
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Exchange by stock
'
Public Function usp_ContractInExchange_Del(ByVal iExchangeId As Variant, ByVal iContractID As Variant) As Boolean
    usp_ContractInExchange_Del = 0 = Execute(False, "usp_ContractInExchange_Del", iExchangeId, iContractID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInExchange_Get(ByVal iContractID As Variant) As ADODB.Recordset
    Set usp_ContractInExchange_Get = Execute(True, "usp_ContractInExchange_Get", iContractID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractInExchange_Save(ByVal iExchangeId As Variant, ByVal iContractID As Variant, ByVal iType1 As Variant, ByVal iType2 As Variant) As Long
    usp_ContractInExchange_Save = Execute(False, "usp_ContractInExchange_Save", iExchangeId, iContractID, iType1, iType2)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Groups
'
Public Function usp_ContractGroup_Del(ByVal iGroup_ID As Long) As Boolean
    usp_ContractGroup_Del = 0 = Execute(False, "usp_ContractGroup_Del", iGroup_ID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractGroup_Get( _
    Optional ByVal iGroup_ID As Variant = Null _
    ) As ADODB.Recordset
    Set usp_ContractGroup_Get = Execute(True, "usp_ContractGroup_Get", _
        iGroup_ID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractGroup_Save(ByVal iGroup_ID As Variant, ByVal vcGroupName As String, ByVal vcDescription As String) As Long
    usp_ContractGroup_Save = Execute(False, "usp_ContractGroup_Save", iGroup_ID, vcGroupName, vcDescription)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Strategies
'
Public Function usp_BoStrategy_Del(ByVal iStrategyID As Long) As Long
    usp_BoStrategy_Del = Execute(False, "usp_BoStrategy_Del", iStrategyID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Strategy_Get( _
    Optional ByVal iStrategyID As Variant = Null _
) As ADODB.Recordset
    Set usp_Strategy_Get = Execute(True, "usp_Strategy_Get", _
        iStrategyID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoStrategy_Save(ByVal iStrategyID As Variant, ByVal vcStrategyName As String, ByVal vcDescription As Variant) As Long
    usp_BoStrategy_Save = Execute(False, "usp_BoStrategy_Save", iStrategyID, vcStrategyName, vcDescription)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Traders
'
Public Function usp_BoTrader_Del(ByVal iTraderID As Long) As Long
    usp_BoTrader_Del = Execute(False, "usp_BoTrader_Del", iTraderID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTrader_Get( _
    Optional ByVal iTraderID As Variant = Null _
) As ADODB.Recordset
    Set usp_BoTrader_Get = Execute(True, "usp_BoTrader_Get", _
        iTraderID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTrader_Save( _
             ByVal iTraderID As Variant, _
    Optional ByVal vcAcronym As Variant = Null, _
    Optional ByVal vcTraderName As Variant = Null, _
    Optional ByVal vcClearingTraderCode As Variant = Null, _
    Optional ByVal iTraderGroupID As Variant = Null) As Long
    usp_BoTrader_Save = Execute(False, "usp_BoTrader_Save", _
        iTraderID, _
        vcAcronym, _
        vcTraderName, _
        vcClearingTraderCode, _
        iTraderGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Trades
'
Public Function usp_BoTrade_StoreToHistoryAndClear(ByVal tiStoreToHistory As Boolean, _
                                ByVal dtLastUTCTradeDate As Date, _
                                ByRef iStoredTradesCount As Long) As Long

    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    iStoredTradesCount = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_BoTrade_StoreToHistoryAndClear"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@tiStoreToHistory", adTinyInt, adParamInput, , IIf(tiStoreToHistory, 1, 0))
        .Parameters.Append .CreateParameter("@dtLastUTCTradeDate", adDBDate, adParamInput, , dtLastUTCTradeDate)
        
        .Parameters.Append .CreateParameter("@iStoredTradesCount", adInteger, adParamOutput)

        '.Prepared = True
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number, .ActiveConnection.Errors.Item(0).Source, _
                .ActiveConnection.Errors.Item(0).Description
        End If

        iStoredTradesCount = gCmn.ReadLng(.Parameters("@iStoredTradesCount"))
        usp_BoTrade_StoreToHistoryAndClear = gCmn.ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to delete old trade & store they into the history"
End Function


Public Function usp_TradeSim_Clear(ByVal tiSimulated As Byte, ByVal tiManual As Byte) As Boolean
    usp_TradeSim_Clear = (0 = Execute(False, "usp_TradeSim_Clear", tiSimulated, tiManual))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRPoints_Import( _
    ByVal sXML As Variant _
) As Long
    usp_IRPoints_Import = Execute(False, "usp_IRPoints_Import", _
        sXML _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Trade_New( _
    ByVal iContractID As Variant, _
    ByVal iQuantity As Variant, _
    ByVal fPrice As Variant, _
    ByVal iIsBuy As Variant, _
    ByVal iTraderID As Variant, _
    ByVal iStrategyID As Variant, _
    ByVal iBrokerID As Variant, _
    ByVal fBrokerCommission As Variant, _
    ByVal iClearingBrokerID As Variant, _
    ByVal fClearingBrokerCommission As Variant, _
    ByVal fTradedIV As Variant, _
    ByVal fSpotReference As Variant, _
    ByVal tiIsPosition As Variant, _
    ByVal iNewTradeID As Variant, _
    ByVal dtNewTradeDate As Variant, _
    ByVal tiStatus As Variant, _
    ByVal iUnderlyingID As Variant, _
    ByVal iOptionRootID As Variant, _
    ByVal iFutureRootID As Variant, _
    ByVal iFutureID As Variant _
) As Long
    usp_Trade_New = Execute(False, "usp_Trade_New", _
        iContractID, _
        iQuantity, _
        fPrice, _
        iIsBuy, _
        iTraderID, _
        iStrategyID, _
        iBrokerID, _
        fBrokerCommission, _
        iClearingBrokerID, _
        fClearingBrokerCommission, _
        fTradedIV, _
        fSpotReference, _
        tiIsPosition, _
        iNewTradeID, _
        dtNewTradeDate, _
        tiStatus, _
        0, _
        iUnderlyingID, _
        iOptionRootID, _
        iFutureRootID, _
        iFutureID, _
        Null _
        )
End Function

Public Function usp_TradeFeed_1_New( _
    ByVal vcTraderAcronym As Variant, _
    ByVal vcExecID As Variant, _
    ByVal tiIsBuy As Variant, _
    ByVal iQuantity As Variant, _
    ByVal fPrice As Variant, _
    ByVal dtTradeDate As Variant, _
    ByVal vcContractSymbol As Variant, _
    ByVal iContractTypeID As Variant, _
    ByVal vcUnderlyingSymbol As Variant, _
    ByVal tiIsCall As Variant, _
    ByVal fStrike As Variant, _
    ByVal dtExpiry As Variant, _
    ByVal iTradeLotSize As Variant, _
    ByVal vcOptionRoot As Variant, _
    ByVal vcStrategyName As Variant _
) As ADODB.Recordset
    Set usp_TradeFeed_1_New = Execute(True, "usp_TradeFeed_1_New", _
            vcTraderAcronym, _
            vcExecID, _
            tiIsBuy, _
            iQuantity, _
            fPrice, _
            dtTradeDate, _
            vcContractSymbol, _
            iContractTypeID, _
            vcUnderlyingSymbol, _
            tiIsCall, _
            IIf(IsNull(fStrike), Null, Round(fStrike, 4)), _
            dtExpiry, _
            iTradeLotSize, _
            0, _
            Null, _
            vcOptionRoot, _
            0, _
            vcStrategyName)
End Function

Public Function usp_TradeByContractAndTrader_Del( _
    ByVal vcTraderAcronym As Variant, _
    ByVal vcUnderlyingSymbol As Variant, _
    ByVal dtExpiry As Variant, _
    ByVal fStrike As Variant, _
    ByVal tiIsCall As Variant, _
    ByVal dtTradeDate As Variant _
) As Long
    usp_TradeByContractAndTrader_Del = Execute(False, "usp_TradeByContractAndTrader_Del", _
            vcTraderAcronym, _
            vcUnderlyingSymbol, _
            dtExpiry, _
            IIf(IsNull(fStrike), Null, Round(fStrike, 4)), _
            tiIsCall, _
            dtTradeDate)
End Function

Public Function usp_TradeByExpiredOptions_Del(ByVal dtDate As Date) As Long
    usp_TradeByExpiredOptions_Del = Execute(False, "usp_TradeByExpiredOptions_Del", dtDate)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'  Holidays
'
Public Function usp_Holiday_Del(ByVal iHolidayID As Long) As Boolean
Dim Ret As Long
    usp_Holiday_Del = Execute(False, "usp_Holiday_Del", iHolidayID)
    usp_Holiday_Del = (Ret = 0)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Holiday_Get(ByVal iExchangeId As Variant, ByVal iHolidayID As Variant) As ADODB.Recordset
    Set usp_Holiday_Get = Execute(True, "usp_Holiday_Get", iExchangeId, iHolidayID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Holiday_Save(ByVal iHolidayID As Variant, ByVal dtHolidayDate As Date, ByVal vcHolidayName As String) As Long
Dim Ret As Long
    Ret = Execute(False, "usp_Holiday_Save", iHolidayID, dtHolidayDate, vcHolidayName)
    usp_Holiday_Save = Ret
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'  ExchangeHolidays
'
Public Function usp_ExchangeHoliday_Del(ByVal iExchangeId As Long, ByVal iHolidayID As Variant) As Boolean
Dim Ret As Long
    Ret = Execute(False, "usp_ExchangeHoliday_Del", iExchangeId, iHolidayID)
    usp_ExchangeHoliday_Del = (Ret = 0)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ExchangeHoliday_Save(ByVal iExchangeId As Long, ByVal iHolidayID As Long) As Boolean
Dim Ret As Long
    Ret = Execute(False, "usp_ExchangeHoliday_Save", iExchangeId, iHolidayID)
    usp_ExchangeHoliday_Save = (Ret = 0)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRPoint_Get(iCurveID As Long) As ADODB.Recordset
    Set usp_IRPoint_Get = Execute(True, "usp_IRPoint_Get", iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRPoint_Save(Optional iIRpointID As Variant = Null, _
                                    Optional iCurveID As Variant = Null, _
                                    Optional iPeriodTypeID As Variant = Null, _
                                    Optional iNum As Variant = Null, _
                                    Optional fShortRate As Variant = Null, _
                                    Optional fHTBRate As Variant = Null, _
                                    Optional fLongRate As Variant = Null, _
                                    Optional fNeutralRate As Variant = Null) As Long
                                    
    usp_IRPoint_Save = Execute(False, "usp_IRPoint_Save", _
                                  iIRpointID, _
                                  iCurveID, _
                                  iPeriodTypeID, _
                                  iNum, _
                                  fShortRate, _
                                  fHTBRate, _
                                  fLongRate, _
                                  fNeutralRate)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRPoint_Del(Optional iIRpointID As Variant = Null, _
                                Optional iCurveID As Variant = Null) As Long
    usp_IRPoint_Del = Execute(False, "usp_IRPoint_Del", iIRpointID, iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRCurve_Get(Optional iCurveID As Variant = Null) As ADODB.Recordset
    Set usp_IRCurve_Get = Execute(True, "usp_IRCurve_Get", iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRCurve_Save(Optional iCurveID As Variant = Null, _
                                    Optional vcCurveName As Variant = Null, _
                                    Optional iRuleTypeID As Variant = Null, _
                                    Optional iCurveTypeID As Variant = Null, _
                                    Optional cPositionThreshold As Variant = Null) As Long
                                    
    usp_IRCurve_Save = Execute(False, "usp_IRCurve_Save", iCurveID, _
                                  vcCurveName, _
                                  iRuleTypeID, _
                                  iCurveTypeID, _
                                  cPositionThreshold)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRCurve_Del(iCurveID As Long) As Long
    usp_IRCurve_Del = Execute(False, "usp_IRCurve_Del", iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Expire Periods procedures
'
'Public Function usp_Expiry_Del( _
'    ByVal iExpiryID As Long _
') As Boolean
'Dim Ret As Long
'    usp_Expiry_Del = 0 = Execute(False, "usp_Expiry_Del" _
'        , iExpiryID _
'        )
'End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function usp_Expiry_Get( _
'    ByVal iExpiryID As Variant, _
'    ByVal vcExpiryPeriod As Variant, _
'    Optional ByVal iExpCalendarID As Variant = Null _
') As ADODB.Recordset
'    Set usp_Expiry_Get = Execute(True, "usp_Expiry_Get" _
'        , iExpiryID _
'        , vcExpiryPeriod _
'        , iExpCalendarID _
'        )
'End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function usp_Expiry_Save( _
'    ByVal iExpiryID As Variant, _
'    ByVal vcExpiryPeriod As Variant, _
'    ByVal dExpiryDate As Variant, _
'    Optional ByVal dVegaWeight As Variant = Null, _
'    Optional ByVal iExpCalendarID As Variant = Null _
') As Long
'    usp_Expiry_Save = Execute(False, "usp_Expiry_Save" _
'        , iExpiryID _
'        , vcExpiryPeriod _
'        , dExpiryDate _
'        , dVegaWeight _
'        , iExpCalendarID _
'        )
'End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_DataInfo_Get(Optional ByVal lDataInfoID As Variant = Null, _
                                Optional ByVal sKeyName As Variant = Null) As ADODB.Recordset
    Set usp_DataInfo_Get = Execute(True, "usp_DataInfo_Get", lDataInfoID, sKeyName)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_WtdVegaMaturity_Get() As ADODB.Recordset
    Set usp_WtdVegaMaturity_Get = Execute(True, "usp_WtdVegaMaturity_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_WtdVegaMaturity_Save(ByVal Value As Long) As ADODB.Recordset
    Execute False, "usp_WtdVegaMaturity_Save", Value
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_CommissionLowPremiumLimit_Get() As ADODB.Recordset
    Set usp_CommissionLowPremiumLimit_Get = Execute(True, "usp_CommissionLowPremiumLimit_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_CommissionLowPremiumLimit_Save(ByVal Value As Double) As ADODB.Recordset
    Execute False, "usp_CommissionLowPremiumLimit_Save", Value
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractBySymbol_Save( _
    iOptionID As Long, _
    strNewSymbol As String _
) As Variant
    usp_ContractBySymbol_Save = Execute( _
        False, _
        "usp_ContractBySymbol_Save", _
        iOptionID, _
        strNewSymbol _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderGroup_Get(ByVal iTraderGroupID As Variant) As ADODB.Recordset
    Set usp_BoTraderGroup_Get = Execute(True, "usp_BoTraderGroup_Get", iTraderGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderGroup_Save(ByVal iTraderGroupID As Variant, ByVal vcGroupName As Variant, ByVal vcGroupDesc As Variant) As Variant
    usp_BoTraderGroup_Save = Execute(False, "usp_BoTraderGroup_Save", iTraderGroupID, vcGroupName, vcGroupDesc)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderGroup_Del(ByVal iTraderGroupID As Long) As Long
    usp_BoTraderGroup_Del = Execute(False, "usp_BoTraderGroup_Del", iTraderGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Traders/Group management
'
Public Function usp_BoTraderInGroup_Get(ByVal iTraderGroupID As Long) As ADODB.Recordset
    Set usp_BoTraderInGroup_Get = Execute(True, "usp_BoTraderInGroup_Get", iTraderGroupID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Stocks by Traders menagement
'
Public Function usp_BoTraderUnderlyings_Get(ByVal iTraderID As Long, Optional ByVal tiOnlySelected As Boolean = True) As ADODB.Recordset
    Set usp_BoTraderUnderlyings_Get = Execute(True, "usp_BoTraderUnderlyings_Get", iTraderID, IIf(tiOnlySelected, 1, 0))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderContract_Get(ByVal iTraderID As Variant, ByVal iContractID As Variant) As ADODB.Recordset
    Set usp_BoTraderContract_Get = Execute(True, "usp_BoTraderContract_Get", iTraderID, iContractID)
End Function

Public Function usp_BoTraderContractStrategy_Get(ByVal iTraderID As Variant, ByVal iUnderlyingID As Variant) As ADODB.Recordset
    Set usp_BoTraderContractStrategy_Get = Execute(True, "usp_BoTraderContractStrategy_Get", iTraderID, iUnderlyingID)
End Function

Public Function usp_BoStrategyTraderContract_Get(ByVal iStrategyID As Variant) As ADODB.Recordset
    Set usp_BoStrategyTraderContract_Get = Execute(True, "usp_BoStrategyTraderContract_Get", iStrategyID)
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderContract_Save(ByVal iTraderID As Variant, ByVal iContractID As Variant) As Long
    usp_BoTraderContract_Save = Execute(False, "usp_BoTraderContract_Save", iTraderID, iContractID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoTraderContract_Del(ByVal iTraderID As Variant, ByVal iContractID As Variant) As Long
    usp_BoTraderContract_Del = Execute(False, "usp_BoTraderContract_Del", iTraderID, iContractID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Book Rule Items menagement
'
Public Function usp_BookRuleItem_Get(ByVal iBookID As Variant) As ADODB.Recordset
    Set usp_BookRuleItem_Get = Execute(True, "usp_BookRuleItem_Get", iBookID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BookRuleItem_Save( _
    iBookRuleItemID As Variant, _
    ByVal vcItemName As Variant, _
    ByVal iRuleItemTypeID As Variant, _
    ByVal iBookID As Variant _
) As Variant
    usp_BookRuleItem_Save = Execute(False, "usp_BookRuleItem_Save", _
        iBookRuleItemID, _
        vcItemName, _
        iRuleItemTypeID, _
        iBookID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BookRuleItem_Del(ByVal iBookRuleItemID As Variant) As Boolean
    usp_BookRuleItem_Del = 0 = Execute(False, "usp_BookRuleItem_Del", iBookRuleItemID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Rule Item Types menagement
'
Public Function usp_RuleItemType_Get(ByVal iRuleItemTypeID As Variant) As ADODB.Recordset
    Set usp_RuleItemType_Get = Execute(True, "usp_RuleItemType_Get", iRuleItemTypeID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_RuleItemType_Save(ByVal iRuleItemTypeID As Variant, ByVal vcItemTypeName As Variant) As Variant
    usp_RuleItemType_Save = Execute(False, "usp_RuleItemType_Save", iRuleItemTypeID, vcItemTypeName)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_RuleItemType_Del(ByVal iRuleItemTypeID As Variant) As Boolean
    usp_RuleItemType_Del = 0 = Execute(False, "usp_RuleItemType_Del", iRuleItemTypeID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Book Rule Item Mask menagement
'
Public Function usp_BookRuleItemMask_Get(ByVal iBookRuleItemID As Variant) As ADODB.Recordset
    Set usp_BookRuleItemMask_Get = Execute(True, "usp_BookRuleItemMask_Get", iBookRuleItemID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BookRuleItemMask_Save(ByVal iBookRuleItemMaskID As Variant, ByVal iBookRuleItemID As Variant, ByVal vcMaskName As Variant) As Variant
    usp_BookRuleItemMask_Save = Execute(False, "usp_BookRuleItemMask_Save", iBookRuleItemMaskID, iBookRuleItemID, vcMaskName)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BookRuleItemMask_Del(ByVal iBookRuleItemMaskID As Variant) As Boolean
    usp_BookRuleItemMask_Del = 0 = Execute(False, "usp_BookRuleItemMask_Del", iBookRuleItemMaskID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Swaps priorities of two books
'
Public Function usp_BookSwap_Save(ByVal iBookID1 As Variant, ByVal iBookID2 As Variant) As Boolean
    usp_BookSwap_Save = 0 = Execute(False, "usp_BookSwap_Save", iBookID1, iBookID2)
End Function

                        '//////////////////////////////
                        ' Report Filters menagement '/
                        '////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Filter_Get(ByVal iFilterID As Variant) As ADODB.Recordset
    Set usp_Filter_Get = Execute(True, "usp_Filter_Get", iFilterID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Filter_Save( _
    iFilterID As Variant, _
    ByVal vcFilterName As Variant, _
    ByVal iTraderID As Variant, _
    ByVal iBookID As Variant, _
    ByVal iStrategyID As Variant, _
    ByVal iContractGroupID As Variant _
) As Variant
    usp_Filter_Save = ExecuteSaveItem("usp_Filter_Save", _
        iFilterID, _
        vcFilterName, _
        iTraderID, _
        iBookID, _
        iStrategyID, _
        iContractGroupID _
        )
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
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function OpenCMDynamic(ByVal Sql As String) As ADODB.Command
    Set OpenCMDynamic = New ADODB.Command
    With OpenCMDynamic
        Set .ActiveConnection = Connection
        .CommandText = Sql
        .CommandType = adCmdText
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractPriceEOD_Save( _
    ByVal vcSymbol As Variant, _
    ByVal fPriceClose As Variant, _
    ByVal iContractTypeID As Variant, _
    ByVal vcExchangeCode As Variant _
) As Boolean
    usp_ContractPriceEOD_Save = 0 = _
    Execute(False, "usp_ContractPriceEOD_Save", _
        vcSymbol, _
        fPriceClose, _
        iContractTypeID, _
        vcExchangeCode _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexUnderlyingBeta_Save( _
    ByVal IndexID As Variant, _
    ByVal UnderlyingID As Variant, _
    ByVal Beta As Double, _
    Optional ByVal IndexSymbol As Variant = Null, _
    Optional ByVal UnderlyingSymbol As Variant = Null _
) As Boolean
    usp_IndexUnderlyingBeta_Save = 0 = _
    Execute(False, "usp_IndexUnderlyingBeta_Save" _
        , IndexID _
        , UnderlyingID _
        , Beta _
        , IndexSymbol _
        , UnderlyingSymbol _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'Alter      Procedure dbo.usp_IndexDefinition_Save
'    iIndexID int = null, --the index identifier
'    iStockID int = null, --the stock identifier
'    fWeight float = null,  --the weight
'    fBeta float = null,
'    vcIndexSymbol varchar(8) = null,
'    vcStockSymbol varchar(8) = null
'
Public Function usp_IndexDefinition_Save( _
    ByVal iIndexID As Long, _
    ByVal iStockID As Long, _
    ByVal Weight As Double, _
    Optional ByVal Beta As Variant = Null _
) As Boolean
    usp_IndexDefinition_Save = 0 = _
    Execute(False, "usp_IndexDefinition_Save" _
        , iIndexID _
        , iStockID _
        , DBWrite(Weight, 0) _
        , Null _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexDefinition_Get( _
    Optional ByVal iIndexID As Long = 0, _
    Optional ByVal iStockID As Long = 0 _
) As ADODB.Recordset
    Set usp_IndexDefinition_Get = Execute(True _
    , "usp_IndexDefinition_Get" _
    , DBWrite(iIndexID) _
    , DBWrite(iStockID) _
    )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexDefinition_Del( _
    ByVal iIndexID As Variant, _
    ByVal iStockID As Variant _
) As Boolean
    usp_IndexDefinition_Del = 0 = _
    Execute(False, "usp_IndexDefinition_Del" _
        , iIndexID _
        , iStockID _
        )
End Function

Public Function usp_IndexUnderlyingBeta_Clear( _
    ByVal iIndexID As Variant, _
    ByVal iUnderlyingID As Variant _
) As Boolean
    usp_IndexUnderlyingBeta_Clear = 0 = _
    Execute(False, "usp_IndexUnderlyingBeta_Clear" _
        , iIndexID _
        , iUnderlyingID _
        )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IndexUnderlyingBeta_Get( _
    Optional ByVal iIndexID As Variant = Null, _
    Optional ByVal iUnderlyingID As Variant = Null _
) As ADODB.Recordset
    Set usp_IndexUnderlyingBeta_Get = Execute(True _
    , "usp_IndexUnderlyingBeta_Get" _
    , iIndexID _
    , iUnderlyingID _
    )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ExpiryCalendar_Get( _
    Optional ByVal iExpCaledarID As Long = 0 _
) As ADODB.Recordset
    Set usp_ExpiryCalendar_Get = Execute(True _
    , "usp_ExpiryCalendar_Get" _
    , DBWrite(iExpCaledarID) _
    )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Underlying_Get(Optional iContractID As Variant = Null, _
                                    Optional ByVal iTraderID As Long = 0, _
                                    Optional ByVal iGroupID As Variant = Null, _
                                    Optional ByVal tiWithPosition As Byte = 0) As ADODB.Recordset
    Set usp_Underlying_Get = Execute(True, "usp_Underlying_Get", iContractID, IIf(iTraderID <> 0, iTraderID, Null), iGroupID, tiWithPosition)
End Function

Public Function usp_TradeLog_Get() As ADODB.Recordset

    Set usp_TradeLog_Get = Execute(True, "usp_TradeLog_Get")
End Function


Public Function usp_EodMsUnderlyingCache_Get(Optional ByVal iUndID As Variant = Null) As ADODB.Recordset
    Set usp_EodMsUnderlyingCache_Get = Execute(True, "usp_EodMsUnderlyingCache_Get", iUndID)
End Function

Public Function usp_EodMsOptionCache_Get(ByVal iUndID As Long) As ADODB.Recordset
    Set usp_EodMsOptionCache_Get = Execute(True, "usp_EodMsOptionCache_Get", iUndID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_EodMsUnderlyingCache_Check( _
    ByVal UnderlyingContractID As Long _
) As Boolean
    usp_EodMsUnderlyingCache_Check = 0 = _
    Execute(False, "usp_EodMsUnderlyingCache_Check" _
        , UnderlyingContractID _
        )
End Function

Public Function usp_EodMsLoad_Init( _
    Optional ByVal UndId As Variant = Null _
) As Boolean
    usp_EodMsLoad_Init = 0 = _
    Execute(False, "usp_EodMsLoad_Init" _
        , UndId _
        )
End Function

Public Function usp_EodMsUnderlyingCache_Save( _
    ByVal UnderlyingID As Long, _
    Optional ByVal UpdateDate As Variant = Null _
) As Boolean
    usp_EodMsUnderlyingCache_Save = 0 = _
    Execute(False, "usp_EodMsUnderlyingCache_Save" _
        , UnderlyingID _
        , UpdateDate _
        )
End Function
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Option_Import(ByVal UnderlyingContractID As Long, _
    ByVal Symbol As String, ByVal IsCall As Boolean, ByVal Strike As Double, _
    ByVal Expiry As Date, ByVal ExpiryOV As Date, ByVal TradingClose As Date, _
    ByVal LotSize As Long, Optional ByVal iMsCacheOptID As Variant = Null _
) As Boolean
    usp_Option_Import = 0 = _
    Execute(False, "usp_Option_Import", UnderlyingContractID _
        , Symbol, IIf(IsCall, 1, 0), Strike _
        , Expiry, ExpiryOV, TradingClose _
        , LotSize, iMsCacheOptID)
End Function
Public Function usp_FindOption( _
    ByVal OptionRootID As Long, _
    ByVal IsCall As Boolean, _
    ByVal Strike As Double, _
    ByVal dtExpiry As Date _
) As Long
    usp_FindOption = _
    Execute(False, "usp_FindOption" _
        , OptionRootID _
        , Strike _
        , IIf(IsCall, 1, 0) _
        , dtExpiry _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Option_Bulk_Import(ByVal lUndId As Long, ByVal ImportText As String) As Boolean
    usp_Option_Bulk_Import = 0 = _
    Execute(False, "usp_Option_Bulk_Import", lUndId, ImportText)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_EodMsOptionCache_Save(ByVal UnderlyingContractID As Long, ByVal Symbol As String, _
    ByVal Expiry As Date, ByVal ExpiryOV As Date, ByVal TradingClose As Date, _
    ByVal Strike As Double, ByVal IsCall As Boolean, ByVal LotSize As Long _
) As Boolean
    usp_EodMsOptionCache_Save = 0 = _
    Execute(False, "usp_EodMsOptionCache_Save", UnderlyingContractID, Symbol _
        , Expiry, ExpiryOV, TradingClose _
        , Strike, IsCall, LotSize)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ExpiryCalendar_Del( _
    ByVal iExpCaledarID As Long _
) As Boolean
    usp_ExpiryCalendar_Del = 0 = _
    Execute(False, "usp_ExpiryCalendar_Del" _
        , iExpCaledarID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
'Public Function usp_ExpiryCalendar_Save( _
'    ByVal iExpCaledarID As Variant, _
'    ByVal vcExpCalendarName As String, _
'    Optional ByVal vcDescription As String _
') As Long
'Dim IsNew As Boolean
'Dim iBaseCalendarID As Long
'Dim ExpDate As Date
'Dim VegaWeight As Double
'    IsNew = IsNull(iExpCaledarID)
'    usp_ExpiryCalendar_Save = _
'    Execute(False, "usp_ExpiryCalendar_Save" _
'        , iExpCaledarID _
'        , DBWrite(vcExpCalendarName, "") _
'        , DBWrite(vcDescription, "") _
'        )
'    If IsNew Then
'        ' fill calendar expirations table with default values
'        iExpCaledarID = usp_ExpiryCalendar_Save
'        If iExpCaledarID > 3 Then
'            iBaseCalendarID = 1
'            With usp_Expiry_Get(Null, Null, iBaseCalendarID)
'                If .RecordCount > 0 Then
'                    .MoveFirst
'                    Do While Not .EOF
'                        ExpDate = CalendarExpiry(!dtExpiryDate, iBaseCalendarID)
'                        VegaWeight = DefaultVegaWeight(ExpDate)
'                        usp_Expiry_Save !iExpiryID, Null, ExpDate, VegaWeight, iExpCaledarID
'                        .MoveNext
'                    Loop
'                End If
'            End With
'        End If
'    End If
'End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_UnderlyingActive_Save( _
    ByVal iContractID As Long, _
    ByVal tiIsActiveMode As Integer _
) As Boolean

    usp_UnderlyingActive_Save = 0 = _
    Execute(False, "usp_UnderlyingActive_Save" _
        , DBWrite(iContractID, 0) _
        , tiIsActiveMode _
        )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_CustomVolatilityPointIsExist_Get( _
    Optional Symbol As Variant = Null _
) As ADODB.Recordset

     Set usp_CustomVolatilityPointIsExist_Get = _
    Execute(True, "usp_CustomVolatilityPointIsExist_Get" _
        , Symbol _
        )
End Function


Public Function usp_CustomStrikeSkewDefaultPointFromIV_Save( _
             ByVal iContractID As Long, _
             ByVal fStrike As Double, _
             ByVal dtExpDate As Date, _
             ByVal iType As Long, _
             ByVal fVolatility As Double _
) As Long
    usp_CustomStrikeSkewDefaultPointFromIV_Save = Execute(False, "usp_CustomStrikeSkewDefaultPointFromIV_Save" _
        , DBWrite(iContractID, 0) _
        , DBWrite(Round(fStrike, 4), 0) _
        , DBWrite(dtExpDate, 0) _
        , DBWrite(iType, 0) _
        , DBWrite(fVolatility, 0) _
        )
End Function


Public Function usp_OptionRoot_Get( _
        iRootID As Variant _
        , iUnderlyingID As Variant _
) As ADODB.Recordset
    Set usp_OptionRoot_Get = Execute(True, "usp_OptionRoot_Get" _
        , iRootID _
        , iUnderlyingID _
        )
End Function

Public Function usp_OptionRoot_Save( _
        iRootID As Long _
        , vcSymbol As Variant _
        , iLotSize As Variant _
        , tiIsFitEnabled As Variant _
        , Optional tiIsSynthetic As Variant = Null _
        , Optional fCashValue As Variant = Null _
        , Optional fSyntheticSkew As Variant = Null _
        , Optional fSyntheticKurt As Variant = Null _
        , Optional fSyntheticYield As Variant = Null _
        , Optional tiSyntheticIsBasket As Variant = Null _
) As Long
    usp_OptionRoot_Save = Execute(False, "usp_OptionRoot_Save" _
        , iRootID _
        , vcSymbol _
        , iLotSize _
        , tiIsFitEnabled _
        , tiIsSynthetic _
        , fCashValue _
        , fSyntheticSkew _
        , fSyntheticKurt _
        , fSyntheticYield _
        , tiSyntheticIsBasket _
        )
End Function

Public Function usp_OptionRootMove_Save( _
        iOldRootID As Long _
        , iNewRootID As Long _
) As ADODB.Recordset
    Set usp_OptionRootMove_Save = Execute(True, "usp_OptionRootMove_Save" _
        , iOldRootID _
        , iNewRootID _
        )
End Function

Public Function usp_SyntheticParams_Get( _
        iRootID As Long, _
        Optional bIsWithPrice As Byte = 0, _
        Optional bIsWithHV As Byte = 0 _
) As ADODB.Recordset
    Set usp_SyntheticParams_Get = Execute(True, "usp_SyntheticParams_Get" _
        , iRootID _
        , bIsWithPrice _
        , bIsWithHV _
        )
End Function


Public Function usp_SyntheticParam_Save( _
        iRootID As Long _
        , iUnderlyingID As Long _
        , fWeight As Double _
        , fSettlement As Double _
) As Long
    usp_SyntheticParam_Save = Execute(False, "usp_SyntheticParam_Save" _
        , iRootID _
        , iUnderlyingID _
        , fWeight _
        , fSettlement _
        )
End Function

Public Function usp_SyntheticParams_Del( _
     ByVal iRootID As Long _
    , ByVal iUnderlyingID As Variant _
) As Boolean
    usp_SyntheticParams_Del = 0 = _
    Execute(False, "usp_SyntheticParams_Del" _
        , iRootID _
        , iUnderlyingID _
        )
End Function


Public Function usp_ContractSymbolAndIsBasket_Get( _
     ByVal iContractID As Variant _
) As ADODB.Recordset
    Set usp_ContractSymbolAndIsBasket_Get = Execute(True, "usp_ContractSymbolAndIsBasket_Get" _
    , iContractID _
    )
End Function


Public Function usp_OptionRootWithStrikes_Get( _
        iRootID As Variant _
        , iUnderlyingID As Variant _
) As ADODB.Recordset
    Set usp_OptionRootWithStrikes_Get = Execute(True, "usp_OptionRootWithStrikes_Get" _
        , iRootID _
        , iUnderlyingID _
        )
End Function

Public Function usp_CorporateAction_Get( _
        iCorpActionID As Variant _
        , iTypeID As Variant _
        , dtEffectiveDate As Variant _
        , iUnderlyingID As Variant _
        , iStatusID As Variant _
) As ADODB.Recordset
    Set usp_CorporateAction_Get = Execute(True, "usp_CorporateAction_Get" _
        , iCorpActionID _
        , iTypeID _
        , dtEffectiveDate _
        , iUnderlyingID _
        , iStatusID _
        )
End Function

Public Function usp_LogCA_Rollback( _
      ByVal iCorporateActionID As Long _
) As Boolean
    usp_LogCA_Rollback = 0 = _
    Execute(False, "usp_LogCA_Rollback" _
        , iCorporateActionID _
        )
End Function

Public Function usp_OptionRootCA_Save( _
     ByVal iCorporateActionID As Long _
     , ByVal iOptionRootID As Long _
     , ByVal vcNewSymbol As Variant _
     , ByVal iNewUnderlyingID As Long _
     , ByVal iMultiplier As Long _
     , ByVal tiIsSynthetic As Boolean _
     , ByVal fCashValue As Double _
) As ADODB.Recordset
    Set usp_OptionRootCA_Save = _
    Execute(True, "usp_OptionRootCA_Save" _
        , iCorporateActionID _
        , iOptionRootID _
        , vcNewSymbol _
        , iNewUnderlyingID _
        , iMultiplier _
        , IIf(tiIsSynthetic, 1, 0) _
        , fCashValue _
        )
End Function

Public Function usp_CorporateAction_Save( _
      ByVal iTypeID As Variant _
     , ByVal dtEffectiveDate As Variant _
     , ByVal iUnderlyingID As Variant _
     , ByVal iStatus As Variant _
     , ByVal iCorporateActionID As Variant _
) As Long
    usp_CorporateAction_Save = _
    Execute(False, "usp_CorporateAction_Save" _
        , iTypeID _
        , dtEffectiveDate _
        , iUnderlyingID _
        , iStatus _
        , iCorporateActionID _
       )
End Function


Public Function usp_SyntheticParamCA_Save( _
     ByVal iCorporateActionID As Long _
     , ByVal iOptionRootID As Long _
     , ByVal iUnderlyingID As Long _
     , ByVal fWeight As Double _
) As Boolean
    usp_SyntheticParamCA_Save = 0 = _
    Execute(False, "usp_SyntheticParamCA_Save" _
        , iCorporateActionID _
        , iOptionRootID _
        , iUnderlyingID _
        , fWeight _
        )
End Function

Public Function usp_OptionStrikeCA_Save( _
     ByVal iCorporateActionID As Long _
     , ByVal iOptionRootID As Long _
     , ByVal fOldStrike As Double _
     , ByVal vcStrikeTicker As String _
     , ByVal fStrike As Double _
) As ADODB.Recordset
    Set usp_OptionStrikeCA_Save = _
    Execute(True, "usp_OptionStrikeCA_Save" _
        , iCorporateActionID _
        , iOptionRootID _
        , Round(fOldStrike, 4) _
        , vcStrikeTicker _
        , Round(fStrike, 4) _
        )
End Function

Public Function usp_ContractPriceCA_Save( _
     ByVal iCorporateActionID As Long _
     , ByVal iUnderlyingID As Variant _
     , ByVal iOptionRootID As Variant _
     , ByVal fPriceMultiplier As Double _
     , ByVal tiIsUnderlying As Boolean _
) As Boolean
    usp_ContractPriceCA_Save = 0 = _
    Execute(False, "usp_ContractPriceCA_Save" _
        , iCorporateActionID _
        , iUnderlyingID _
        , iOptionRootID _
        , fPriceMultiplier _
        , IIf(tiIsUnderlying, 1, 0) _
        )
End Function

Public Function usp_TradeCA_Get( _
    ByVal iTraderID As Variant, _
    ByVal iContractID As Variant, _
    ByVal iUnderlyingID As Variant, _
    ByVal iOptionRootID As Variant _
) As ADODB.Recordset
    Set usp_TradeCA_Get = Execute(True, "usp_TradeCA_Get", _
        iTraderID, _
        iContractID, _
        iUnderlyingID, _
        iOptionRootID _
        )
End Function


Public Function usp_TradeCA_Del( _
     ByVal iCorporateActionID As Long _
     , ByVal iTradeID As Long _
) As Boolean
    usp_TradeCA_Del = 0 = _
    Execute(False, "usp_TradeCA_Del" _
        , iCorporateActionID _
        , iTradeID _
        )
End Function
            
Public Function usp_TradeCA_Save( _
    ByVal iCorpActionID As Long, _
    ByVal iTradeID As Variant, _
    ByVal iQuantity As Long, _
    ByVal fPrice As Double, _
    Optional ByVal iContractID As Variant = Null, _
    Optional ByVal tiIsClose As Variant = Null, _
    Optional ByVal tiIsBuy As Variant = Null, _
    Optional ByVal iTraderID As Variant = Null, _
    Optional ByVal tiIsPosition As Variant = Null, _
    Optional ByVal tiIsSimulated As Variant = Null _
) As Boolean
    usp_TradeCA_Save = 0 = Execute(False, "usp_TradeCA_Save", _
            iCorpActionID, _
            iTradeID, _
            iQuantity, _
            fPrice, _
            iContractID, _
            tiIsClose, _
            tiIsBuy, _
            iTraderID, _
            tiIsPosition, _
            tiIsSimulated _
        )
End Function

Public Function usp_ContractType_Change( _
     ByVal iContractID As Long _
) As Boolean
    usp_ContractType_Change = 0 = _
    Execute(False, "usp_ContractType_Change" _
        , iContractID _
        )
End Function


Public Function usp_SyntheticIndexBeta_Get( _
        ByVal iOptRootID As Long _
) As ADODB.Recordset
    Set usp_SyntheticIndexBeta_Get = Execute(True, "usp_SyntheticIndexBeta_Get" _
        , iOptRootID _
        )
End Function


Public Function usp_SyntheticIndexBeta_Save( _
        ByVal iOptRootID As Long _
        , ByVal iIndexID As Long _
        , ByVal fBeta As Double _
) As Long
    usp_SyntheticIndexBeta_Save = Execute(False, "usp_SyntheticIndexBeta_Save" _
        , iOptRootID _
        , iIndexID _
        , fBeta _
        )
End Function

Public Function usp_ExpiryVegaWeightSave_Save( _
        ByVal dtExpiry As Date _
        , ByVal dValue As Double _
) As Long
        usp_ExpiryVegaWeightSave_Save = Execute(False, "usp_ExpiryVegaWeightSave_Save", dtExpiry, dValue)
End Function

Public Function usp_SyntheticIndexBeta_Del( _
     ByVal iOptRootID As Long _
    , ByVal iIndexID As Variant _
) As Boolean
    usp_SyntheticIndexBeta_Del = 0 = _
    Execute(False, "usp_SyntheticIndexBeta_Del" _
        , iOptRootID _
        , iIndexID _
        )
End Function

'Public Function usp_ExpiryInCalendar_Get( _
'        ByVal iExpiryID As Long _
'        , ByVal iExpiryCalendarID As Long _
') As ADODB.Recordset
'    Set usp_ExpiryInCalendar_Get = Execute(True, "usp_ExpiryInCalendar_Get" _
'        , iExpiryID _
'        , iExpiryCalendarID _
'        )
'End Function

Public Function usp_Trade_ClearDeleted() As Boolean
    usp_Trade_ClearDeleted = 0 = Execute(False, "usp_Trade_ClearDeleted")
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_OrderExecDest_Del(ByVal iDestID As Long, ByVal tiIsRaiseError As Byte) As Boolean
    usp_OrderExecDest_Del = 0 = Execute(False, "usp_OrderExecDest_Del", iDestID, tiIsRaiseError)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_OrderExecDest_Get( _
    Optional ByVal iDestID As Variant = Null _
) As ADODB.Recordset
    Set usp_OrderExecDest_Get = Execute(True, "usp_OrderExecDest_Get", _
        iDestID _
    )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_OrderExecDest_Save( _
    ByVal iDestID As Variant, _
    ByVal vcCode As String, _
    ByVal vcName As String, _
    ByVal tiIsOption As Boolean, _
    ByVal tiIsStock As Boolean _
) As Long
    usp_OrderExecDest_Save = Execute(False, "usp_OrderExecDest_Save", iDestID, vcCode, vcName, IIf(tiIsOption, 1, 0), IIf(tiIsStock, 1, 0))
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_FixOldOrder_Del() As Boolean
    usp_FixOldOrder_Del = 0 = Execute(False, "usp_FixOldOrder_Del")
End Function

Public Function usp_TntCardOld_Del() As Boolean
    usp_TntCardOld_Del = 0 = Execute(False, "usp_TntCardOld_Del", Null)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_TntCounterParty_Get( _
    Optional ByVal iCptID As Variant = Null _
) As ADODB.Recordset
    Set usp_TntCounterParty_Get = Execute(True, "usp_TntCounterParty_Get", _
        iCptID _
    )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_TntCounterParty_Save( _
    ByVal iCptID As Variant, _
    ByVal vcName As String, _
    ByVal vcDesc As String, _
    ByVal tiRaiseError As Boolean _
) As Long
    usp_TntCounterParty_Save = Execute(False, "usp_TntCounterParty_Save", iCptID, vcName, vcDesc, IIf(tiRaiseError, 1, 0))
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_TntCounterParty_Del(ByVal iCptID As Long, ByVal tiIsRaiseError As Byte) As Long
    usp_TntCounterParty_Del = Execute(False, "usp_TntCounterParty_Del", iCptID, tiIsRaiseError)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoUnderlyings_Get() As ADODB.Recordset
    Set usp_BoUnderlyings_Get = Execute(True, "usp_BoUnderlyings_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoContractExportSymbol_Save( _
    ByVal iContractID As Long, _
    ByVal vcExportSymbol As Variant _
) As Long
    usp_BoContractExportSymbol_Save = Execute(False, "usp_BoContractExportSymbol_Save", iContractID, vcExportSymbol)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IsNewStrategyForUnderlying_Get( _
) As Long
    usp_IsNewStrategyForUnderlying_Get = Execute(False, "usp_IsNewStrategyForUnderlying_Get")
End Function

Public Function usp_BoTradeNew_Get( _
   ByVal dtLastTradeDateUTC As Variant _
) As ADODB.Recordset
    Set usp_BoTradeNew_Get = Execute(True, "usp_BoTradeNew_Get", dtLastTradeDateUTC)
End Function


Public Function usp_BoTradeNewContract_Get( _
   ByVal dtLastTradeDateUTC As Variant _
) As ADODB.Recordset
    Set usp_BoTradeNewContract_Get = Execute(True, "usp_BoTradeNewContract_Get", dtLastTradeDateUTC)
End Function

Public Function usp_BoOptionByParams_Get( _
   ByVal iUnderlyingID As Long, _
   ByVal dtExpiry As Date, _
   ByVal fStrike As Double, _
   ByVal tiIsCall As Boolean _
) As ADODB.Recordset
    Set usp_BoOptionByParams_Get = Execute(True, "usp_BoOptionByParams_Get", _
        iUnderlyingID, _
        dtExpiry, _
        fStrike, _
        IIf(tiIsCall, 1, 0) _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFuture_Get( _
    Optional ByVal iFutureID As Variant = Null _
) As ADODB.Recordset
    Set usp_BoFuture_Get = Execute(True, "usp_BoFuture_Get" _
        , iFutureID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFutureRoot_Get( _
    Optional ByVal nIndexID As Long _
) As ADODB.Recordset
    Set usp_BoFutureRoot_Get = Execute(True, "usp_BoFutureRoot_Get" _
        , nIndexID _
        )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFutureRoot_Del(ByVal iFutureRootID As Long) As Long
    usp_BoFutureRoot_Del = Execute(False, "usp_BoFutureRoot_Del", iFutureRootID)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_EodOldFutures_Del( _
    ByVal iFutureRootID As Long _
) As ADODB.Recordset
    Set usp_EodOldFutures_Del = Execute(True, "usp_EodOldFutures_Del" _
        , iFutureRootID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_EodOldFutureOptions_Del( _
    ByVal iFutureID As Long _
) As ADODB.Recordset
    Set usp_EodOldFutureOptions_Del = Execute(True, "usp_EodOldFutureOptions_Del" _
        , iFutureID _
        )
End Function


Public Function usp_BoFutureRoot_Save(ByVal iFutureRootID As Variant, _
        ByVal iUnderlyingID As Variant, _
        ByVal vcFutureRootSymbol As String, _
        ByVal vcFutureRootName As String, _
        ByVal iFutureLotSize As Long, _
        ByVal iOptionLotSize As Long, _
        ByVal vcExportSymbol$, _
        ByVal tiQuoteFormat As Long, _
        ByVal iQuotationUnitAmmount As Long, _
        ByVal iQuotationUnitID As Long, _
        ByVal dQuoteDenomination As Double, _
        ByVal dTickSize As Double, _
        ByVal dTickValue As Double _
) As Long
    
    usp_BoFutureRoot_Save = _
    Execute(False, "usp_BoFutureRoot_Save" _
        , iFutureRootID _
        , iUnderlyingID _
        , vcFutureRootSymbol _
        , vcFutureRootName _
        , iFutureLotSize _
        , iOptionLotSize _
        , vcExportSymbol _
        , 1 _
        , 1 _
        , tiQuoteFormat _
        , iQuotationUnitAmmount _
        , iQuotationUnitID _
        , dQuoteDenomination _
        , dTickSize _
        , dTickValue)
End Function

Public Function usp_BoFutureRootWithSymbol_Exist(ByVal vcFutureRootSymbol As String) As Byte
    usp_BoFutureRootWithSymbol_Exist = _
    Execute(False, "usp_BoFutureRootWithSymbol_Exist", vcFutureRootSymbol)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_EodFutureOption_Import( _
    ByVal iFutureID As Long, _
    ByVal fStrike As Double, _
    ByVal ExpirationDate As Date, _
    ByVal tiIsCall As Boolean, _
    ByVal vcSymbol As String, _
    ByVal vcContractName As String, _
    ByVal dClosePrice As Double, _
    ByRef vcBadContractName As String _
) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_EodFutureOption_Import"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iFutureID", adInteger, adParamInput, , iFutureID)
        .Parameters.Append .CreateParameter("@fStrike", adDouble, adParamInput, , fStrike)
        .Parameters.Append .CreateParameter("@dtExpiryDate", adDate, adParamInput, , ExpirationDate)
        .Parameters.Append .CreateParameter("@tiIsCall", adTinyInt, adParamInput, , IIf(tiIsCall, 1, 0))
        .Parameters.Append .CreateParameter("@vcSymbol", adVarChar, adParamInput, 20, vcSymbol)
        .Parameters.Append .CreateParameter("@vcContractName", adVarChar, adParamInput, 255, vcContractName)
        .Parameters.Append .CreateParameter("@vcBadContractName", adVarChar, adParamOutput, 255)
        .Parameters.Append .CreateParameter("@fPriceClose", adDouble, adParamInput, , dClosePrice)
               
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        vcBadContractName = gCmn.ReadStr(.Parameters("@vcBadContractName"))
        
        usp_EodFutureOption_Import = gCmn.ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to import future option."
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFutureOptionPair_Del( _
    ByVal iOptionIDCall As Variant, _
    ByVal iOptionIDPut As Variant _
) As Boolean
    usp_BoFutureOptionPair_Del = 0 = Execute(False, "usp_BoFutureOptionPair_Del", _
        iOptionIDCall, _
        iOptionIDPut _
        )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoFuture_Import( _
    ByVal iFutureID As Variant, _
    ByVal iFutureRootID As Variant, _
    ByVal dtMaturityDate As Date, _
    ByVal vcSymbol As String, _
    ByVal vcContractName As String, _
    ByVal iLotSizeID As Long, _
    ByVal iOptionLotSizeID As Long, _
    ByVal dClosePrice As Double, _
    ByRef vcBadContractName As String, _
    ByVal dTickSize As Double, _
    ByVal iQuotationUnitID As Long, _
    ByVal vcQuotationUnitCurrency As String, _
    ByVal ExpirationDate As Date _
) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_BoFuture_Import"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iFutureID", adInteger, adParamInput, , iFutureID)
        .Parameters.Append .CreateParameter("@iFutureRootID", adInteger, adParamInput, , iFutureRootID)
        .Parameters.Append .CreateParameter("@dtMaturityDate", adDate, adParamInput, , dtMaturityDate)
        .Parameters.Append .CreateParameter("@vcSymbol", adVarChar, adParamInput, 20, vcSymbol)
        .Parameters.Append .CreateParameter("@vcContractName", adVarChar, adParamInput, 255, vcContractName)
        .Parameters.Append .CreateParameter("@iLotSizeID", adInteger, adParamInput, , iLotSizeID)
        .Parameters.Append .CreateParameter("@iOptionLotSizeID", adInteger, adParamInput, , iOptionLotSizeID)
        .Parameters.Append .CreateParameter("@fPriceClose", adDouble, adParamInput, , dClosePrice) '@dClosePrice => @fPriceClose fokiny
        .Parameters.Append .CreateParameter("@vcBadFutureName", adVarChar, adParamOutput, 255)
        .Parameters.Append .CreateParameter("@fTickSize", adDouble, adParamInput, , dTickSize)
        .Parameters.Append .CreateParameter("@iQuotationUnitID", adInteger, adParamInput, , iQuotationUnitID)
        .Parameters.Append .CreateParameter("@vcQuotationUnitCurrency", adVarChar, adParamInput, 3, vcQuotationUnitCurrency)
        .Parameters.Append .CreateParameter("@dtExpiryDate", adDate, adParamInput, , ExpirationDate)
               
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        vcBadContractName = gCmn.ReadStr(.Parameters("@vcBadFutureName"))
        
        usp_BoFuture_Import = gCmn.ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to import future."
End Function

Public Function usp_BoIndexWithBasketInfo_Get() As ADODB.Recordset
    Set usp_BoIndexWithBasketInfo_Get = Execute(True, "usp_BoIndexWithBasketInfo_Get")
End Function

Public Function usp_BoIndexBasketWeight_Save(ByRef iIndexID&, ByVal vcIndexSymbol$, _
                                    ByRef iStockID&, ByVal vcStockSymbol$, _
                                    ByVal fWeight#, ByVal tiAddNewIndex As Byte, _
                                    ByVal tiAddNewStock As Byte, ByVal tiRaiseError As Byte, _
                                    ByRef vcMessage$) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    usp_BoIndexBasketWeight_Save = 0
    vcMessage = ""
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_BoIndexBasketWeight_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iIndexID", adInteger, adParamInputOutput, , IIf(iIndexID <> 0, iIndexID, Null))
        .Parameters.Append .CreateParameter("@vcIndexSymbol", adVarChar, adParamInput, 20, vcIndexSymbol)
        .Parameters.Append .CreateParameter("@iStockID", adInteger, adParamInputOutput, , IIf(iStockID <> 0, iStockID, Null))
        .Parameters.Append .CreateParameter("@vcStockSymbol", adVarChar, adParamInput, 20, vcStockSymbol)
        .Parameters.Append .CreateParameter("@fWeight", adDouble, adParamInput, , fWeight)
        .Parameters.Append .CreateParameter("@tiAddNewIndex", adTinyInt, adParamInput, , tiAddNewIndex)
        .Parameters.Append .CreateParameter("@tiAddNewStock", adTinyInt, adParamInput, , tiAddNewStock)
        .Parameters.Append .CreateParameter("@tiRaiseError", adTinyInt, adParamInput, , tiRaiseError)
        .Parameters.Append .CreateParameter("@vcMessage", adVarChar, adParamOutput, 255)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iIndexID = gCmn.ReadLng(.Parameters("@iIndexID"))
        iStockID = gCmn.ReadLng(.Parameters("@iStockID"))
        vcMessage = gCmn.ReadStr(.Parameters("@vcMessage"))
        
        usp_BoIndexBasketWeight_Save = gCmn.ReadLng(.Parameters(0))
    End With
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to save index basket weight"
End Function

Public Function usp_BoIndexWithBasketInfo_ClearWeights&(ByVal iIndexID&)
    usp_BoIndexWithBasketInfo_ClearWeights = Execute(False, "usp_BoIndexWithBasketInfo_ClearWeights", IIf(iIndexID <> 0, iIndexID, Null))
End Function

Public Function usp_AspUserStatus_Get(ByVal sUserName$) As Long
    Dim cmd As New ADODB.Command
    
    usp_AspUserStatus_Get = -3 ' ASP_SUBSCRIPTION_STATUS_NOT_SUBSCRIBED
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_AspUserStatus_Get"

        .Parameters.Append .CreateParameter("@sUserName", adVarChar, adParamInput, 63, sUserName)
        .Parameters.Append .CreateParameter("@iStatus", adInteger, adParamOutput)

        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        usp_AspUserStatus_Get = gCmn.ReadLng(.Parameters("@iStatus"))
    End With
End Function

Public Function usp_BoCustomStrikeSkewPoint_ByUnderlying_Del(ByVal iUnderlyingID&) As Boolean
    usp_BoCustomStrikeSkewPoint_ByUnderlying_Del = (0 = Execute(False, "usp_BoCustomStrikeSkewPoint_ByUnderlying_Del", iUnderlyingID))
End Function

Public Function usp_BoCustomStrikeSkewPoint_Default_Save(ByVal iContractID&, ByVal fStrike#, ByVal dtExpiry As Date, ByVal fVolatility#) As Boolean
    usp_BoCustomStrikeSkewPoint_Default_Save = (0 = Execute(False, "usp_BoCustomStrikeSkewPoint_Default_Save", iContractID, fStrike, dtExpiry, fVolatility))
End Function

Public Function usp_MmCustomDividend_Get(Optional ByVal iUndID As Long = 0) As ADODB.Recordset
    Set usp_MmCustomDividend_Get = ExecuteLBO(True, "usp_MmCustomDividend_Get", IIf(iUndID <> 0, iUndID, Null))
End Function
Public Function usp_MmVegaWeight_Get() As ADODB.Recordset
    Set usp_MmVegaWeight_Get = Execute(True, "usp_MmVegaWeight_Get")
End Function

Private Function ExecuteLBO(ByVal bReturnRST As Boolean, ByVal sCommandText As String, ParamArray arrParams() As Variant) As Variant
    Dim cmd As New ADODB.Command
    Dim rst As New ADODB.Recordset
    Dim i&, nLBnd&, nUBnd&, bRetry As Boolean
    Dim parRet As ADODB.Parameter

    bRetry = False
    
Retry:
    On Error GoTo EH
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = sCommandText

        If Not bReturnRST Then
            Set parRet = .CreateParameter("return", adInteger, adParamReturnValue)
            .Parameters.Append parRet
        End If

        nLBnd = LBound(arrParams)
        nUBnd = UBound(arrParams)
        For i = nLBnd To nUBnd
            .Parameters.Append CreateParam(i, arrParams(i))
        Next

        '.Prepared = True
        If bReturnRST Then
            'rst.CursorType = adOpenStatic
            'rst.CursorLocation = adUseClient
            rst.LockType = adLockBatchOptimistic
            rst.Open cmd
            Set ExecuteLBO = rst
            Set rst = Nothing
        Else
            .Execute
            ExecuteLBO = parRet.Value
        End If

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

    End With
    Exit Function
EH:
    Dim s$
    
    Dim sDescription$, nHelpContext&, sHelpFile$, nNumber&, sSource$
    sDescription = Err.Description
    nHelpContext = Err.HelpContext
    sHelpFile = Err.HelpFile
    nNumber = Err.Number
    sSource = Err.Source

    If InStr(sDescription, "Connect") > 0 Then
        If Not bRetry Then
            bRetry = True
            On Error Resume Next
            m_DB.Close
            Resume Retry
        End If
    End If
    
    s = "exec " & sCommandText
    nLBnd = LBound(arrParams)
    nUBnd = UBound(arrParams)
    For i = nLBnd To nUBnd
        s = s & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(s, 1) = "," Then
        s = Left$(s, Len(s) - 1)
    End If
    
    Err.Raise nNumber, sSource, s & vbCrLf & sDescription, sHelpFile, nHelpContext
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_QuotationUnit_Get( _
    Optional ByVal iQuotationUnitID As Long = 0 _
) As ADODB.Recordset
    Set usp_QuotationUnit_Get = Execute(True _
    , "usp_QuotationUnit_Get" _
    , DBWrite(iQuotationUnitID) _
    )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_BoCommodity_Get( _
    Optional ByVal iICID As Variant = Null _
) As ADODB.Recordset
    Set usp_BoCommodity_Get = Execute(True, "usp_BoCommodity_Get" _
        , iICID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_FutureIC_Get( _
    iICID As Long _
) As ADODB.Recordset
    Set usp_FutureIC_Get = Execute(True, "usp_FutureIC_Get", iICID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_FutureUnderlyingType_Get( _
    Optional ByVal iCptID As Variant = Null _
) As ADODB.Recordset
    Set usp_FutureUnderlyingType_Get = Execute(True, "usp_FutureUnderlyingType_Get", _
        iCptID _
    )
End Function

Public Function usp_ICFutures_Get( _
    iICID As Long _
) As ADODB.Recordset
    Set usp_ICFutures_Get = Execute(True, "usp_ICFutures_Get", iICID)
End Function

Public Function usp_BoFutureRoot_Update(ByVal iFutureRootID As Variant, _
        ByVal iUnderlyingID As Variant, _
        ByVal vcFutureRootSymbol As String, _
        ByVal vcFutureRootName As String, _
        ByVal iFutureLotSize As Long, _
        ByVal iOptionLotSize As Long, _
        ByVal vcExportSymbol$, _
        ByVal tiQuoteFormat As Long, _
        ByVal iQuotationUnitAmmount As Long, _
        ByVal iQuotationUnitID As Long, _
        ByVal dQuoteDenomination As Double, _
        ByVal dTickSize As Double, _
        ByVal dTickValue As Double _
) As Long
    
    usp_BoFutureRoot_Update = _
    Execute(False, "usp_BoFutureRoot_Save" _
        , iFutureRootID _
        , iUnderlyingID _
        , vcFutureRootSymbol _
        , vcFutureRootName _
        , iFutureLotSize _
        , iOptionLotSize _
        , vcExportSymbol _
        , 1 _
        , 1 _
        , tiQuoteFormat _
        , iQuotationUnitAmmount _
        , iQuotationUnitID _
        , dQuoteDenomination _
        , dTickSize _
        , dTickValue)
End Function

Public Function usp_BoCommodity_Del(ByVal iCID As Long, ByVal tiIsRaiseError As Byte) As Long
    usp_BoCommodity_Del = Execute(False, "usp_BoCommodity_Del", iCID, tiIsRaiseError)
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IC_Save(Optional iICID As Variant = Null, _
    Optional vcSymbol As Variant = Null, _
    Optional vcName As Variant = Null, _
    Optional iICType As Long, _
    Optional tiGreeksCalcType As Variant = Null, _
    Optional tiIsActive As Variant = Null, _
    Optional iFutureUnderlyingTypeID As Long, _
    Optional tiCheckExistance As Variant = Null, _
    Optional iUndPriceProfileID As Variant = Null, _
    Optional iOptPriceProfileID As Variant = Null _
 ) As Long
    usp_IC_Save = Execute(False, "usp_IC_Save" _
        , iICID _
        , vcSymbol _
        , vcName _
        , iICType _
        , tiGreeksCalcType _
        , tiIsActive _
        , iFutureUnderlyingTypeID _
        , tiCheckExistance _
        , iUndPriceProfileID _
        , iOptPriceProfileID)
        
End Function

Public Function usp_Contract_ClosePrice_Save( _
    ByVal iContractID As Long, _
    ByVal fPriceClose As Double _
) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Contract_ClosePrice_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , iContractID)
        .Parameters.Append .CreateParameter("@dContractClosePrice", adDouble, adParamInput, , fPriceClose)
               
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to import contract close price."
End Function

Public Function usp_DefaultIVSurfaceStock_Get(iUnderlyingID As Variant) As ADODB.Recordset
    Set usp_DefaultIVSurfaceStock_Get = Execute(True, "usp_DefaultIVSurfaceStock_Get", iUnderlyingID)
End Function

Public Function usp_DefaultIVSurfaceFutureUnd_Get(iUnderlyingID As Variant) As ADODB.Recordset
    Set usp_DefaultIVSurfaceFutureUnd_Get = Execute(True, "usp_DefaultIVSurfaceFutureUnd_Get", iUnderlyingID)
End Function
Public Function usp_BO_DividentsImport(strXML As String) As ADODB.Recordset
    Set usp_BO_DividentsImport = Execute(True, "usp_BO_DividentsImport", strXML)
End Function
