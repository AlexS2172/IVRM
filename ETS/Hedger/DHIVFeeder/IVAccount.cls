VERSION 1.0 CLASS
BEGIN
  MultiUse = -1  'True
  Persistable = 0  'NotPersistable
  DataBindingBehavior = 0  'vbNone
  DataSourceBehavior  = 0  'vbNone
  MTSTransactionMode  = 0  'NotAnMTSObject
END
Attribute VB_Name = "IVAccount"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = False
Attribute VB_Exposed = False
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Private Const THIS = "IVAccount"

Public pHttpFeeder As New HttpFeeder

Public VolAppLogin As String
Public VolAppPassword As String

Public IVEnabled As Boolean

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function IVInit() As Boolean
    On Error GoTo Herr
    
    IVInit = False
    
    If ConnectToIV Then
        IVInit = True
    Else
        Err.Raise vbObjectError, "IVInit", "Could't connect to IVolatility"
    End If
    
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    Err.Raise vbObjectError, "Load IV data", "Connecting to IV server failed due to the following error: " & vbCrLf & Message
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Function ConnectToIV() As Boolean
    Dim SessionKey As String
    g_SID = ""
    SessionKey = GetAuthKey(VolAppLogin)
    
    If Trim(SessionKey) = "" Then
        Err.Raise vbObjectError, "Load IV data", "Connection can not be established"
        ConnectToIV = False
    ElseIf Len(SessionKey) > 12 Then
        Err.Raise vbObjectError, "Load IV data", "Server did not authorize this request"
        ConnectToIV = False
    Else
        g_SID = GetMd5Key(SessionKey & VolAppPassword)
        ConnectToIV = True
    End If
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetAuthKey(Login As String) As String
    GetAuthKey = pHttpFeeder.HttpCall("auth.asp?name=" & Login)
End Function

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Private Function GetMd5Key(sKey As String) As String
    Dim MD5_obj As WORKFLOWICLib.MD5
    On Error GoTo Herr
    Set MD5_obj = New WORKFLOWICLib.MD5
    GetMd5Key = MD5_obj.MDString(sKey)
    Set MD5_obj = Nothing
    Exit Function
Herr:
    Dim Message As String
    Message = Err.Description
    Err.Raise vbObjectError, "WorkFlowIC.dll", "Call to WorkFlowIC.dll failed due to the following error: " & vbCrLf & Message
End Function

