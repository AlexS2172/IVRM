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
Option Explicit

Private m_DB As ADODB.Connection
Private m_ConnectionString As String
Private m_IvDB As ADODB.Connection
Private m_IvConnectionString As String

Public Sub Init(ByVal ConnectionString As String, Optional ByVal IvConnectionString As String = "")
    m_ConnectionString = ConnectionString
    m_IvConnectionString = IvConnectionString
End Sub

Private Sub Class_Terminate()
    On Error Resume Next
    m_DB.Close
    If Not m_IvDB Is Nothing Then
        If m_IvDB.State = adStateOpen Then m_IvDB.Close
    End If
    Set m_DB = Nothing
End Sub
Public Function CheckIvDBConnection() As Boolean
    On Error GoTo EH
    Dim cnn As New ADODB.Connection

    CheckIvDBConnection = False
    
    cnn.ConnectionString = m_IvConnectionString
    cnn.ConnectionTimeout = 30
    If cnn.ConnectionString <> "" Then
        cnn.Open
    Else
        cnn.Provider = "SQLOLEDB"
        Err.Raise vbObjectError + 1023, "Application Settings", "Invalid IV database settings."
    End If
    
    CheckIvDBConnection = True
    Exit Function
EH:
    MsgBox "Fail to connect to IV database.", vbCritical
    'On Error Resume Next
    'CheckDBConnection = ChangeDatabase(frmOwner, cnn)
End Function
Public Function CheckDBConnection(ByRef frmOwner As Form, ByVal bFirstRun As Boolean) As Boolean
    On Error GoTo EH
    Dim cnn As New ADODB.Connection

    CheckDBConnection = False
    
    cnn.ConnectionString = m_ConnectionString
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
    If Not bFirstRun Then gCmn.ErrorMsgBox frmOwner, "Fail to connect to database."
    'On Error Resume Next
    'CheckDBConnection = ChangeDatabase(frmOwner, cnn)
End Function

'Public Function ChangeDatabase(ByRef frmOwner As Form, Optional ByRef aConn As ADODB.Connection = Nothing, _
'                                Optional ByVal bSaveChanges As Boolean = True, Optional ByRef sConnectionString As String = "") As Boolean
'    On Error GoTo EH
'    Dim dlgDB As New MSDASC.DataLinks
'    Dim cnn As ADODB.Connection, sConnString$
'    ChangeDatabase = False
'
'    dlgDB.hWnd = frmOwner.hWnd
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
'                    g_Params.DbConnection = sConnString
'                    g_Params.SystemDbConnection = sConnString
'                Else
'                    sConnectionString = sConnString
'                End If
'                ChangeDatabase = True
'                Exit Do
'            Else
'                gCmn.ErrorMsgBox frmOwner, "Fail to change database settings."
'            End If
'            On Error GoTo EH
'        Else
'            Exit Do
'        End If
'    Loop
'
'    Exit Function
'EH:
'    gCmn.ErrorMsgBox frmOwner, "Fail to change database settings."
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
        m_DB.ConnectionString = m_ConnectionString
        m_DB.ConnectionTimeout = 60
        m_DB.CommandTimeout = 120
        m_DB.Open
    End If
    
    Set Connection = m_DB
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to connect to database."
End Function
Public Function IvConnection() As ADODB.Connection
    Dim bOpen As Boolean
            
    If Not m_IvDB Is Nothing Then
        On Error Resume Next
        bOpen = True
        bOpen = (m_IvDB.State = ADODB.adStateClosed)
    Else
        On Error GoTo EH
        Set m_IvDB = New ADODB.Connection
        bOpen = True
    End If
        
    If bOpen Then
        On Error GoTo EH
        Set m_IvDB = New ADODB.Connection
        m_IvDB.CursorLocation = ADODB.adUseClient
        m_IvDB.ConnectionString = m_IvConnectionString
        m_IvDB.ConnectionTimeout = 60
        m_IvDB.CommandTimeout = 120
        m_IvDB.Open
    End If
    
    Set IvConnection = m_IvDB
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to connect to database."
End Function

Public Function GetDbPath() As String
    On Error Resume Next
    GetDbPath = "N/A"
    GetDbPath = m_DB.Properties("Data Source Name").Value & "\" & m_DB.Properties("Current Catalog").Value
End Function

Private Function ParseParam(ByVal v As Variant) As String
    On Error Resume Next
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
            ParseParam = val(v)
    End Select
End Function

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
                
            Case vbByte
                .Type = adTinyInt
                .Value = CByte(varValue)
            
            Case vbBoolean
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
Private Function IvExecute(ByVal bReturnRST As Boolean, ByVal sCommandText As String, ParamArray arrParams() As Variant) As Variant
    Dim cmd As New ADODB.Command
    Dim rst As New ADODB.Recordset
    Dim i&, nLBnd&, nUBnd&, bRetry As Boolean
    Dim parRet As ADODB.Parameter

    bRetry = False
    
Retry:
    On Error GoTo EH
    
    With cmd
        Set .ActiveConnection = IvConnection
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
            rst.CursorType = adOpenStatic
            rst.CursorLocation = adUseClient
            rst.Open cmd, , adOpenStatic
            Set IvExecute = rst
            Set rst = Nothing
        Else
            .Execute
            IvExecute = parRet.Value
        End If

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

    End With
    Exit Function
EH:
    Dim S$
    
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
            m_IvDB.Close
            Resume Retry
        End If
    End If
    
    S = "exec " & sCommandText
    nLBnd = LBound(arrParams)
    nUBnd = UBound(arrParams)
    For i = nLBnd To nUBnd
        S = S & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(S, 1) = "," Then
        S = Left$(S, Len(S) - 1)
    End If
    
    Err.Raise nNumber, sSource, S & vbCrLf & sDescription, sHelpFile, nHelpContext
End Function
Private Function Execute(ByVal bReturnRST As Boolean, ByVal sCommandText As String, ParamArray arrParams() As Variant) As Variant
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
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

    End With
    Exit Function
EH:
    Dim S$
    
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
    
    S = "exec " & sCommandText
    nLBnd = LBound(arrParams)
    nUBnd = UBound(arrParams)
    For i = nLBnd To nUBnd
        S = S & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(S, 1) = "," Then
        S = Left$(S, Len(S) - 1)
    End If
    
    Err.Raise nNumber, sSource, S & vbCrLf & sDescription, sHelpFile, nHelpContext
End Function

Public Function usp_Underlying_Get(Optional iContractID As Variant = Null, _
                                    Optional ByVal iTraderID As Long = 0, _
                                    Optional ByVal iGroupID As Variant = Null, _
                                    Optional ByVal tiWithPosition As Byte = 0) As ADODB.Recordset
    Set usp_Underlying_Get = Execute(True, "usp_Underlying_Get", iContractID, IIf(iTraderID <> 0, iTraderID, Null), iGroupID, tiWithPosition)
End Function

Public Function usp_UnderlyingExpiry_Get(Optional iContractID As Variant = Null, _
                                    Optional ByVal iTraderID As Long = 0, _
                                    Optional ByVal iGroupID As Variant = Null, _
                                    Optional ByVal dtMinExpiry As Variant = Null) As ADODB.Recordset
    Set usp_UnderlyingExpiry_Get = Execute(True, "usp_UnderlyingExpiry_Get", iContractID, IIf(iTraderID <> 0, iTraderID, Null), iGroupID, dtMinExpiry)
End Function

Public Function usp_UnderlyingExpiryByList_Get(ByVal dtMinExpiry As Variant, _
                                    ByRef vcUnderlyingID1 As String, _
                                    ByRef vcUnderlyingID2 As String) As ADODB.Recordset
    Set usp_UnderlyingExpiryByList_Get = Execute(True, "usp_UnderlyingExpiryByList_Get", _
                                    dtMinExpiry, vcUnderlyingID1, vcUnderlyingID2)
End Function

Public Function usp_UnderlyingOptionByExpiry_Get(ByVal iUnderlyingID As Long, _
                                                ByVal dtExpiry As Date) As ADODB.Recordset
    Set usp_UnderlyingOptionByExpiry_Get = Execute(True, "usp_UnderlyingOptionByExpiry_Get", iUnderlyingID, dtExpiry)
End Function

Public Function usp_UnderlyingPos_Get(Optional iContractID As Variant = Null, _
                                    Optional ByVal iTraderID As Long = 0, _
                                    Optional ByVal iGroupID As Variant = Null, _
                                    Optional ByVal tiWithSimulated As Byte = 0) As ADODB.Recordset
    Set usp_UnderlyingPos_Get = Execute(True, "usp_UnderlyingPos_Get", iContractID, IIf(iTraderID <> 0, iTraderID, Null), iGroupID, tiWithSimulated)
End Function

Public Function usp_UnderlyingPosByList_Get(ByVal iTraderID As Long, _
                                    ByVal tiWithSimulated As Byte, _
                                    ByRef vcUnderlyingID1 As String, _
                                    ByRef vcUnderlyingID2 As String) As ADODB.Recordset
    Set usp_UnderlyingPosByList_Get = Execute(True, "usp_UnderlyingPosByList_Get", IIf(iTraderID <> 0, iTraderID, Null), _
                                        tiWithSimulated, vcUnderlyingID1, vcUnderlyingID2)
End Function

Public Function usp_MmUnderlying_Get(Optional ByVal iTraderID As Long = 0) As ADODB.Recordset
    Set usp_MmUnderlying_Get = Execute(True, "usp_MmUnderlying_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_UnderlyingOptions_Get(Optional iUnderlyingID As Variant = Null, _
                                    Optional ByVal iTraderID As Long = 0, _
                                    Optional ByVal iGroupID As Variant = Null, _
                                    Optional ByVal tiWithPosition As Byte = 0, _
                                    Optional ByVal dtMinExpiry As Variant = Null) As ADODB.Recordset
    Set usp_UnderlyingOptions_Get = Execute(True, "usp_UnderlyingOptions_Get", iUnderlyingID, _
                                IIf(iTraderID <> 0, iTraderID, Null), iGroupID, tiWithPosition, dtMinExpiry)
End Function
Public Function usp_MmDividend_Save(ByVal iContractID As Variant, Optional ByVal iDivType As Variant = Null, Optional ByVal fYield As Variant = Null, _
                                    Optional ByVal fDivAmt As Variant = Null, Optional ByVal iDivFreq As Variant = Null, Optional ByVal dtDivDate As Variant = Null, _
                                    Optional ByVal fDivAmtCust As Variant = Null, Optional ByVal iDivFreqCust As Variant = Null, Optional ByVal dtDivDateCust As Variant = Null)
                                    
    usp_MmDividend_Save = Execute(False, "usp_MmDividend_Save", iContractID, iDivType, fDivAmt, dtDivDate, iDivFreq, _
                                                                fDivAmtCust, dtDivDateCust, iDivFreqCust, fYield)
End Function

Public Function usp_Stock_Save(ByVal iStockID As Variant, ByVal iExchangeId As Variant, ByVal vcSymbol As Variant, ByVal vcContractName As Variant, _
                                ByVal bIsHTB As Variant, ByVal iDivFreq As Variant, ByVal fDivAmt As Variant, ByVal dtDivDate As Variant, _
                                ByVal tiIsDivCustom As Variant, ByVal fDivAmtCustom As Variant, ByVal iDivFreqCustom As Variant, ByVal dtDivDateCustom As Variant, _
                                ByVal tiCalcOptionType As Variant, ByVal tiCalcModelType As Variant, ByVal iExpCalendarID As Variant, ByVal tiIsActive As Variant, _
                                ByVal fSkew As Variant, ByVal fKurt As Variant, ByVal bIsHedgeSymbol As Variant, ByVal tiCheckExistance As Byte, _
                                ByVal iUndPriceProfileID As Variant, ByVal iOptPriceProfileID As Variant, ByVal bIsAllowLending As Variant) As Long
    
    usp_Stock_Save = Execute(False, "usp_Stock_Save", _
                                iStockID, iExchangeId, vcSymbol, vcContractName, _
                                bIsHTB, iDivFreq, fDivAmt, dtDivDate, _
                                tiIsDivCustom, fDivAmtCustom, iDivFreqCustom, dtDivDateCustom, _
                                tiCalcOptionType, tiCalcModelType, iExpCalendarID, tiIsActive, _
                                fSkew, fKurt, tiCheckExistance, iUndPriceProfileID, iOptPriceProfileID, bIsHedgeSymbol, bIsAllowLending)
End Function

Public Function usp_Stock_Import(ByVal iContractID As Variant, ByVal bIsStock As Variant, ByVal vcContractName As Variant, _
                                ByVal iDivFreq As Variant, ByVal fDivAmt As Variant, ByVal dtDivDate As Variant, _
                                ByVal fPriceAsk As Variant, ByVal fPriceBid As Variant, ByVal fPriceOpen As Variant, _
                                ByVal fPriceClose As Variant, ByVal fPriceLow As Variant, ByVal fPriceHigh As Variant, _
                                ByVal iVolume As Variant, ByVal iExchangeId As Variant) As Long
    
    usp_Stock_Import = Execute(False, "usp_Stock_Import", _
                                iContractID, bIsStock, vcContractName, _
                                iDivFreq, fDivAmt, dtDivDate, _
                                fPriceAsk, fPriceBid, fPriceOpen, _
                                fPriceClose, fPriceLow, fPriceHigh, _
                                iVolume, iExchangeId)
End Function

Public Function usp_Index_Save(ByVal iIndexID As Variant, ByVal vcSymbol As Variant, ByVal vcContractName As Variant, _
                                ByVal fYield As Variant, ByVal bIsHTB As Variant, ByVal tiCalcOptionType As Variant, _
                                ByVal tiCalcModelType As Variant, ByVal iExpCalendarID As Variant, ByVal tiIsBasket As Variant, _
                                ByVal tiIsActive As Variant, ByVal fSkew As Variant, ByVal fKurt As Variant, ByVal bIsHedgeSymbol As Variant, ByVal tiCheckExistance As Byte, _
                                ByVal SOQ As Variant, _
                                ByVal iUndPriceProfileID As Variant, ByVal iOptPriceProfileID As Variant, Optional ByVal tiIsDivCustom As Variant = Null, Optional ByVal bIsAllowLending As Variant) As Long
    
    usp_Index_Save = Execute(False, "usp_Index_Save", _
                                iIndexID, vcSymbol, vcContractName, _
                                fYield, bIsHTB, tiCalcOptionType, tiCalcModelType, iExpCalendarID, _
                                tiIsBasket, tiIsActive, fSkew, fKurt, tiCheckExistance, SOQ, iUndPriceProfileID, iOptPriceProfileID, bIsHedgeSymbol, Null, tiIsDivCustom, Null, Null, Null, Null, Null, Null, bIsAllowLending)
End Function

Public Function usp_MmFuture_Save(ByVal iFutureID&, _
                                Optional ByVal iUndPriceProfileID As Variant = Null, _
                                Optional ByVal iOptPriceProfileID As Variant = Null, _
                                Optional ByVal vcFutureName As Variant = Null, _
                                Optional ByVal tiCalcOptionType As Variant = Null, _
                                Optional ByVal tiIsActive As Variant = Null, _
                                Optional ByRef dBasis As Variant = Null, _
                                Optional ByRef bUseInCalc As Variant = Null, _
                                Optional ByRef dRatio As Variant = Null _
                                ) As Long
   

    usp_MmFuture_Save = Execute(False, "usp_MmFuture_Save", _
                                iFutureID, vcFutureName, _
                                tiCalcOptionType, tiIsActive, _
                                iUndPriceProfileID, iOptPriceProfileID, bUseInCalc, dBasis, dRatio)

End Function

Public Sub usp_StockCustomDiv_Save(ByVal iStockID As Long, ByVal tiIsDivCustom As Byte, _
                                    Optional ByVal fDivAmtCustom As Variant = Null, _
                                    Optional ByVal iDivFreqCustom As Variant = Null, _
                                    Optional ByVal dtDivDateCustom As Variant = Null)
    
    Execute False, "usp_Stock_Save", iStockID, Null, Null, Null, Null, Null, Null, Null, _
                                    tiIsDivCustom, fDivAmtCustom, iDivFreqCustom, dtDivDateCustom
End Sub


Public Function usp_MmUnderlyingExchanges_Get(ByVal iUnderlyingID&) As ADODB.Recordset
    Set usp_MmUnderlyingExchanges_Get = Execute(True, "usp_MmUnderlyingExchanges_Get", iUnderlyingID)
End Function

Public Function usp_MmFutureExchanges_Get(ByVal iUnderlyingID&) As ADODB.Recordset
    Set usp_MmFutureExchanges_Get = Execute(True, "usp_MmFutureExchanges_Get", iUnderlyingID)
End Function

Public Function usp_MmOptionByUnderlying_Get(ByVal iUnderlyingID&, ByVal dtMinExpiry As Date) As ADODB.Recordset
    Set usp_MmOptionByUnderlying_Get = Execute(True, "usp_MmOptionByUnderlying_Get", iUnderlyingID, dtMinExpiry)
End Function

Public Function usp_MmFutureByIndex_Get(ByVal iIndexID&, ByVal dtMinExpiry As Date) As ADODB.Recordset
    Set usp_MmFutureByIndex_Get = Execute(True, "usp_MmFutureByIndex_Get", iIndexID, dtMinExpiry)
End Function

Public Function usp_MmOptionByFuture_Get(ByVal iFutureID&, ByVal dtMinExpiry As Date) As ADODB.Recordset
    Set usp_MmOptionByFuture_Get = Execute(True, "usp_MmOptionByFuture_Get", iFutureID, dtMinExpiry)
End Function

Public Function usp_MmFutureByUnderlying_Get(ByVal iUnderlyingID&, ByVal dtMinExpiry As Date) As ADODB.Recordset
    Set usp_MmFutureByUnderlying_Get = Execute(True, "usp_MmFutureByUnderlying_Get", iUnderlyingID, dtMinExpiry)
End Function

Public Function usp_EodMsUnderlyingCache_Get(Optional ByVal iUndID As Variant = Null) As ADODB.Recordset
    Set usp_EodMsUnderlyingCache_Get = Execute(True, "usp_EodMsUnderlyingCache_Get", iUndID)
End Function

Public Function usp_EodMsOptionCache_Get(ByVal iUndID As Long) As ADODB.Recordset
    Set usp_EodMsOptionCache_Get = Execute(True, "usp_EodMsOptionCache_Get", iUndID)
End Function

Public Function usp_EodMsUnderlyingCache_Check(ByVal UnderlyingContractID As Long) As Boolean
    usp_EodMsUnderlyingCache_Check = (0 = Execute(False, "usp_EodMsUnderlyingCache_Check", UnderlyingContractID))
End Function

Public Function usp_EodMsLoad_Init(Optional ByVal UndID As Variant = Null) As Boolean
    usp_EodMsLoad_Init = (0 = Execute(False, "usp_EodMsLoad_Init", UndID))
End Function

Public Function usp_EodMsUnderlyingCache_Save(ByVal UnderlyingID As Long, _
                                            Optional ByVal UpdateDate As Variant = Null) As Boolean
    usp_EodMsUnderlyingCache_Save = (0 = Execute(False, "usp_EodMsUnderlyingCache_Save", UnderlyingID, UpdateDate))
End Function

Public Function usp_Option_Import(ByVal UnderlyingContractID As Long, ByVal Symbol As String, _
                                    ByVal IsCall As Boolean, ByVal Strike As Double, _
                                    ByVal Expiry As Date, ByVal ExpiryOV As Date, ByVal TradingClose As Date, _
                                    ByVal LotSize As Long, Optional ByVal iMsCacheOptID As Variant = Null) As Boolean
                                    
    usp_Option_Import = (0 = Execute(False, "usp_Option_Import", UnderlyingContractID, Symbol, _
                                    IIf(IsCall, 1, 0), Strike, Expiry, ExpiryOV, TradingClose, LotSize, iMsCacheOptID))
End Function

Public Function usp_EodMsOptionCache_Save(ByVal UnderlyingContractID As Long, ByVal Symbol As String, _
                                    ByVal Expiry As Date, ByVal ExpiryOV As Date, ByVal TradingClose As Date, _
                                    ByVal Strike As Double, ByVal IsCall As Boolean, _
                                    ByVal LotSize As Long) As Boolean
    usp_EodMsOptionCache_Save = (0 = Execute(False, "usp_EodMsOptionCache_Save", UnderlyingContractID, Symbol, _
                                                               Expiry, ExpiryOV, TradingClose, Strike, IsCall, LotSize))
End Function

Public Function usp_MmIndex_Get() As ADODB.Recordset
    Set usp_MmIndex_Get = Execute(True, "usp_MmIndex_Get")
End Function

Public Function usp_MmIndexDefinition_Get() As ADODB.Recordset
    Set usp_MmIndexDefinition_Get = Execute(True, "usp_MmIndexDefinition_Get")
End Function

Public Function usp_IndexDefinition_Save(ByVal iIndexID As Long, ByVal iStockID As Long, ByVal fWeight As Variant, _
                    ByVal fBeta As Variant, Optional ByVal vcIndexSymbol As Variant = Null, Optional ByVal vcStockSymbol As Variant = Null) As Long
                    
    usp_IndexDefinition_Save = Execute(False, "usp_IndexDefinition_Save", _
                                    iIndexID, iStockID, fWeight, Null, vcIndexSymbol, vcStockSymbol)
End Function

Public Function usp_IndexUnderlyingBeta_Save(ByVal iIndexID As Long, ByVal iUnderlyingID As Long, _
                    ByVal fBeta As Variant, Optional ByVal vcIndexSymbol As Variant = Null, Optional ByVal vcUnderlyingSymbol As Variant = Null) As Long
                    
    usp_IndexUnderlyingBeta_Save = Execute(False, "usp_IndexUnderlyingBeta_Save", _
                                    iIndexID, iUnderlyingID, fBeta, vcIndexSymbol, vcUnderlyingSymbol)
End Function

Public Function usp_MmIndexUnderlyingBeta_Get() As ADODB.Recordset
    Set usp_MmIndexUnderlyingBeta_Get = Execute(True, "usp_IndexUnderlyingBeta_Get")
End Function

Public Function usp_CustomStrikeSkewDefaultPointFromIV_Save&(iUnderlyingContractID&, fStrike#, dtExpDate As Date, iType&, fVolatility#)
    
    usp_CustomStrikeSkewDefaultPointFromIV_Save = Execute(False, "usp_CustomStrikeSkewDefaultPointFromIV_Save", _
                                                                iUnderlyingContractID, fStrike, dtExpDate, iType, fVolatility)
End Function

Public Function usp_ContractInGroup_Get(Optional ByVal iGroupID As Variant = Null, Optional ByVal iContractID As Variant = Null) As ADODB.Recordset
    Set usp_ContractInGroup_Get = Execute(True, "usp_ContractInGroup_Get", iGroupID, iContractID)
End Function

Public Function usp_ContractInGroupByContract_Save(ByVal iContractID As Long, ByVal iGroupID As Long) As Long
    usp_ContractInGroupByContract_Save = Execute(False, "usp_ContractInGroupByContract_Save", iContractID, iGroupID)
End Function

Public Function usp_VolaBandsEOD_Init_Save&()
    usp_VolaBandsEOD_Init_Save = Execute(False, "usp_VolaBandsEOD_Init_Save")
End Function

Public Function usp_VolaBandsEOD_Complete_Save&()
    usp_VolaBandsEOD_Complete_Save = Execute(False, "usp_VolaBandsEOD_Complete_Save")
End Function

Public Function usp_VolaBandsEOD_Save&(ByVal vcSymbol$, ByVal iExpPeriod&, ByVal fStrikeMoneyness#, _
                                        ByVal fIVBandLow#, ByVal fIVBandHi#)
    usp_VolaBandsEOD_Save = Execute(False, "usp_VolaBandsEOD_Save", _
                                    vcSymbol, iExpPeriod, fStrikeMoneyness, fIVBandLow, fIVBandHi)
End Function

Public Function usp_Broker_Get(ByVal iBrokerID As Variant) As ADODB.Recordset
    Set usp_Broker_Get = Execute(True, "usp_Broker_Get", iBrokerID)
End Function

Public Function usp_Exchange_Get(ByVal iExchangeId As Variant) As ADODB.Recordset
    Set usp_Exchange_Get = Execute(True, "usp_Exchange_Get", iExchangeId)
End Function

Public Function usp_ContractGroup_Get(ByVal iGroupID As Variant) As ADODB.Recordset
    Set usp_ContractGroup_Get = Execute(True, "usp_ContractGroup_Get", iGroupID)
End Function

Public Function usp_ContractGroup_Save(ByVal iGroupID As Variant, ByVal vcGroupName As String, _
                                    ByVal vcDescription As String, ByVal tiRaiseError As Byte) As Long
    usp_ContractGroup_Save = Execute(False, "usp_ContractGroup_Save", iGroupID, vcGroupName, vcDescription, tiRaiseError)
End Function

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
    ByVal vcExchangeCode As Variant, _
    ByVal fPriceTheo As Variant _
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
        vcExchangeCode, _
        fPriceTheo _
        )
End Function

Public Function usp_TraderContractByTrader_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_TraderContractByTrader_Get = Execute(True, "usp_TraderContractByTrader_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_Strategy_Get(ByVal iStrategyID As Variant) As ADODB.Recordset
    Set usp_Strategy_Get = Execute(True, "usp_Strategy_Get", iStrategyID)
End Function

Public Function usp_MmVegaWeight_Get() As ADODB.Recordset
    Set usp_MmVegaWeight_Get = Execute(True, "usp_MmVegaWeight_Get")
End Function

Public Function usp_Trader_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_Trader_Get = Execute(True, "usp_Trader_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_TraderGroup_Get(ByVal iTraderGroupID As Long) As ADODB.Recordset
    Set usp_TraderGroup_Get = Execute(True, "usp_TraderGroup_Get", IIf(iTraderGroupID <> 0, iTraderGroupID, Null))
End Function

Public Function usp_Trade_Del(ByVal iTradeID As Long) As Long
    usp_Trade_Del = Execute(False, "usp_Trade_Del", iTradeID)
End Function

Public Function usp_MmTradeSeqByTrader_Get(ByVal iTraderID As Long, _
                                        ByVal iMinSeqNum As Long, _
                                        ByVal iMaxSeqNum As Long) As ADODB.Recordset
    Set usp_MmTradeSeqByTrader_Get = Execute(True, "usp_MmTradeSeqByTrader_Get", _
                                    IIf(iTraderID <> 0, iTraderID, Null), iMinSeqNum, iMaxSeqNum)
End Function

Public Function usp_LastTradeSeqNum_Get() As Long
    Dim rs As ADODB.Recordset
    usp_LastTradeSeqNum_Get = 0
    Set rs = Execute(True, "usp_LastTradeSeqNum_Get")
    If Not rs.EOF Then usp_LastTradeSeqNum_Get = ReadLng(rs!iSeqNum)
    Set rs = Nothing
End Function

Public Function usp_MmTradeByTrader_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_MmTradeByTrader_Get = Execute(True, "usp_MmTradeByTrader_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_MmCancelledOrderExec_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_MmCancelledOrderExec_Get = Execute(True, "usp_MmCancelledOrderExec_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_Trade_New(ByVal iContractID As Long, _
                                ByVal iQuantity As Long, _
                                ByVal fPrice As Double, _
                                ByVal tiIsBuy As Byte, _
                                ByVal iTraderID As Long, _
                                ByVal iStrategyID As Long, _
                                ByVal iBrokerID As Long, _
                                ByVal fBrokerCommission As Double, _
                                ByVal iClearingBrokerID As Long, _
                                ByVal fClearingBrokerCommission As Double, _
                                ByVal fTradedIV As Double, _
                                ByVal fSpotReference As Double, _
                                ByVal tiIsPosition As Byte, _
                                ByRef iNewTradeID As Long, _
                                ByRef dtNewTradeDate As Date, _
                                ByVal tiStatus As Byte, _
                                ByVal iMark As Long, _
                                ByVal iUnderlyingID As Long, _
                                ByVal iOptionRootID As Long, _
                                ByVal iFutureRootID As Long, _
                                ByVal iFutureID As Long, _
                                ByRef vcImportID As String, _
                                ByVal iOrderID As Long, _
                                ByVal iExecStatus As Long) As Long

    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    iNewTradeID = 0
    dtNewTradeDate = 0
    usp_Trade_New = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_New"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , iContractID)
        .Parameters.Append .CreateParameter("@iQuantity", adInteger, adParamInput, , iQuantity)
        .Parameters.Append .CreateParameter("@fPrice", adDouble, adParamInput, , fPrice)
        .Parameters.Append .CreateParameter("@tiIsBuy", adTinyInt, adParamInput, , tiIsBuy)
        .Parameters.Append .CreateParameter("@iTraderID", adInteger, adParamInput, , IIf(iTraderID <> 0, iTraderID, Null))
        .Parameters.Append .CreateParameter("@iStrategyID", adInteger, adParamInput, , IIf(iStrategyID <> 0, iStrategyID, Null))
        .Parameters.Append .CreateParameter("@iBrokerID", adInteger, adParamInput, , IIf(iBrokerID <> 0, iBrokerID, Null))
        .Parameters.Append .CreateParameter("@fBrokerCommission", adDouble, adParamInput, , fBrokerCommission)
        .Parameters.Append .CreateParameter("@iClearingBrokerID", adInteger, adParamInput, , IIf(iClearingBrokerID <> 0, iClearingBrokerID, Null))
        .Parameters.Append .CreateParameter("@fClearingBrokerCommission", adDouble, adParamInput, , fClearingBrokerCommission)
        .Parameters.Append .CreateParameter("@fTradedIV", adDouble, adParamInput, , fTradedIV)
        .Parameters.Append .CreateParameter("@fSpotReference", adDouble, adParamInput, , fSpotReference)
        .Parameters.Append .CreateParameter("@tiIsPosition", adTinyInt, adParamInput, , tiIsPosition)
        
        .Parameters.Append .CreateParameter("@iNewTradeID", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtNewTradeDate", adDBDate, adParamInputOutput, , IIf(dtNewTradeDate = 0, Null, dtNewTradeDate))

        .Parameters.Append .CreateParameter("@tiStatus", adTinyInt, adParamInput, , tiStatus)
        .Parameters.Append .CreateParameter("@iMark", adInteger, adParamInput, , iMark)

        .Parameters.Append .CreateParameter("@iUnderlyingID", adInteger, adParamInput, , iUnderlyingID)
        .Parameters.Append .CreateParameter("@iOptionRootID", adInteger, adParamInput, , IIf(iOptionRootID <> 0, iOptionRootID, Null))
        .Parameters.Append .CreateParameter("@iFutureRootID", adInteger, adParamInput, , IIf(iFutureRootID <> 0, iFutureRootID, Null))
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , IIf(iFutureID <> 0, iFutureID, Null))
        .Parameters.Append .CreateParameter("@vcImportID", adVarChar, adParamOutput, 20)
        .Parameters.Append .CreateParameter("@iOrderID", adInteger, adParamInput, , IIf(iOrderID <> 0, iOrderID, Null))
        .Parameters.Append .CreateParameter("@iExecStatus", adInteger, adParamInput, , IIf(iExecStatus <> 0, iExecStatus, Null))

        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iNewTradeID = ReadLng(.Parameters("@iNewTradeID"))
        dtNewTradeDate = ReadDate(.Parameters("@dtNewTradeDate"))
        vcImportID = ReadStr(.Parameters("@vcImportID"))

        usp_Trade_New = ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to create trade"
End Function

Public Function usp_Trade_UpdateIV(ByVal iTradeID As Long, ByVal fTradedIV As Double) As Boolean
    On Error GoTo EH
    Dim cmd As New ADODB.Command

    usp_Trade_UpdateIV = False
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_UpdateIV"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iTradeID", adInteger, adParamInput, , iTradeID)
        .Parameters.Append .CreateParameter("@fTradeIV", adDouble, adParamInput, , fTradedIV)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        usp_Trade_UpdateIV = True
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to update trade IV " & CStr(Err.Number) & "Description: " & Err.Description

End Function


Public Function usp_Trade_Update(ByVal iOldTradeID As Long, _
                                ByVal iContractID As Long, _
                                ByVal iQuantity As Long, _
                                ByVal fPrice As Double, _
                                ByVal tiIsBuy As Byte, _
                                ByVal iTraderID As Long, _
                                ByVal iStrategyID As Long, _
                                ByVal iBrokerID As Long, _
                                ByVal fUndBrokerCommission As Double, _
                                ByVal iClearingBrokerID As Long, _
                                ByVal fUndClearingBrokerCommission As Double, _
                                ByVal fTradedIV As Double, _
                                ByVal fSpotReference As Double, _
                                ByVal tiIsPosition As Byte, _
                                ByRef iNewTradeID As Long, _
                                ByRef dtNewTradeDate As Date, _
                                ByVal tiStatus As Byte, _
                                ByVal iMark As Long, _
                                ByVal iUnderlyingID As Long, _
                                ByVal iOptionRootID As Long, _
                                ByVal iFutureRootID As Long, _
                                ByVal iFutureID As Long, _
                                ByRef vcImportID As String, _
                                ByVal iOrderID As Long, _
                                ByVal iExecStatus As Long) As Long

    On Error GoTo EH
    Dim cmd As New ADODB.Command

    iNewTradeID = 0
    usp_Trade_Update = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_Update"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iOldTradeID", adInteger, adParamInput, , iOldTradeID)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , iContractID)
        .Parameters.Append .CreateParameter("@iQuantity", adInteger, adParamInput, , iQuantity)
        .Parameters.Append .CreateParameter("@fPrice", adDouble, adParamInput, , fPrice)
        .Parameters.Append .CreateParameter("@tiIsBuy", adTinyInt, adParamInput, , tiIsBuy)
        .Parameters.Append .CreateParameter("@iTraderID", adInteger, adParamInput, , IIf(iTraderID <> 0, iTraderID, Null))
        .Parameters.Append .CreateParameter("@iStrategyID", adInteger, adParamInput, , IIf(iStrategyID <> 0, iStrategyID, Null))
        .Parameters.Append .CreateParameter("@iBrokerID", adInteger, adParamInput, , IIf(iBrokerID <> 0, iBrokerID, Null))
        .Parameters.Append .CreateParameter("@fBrokerCommission", adDouble, adParamInput, , fUndBrokerCommission)
        .Parameters.Append .CreateParameter("@iClearingBrokerID", adInteger, adParamInput, , IIf(iClearingBrokerID <> 0, iClearingBrokerID, Null))
        .Parameters.Append .CreateParameter("@fClearingBrokerCommission", adDouble, adParamInput, , fUndClearingBrokerCommission)
        .Parameters.Append .CreateParameter("@fTradedIV", adDouble, adParamInput, , fTradedIV)
        .Parameters.Append .CreateParameter("@fSpotReference", adDouble, adParamInput, , fSpotReference)
        .Parameters.Append .CreateParameter("@tiIsPosition", adTinyInt, adParamInput, , tiIsPosition)
        
        .Parameters.Append .CreateParameter("@iNewTradeID", adInteger, adParamOutput)
        
        If dtNewTradeDate = 0 Then
            .Parameters.Append .CreateParameter("@dtNewTradeDate", adDBDate, adParamOutput)
        Else
            .Parameters.Append .CreateParameter("@dtNewTradeDate", adDBDate, adParamInputOutput, , dtNewTradeDate)
        End If
        
        .Parameters.Append .CreateParameter("@tiStatus", adTinyInt, adParamInput, , tiStatus)
        .Parameters.Append .CreateParameter("@iMark", adInteger, adParamInput, , iMark)

        .Parameters.Append .CreateParameter("@iUnderlyingID", adInteger, adParamInput, , iUnderlyingID)
        .Parameters.Append .CreateParameter("@iOptionRootID", adInteger, adParamInput, , IIf(iOptionRootID <> 0, iOptionRootID, Null))
        .Parameters.Append .CreateParameter("@iFutureRootID", adInteger, adParamInput, , IIf(iFutureRootID <> 0, iFutureRootID, Null))
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , IIf(iFutureID <> 0, iFutureID, Null))
        .Parameters.Append .CreateParameter("@vcImportID", adVarChar, adParamOutput, 20)
        .Parameters.Append .CreateParameter("@iOrderID", adInteger, adParamInput, , IIf(iOrderID <> 0, iOrderID, Null))
        .Parameters.Append .CreateParameter("@iExecStatus", adInteger, adParamInput, , IIf(iExecStatus <> 0, iExecStatus, Null))
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iNewTradeID = ReadLng(.Parameters("@iNewTradeID"))
        dtNewTradeDate = ReadDate(.Parameters("@dtNewTradeDate"))
        vcImportID = ReadStr(.Parameters("@vcImportID"))
        usp_Trade_Update = ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to update trade"
End Function

Public Function usp_Trade_Exec(ByVal tiExercise As Byte, _
                                ByVal iOptTradeID As Long, _
                                ByVal iOptQuantity As Long, _
                                ByVal tiOptIsBuy As Byte, _
                                ByVal tiOptStatus As Byte, _
                                ByVal fOptBrokerCommission As Double, _
                                ByVal fOptClearingBrokerCommission As Double, _
                                ByVal iUndQuantity As Long, _
                                ByVal fUndPrice As Double, _
                                ByVal tiUndIsBuy As Byte, _
                                ByVal tiUndStatus As Byte, _
                                ByVal fUndBrokerCommission As Double, _
                                ByVal fUndClearingBrokerCommission As Double, _
                                ByVal tiIsFutures As Byte, _
                                ByVal priceForIndex As Double, _
                                ByVal tiIsIndex As Byte, _
                                ByRef iNewOptSeqNum As Long, _
                                ByRef iNewOptTradeID As Long, _
                                ByRef dtNewOptTradeDate As Date, _
                                ByRef iNewUndSeqNum As Long, _
                                ByRef iNewUndTradeID As Long, _
                                ByRef dtNewUndTradeDate As Date) As Long
                                        
    On Error GoTo EH
    Dim cmd As New ADODB.Command

    iNewOptSeqNum = 0
    iNewOptTradeID = 0
    dtNewOptTradeDate = 0
    iNewUndSeqNum = 0
    iNewUndTradeID = 0
    dtNewUndTradeDate = 0
    usp_Trade_Exec = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_Exec"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@tiExercise", adTinyInt, adParamInput, , tiExercise)
        .Parameters.Append .CreateParameter("@iOptTradeID", adInteger, adParamInput, , iOptTradeID)
        .Parameters.Append .CreateParameter("@iOptQuantity", adInteger, adParamInput, , iOptQuantity)
        .Parameters.Append .CreateParameter("@tiOptIsBuy", adTinyInt, adParamInput, , tiOptIsBuy)
        .Parameters.Append .CreateParameter("@tiOptStatus", adTinyInt, adParamInput, , tiOptStatus)
        .Parameters.Append .CreateParameter("@fOptBrokerCommission", adDouble, adParamInput, , fOptBrokerCommission)
        .Parameters.Append .CreateParameter("@fOptClearingBrokerCommission", adDouble, adParamInput, , fOptClearingBrokerCommission)
        .Parameters.Append .CreateParameter("@iUndQuantity", adInteger, adParamInput, , iUndQuantity)
        .Parameters.Append .CreateParameter("@fUndPrice", adDouble, adParamInput, , fUndPrice)
        .Parameters.Append .CreateParameter("@tiUndIsBuy", adTinyInt, adParamInput, , tiUndIsBuy)
        .Parameters.Append .CreateParameter("@tiUndStatus", adTinyInt, adParamInput, , tiUndStatus)
        .Parameters.Append .CreateParameter("@fUndBrokerCommission", adDouble, adParamInput, , fUndBrokerCommission)
        .Parameters.Append .CreateParameter("@fUndClearingBrokerCommission", adDouble, adParamInput, , fUndClearingBrokerCommission)
        .Parameters.Append .CreateParameter("@tiIsFutures", adTinyInt, adParamInput, , tiIsFutures)
        .Parameters.Append .CreateParameter("@priceForIndexExer", adDouble, adParamInput, , priceForIndex)
        .Parameters.Append .CreateParameter("@tiIsIndex", adTinyInt, adParamInput, , tiIsIndex)

        .Parameters.Append .CreateParameter("@iNewOptSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewOptTradeID", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtNewOptTradeDate", adDBDate, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewUndSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewUndTradeID", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtNewUndTradeDate", adDBDate, adParamOutput)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iNewOptSeqNum = ReadLng(.Parameters("@iNewOptSeqNum"))
        iNewOptTradeID = ReadLng(.Parameters("@iNewOptTradeID"))
        dtNewOptTradeDate = ReadDate(.Parameters("@dtNewOptTradeDate"))
        iNewUndSeqNum = ReadLng(.Parameters("@iNewUndSeqNum"))
        iNewUndTradeID = ReadLng(.Parameters("@iNewUndTradeID"))
        dtNewUndTradeDate = ReadDate(.Parameters("@dtNewUndTradeDate"))
        
        usp_Trade_Exec = ReadLng(.Parameters("return"))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to exercise trade."
End Function


Public Function usp_Trade_SU_Exec(ByVal iOptTradeID As Long, _
                                ByVal iUnderlyingID As Long, _
                                ByVal iUndQuantity As Long, _
                                ByVal fUndPrice As Double, _
                                ByVal tiUndIsBuy As Byte, _
                                ByVal tiUndStatus As Byte, _
                                ByVal fUndBrokerCommission As Double, _
                                ByVal fUndClearingBrokerCommission As Double, _
                                ByRef iNewUndSeqNum As Long, _
                                ByRef iNewUndTradeID As Long, _
                                ByRef dtNewUndTradeDate As Date) As Long
                                        
    On Error GoTo EH
    Dim cmd As New ADODB.Command

    iNewUndSeqNum = 0
    iNewUndTradeID = 0
    dtNewUndTradeDate = 0
    usp_Trade_SU_Exec = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_SU_Exec"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iOptTradeID", adInteger, adParamInput, , iOptTradeID)
        .Parameters.Append .CreateParameter("@iUnderlyingID", adInteger, adParamInput, , iUnderlyingID)
        .Parameters.Append .CreateParameter("@iUndQuantity", adInteger, adParamInput, , iUndQuantity)
        .Parameters.Append .CreateParameter("@fUndPrice", adDouble, adParamInput, , fUndPrice)
        .Parameters.Append .CreateParameter("@tiUndIsBuy", adTinyInt, adParamInput, , tiUndIsBuy)
        .Parameters.Append .CreateParameter("@tiUndStatus", adTinyInt, adParamInput, , tiUndStatus)
        .Parameters.Append .CreateParameter("@fUndBrokerCommission", adDouble, adParamInput, , fUndBrokerCommission)
        .Parameters.Append .CreateParameter("@fUndClearingBrokerCommission", adDouble, adParamInput, , fUndClearingBrokerCommission)

        .Parameters.Append .CreateParameter("@iNewUndSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewUndTradeID", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtNewUndTradeDate", adDBDate, adParamOutput)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iNewUndSeqNum = ReadLng(.Parameters("@iNewUndSeqNum"))
        iNewUndTradeID = ReadLng(.Parameters("@iNewUndTradeID"))
        dtNewUndTradeDate = ReadDate(.Parameters("@dtNewUndTradeDate"))
        
        usp_Trade_SU_Exec = ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to exercise trade."
End Function

Public Function usp_Trade_Match(ByVal iOldManualTradeID As Long, _
                                ByVal iOldActualTradeID As Long, _
                                ByRef iOldManualSeqNum As Long, _
                                ByRef iOldActualSeqNum As Long, _
                                ByRef iNewActualSeqNum As Long, _
                                ByRef iNewActualTradeID As Long, _
                                ByRef dtNewActualTradeDate As Date) As Long

    On Error GoTo EH
    Dim cmd As New ADODB.Command

    iOldManualSeqNum = 0
    iOldActualSeqNum = 0
    iNewActualSeqNum = 0
    iNewActualTradeID = 0
    'dtNewActualTradeDate = 0
    usp_Trade_Match = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_Match"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iOldManualTradeID", adInteger, adParamInput, , IIf(iOldManualTradeID <> 0, iOldManualTradeID, Null))
        .Parameters.Append .CreateParameter("@iOldActualTradeID", adInteger, adParamInput, , IIf(iOldActualTradeID <> 0, iOldActualTradeID, Null))
        .Parameters.Append .CreateParameter("@iOldManualSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iOldActualSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewActualSeqNum", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iNewActualTradeID", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtNewActualTradeDate", adDBDate, adParamInputOutput, , IIf(dtNewActualTradeDate <> 0, dtNewActualTradeDate, Null))
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iOldManualSeqNum = ReadLng(.Parameters("@iOldManualSeqNum"))
        iOldActualSeqNum = ReadLng(.Parameters("@iOldActualSeqNum"))
        iNewActualSeqNum = ReadLng(.Parameters("@iNewActualSeqNum"))
        iNewActualTradeID = ReadLng(.Parameters("@iNewActualTradeID"))
        dtNewActualTradeDate = ReadDate(.Parameters("@dtNewActualTradeDate"))
        
        usp_Trade_Match = ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to match trade"
End Function

Public Function usp_Trade_PositionTransfer(ByVal iContractID As Long, _
                                        ByVal iQuantity As Long, _
                                        ByVal fPrice As Double, _
                                        ByVal tiStatus As Byte, _
                                        ByVal iTraderIDFrom As Long, _
                                        ByVal iStrategyIDFrom As Long, _
                                        ByVal iMarkFrom As Long, _
                                        ByVal tiIsBuyFrom As Byte, _
                                        ByVal iTraderIDTo As Long, _
                                        ByVal iStrategyIDTo As Long, _
                                        ByVal iMarkTo As Long, _
                                        ByVal tiIsBuyTo As Byte, _
                                        ByRef iSeqNumFrom As Long, _
                                        ByRef iTradeIDFrom As Long, _
                                        ByRef dtTradeDateFrom As Date, _
                                        ByRef iSeqNumTo As Long, _
                                        ByRef iTradeIDTo As Long, _
                                        ByRef dtTradeDateTo As Date, _
                                        ByVal iUnderlyingID As Long, _
                                        ByVal iOptionRootID As Long, _
                                        ByVal iFutureRootID As Long, _
                                        ByVal iFutureID As Long) As Long

    On Error GoTo EH
    Dim cmd As New ADODB.Command

    iSeqNumFrom = 0
    iTradeIDFrom = 0
    dtTradeDateFrom = 0
    iSeqNumTo = 0
    iTradeIDTo = 0
    dtTradeDateTo = 0
    
    usp_Trade_PositionTransfer = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_Trade_PositionTransfer"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , iContractID)
        .Parameters.Append .CreateParameter("@iQuantity", adInteger, adParamInput, , iQuantity)
        .Parameters.Append .CreateParameter("@fPrice", adDouble, adParamInput, , fPrice)
        .Parameters.Append .CreateParameter("@tiStatus", adTinyInt, adParamInput, , tiStatus)

        .Parameters.Append .CreateParameter("@iTraderIDFrom", adInteger, adParamInput, , iTraderIDFrom)
        .Parameters.Append .CreateParameter("@iStrategyIDFrom", adInteger, adParamInput, , IIf(iStrategyIDFrom <> 0, iStrategyIDFrom, Null))
        .Parameters.Append .CreateParameter("@iMarkFrom", adInteger, adParamInput, , iMarkFrom)
        .Parameters.Append .CreateParameter("@tiIsBuyFrom", adTinyInt, adParamInput, , tiIsBuyFrom)

        .Parameters.Append .CreateParameter("@iTraderIDTo", adInteger, adParamInput, , iTraderIDTo)
        .Parameters.Append .CreateParameter("@iStrategyIDTo", adInteger, adParamInput, , IIf(iStrategyIDTo <> 0, iStrategyIDTo, Null))
        .Parameters.Append .CreateParameter("@iMarkTo", adInteger, adParamInput, , iMarkTo)
        .Parameters.Append .CreateParameter("@tiIsBuyTo", adTinyInt, adParamInput, , tiIsBuyTo)

        .Parameters.Append .CreateParameter("@iSeqNumFrom", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iTradeIDFrom", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtTradeDateFrom", adDBDate, adParamOutput)

        .Parameters.Append .CreateParameter("@iSeqNumTo", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@iTradeIDTo", adInteger, adParamOutput)
        .Parameters.Append .CreateParameter("@dtTradeDateTo", adDBDate, adParamOutput)
        
        .Parameters.Append .CreateParameter("@iUnderlyingID", adInteger, adParamInput, , iUnderlyingID)
        .Parameters.Append .CreateParameter("@iOptionRootID", adInteger, adParamInput, , IIf(iOptionRootID <> 0, iOptionRootID, Null))
        .Parameters.Append .CreateParameter("@iFutureRootID", adInteger, adParamInput, , IIf(iFutureRootID <> 0, iFutureRootID, Null))
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , IIf(iFutureID <> 0, iFutureID, Null))
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        iSeqNumFrom = ReadLng(.Parameters("@iSeqNumFrom"))
        iTradeIDFrom = ReadLng(.Parameters("@iTradeIDFrom"))
        dtTradeDateFrom = ReadDate(.Parameters("@dtTradeDateFrom"))
        
        iSeqNumTo = ReadLng(.Parameters("@iSeqNumTo"))
        iTradeIDTo = ReadLng(.Parameters("@iTradeIDTo"))
        dtTradeDateTo = ReadDate(.Parameters("@dtTradeDateTo"))
        
        usp_Trade_PositionTransfer = ReadLng(.Parameters(0))
    End With
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to create trades for position transfer."
End Function

Public Function usp_MmContractForTradeByUnd_Get(ByVal iUnderlyingID As Long) As ADODB.Recordset
    Set usp_MmContractForTradeByUnd_Get = Execute(True, "usp_MmContractForTradeByUnd_Get", iUnderlyingID)
End Function

Public Function usp_IRPoint_Get(ByVal iCurveID As Variant, Optional ByVal IsGlobal As Variant, Optional ByVal iContractID As Variant) As ADODB.Recordset
    Set usp_IRPoint_Get = Execute(True, "usp_IRPoint_Get", iCurveID, IsGlobal, iContractID)
End Function

Public Function usp_IRCurve_Get(Optional iCurveID As Variant = Null) As ADODB.Recordset
    Set usp_IRCurve_Get = Execute(True, "usp_IRCurve_Get", iCurveID)
End Function

Public Function usp_RMScenario_Del(ByVal iScenarioID As Long) As Boolean
    usp_RMScenario_Del = (Execute(False, "usp_RMScenario_Del", iScenarioID) = 0)
End Function

Public Function usp_RMScenario_Get(ByVal iScenarioID As Variant) As ADODB.Recordset
    Set usp_RMScenario_Get = Execute(True, "usp_RMScenario_Get", iScenarioID)
End Function

Public Function usp_RMScenario_Save(ByVal iRMScenarioID As Variant, _
                                    ByVal vcScenarioName As String, _
                                    ByVal vcDescription As String, _
                                    ByVal iPoints1 As Long, _
                                    ByVal fStep1 As Double, _
                                    ByVal tiIsAbs1 As Integer, _
                                    ByVal tiType1 As Integer, _
                                    ByVal iPoints2 As Long, _
                                    ByVal fStep2 As Double, _
                                    ByVal tiIsAbs2 As Integer, _
                                    ByVal tiType2 As Integer, _
                                    ByVal tiVolaShift As Integer, _
                                    ByVal iHours As Integer, _
                                    ByVal iMinutes As Integer, _
                                    ByVal iShowChange As Integer, _
                                    ByVal iVolaSpecificShift As Long, _
                                    ByVal iSpotShift As Long, _
                                    ByVal iSpotSpecificShift As Long, _
                                    ByVal iCalcModel As Long, _
                                    ByVal iCorrIndex As Long, _
                                    ByVal vcParams As String) As Long
    
    Dim iResult As Long
    Dim iNewScenarioNumber As Long
    iResult = Execute(False, "usp_RMScenario_Save", iRMScenarioID, vcScenarioName, _
                                                    vcDescription, iPoints1, fStep1, tiIsAbs1, _
                                                     tiType1, iPoints2, fStep2, tiIsAbs2, tiType2, tiVolaShift, _
                                                     iHours, iMinutes, iShowChange, iVolaSpecificShift, _
                                                     iSpotShift, iSpotSpecificShift, iCalcModel, _
                                                     iCorrIndex, vcParams)
    If iResult > 0 Then
        usp_RMScenario_Save = iResult
    Else
        usp_RMScenario_Save = 0
    End If
End Function

'Public Function usp_ExpiryInCalendar_Get(ByVal iExpiryID As Variant, ByRef iExpCalendarID As Variant) As ADODB.Recordset
'    Set usp_ExpiryInCalendar_Get = Execute(True, "usp_ExpiryInCalendar_Get", iExpiryID, iExpCalendarID)
'End Function

Public Function usp_ExpiryVegaWeightSave_Save(ByVal dtExpiry As Date, _
                                  ByVal fVegaWeight As Double) As Long
    usp_ExpiryVegaWeightSave_Save = Execute(False, "usp_ExpiryVegaWeightSave_Save", dtExpiry, fVegaWeight)
End Function

Public Function usp_DataInfo_Get(Optional ByVal lDataInfoID As Variant = Null, _
                                Optional ByVal sKeyName As Variant = Null) As ADODB.Recordset
    Set usp_DataInfo_Get = Execute(True, "usp_DataInfo_Get", lDataInfoID, sKeyName)
End Function

Public Function usp_WtdVegaMaturity_Get() As ADODB.Recordset
    Set usp_WtdVegaMaturity_Get = Execute(True, "usp_WtdVegaMaturity_Get")
End Function

Public Function usp_WtdVegaMaturity_Save(ByVal nMaturity As Long) As Long
    usp_WtdVegaMaturity_Save = Execute(False, "usp_WtdVegaMaturity_Save", nMaturity)
End Function

Public Function usp_MmOptionRoot_Get(ByVal iUnderlyingID As Long) As ADODB.Recordset
    Set usp_MmOptionRoot_Get = Execute(True, "usp_MmOptionRoot_Get", IIf(iUnderlyingID <> 0, iUnderlyingID, Null))
End Function

Public Function usp_MmFutureRoot_Get(ByRef iUnderlyingID&) As ADODB.Recordset
    Set usp_MmFutureRoot_Get = Execute(True, "usp_MmFutureRoot_Get", IIf(iUnderlyingID <> 0, iUnderlyingID, Null))
End Function

Public Function usp_MmFuture_Get(ByVal iFutureRootID&) As ADODB.Recordset
    Set usp_MmFuture_Get = Execute(True, "usp_MmFuture_Get", IIf(iFutureRootID <> 0, iFutureRootID, Null))
End Function

Public Function usp_MmSyntheticRootParams_Get(ByVal iOptionRootID&) As ADODB.Recordset
    Set usp_MmSyntheticRootParams_Get = Execute(True, "usp_MmSyntheticRootParams_Get", IIf(iOptionRootID <> 0, iOptionRootID, Null))
End Function

Public Function usp_MmSyntheticIndexBeta_Get() As ADODB.Recordset
    Set usp_MmSyntheticIndexBeta_Get = Execute(True, "usp_MmSyntheticIndexBeta_Get")
End Function

Public Function usp_ActTrdEditable_Get() As ADODB.Recordset
    Set usp_ActTrdEditable_Get = Execute(True, "usp_ActTrdEditable_Get")
End Function

Public Function usp_ActTrdEditable_Save(ByVal nValue As Long) As ADODB.Recordset
    Execute False, "usp_ActTrdEditable_Save", nValue
End Function

Public Function usp_CommissionLowPremiumLimit_Get() As Double
    usp_CommissionLowPremiumLimit_Get = ReadDbl(Execute(True, "usp_CommissionLowPremiumLimit_Get")("fCommissionLowPremiumLimit"))
End Function

Public Function usp_CorrelationDH_Get(ByVal nContract1 As Long, _
                                    ByVal nContract2 As Long, ByVal nContractType1 As Long, _
                                    ByVal nContractType2 As Long, ByRef fPriceCorr As Double, _
                                    ByRef fVolaCorr As Double, ByRef dtUpdateDate As Date) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    usp_CorrelationDH_Get = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_CorrelationDH_Get"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID1", adInteger, adParamInput, , nContract1)
        .Parameters.Append .CreateParameter("@iContractID2", adInteger, adParamInput, , nContract2)
        .Parameters.Append .CreateParameter("@iContractTypeID1", adInteger, adParamInput, , nContractType1)
        .Parameters.Append .CreateParameter("@iContractTypeID2", adInteger, adParamInput, , nContractType2)
        .Parameters.Append .CreateParameter("@fCorrelationPrice", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@fCorrelationVola", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@dtUpdateDate", adDBDate, adParamOutput)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        fPriceCorr = ReadDbl(.Parameters("@fCorrelationPrice"))
        fVolaCorr = ReadDbl(.Parameters("@fCorrelationVola"))
        dtUpdateDate = ReadDate(.Parameters("@dtUpdateDate"))
        
        usp_CorrelationDH_Get = ReadLng(.Parameters(0))
    End With
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to get correlation IV.com data"
End Function

Public Function usp_CorrelationDH_Save(ByVal nContract1 As Long, _
                                    ByVal nContract2 As Long, ByVal nContractType1 As Long, _
                                    ByVal nContractType2 As Long, ByVal fPriceCorr As Double, _
                                    ByVal fVolaCorr As Double) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    usp_CorrelationDH_Save = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_CorrelationDH_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID1", adInteger, adParamInput, , nContract1)
        .Parameters.Append .CreateParameter("@iContractID2", adInteger, adParamInput, , nContract2)
        .Parameters.Append .CreateParameter("@iContractTypeID1", adInteger, adParamInput, , nContractType1)
        .Parameters.Append .CreateParameter("@iContractTypeID2", adInteger, adParamInput, , nContractType2)
        .Parameters.Append .CreateParameter("@fCorrelationPrice", adDouble, adParamInput, , fPriceCorr)
        .Parameters.Append .CreateParameter("@fCorrelationVola", adDouble, adParamInput, , fVolaCorr)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        usp_CorrelationDH_Save = ReadLng(.Parameters(0))
    End With
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to save correlation IV.com data"
End Function

Public Function usp_DataHistoryDH_Get(ByVal nContract As Long, _
                                    ByVal nContractType As Long, ByRef fIV As Double, _
                                    ByRef fHV As Double, ByRef fVolOfVol As Double, _
                                    ByRef fVega As Double, ByRef dtUpdateDate As Date) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    usp_DataHistoryDH_Get = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_DataHistoryDH_Get"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , nContract)
        .Parameters.Append .CreateParameter("@iContractTypeID", adInteger, adParamInput, , nContractType)
        .Parameters.Append .CreateParameter("@fCurrentIV", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@fCurrentHV", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@fCurrentVolOfVol", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@fCurrentVega", adDouble, adParamOutput)
        .Parameters.Append .CreateParameter("@dtUpdateDate", adDBDate, adParamOutput)
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        fIV = ReadDbl(.Parameters("@fCurrentIV"))
        fHV = ReadDbl(.Parameters("@fCurrentHV"))
        fVolOfVol = ReadDbl(.Parameters("@fCurrentVolOfVol"))
        fVega = ReadDbl(.Parameters("@fCurrentVega"))
        dtUpdateDate = ReadDate(.Parameters("@dtUpdateDate"))
        
        usp_DataHistoryDH_Get = ReadLng(.Parameters(0))
    End With
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to get historical IV.com data"
End Function

Public Function usp_DataHistoryDH_Save(ByVal nContract As Long, ByVal nContractType As Long, _
                                    ByVal fIV As Double, ByVal fHV As Double, _
                                    ByVal fVolOfVol As Double, ByVal fVega As Double) As Long
    On Error GoTo EH
    Dim cmd As New ADODB.Command
    
    usp_DataHistoryDH_Save = 0
    
    With cmd
        Set .ActiveConnection = Connection
        .CommandTimeout = 120

        .CommandType = adCmdStoredProc
        .CommandText = "usp_DataHistoryDH_Save"

        .Parameters.Append .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append .CreateParameter("@iContractID", adInteger, adParamInput, , nContract)
        .Parameters.Append .CreateParameter("@iContractTypeID", adInteger, adParamInput, , nContractType)
        .Parameters.Append .CreateParameter("@fCurrentIV", adDouble, adParamInput, , IIf(fIV > BAD_DOUBLE_VALUE, fIV, Null))
        .Parameters.Append .CreateParameter("@fCurrentHV", adDouble, adParamInput, , IIf(fHV > BAD_DOUBLE_VALUE, fHV, Null))
        .Parameters.Append .CreateParameter("@fCurrentVolOfVol", adDouble, adParamInput, , IIf(fVolOfVol > BAD_DOUBLE_VALUE, fVolOfVol, Null))
        .Parameters.Append .CreateParameter("@fCurrentVega", adDouble, adParamInput, , IIf(fVega > BAD_DOUBLE_VALUE, fVega, Null))
        
        .Execute

        If .ActiveConnection.Errors.Count > 0 Then
            Err.Raise .ActiveConnection.Errors.Item(0).Number
        End If

        usp_DataHistoryDH_Save = ReadLng(.Parameters(0))
    End With
    
    Exit Function
EH:
    gCmn.ErrorHandler "Fail to save historical IV.com data"
End Function

Public Function usp_UndPriceToleranceValue_Get() As Double
    usp_UndPriceToleranceValue_Get = Execute(True, "usp_UndPriceToleranceValue_Get")("fUndPriceToleranceValue")
End Function

Public Function usp_UndPriceToleranceValue_Save(ByVal fUndPriceToleranceValue As Double) As Long
    usp_UndPriceToleranceValue_Save = Execute(False, "usp_UndPriceToleranceValue_Save", fUndPriceToleranceValue)
End Function

Public Function usp_PriceRoundingType_Get() As Byte
    usp_PriceRoundingType_Get = Execute(True, "usp_PriceRoundingType_Get")("tiPriceRoundingType")
End Function
Public Function usp_IvUnderlyingHistoricalPrices_Get(ByVal undSymbol As String, ByVal dt1From As Date, ByVal dt1To As Date) As ADODB.Recordset
    Set usp_IvUnderlyingHistoricalPrices_Get = IvExecute(True, "usp_GetUnderlyingHistoricalPrices", undSymbol, dt1From, dt1To)
End Function
Public Function usp_PriceRoundingType_Save(ByVal tiPriceRoundingType As Byte) As Long
    usp_PriceRoundingType_Save = Execute(False, "usp_PriceRoundingType_Save", tiPriceRoundingType)
End Function

Public Function usp_PriceProfile_Get(ByVal iPriceProfileID As Long) As ADODB.Recordset
    Set usp_PriceProfile_Get = Execute(True, "usp_PriceProfile_Get", IIf(iPriceProfileID = 0, Null, iPriceProfileID))
End Function

Public Function usp_PriceProfile_Save(ByVal iPriceProfileID As Long, ByVal vcPriceProfileName As String, _
                                    ByVal vcPriceProfileDesc As String, ByVal tiIsOptionProfile As Byte, _
                                    ByVal tiAlwaysLast As Byte, ByVal tiMidmarketPnL As Byte, _
                                    ByVal tiSinglePriceRule As Byte, ByVal tiBothPriceRule As Byte, ByVal tiUseZeroBidRule As Byte) As Long
                                    
    usp_PriceProfile_Save = Execute(False, "usp_PriceProfile_Save", IIf(iPriceProfileID = 0, Null, iPriceProfileID), _
                                    vcPriceProfileName, vcPriceProfileDesc, tiIsOptionProfile, _
                                    tiAlwaysLast, tiMidmarketPnL, tiSinglePriceRule, tiBothPriceRule, tiUseZeroBidRule)
End Function

Public Function usp_PriceProfile_Del(ByVal iPriceProfileID As Long) As Long
    usp_PriceProfile_Del = Execute(False, "usp_PriceProfile_Del", iPriceProfileID)
End Function

Public Function usp_PriceProfile_CheckDefaults() As Long
    usp_PriceProfile_CheckDefaults = Execute(False, "usp_PriceProfile_CheckDefaults")
End Function

Public Function usp_ReportsUnderlyingOptions_Get(ByVal iUnderlyingID As Long, Optional ByVal dtMinExpDate As Date = 0) As ADODB.Recordset
    Set usp_ReportsUnderlyingOptions_Get = Execute(True, "usp_ReportsUnderlyingOptions_Get", iUnderlyingID, IIf(dtMinExpDate = 0, Null, dtMinExpDate))
End Function

Public Function usp_UnderlyingOptionsForOrder_Get(ByVal iUnderlyingID As Long) As ADODB.Recordset
    Set usp_UnderlyingOptionsForOrder_Get = Execute(True, "usp_UnderlyingOptionsForOrder_Get", iUnderlyingID)
End Function

Public Function usp_OrderByTrader_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_OrderByTrader_Get = Execute(True, "usp_OrderByTrader_Get", IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_OrderExec_Get(ByVal iOrderID As Long, ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_OrderExec_Get = Execute(True, "usp_OrderExec_Get", _
                            IIf(iOrderID <> 0, iOrderID, Null), IIf(iTraderID <> 0, iTraderID, Null))
End Function

Public Function usp_OrderLastSeqNum_Get() As Long
    Dim rs As ADODB.Recordset
    usp_OrderLastSeqNum_Get = 0
    Set rs = Execute(True, "usp_OrderLastSeqNum_Get")
    If Not rs.EOF Then usp_OrderLastSeqNum_Get = ReadLng(rs!iSeqNum)
    Set rs = Nothing
End Function

Public Function usp_OrderExecLastSeqNum_Get() As Long
    Dim rs As ADODB.Recordset
    usp_OrderExecLastSeqNum_Get = 0
    Set rs = Execute(True, "usp_OrderExecLastSeqNum_Get")
    If Not rs.EOF Then usp_OrderExecLastSeqNum_Get = ReadLng(rs!iSeqNum)
    Set rs = Nothing
End Function

Public Function usp_OrderExecDest_Get(ByVal iDestID As Long) As ADODB.Recordset
    Set usp_OrderExecDest_Get = Execute(True, "usp_OrderExecDest_Get", IIf(iDestID <> 0, iDestID, Null))
End Function

Public Function usp_OrderSeqByTrader_Get(ByVal iTraderID As Long, _
                                        ByVal iMinSeqNum As Long, _
                                        ByVal iMaxSeqNum As Long) As ADODB.Recordset
                                        
    Set usp_OrderSeqByTrader_Get = Execute(True, "usp_OrderSeqByTrader_Get", IIf(iTraderID <> 0, iTraderID, Null), _
                                                                IIf(iMinSeqNum <> 0, iMinSeqNum, Null), _
                                                                IIf(iMaxSeqNum <> 0, iMaxSeqNum, Null))
End Function

Public Function usp_OrderExecSeq_Get(ByVal iOrderID As Long, _
                                        ByVal iTraderID As Long, _
                                        ByVal iMinSeqNum As Long, _
                                        ByVal iMaxSeqNum As Long) As ADODB.Recordset
                                        
    Set usp_OrderExecSeq_Get = Execute(True, "usp_OrderExecSeq_Get", _
                            IIf(iOrderID <> 0, iOrderID, Null), IIf(iTraderID <> 0, iTraderID, Null), _
                                                                IIf(iMinSeqNum <> 0, iMinSeqNum, Null), _
                                                                IIf(iMaxSeqNum <> 0, iMaxSeqNum, Null))
End Function

Public Function usp_TntCounterParty_Get(ByVal iCptID As Long) As ADODB.Recordset
    Set usp_TntCounterParty_Get = Execute(True, "usp_TntCounterParty_Get", IIf(iCptID <> 0, iCptID, Null))
End Function

Public Function usp_TntCounterParty_Save(ByVal iCptID As Long, _
                                    Optional ByRef vcName As Variant = Null, _
                                    Optional ByRef vcDesc As Variant = Null) As Long
                                    
    usp_TntCounterParty_Save = Execute(False, "usp_TntCounterParty_Save", IIf(iCptID = 0, Null, iCptID), _
                                    vcName, vcDesc)
End Function

Public Function usp_FixOldOrder_Del() As Long
    usp_FixOldOrder_Del = Execute(False, "usp_FixOldOrder_Del")
End Function

Public Function usp_TntOption_Get(ByVal iOptRootID As Long, ByVal tiIsCall As Byte, _
                                ByVal fStrike As Double, ByVal dtExpiry As Date) As ADODB.Recordset
    Set usp_TntOption_Get = Execute(True, "usp_TntOption_Get", iOptRootID, tiIsCall, fStrike, dtExpiry)
End Function

Public Function usp_TntCardByTrader_Get(ByVal iTraderID As Long) As ADODB.Recordset
    Set usp_TntCardByTrader_Get = Execute(True, "usp_TntCardByTrader_Get", iTraderID)
End Function

Public Function usp_TntCard_Update(ByVal iTraderID As Long, ByVal iCardID As Long, ByVal tiStatus As Byte) As Long
    usp_TntCard_Update = Execute(False, "usp_TntCard_Update", iTraderID, iCardID, tiStatus)
End Function

Public Function usp_TntCard_Del(ByVal iTraderID As Long, ByVal iCardID As Long) As Long
    usp_TntCard_Del = Execute(False, "usp_TntCard_Del", iTraderID, iCardID)
End Function

Public Function usp_TntCardOld_Del(ByVal iTraderID As Long, Optional ByVal dtCardDate As Date = 0#) As Long
    usp_TntCardOld_Del = Execute(False, "usp_TntCardOld_Del", iTraderID, IIf(dtCardDate > 0#, dtCardDate, Null))
End Function

Public Function usp_TntCardOut_New(ByVal iTraderID As Long, _
                                ByVal iCptID As Long, _
                                ByVal dtTradeDate As Date, _
                                ByVal tiIsBuy As Byte, _
                                ByVal iQty As Long, _
                                ByVal fPrice As Double, _
                                ByVal vcPoeID As String, _
                                ByVal tiStatus As Byte, _
                                ByVal vcAccount As String, _
                                ByVal vcExecutingFirm As String, _
                                ByVal vcCMTA As String, _
                                ByVal vcQuantifier As String, _
                                ByVal vcContraFirm As String, _
                                ByVal vcSpreadIndicator As String, _
                                ByVal vcOptionalData As String, _
                                ByVal iUndID As Long, _
                                ByVal iRootID As Long, _
                                ByVal vcRootSymbol As String, _
                                ByVal iOptID As Long, _
                                ByVal vcOptSymbol As String, _
                                ByVal dtOptExpiry As Date, _
                                ByVal fOptStrike As Double, _
                                ByVal tiOptIsCall As Byte, _
                                ByVal iMaxCardID As Long _
                                ) As ADODB.Recordset
    Set usp_TntCardOut_New = Execute(True, "usp_TntCardOut_New", iTraderID, _
                                                                iCptID, _
                                                                dtTradeDate, _
                                                                tiIsBuy, _
                                                                iQty, _
                                                                fPrice, _
                                                                vcPoeID, _
                                                                tiStatus, _
                                                                vcAccount, _
                                                                vcExecutingFirm, _
                                                                vcCMTA, _
                                                                vcQuantifier, _
                                                                vcContraFirm, _
                                                                vcSpreadIndicator, _
                                                                vcOptionalData, _
                                                                iUndID, _
                                                                iRootID, _
                                                                vcRootSymbol, _
                                                                iOptID, _
                                                                vcOptSymbol, _
                                                                dtOptExpiry, _
                                                                fOptStrike, _
                                                                tiOptIsCall, _
                                                                iMaxCardID)
End Function

Public Function usp_TntCardIn_New(ByVal iTraderID As Long, _
                                ByVal iCptID As Long, _
                                ByVal dtTradeDate As Date, _
                                ByVal tiIsBuy As Byte, _
                                ByVal iQty As Long, _
                                ByVal fPrice As Double, _
                                ByVal vcPoeID As String, _
                                ByVal tiStatus As Byte, _
                                ByVal vcAccount As String, _
                                ByVal vcExecutingFirm As String, _
                                ByVal vcCMTA As String, _
                                ByVal vcQuantifier As String, _
                                ByVal vcContraFirm As String, _
                                ByVal vcSpreadIndicator As String, _
                                ByVal vcOptionalData As String, _
                                ByVal iUndID As Long, _
                                ByVal iRootID As Long, _
                                ByVal vcRootSymbol As String, _
                                ByVal iOptID As Long, _
                                ByVal vcOptSymbol As String, _
                                ByVal dtOptExpiry As Date, _
                                ByVal fOptStrike As Double, _
                                ByVal tiOptIsCall As Byte, _
                                ByVal iMaxCardID As Long _
                                ) As ADODB.Recordset
    Set usp_TntCardIn_New = Execute(True, "usp_TntCardIn_New", iTraderID, _
                                                                iCptID, _
                                                                dtTradeDate, _
                                                                tiIsBuy, _
                                                                iQty, _
                                                                fPrice, _
                                                                vcPoeID, _
                                                                tiStatus, _
                                                                vcAccount, _
                                                                vcExecutingFirm, _
                                                                vcCMTA, _
                                                                vcQuantifier, _
                                                                vcContraFirm, _
                                                                vcSpreadIndicator, _
                                                                vcOptionalData, _
                                                                iUndID, _
                                                                iRootID, _
                                                                vcRootSymbol, _
                                                                iOptID, _
                                                                vcOptSymbol, _
                                                                dtOptExpiry, _
                                                                fOptStrike, _
                                                                tiOptIsCall, _
                                                                iMaxCardID)
End Function

Public Function usp_MmStrategy_Save(ByVal iStrategyID&, _
                                    ByRef vcStrategyName As Variant, _
                                    ByRef vcDescription As Variant) As Long
    usp_MmStrategy_Save = Execute(False, "usp_MmStrategy_Save", IIf(iStrategyID <> 0, iStrategyID, Null), vcStrategyName, vcDescription)
End Function

Public Function usp_MmHedgeData_Get() As ADODB.Recordset
    Set usp_MmHedgeData_Get = Execute(True, "usp_MmHedgeData_Get")
End Function

Public Function usp_MmHedgeData_Save&(ByVal iTraderID&, ByVal iStrategyID&, ByVal iContractID&, ByVal tiState As Byte)
    usp_MmHedgeData_Save = Execute(False, "usp_MmHedgeData_Save", _
                                        IIf(iTraderID <> 0, iTraderID, Null), _
                                        IIf(iStrategyID <> 0, iStrategyID, Null), _
                                        IIf(iContractID <> 0, iContractID, Null), _
                                        tiState)
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

        usp_AspUserStatus_Get = ReadLng(.Parameters("@iStatus"))
    End With
End Function


Public Function usp_MmCustomDividend_Get(Optional ByVal iUndID As Long = 0) As ADODB.Recordset
    Set usp_MmCustomDividend_Get = ExecuteLBO(True, "usp_MmCustomDividend_Get", IIf(iUndID <> 0, iUndID, Null))
End Function
Public Function usp_MmEtsCustomDividend_Get(Optional ByVal iUndID As Long = 0, Optional ByVal tiIsLoadForIndex As Variant = Null) As ADODB.Recordset
    Set usp_MmEtsCustomDividend_Get = ExecuteLBO(True, "usp_MmEtsCustomDividend_Get", IIf(iUndID <> 0, iUndID, Null), tiIsLoadForIndex)
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
    Dim S$
    
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
    
    S = "exec " & sCommandText
    nLBnd = LBound(arrParams)
    nUBnd = UBound(arrParams)
    For i = nLBnd To nUBnd
        S = S & " " & ParseParam(arrParams(i)) & ","
    Next
    If Right$(S, 1) = "," Then
        S = Left$(S, Len(S) - 1)
    End If
    
    Err.Raise nNumber, sSource, S & vbCrLf & sDescription, sHelpFile, nHelpContext
End Function


Public Function usp_OptionRoot_Save( _
        iRootID As Long _
        , tiIsFitEnabled As Variant _
) As Long
    usp_OptionRoot_Save = Execute(False, "usp_OptionRoot_Save" _
        , iRootID _
        , Null _
        , Null _
        , tiIsFitEnabled _
)
End Function

Public Function usp_RegUserAction_Save( _
          LogOnOut As Variant _
        , DBLogin As Variant _
        , ServerPath As Variant _
        , CompanyName As Variant _
) As Long
On Error Resume Next
    usp_RegUserAction_Save = Execute(False, "usp_RegUserAction_Save" _
        , Now _
        , LogOnOut _
        , DBLogin _
        , ServerPath _
        , CompanyName _
)
End Function

Public Function usp_MmExpiredPositions_Check_Simple(iDaysTilExp As Integer) As ADODB.Recordset
    Set usp_MmExpiredPositions_Check_Simple = Execute(True, "usp_MmExpiredPositions_Check", -iDaysTilExp)
End Function

Public Function usp_Underlying20dHVola_Get(ByVal vcSymbolName As Variant) As ADODB.Recordset
    Set usp_Underlying20dHVola_Get = Execute(True, "usp_Underlying20dHVola_Get", vcSymbolName)
End Function

Public Function usp_MmIndexUnderlyingBeta_Eod_Get(ByVal iContractID As Variant, ByVal iHedgeSymbolID As Variant) As ADODB.Recordset
    Set usp_MmIndexUnderlyingBeta_Eod_Get = Execute(True, "usp_MmIndexUnderlyingBeta_Eod_Get", iContractID, iHedgeSymbolID)
End Function

Public Function usp_HedgeSymbolBeta_Load(ByVal iHedgeSymbolID As Long) As Long
    usp_HedgeSymbolBeta_Load = Execute(False, "usp_HedgeSymbolBeta_Load", iHedgeSymbolID)
End Function

Public Function usp_IndexUnderlyingBeta_Get(ByVal iHedgeSymbolID As Long) As ADODB.Recordset
    Set usp_IndexUnderlyingBeta_Get = Execute(True, "usp_IndexUnderlyingBeta_Get", iHedgeSymbolID)
End Function

Public Function usp_IsManualVol_Save(ByVal iContractID As Long, ByVal bIsManualActivePrice As Byte) As Long
On Error Resume Next
    usp_IsManualVol_Save = Execute(False, "usp_IsManualVol_Save", iContractID, bIsManualActivePrice)
End Function

Public Function usp_MmManualPrice_Save(ByVal iContractID As Long, ByVal manualActivePrice As Double) As Long
On Error Resume Next
    usp_MmManualPrice_Save = Execute(False, "usp_MmManualPrice_Save", iContractID, manualActivePrice)
End Function

Public Function usp_MmManualPrice_Del(ByVal iContractID As Long) As Long
On Error Resume Next
    usp_MmManualPrice_Del = Execute(False, "usp_MmManualPrice_Del", iContractID)
End Function

Public Function usp_MmManualPrice_Get() As ADODB.Recordset
    Set usp_MmManualPrice_Get = Execute(True, "usp_MmManualPrice_Get")
End Function

Public Function usp_AssetGroupElement_Save(ByVal iHeadId As Long, ByVal iElementID As Long, ByVal bUseHead As Long, ByVal fCoeff As Double) As Long
On Error Resume Next
    usp_AssetGroupElement_Save = Execute(False, "usp_AssetGroupElement_Save", iHeadId, iElementID, bUseHead, fCoeff)
End Function

Public Function usp_IRPoint_Del(ByVal iIRpointID As Long, ByVal iCurveID As Long) As Long
    usp_IRPoint_Del = Execute(False, "usp_IRPoint_Del", iIRpointID, iCurveID)
End Function

Public Function usp_IRPoint_Save(ByVal iIRpointID As Long, _
                                 ByVal iCurveID As Long, _
                                 ByVal iPeriodTypeID As Long, _
                                 ByVal iNum As Long, _
                                 ByVal fShortRate As Double, _
                                 ByVal fLongRate As Double, _
                                 ByVal iContractID As Long) As Long
    usp_IRPoint_Save = Execute(False, "usp_IRPoint_Save_CV", IIf(iIRpointID <> BAD_LONG_VALUE, iIRpointID, Null), _
                                                          iCurveID, _
                                                          iPeriodTypeID, _
                                                          iNum, _
                                                          fShortRate, _
                                                          Null, _
                                                          fLongRate, _
                                                          Null, _
                                                          iContractID)
End Function

Public Function usp_IRPoint_Update(ByVal iPeriodTypeID As Long, _
                                   ByVal iNum As Long, _
                                   ByVal fShortRate As Double, _
                                   ByVal fLongRate As Double, _
                                   ByVal iContractID As Long) As Long
    usp_IRPoint_Update = Execute(False, "usp_IRPoint_Update_CV", iPeriodTypeID, _
                                                              iNum, _
                                                              fShortRate, _
                                                              fLongRate, _
                                                              Null, _
                                                              iContractID)
End Function

'Public Function usp_ProjectionShedule_Get(Optional ByVal vcUserName As String = STR_NA, _
'                                          Optional ByVal iSheduleID As Long = BAD_LONG_VALUE) As ADODB.Recordset
'    Set usp_ProjectionShedule_Get = Execute(True, "usp_ProjectionShedule_Get", IIf(vcUserName <> STR_NA, vcUserName, Null), _
'                                                                               IIf(iSheduleID <> BAD_LONG_VALUE, iSheduleID, Null))
'End Function
'
'Public Function usp_ProjectionShedule_Del(ByVal iSheduleID As Long) As Long
'    usp_ProjectionShedule_Del = Execute(False, "usp_ProjectionShedule_Del", iSheduleID)
'End Function
'
'Public Function usp_ProjectionShedule_Save(ByVal vcUserName As String, _
'                                           Optional ByVal iSheduleID As Long = BAD_LONG_VALUE, _
'                                           Optional ByVal vcProjectionFile As String = STR_NA, _
'                                           Optional ByVal vcLayoutFile As String = STR_NA, _
'                                           Optional ByVal tiEnabled As Boolean = False, _
'                                           Optional ByVal dtStartTime As Date, _
'                                           Optional ByVal tiDaily As Boolean = False) As Long
'    usp_ProjectionShedule_Save = Execute(False, "usp_ProjectionShedule_Save", IIf(iSheduleID <> BAD_LONG_VALUE, iSheduleID, Null), _
'                                                                              vcUserName, _
'                                                                              vcProjectionFile, _
'                                                                              vcLayoutFile, _
'                                                                              tiEnabled, _
'                                                                              dtStartTime, _
'                                                                              tiDaily)
'End Function
'
'Public Function usp_ProjectionShedule_CalcComplete(ByVal iSheduleID As Long, ByVal dtCalcDate As Date, ByVal sFileName As String) As Long
'    usp_ProjectionShedule_CalcComplete = Execute(False, "usp_ProjectionShedule_CalcComplete", iSheduleID, dtCalcDate, sFileName)
'End Function

Public Function usp_BR_Reports_Get(Optional ByVal iReportID As Long = BAD_LONG_VALUE) As ADODB.Recordset
    Set usp_BR_Reports_Get = Execute(True, "usp_BR_Reports_Get", IIf(iReportID <> BAD_LONG_VALUE, iReportID, Null))
End Function

Public Function usp_BR_Report_Del(ByVal iReportID As Long) As Long
    usp_BR_Report_Del = Execute(False, "usp_BR_Report_Del", iReportID)
End Function

Public Function usp_BR_Report_Save(ByVal vcUserName As String, _
                                   ByVal tiStatus As Boolean, _
                                   ByVal vcLayoutFile As String, _
                                   ByVal iGroupID As Long, _
                                   ByVal iTraderID As Long, _
                                   ByVal iStrategyID As Long, _
                                   ByVal iTraderGroupID As Long, _
                                   ByVal tiPositions As Boolean, _
                                   ByVal vcResultFilePath As String, _
                                   ByVal dtLastRun As Date, _
                                   ByVal vcResultFile As String, _
                                   ByVal vcFileNameHeader As String, _
                                   ByVal iReportFormat As Long, _
                                   Optional ByVal iReportID As Long = BAD_LONG_VALUE) As Long
    usp_BR_Report_Save = Execute(False, "usp_BR_Report_Save", IIf(iReportID <> BAD_LONG_VALUE, iReportID, Null), _
                                                                vcUserName, _
                                                                tiStatus, _
                                                                vcLayoutFile, _
                                                                iGroupID, _
                                                                iTraderID, _
                                                                iStrategyID, _
                                                                iTraderGroupID, _
                                                                tiPositions, _
                                                                vcResultFilePath, _
                                                                dtLastRun, _
                                                                vcResultFile, _
                                                                vcFileNameHeader, _
                                                                iReportFormat)
End Function

Public Function usp_BR_Report_Complete(ByVal iReportID As Long, ByVal dtDate As Date, ByVal vcFileName As String) As Long
    usp_BR_Report_Complete = Execute(False, "usp_BR_Report_Complete", iReportID, dtDate, vcFileName)
End Function

Public Function usp_BR_Shedule_Get() As ADODB.Recordset
    Set usp_BR_Shedule_Get = Execute(True, "usp_BR_Shedule_Get")
End Function

Public Function usp_BR_Shedule_Del(ByVal iID As Long) As Long
    usp_BR_Shedule_Del = Execute(False, "usp_BR_Shedule_Del", iID)
End Function

Public Function usp_BR_Shedule_Save(ByVal dtTime As Date, _
                                    ByVal tiStatus As Boolean, _
                                   Optional ByVal iID As Long = BAD_LONG_VALUE) As Long
    usp_BR_Shedule_Save = Execute(False, "usp_BR_Shedule_Save", IIf(iID <> BAD_LONG_VALUE, iID, Null), _
                                                                dtTime, _
                                                                tiStatus)
End Function

Public Function usp_BR_Shedule_Complete(ByVal iSheduleID As Long, ByVal dtDate As Date) As Long
    usp_BR_Shedule_Complete = Execute(False, "usp_BR_Shedule_Complete", iSheduleID, dtDate)
End Function

Public Function usp_cvContext_Get(ByVal iID As Long, ByVal iInfoLevel As Long) As ADODB.Recordset
    Set usp_cvContext_Get = Execute(True, "usp_cvContext_Get", iID, iInfoLevel)
End Function

Public Function usp_cvContextID_Get(ByVal strContextName As String) As Long
    usp_cvContextID_Get = Execute(False, "usp_cvContextID_Get", strContextName)
End Function

Public Function usp_cvCV_Del(ByVal iID As Long) As Long
    usp_cvCV_Del = Execute(False, "usp_cvCV_Del", iID)
End Function
