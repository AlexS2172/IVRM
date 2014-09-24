Attribute VB_Name = "basDHMTFeeder1"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

Public Const APP_KEY = "EgarHedger"
Public Const APP_SETTINGS = "Settings"

Public Const APP_NAME = "Egar Hedger"
Public Const APP_TITLE = "Egar Hedger"

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

