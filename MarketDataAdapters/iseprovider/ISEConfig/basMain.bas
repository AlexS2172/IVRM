Attribute VB_Name = "basMain"
'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Option Explicit

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub Main()
    On Error GoTo Herr
    frmISEConfig1.Execute
    Exit Sub
Herr:
    ShowError
    End
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowError(Optional ByVal Title As String = "")
Dim Msg As String
    Msg = Err.Description
    If Len(Title) = 0 Then
        Msg = Title & vbCrLf & Msg
    End If
    If Len(Msg) > 0 Then
        MsgBox Msg, vbCritical
    End If
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowWarning(ByVal Msg As String)
    MsgBox Msg, vbExclamation
End Sub

'~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
'
Public Sub ShowMessage(ByVal Msg As String)
    MsgBox Msg, vbInformation
End Sub

