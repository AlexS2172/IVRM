VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "clsDataSource"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = True
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Enum enumSPType
    SP_Recorset
    SP_ReturnValue
    SP_OutputParam
End Enum

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetConnection() As ADODB.Connection
    Set GetConnection = CN
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function OpenRSDynamic(ByVal Sql As String) As ADODB.Recordset
Dim cm As ADODB.Command
    Set cm = New ADODB.Command
    With cm
        Set .ActiveConnection = GetConnection
        .CommandText = Sql
        .CommandType = adCmdText
    End With
    Set OpenRSDynamic = New ADODB.Recordset
    With OpenRSDynamic
        .LockType = adLockOptimistic
        .CursorType = adOpenDynamic
        .Open cm
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Universal procedure to execute stored procedures
' Returns static recordset or return value
'
Public Function fnExecStorProc( _
    ByVal SPType As enumSPType, _
    ByVal Sql As String, _
    ParamArray Params() _
) As Variant
                            
Dim cmd As New ADODB.Command
Dim rst As New ADODB.Recordset
Dim i As Long, k As Long
Dim parRet As ADODB.Parameter
    With cmd
        Set .ActiveConnection = GetConnection
        
        .CommandText = Sql
        .CommandType = adCmdStoredProc
        
        Set parRet = .CreateParameter("return", adInteger, adParamReturnValue)
        .Parameters.Append parRet
        
        For k = LBound(Params) To UBound(Params)
            Dim a As ADODB.Parameter
            Set a = CreateParam(Params(k))
            .Parameters.Append a
        Next k
        
        If SPType = SP_OutputParam Then
            .Parameters.Item(1).Direction = adParamInputOutput
        End If
        
        If SPType = SP_Recorset Then
            rst.CursorType = adOpenStatic
            rst.CursorLocation = adUseClient
            rst.Open cmd, , adOpenStatic
            Set fnExecStorProc = rst
            Set rst = Nothing
        Else
            .Execute
            fnExecStorProc = parRet.Value
        End If
        
        If SPType = SP_OutputParam Then
            Params(LBound(Params)) = .Parameters.Item(1).Value
        End If
    
    End With
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' creating parameter for executing a command as a string
'
Private Function ParseParam(ByVal v As Variant) As String
    Select Case VarType(v)
    Case vbDate
        ParseParam = "'" & Format(v, "yyyymmdd") & "'"
    Case vbBoolean
        ParseParam = IIf(v, "1", "0")
    Case vbString
        ParseParam = "'" & Replace(v, "'", "''") & "'"
    Case vbNull
        ParseParam = "Null"
    Case Else
        ParseParam = Val(v)
    End Select
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' creating parameter for executing a command with manual parameters appending
'
Private Function CreateParam(ByVal v As Variant) As ADODB.Parameter
Static i As Long
Dim parX As ADODB.Parameter
    
    i = i + 1
    If i > 1000 Then i = 1
    
    Set parX = New ADODB.Parameter
    With parX
        .Name = "par" & i
        Select Case VarType(v)
        Case vbDate
            .type = adDBTimeStamp
            .Value = CDbl(v)
        Case vbBoolean
            .type = adBoolean
            .Value = CBool(v)
        Case vbString
            .type = adVarChar
            .Size = IIf(Len(v) = 0, 255, Len(v))
            .Value = CStr(v)
        Case vbNull
            .type = adInteger
            .Value = Null
        Case vbLong, vbInteger
            .type = adInteger
            .Value = v
        Case Else
            .type = adDouble
            .Value = CDbl(v)
        End Select
    End With
    Set CreateParam = parX
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_List(ByVal Sql As String) As ADODB.Recordset
    Set usp_List = fnExecStorProc(SP_Recorset, Sql)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Get(ByVal Sql As String, ByVal ID As Long) As ADODB.Recordset
    Set usp_Get = fnExecStorProc(SP_Recorset, Sql, DBWrite(ID, 0))
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Del(ByVal Sql As String, ByVal ID As Long) As Boolean
    usp_Del = 0 = fnExecStorProc(SP_ReturnValue, Sql, DBWrite(ID, 0))
End Function

''~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
''
'Public Function usp_Set(ByVal Sql As String, ID As Long, ParamArray Params())
'Dim ItemID As Variant
'    ItemID = DBWrite(ID, 0)
'    usp_Set = fnExecStorProc(SP_OutputParam, Sql, ItemID, Params())
'    ID = DBRead(ItemID, 0)
'End Function

'//////////////////////////////
' Report Filters menagement '/
'////////////////////////////

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' this procedure is getting information from Filter table
'
Public Function usp_FilterList_Get() As ADODB.Recordset
    Set usp_FilterList_Get = usp_List("usp_FilterList_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' this procedure is getting information from Filter table
'
Public Function usp_Filter_Get(ByVal iFilterID As Long) As ADODB.Recordset
    Set usp_Filter_Get = usp_Get("usp_Filter_Get", iFilterID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Filter_Del(ByVal iFilterID As Long) As Boolean
    usp_Filter_Del = usp_Del("usp_Filter_Del", iFilterID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Filter_Save( _
    FilterID As Long, _
    ByVal FilterName As String, _
    ByVal TraderID As Long, _
    ByVal StrategyID As Long, _
    ByVal GroupID As Long _
) As Variant
Dim ItemID As Variant
    ItemID = DBWrite(FilterID, 0)
    usp_Filter_Save = fnExecStorProc(SP_OutputParam, "usp_Filter_Save", _
        ItemID, _
        FilterName, _
        DBWrite(TraderID, 0), _
        DBWrite(StrategyID, 0), _
        DBWrite(GroupID, 0) _
        )
    FilterID = DBRead(ItemID, 0)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractSymbol_Get( _
    ByVal iGroupID As Long, _
    ByVal bIsAll As Boolean, _
    ByVal bStocksOnly As Boolean, _
    ByVal bIndexesOnly As Boolean, _
    ByVal bOptionsOnly As Boolean _
) As ADODB.Recordset
    Set usp_ContractSymbol_Get = fnExecStorProc( _
        SP_Recorset, _
        "usp_ContractSymbol_Get", _
        DBWrite(iGroupID, 0), _
        DBBit(bIsAll), _
        DBBit(bStocksOnly), _
        DBBit(bIndexesOnly), _
        DBBit(bOptionsOnly) _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'     iTraderID int = Null,
'
Public Function usp_Trader_Get( _
    Optional ByVal iTraderID As Variant = Null _
) As ADODB.Recordset
    Set usp_Trader_Get = fnExecStorProc(SP_Recorset, "usp_Trader_Get", _
        iTraderID _
        )
End Function


'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Strategy_Get( _
    Optional ByVal iStrategyID As Variant = Null _
) As ADODB.Recordset
    Set usp_Strategy_Get = fnExecStorProc(SP_Recorset, "usp_Strategy_Get", _
        iStrategyID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_ContractGroup_Get( _
    Optional ByVal iGroup_ID As Variant = Null _
    ) As ADODB.Recordset
    Set usp_ContractGroup_Get = fnExecStorProc(SP_Recorset, "usp_ContractGroup_Get", _
        iGroup_ID _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRCurve_Get(Optional ByVal iCurveID As Long) As ADODB.Recordset
    Set usp_IRCurve_Get = usp_Get("usp_IRCurve_Get", iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_IRPoint_Get(ByVal iCurveID As Long) As ADODB.Recordset
    Set usp_IRPoint_Get = usp_Get("usp_IRPoint_Get", iCurveID)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_WtdVegaMaturity_Get() As ADODB.Recordset
    Set usp_WtdVegaMaturity_Get = usp_List("usp_WtdVegaMaturity_Get")
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Expiry_Get( _
    ByVal iExpiry_ID As Variant, _
    ByVal vcExpiry_Period As Variant _
) As ADODB.Recordset
    Set usp_Expiry_Get = fnExecStorProc(SP_Recorset, _
        "usp_Expiry_Get", _
        iExpiry_ID, _
        vcExpiry_Period _
        )
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_WtdVegaMaturity_Save(ByVal Value As Long) As ADODB.Recordset
    fnExecStorProc SP_ReturnValue, "usp_WtdVegaMaturity_Save", Value
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function usp_Expiry_Save( _
    ByVal iExpiry_ID As Variant, _
    ByVal vcExpiry_Period As Variant, _
    ByVal dExpiry_Date As Variant, _
    Optional ByVal dVegaWeight As Variant = Null _
) As Long
    usp_Expiry_Save = fnExecStorProc(SP_ReturnValue, _
        "usp_Expiry_Save", _
        iExpiry_ID, _
        vcExpiry_Period, _
        dExpiry_Date, _
        dVegaWeight _
        )
End Function

