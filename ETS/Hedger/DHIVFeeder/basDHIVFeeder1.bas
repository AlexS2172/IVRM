Attribute VB_Name = "basDHIVFeeder1"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit
    
'VolServerName
'VolServerPort
'VolServerLogin
'VolServerPassword

'VolServerAppPath

'VolAppPassword
'VolAppLogin

'IsSSLUse
'CertPath

Public Const APP_NAME = "EgarHedger"
Public Const APP_REG_KEY = "SOFTWARE\Egar\ETS\Hedger\"
Public Const APP_SETTINGS = APP_REG_KEY & "ConnectionSettings"

Public g_SID As String
Public LastError As String

Public Declare Sub Sleep Lib "kernel32" (ByVal dwMilliseconds As Long)

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
' Shows error message box
'
Public Sub ShowError(Optional ByVal Header As String = "Unexpected Error")
Dim Msg As String
    Screen.MousePointer = vbNormal
    DoEvents
    If Err.Number = 0 Then Exit Sub
    Msg = Header & vbCrLf & _
        "Error number:" & Err.Number & " (0x" & Hex(Err.Number) & ")" & vbCrLf & _
        "Description: " & Err.Description
    MsgBox Msg, vbCritical
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function GetItemDouble(Items As Collection, ByVal Index As Long, Optional DefaultValue As Double = 0, Optional MustExist As Boolean = False, Optional ByVal Info As String) As String
Dim v As Variant
    v = GetItem(Items, Index, "", MustExist, Info)
    If v = "" Then
        GetItemDouble = DefaultValue
    Else
        GetItemDouble = ScanDouble(v)
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ScanDouble(ByVal Value As String) As Double
    On Error GoTo WrongFormat
    ScanDouble = Val(Value)
    Exit Function
WrongFormat:
    On Error GoTo Herr:
    ScanDouble = CDbl(Value)
    Exit Function
Herr:
    Err.Raise vbObjectError, "Load Portfolio", "Wrong format for values of type 'double'."
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function VolGetSetting(ByVal ParamName As String, Optional ByVal DefaultValue As String = "") As String
Dim Buf As String
    If GetStrKeyValue(HKEY_CURRENT_USER, APP_SETTINGS, ParamName, Buf, DefaultValue) Then
        VolGetSetting = Buf
    End If
    If VolGetSetting = "" Then
        VolGetSetting = DefaultValue
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub VolSetSetting(ByVal ParamName As String, ByVal Value As String)
    SetStrKeyValue HKEY_CURRENT_USER, APP_SETTINGS, ParamName, Value
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function VolGetSettingLong(ByVal ParamName As String, Optional ByVal DefaultValue As Long = 0) As Long
Dim Buf As Long
    If GetLongKeyValue(HKEY_CURRENT_USER, APP_SETTINGS, ParamName, Buf, DefaultValue) Then
        VolGetSettingLong = Buf
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub VolSetSettingLong(ByVal ParamName As String, ByVal Value As Long)
    SetLongKeyValue HKEY_CURRENT_USER, APP_SETTINGS, ParamName, Value
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Swap(a, b)
Dim c
    c = a
    a = b
    b = c
End Sub

